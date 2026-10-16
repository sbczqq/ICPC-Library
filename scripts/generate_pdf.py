"""Generate the notebook with protected math and converged page references."""
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import textwrap

CURRENT_DIR = Path(__file__).resolve().parent
CONTENT_DIR = CURRENT_DIR.parent / "content"
OUTPUT_TEX = CURRENT_DIR / "notebook.tex"
CODE_EXTENSIONS = {".cpp", ".cc", ".h", ".hpp", ".py"}
MATH_PATTERN = re.compile(
    r"(?<!\\)(\$\$.*?(?<!\\)\$\$|\$[^$]*?(?<!\\)\$|\\\[.*?\\\]|\\\(.*?\\\))", re.DOTALL
)
DISPLAY_PATTERN = re.compile(r"(?<!\\)\$\$(.*?)(?<!\\)\$\$|\\\[(.*?)\\\]", re.DOTALL)
EMOJI_PATTERN = re.compile(r"[\U0001F000-\U0001FAFF\u231A-\u231B\u23E9-\u23F3\u2600-\u27BF\uFE0F\u200D]")
ESCAPES = {
    "\\": r"\textbackslash{}", "&": r"\&", "%": r"\%", "#": r"\#",
    "_": r"\_", "{": r"\{", "}": r"\}", "^": r"\textasciicircum{}",
    "~": r"\textasciitilde{}", "$": r"\$", "<": r"\textless{}", ">": r"\textgreater{}",
}
UNICODE_MATH = {
    "→": r"\to", "←": r"\leftarrow", "≤": r"\le", "≥": r"\ge",
    "≠": r"\ne", "×": r"\times", "∞": r"\infty", "∑": r"\sum",
    "∈": r"\in", "−": "-",
}


def remove_emoji(text):
    # Preserve meaningful Unicode such as arrows and mathematical operators.
    return EMOJI_PATTERN.sub("", text)


def escape_text(text):
    """Escape once, character by character (including literal backslashes)."""
    result = []
    for char in remove_emoji(text):
        if char in UNICODE_MATH:
            result.append(r"\ensuremath{" + UNICODE_MATH[char] + "}")
        else:
            result.append(ESCAPES.get(char, char))
    return "".join(result)


def sanitize_latex(text):
    """Escape text without altering any complete math span."""
    result, start = [], 0
    for match in MATH_PATTERN.finditer(text):
        result.extend((escape_text(text[start:match.start()]), match.group()))
        start = match.end()
    result.append(escape_text(text[start:]))
    return "".join(result)


INLINE_PATTERN = re.compile(
    r"(?P<code>\x60+)(?P<codebody>.+?)(?P=code)"
    r"|(?P<math>(?<!\\)\$[^$\n]+?(?<!\\)\$|\\\(.*?\\\))"
    r"|\*\*(?P<bold>.+?)\*\*"
    r"|(?<!\*)\*(?P<emphasis>[^*]+)\*(?!\*)"
    r"|\[(?P<label>[^\]]+)\]\((?P<url>[^\s)]+)\)"
)


def render_inline(text):
    parts, start = [], 0
    for match in INLINE_PATTERN.finditer(text):
        parts.append(escape_text(text[start:match.start()]))
        if match.group("code"):
            # Wrap long identifiers/expressions without changing their characters.
            code = r"\allowbreak{}".join(escape_text(c) for c in match.group("codebody"))
            parts.append(r"\texttt{" + code + "}")
        elif match.group("math"):
            parts.append(match.group("math"))
        elif match.group("bold"):
            parts.append(r"\textbf{" + render_inline(match.group("bold")) + "}")
        elif match.group("emphasis"):
            parts.append(r"\textit{" + render_inline(match.group("emphasis")) + "}")
        else:
            url = r"\allowbreak{}".join(escape_text(c) for c in match.group("url"))
            parts.append(render_inline(match.group("label")) + r" (\texttt{" + url + "})")
        start = match.end()
    parts.append(escape_text(text[start:]))
    return "".join(parts)


