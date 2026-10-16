"""Regression checks for the notebook's previously broken conversion/build cases."""
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch
import generate_pdf as generator


class NotebookTests(unittest.TestCase):
    def test_display_math_is_not_escaped_or_split(self):
        formula = "\\sum_{i=0}^n\\binom{n}{i}^2\n=\\binom{2n}{n}"
        rendered = generator.convert_md_to_latex("$$" + formula + "$$")
        self.assertIn("\\NotebookMath{" + formula + "}", rendered)
        self.assertNotIn("textasciicircum", rendered)

    def test_fenced_code_keeps_spaces_backslashes_and_dollars(self):
        source = "  1\n / \\\n2   3\n// $not_math$\n"
        rendered = generator.convert_md_to_latex("```text\n" + source + "```\n")
        self.assertIn(source.rstrip(), rendered)
        self.assertIn("\\begin{lstlisting}", rendered)
        self.assertNotIn("```", rendered)

    def test_inline_math_and_inline_code_are_distinct(self):
        rendered = generator.render_inline("$x_i^2$ and `a_b^2 < n` →")
        self.assertIn("$x_i^2$", rendered)
        self.assertIn("\\_", rendered)
        self.assertIn("\\textless{}", rendered)
        self.assertIn("\\ensuremath{\\to}", rendered)

    def test_nested_lists_and_single_title(self):
        md = "Markdown\n## Title\n\n* **outer**\n  1. inner\n  2. next\n* final\n"
        rendered = generator.convert_md_to_latex(md, skip_title="Title")
        self.assertNotIn("Markdown", rendered)
        self.assertNotIn("Title", rendered)
        self.assertIn("\\begin{itemize}", rendered)
        self.assertIn("\\begin{enumerate}[start=1]", rendered)
        self.assertLess(rendered.index("\\end{enumerate}"), rendered.index("\\item final"))

    def test_filename_punctuation_does_not_separate_doc_and_code(self):
        self.assertEqual(generator.algorithm_id("最小·生成树prim_doc.md"), generator.algorithm_id("最小生成树prim.cpp"))

    def test_plain_title_and_reference_url_are_preserved(self):
        title = "最小生成树 - Prim 算法"
        self.assertEqual(generator.document_title(title + "\n\n算法说明"), title)
        self.assertNotIn(title, generator.convert_md_to_latex(title + "\n\n算法说明", skip_title=title))
        rendered = generator.render_inline("[参考](https://example.org/a_b/#树上差分)")
        plain = rendered.replace(r"\allowbreak{}", "").replace(r"\_", "_").replace(r"\#", "#")
        self.assertIn("https://example.org/a_b/#树上差分", plain)

    def test_compile_requires_third_pass_after_toc_expands(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            tex = root / "notebook.tex"
            tex.write_text("source", encoding="utf-8")
            calls = []

            def fake_run(args, cwd, **kwargs):
                calls.append(args)
                directory = Path(cwd)
                state = "old pages" if len(calls) == 1 else "correct pages"
                for ext in (".aux", ".toc", ".out"):
                    (directory / ("notebook" + ext)).write_text(state)
                (directory / "notebook.log").write_text("clean log")
                (directory / "notebook.pdf").write_bytes(b"new PDF")
                return type("Result", (), {"returncode": 0})()

            with patch.object(generator, "CURRENT_DIR", root), patch.object(generator, "OUTPUT_TEX", tex), patch.object(generator.shutil, "which", return_value="xelatex"), patch.object(generator.subprocess, "run", side_effect=fake_run):
                generator.compile_pdf()
            self.assertEqual(len(calls), 3)
            self.assertEqual((root / "notebook.toc").read_text(), "correct pages")

    def test_failed_build_keeps_previous_pdf_and_reports_error(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            tex = root / "notebook.tex"
            tex.write_text("source")
            pdf = root / "notebook.pdf"
            pdf.write_bytes(b"previous PDF")
            failure = type("Result", (), {"returncode": 1, "stdout": b"bad formula"})()
            with patch.object(generator, "CURRENT_DIR", root), patch.object(generator, "OUTPUT_TEX", tex), patch.object(generator.shutil, "which", return_value="xelatex"), patch.object(generator.subprocess, "run", return_value=failure):
                with self.assertRaisesRegex(RuntimeError, "bad formula"):
                    generator.compile_pdf()
            self.assertEqual(pdf.read_bytes(), b"previous PDF")


if __name__ == "__main__":
    unittest.main()