def code_listing(code, language="cpp", example=False):
    code = remove_emoji(code)
    # Ignore stray Markdown fence lines copied into a source file.
    code = re.sub(r"(?m)^\s*\x60{3}[^\n]*$", "", code)
    options = "language=Python" if language in {"py", "python"} else "language=C++"
    if language in {"text", "txt", ""}:
        options = "language={}"
    if example:
        options += ",numbers=none,basicstyle=\\footnotesize\\ttfamily"
    return "\\begin{lstlisting}[" + options + "]\n" + code.rstrip() + "\n\\end{lstlisting}\n"


def document_title(md_text):
    match = re.search(r"^#{1,6}\s+(.+?)\s*#*\s*$", md_text, re.M)
    if match:
        return remove_emoji(match.group(1)).strip()
    # Older notes such as Prim use an unmarked first-line title.
    return next((remove_emoji(line).strip() for line in md_text.splitlines()
                 if line.strip() and line.strip().lower() != "markdown"), None)


def convert_md_to_latex(md_text, skip_title=None):
    """Render nested lists, fences, inline code and multiline display math."""
    displays, fences = [], []

    def save_display(match):
        displays.append(match.group(1) if match.group(1) is not None else match.group(2))
        return "\nNOTEBOOKDISPLAY" + str(len(displays) - 1) + "\n"

    def save_fence(match):
        fences.append((match.group(2).strip(), textwrap.dedent(match.group(3))))
        return "\nNOTEBOOKFENCE" + str(len(fences) - 1) + "\n"

    # Extract fences before math; literal dollars inside code are not formulae.
    fence_pattern = re.compile(r"(?m)^[ \t]*(\x60{3,}|~{3,})([^\n]*)\n(.*?)^[ \t]*\1[ \t]*$", re.DOTALL)
    md_text = fence_pattern.sub(save_fence, md_text)
    # Protect complete displays before splitting lines.
    md_text = DISPLAY_PATTERN.sub(save_display, md_text)
    output, stack, paragraph = [], [], []
    title_skipped = False

    def flush():
        if paragraph:
            output.append(render_inline(" ".join(paragraph)) + r"\par")
            paragraph.clear()

    def close_lists(min_indent=-1):
        while stack and stack[-1][0] >= min_indent:
            output.append(r"\end{" + stack.pop()[1] + "}")

    for raw_line in md_text.splitlines():
        line = raw_line.strip()
        indent = len(raw_line.expandtabs(4)) - len(raw_line.expandtabs(4).lstrip())
        if not line:
            flush()
            continue
        if line.lower() == "markdown" and not output and not paragraph:
            continue
        if not title_skipped and line == skip_title:
            title_skipped = True
            continue
        heading = re.match(r"^#{1,6}\s+(.+?)\s*#*\s*$", line)
        if heading:
            flush()
            close_lists()
            title = remove_emoji(heading.group(1)).strip()
            if not title_skipped and skip_title == title:
                title_skipped = True
                continue
            output.append(r"\par\addvspace{0.7ex}\noindent\textbf{" + render_inline(title) + r"}\par\nobreak")
            continue
        marker = re.fullmatch(r"NOTEBOOK(DISPLAY|FENCE)(\d+)", line)
        if marker:
            flush()
            kind, index = marker.group(1), int(marker.group(2))
            if kind == "DISPLAY":
                output.append(r"\NotebookMath{" + displays[index].strip() + "}")
            else:
                close_lists()
                language, code = fences[index]
                output.append(code_listing(code, language, example=True))
            continue
        if re.fullmatch(r"([-*_])(?:\s*\1){2,}", line):
            flush()
            close_lists()
            continue
        item = re.match(r"^(?:([-*+])(?:\s+|$)|(\d+)[.)]\s+)(.*)$", line)
        if item:
            flush()
            environment = "enumerate" if item.group(2) else "itemize"
            while stack and (stack[-1][0] > indent or (stack[-1][0] == indent and stack[-1][1] != environment)):
                output.append(r"\end{" + stack.pop()[1] + "}")
            if not stack or stack[-1][0] < indent:
                options = "[start=" + item.group(2) + "]" if item.group(2) else ""
                output.append(r"\begin{" + environment + "}" + options)
                stack.append((indent, environment))
            output.append(r"\item " + render_inline(item.group(3)))
            continue
        if stack and indent <= stack[-1][0]:
            flush()
            close_lists(indent)
        paragraph.append(line)
    flush()
    close_lists()
    return "\n".join(output)


LATEX_HEAD = r"""\documentclass[10pt,landscape,a4paper]{article}
\usepackage[UTF8,scheme=chinese,fontset=none]{ctex}
\usepackage{multicol,listings,xcolor,geometry,fancyhdr,amsmath,amssymb,enumitem,graphicx}
\IfFontExistsTF{Consolas}{\setmonofont{Consolas}}{}
\IfFontExistsTF{SimSun}{
  \setCJKmainfont[AutoFakeBold=2,AutoFakeSlant=0.2]{SimSun}
  \setCJKmonofont[AutoFakeBold=2,AutoFakeSlant=0.2]{SimSun}
  \setCJKsansfont{Microsoft YaHei}
}{
  \setCJKmainfont[AutoFakeBold=2,AutoFakeSlant=0.2]{FandolSong-Regular}
  \setCJKmonofont[AutoFakeBold=2,AutoFakeSlant=0.2]{FandolSong-Regular}
  \setCJKsansfont{FandolHei-Regular}
}
\geometry{top=0.55cm,bottom=1cm,left=1cm,right=0.8cm,includehead,headheight=14pt,headsep=5pt}
\setlength{\footskip}{0.45cm}
\setlength{\columnsep}{0.6cm}
\setlength{\parindent}{0pt}
\setlength{\parskip}{0.35ex plus 0.1ex}
\setlength{\emergencystretch}{2em}
\setcounter{tocdepth}{2}
\pagestyle{fancy}
\fancyhf{}
\fancyhead[L]{\small\textbf{ICPC Template Library}}
\fancyfoot[C]{\thepage}
\fancypagestyle{plain}{
  \fancyhf{}
  \fancyhead[L]{\small\textbf{ICPC Template Library}}
  \fancyfoot[C]{\thepage}
}
\lstset{
  language=C++,basicstyle=\small\ttfamily,keywordstyle=\bfseries\color{blue},
  commentstyle=\color{teal!80!black},stringstyle=\color{red},
  numbers=left,numberstyle=\tiny\color{gray},numbersep=4pt,
  breaklines=true,breakatwhitespace=false,breakautoindent=true,
  frame=single,tabsize=2,extendedchars=false,columns=fullflexible,keepspaces=true,
  xleftmargin=1em,framexleftmargin=0.2em,showstringspaces=false,
  literate={→}{{$\to$}}1 {←}{{$\leftarrow$}}1 {≤}{{$\le$}}1 {≥}{{$\ge$}}1
           {≠}{{$\ne$}}1 {×}{{$\times$}}1 {∞}{{$\infty$}}1
}
\setlist{nosep,leftmargin=1.15em,labelsep=0.3em,topsep=0.3ex}
\newsavebox{\NotebookMathBox}
\newcommand{\NotebookMath}[1]{%
  \par\begingroup
  \sbox{\NotebookMathBox}{$\displaystyle #1$}%
  \noindent\makebox[\linewidth][c]{%
    \ifdim\wd\NotebookMathBox>\linewidth
      \resizebox{\linewidth}{!}{\usebox{\NotebookMathBox}}%
    \else\usebox{\NotebookMathBox}\fi}%
  \par\endgroup}
\begin{document}
\tableofcontents
\clearpage
\begin{multicols*}{3}
\raggedcolumns
"""


def algorithm_id(filename):
    name = re.sub(r"(_doc|-doc|文档|说明|doc)$", "", Path(filename).stem, flags=re.I)
    # Pair 最小·生成树prim_doc.md with 最小生成树prim.cpp, for example.
    return re.sub(r"[\W_]+", "", name.casefold())


def generate_source():
    parts, document_count, code_count = [LATEX_HEAD], 0, 0
    for section_path in sorted(CONTENT_DIR.iterdir()):
        if not section_path.is_dir() or section_path.name.startswith("."):
            continue
        files = sorted(p for p in section_path.iterdir() if p.is_file() and not p.name.startswith(".") and p.suffix.lower() in CODE_EXTENSIONS | {".md"})
        if not files:
            continue
        groups = {}
        for path in files:
            groups.setdefault(algorithm_id(path.name), []).append(path)
        section = sanitize_latex(section_path.name)
        parts.append(f"\\section*{{{section}}}\n\\addcontentsline{{toc}}{{section}}{{{section}}}\n")
        for key in sorted(groups):
            group = groups[key]
            documents = [(p, p.read_text(encoding="utf-8-sig")) for p in group if p.suffix.lower() == ".md"]
            title = next((document_title(md) for _, md in documents if document_title(md)), group[0].stem)
            escaped_title = sanitize_latex(title)
            parts.append(f"\\subsection*{{{escaped_title}}}\n\\addcontentsline{{toc}}{{subsection}}{{{escaped_title}}}\n")
            for path, md in documents:
                parts.append(f"% Source: {path.relative_to(CONTENT_DIR).as_posix()}\n")
                parts.append(convert_md_to_latex(md, skip_title=document_title(md)) + "\n")
                document_count += 1
            for path in group:
                if path.suffix.lower() not in CODE_EXTENSIONS:
                    continue
                parts.append(r"\par\noindent{\scriptsize\texttt{File: " + escape_text(path.name) + "}}\par\n")
                parts.append(code_listing(path.read_text(encoding="utf-8-sig"), path.suffix.lstrip(".")))
                code_count += 1
    parts.append("\\end{multicols*}\n\\end{document}\n")
    OUTPUT_TEX.write_text("".join(parts), encoding="utf-8")
    print(f"Processed {document_count} documents and {code_count} code files.")


def compile_pdf(max_passes=6):
    executable = shutil.which("xelatex")
    if not executable:
        raise RuntimeError("XeLaTeX not found. Install MiKTeX or TeX Live and add xelatex to PATH.")
    # Preserve the last working PDF until the new build passes validation.
    with tempfile.TemporaryDirectory(prefix="icpc-notebook-") as temporary:
        build_dir = Path(temporary)
        shutil.copy2(OUTPUT_TEX, build_dir / "notebook.tex")

        def reference_state():
            return tuple((build_dir / ("notebook" + ext)).read_bytes() if (build_dir / ("notebook" + ext)).exists() else b"" for ext in (".toc", ".aux", ".out"))

        for pass_number in range(1, max_passes + 1):
            before = reference_state()
            result = subprocess.run(
                [executable, "-interaction=nonstopmode", "-halt-on-error", "-file-line-error", "notebook.tex"],
                cwd=build_dir, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
            )
            log_file = build_dir / "notebook.log"
            if log_file.exists():
                shutil.copy2(log_file, CURRENT_DIR / "notebook.log")
            if result.returncode:
                tail = result.stdout.decode("utf-8", errors="replace").splitlines()[-25:]
                raise RuntimeError("XeLaTeX failed:\n" + "\n".join(tail))
            stable = before == reference_state()
            print(f"XeLaTeX pass {pass_number}: " + ("references stable" if stable else "updating references"))
            if pass_number >= 2 and stable:
                break
        else:
            raise RuntimeError(f"References did not converge after {max_passes} passes; PDF was not replaced.")
        log = log_file.read_text(encoding="utf-8", errors="replace")
        problems = re.findall(r"(?:Missing character:.*|Overfull \\[hv]box[^\n]*|LaTeX Warning:[^\n]*)", log)
        if problems:
            raise RuntimeError("PDF validation failed; previous PDF was kept:\n" + "\n".join(problems))
        for ext in (".pdf", ".aux", ".toc", ".log"):
            shutil.copy2(build_dir / ("notebook" + ext), CURRENT_DIR / ("notebook" + ext))
        print(f"[SUCCESS] {CURRENT_DIR / 'notebook.pdf'}")


def generate_latex():
    generate_source()
    compile_pdf()


if __name__ == "__main__":
    try:
        generate_latex()
    except (OSError, ValueError, RuntimeError) as error:
        raise SystemExit(f"[ERROR] {error}")
