import os
import subprocess
import re

# 路径配置
CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
CONTENT_DIR = os.path.join(CURRENT_DIR, "..", "content")
OUTPUT_TEX = os.path.join(CURRENT_DIR, "notebook.tex")

# Keep complete inline and display math untouched while escaping surrounding text.
# The old ``\$.*?\$`` expression split ``$$...$$`` into two empty math blocks,
# so characters such as ``^``, ``_`` and ``{}`` inside display formulae were
# escaped as ordinary text and rendered incorrectly.
MATH_PATTERN = re.compile(
    r'(?<!\\)(\$\$.*?(?<!\\)\$\$|\$.*?(?<!\\)\$)',
    re.DOTALL,
)

def sanitize_latex(text):
    """移除 Emoji 并转义 LaTeX 保留字符，同时保护数学公式"""
    # 移除 LaTeX 无法处理的 Emoji [cite: 5340, 5421]
    text = re.sub(r'[^\u0000-\u05C0\u2100-\u214F\u4E00-\u9FFF\u3000-\u303F\uff00-\uffef]', '', text)
    
    # 同时保护行内公式 $...$ 和块级公式 $$...$$
    math_blocks = []
    def save_math(m):
        math_blocks.append(m.group(0))
        return f"MATHPH{len(math_blocks)-1}PH"
    text = MATH_PATTERN.sub(save_math, text)

    # 转义 LaTeX 保留字符：解决日志中的 ^ 和 _ 报错 [cite: 5341, 5422]
    chars = {
        '&': r'\&', '%': r'\%', '#': r'\#', '_': r'\_',
        '{': r'\{', '}': r'\}', '^': r'\textasciicircum{}',
        '~': r'\textasciitilde{}'
    }
    for char, escaped in chars.items():
        text = text.replace(char, escaped)

    # 还原数学公式
    for i, block in enumerate(math_blocks):
        text = text.replace(f"MATHPH{i}PH", block)
    return text

def convert_md_to_latex(md_text):
    """将 Markdown 转换为 LaTeX，并处理列表防止堆栈溢出 [cite: 5436]"""
    lines = md_text.split('\n')
    latex_output = []
    in_list = False
    
    for line in lines:
        line = line.strip()
        if not line:
            if in_list:
                latex_output.append(r"\end{itemize}")
                in_list = False
            latex_output.append("\n")
            continue
            
        # 内部标题加粗处理
        if line.startswith('#'):
            if in_list:
                latex_output.append(r"\end{itemize}")
                in_list = False
            title = line.lstrip('#').strip()
            latex_output.append(r"\noindent\textbf{" + sanitize_latex(title) + r"} \\")
            continue
            
        # 列表处理 [cite: 5418, 5431]
        if line.startswith('- '):
            if not in_list:
                latex_output.append(r"\begin{itemize}")
                in_list = True
            content = sanitize_latex(line[2:].strip())
            latex_output.append(r"\item " + content)
            continue
        
        if in_list and not line.startswith('- '):
            latex_output.append(r"\end{itemize}")
            in_list = False

        # 正文内容
        line_content = sanitize_latex(line)
        line_content = re.sub(r'\*\*(.*?)\*\*', r'\\textbf{\1}', line_content)
        line_content = re.sub(r'`(.*?)`', r'\\texttt{\1}', line_content)
        latex_output.append(line_content + r" \\")

    if in_list: latex_output.append(r"\end{itemize}")
    return "\n".join(latex_output)

def generate_latex():
    print("1. Grouping and processing files...")
    
    latex_head = r"""
\documentclass[10pt, landscape, a4paper]{article}
\usepackage[UTF8, scheme=chinese]{ctex}
\usepackage{multicol, listings, xcolor, geometry, fancyhdr, amsmath, amssymb, enumitem}
\geometry{top=1cm, bottom=1cm, left=0.8cm, right=0.8cm}
\setlength{\footskip}{0.45cm}
\setlength{\columnsep}{0.6cm}
\setcounter{tocdepth}{2}
\pagestyle{fancy}
\fancyhf{}
\fancyhead[L]{\textbf{ICPC Template Library}}
\fancyfoot[C]{\thepage}
\fancypagestyle{plain}{
    \fancyhf{}
    \fancyhead[L]{\textbf{ICPC Template Library}}
    \fancyfoot[C]{\thepage}
}
\lstset{
    language=C++, basicstyle=\small\ttfamily, keywordstyle=\bfseries\color{blue},
    commentstyle=\itshape\color{teal!80!black}, stringstyle=\color{red},
    numbers=left, numberstyle=\tiny\color{gray}, breaklines=true,
    frame=single, tabsize=2, extendedchars=false, columns=flexible, keepspaces=true,
    showstringspaces=false
}
\setlist{nosep}
\begin{document}
\tableofcontents \newpage
\begin{multicols*}{3} 
"""
    full_tex = latex_head

    for section in sorted(os.listdir(CONTENT_DIR)):
        section_path = os.path.join(CONTENT_DIR, section)
        if not os.path.isdir(section_path) or section.startswith('.'): continue

        full_tex += f"\\section*{{{section.capitalize()}}}\n"
        full_tex += f"\\addcontentsline{{toc}}{{section}}{{{section.capitalize()}}}\n"
        
        # 核心逻辑：按算法名分组
        algo_groups = {} # key: clean_name, value: list of filenames
        for f in sorted(os.listdir(section_path)):
            if f.startswith('.'): continue
            base, ext = os.path.splitext(f)
            # 提取算法核心名：如 fft_doc -> fft
            algo_id = re.sub(r'(_doc|-doc|文档|说明|doc)$', '', base, flags=re.IGNORECASE).lower()
            if algo_id not in algo_groups: algo_groups[algo_id] = []
            algo_groups[algo_id].append(f)

        # 按算法 ID 排序处理
        for algo_id in sorted(algo_groups.keys()):
            # 同组内：MD 优先打印
            group_files = sorted(algo_groups[algo_id], key=lambda x: (0 if x.lower().endswith('.md') else 1, x))
            
            # 使用组内第一个 MD 的标题作为目录项，如果没有 MD 则用文件名
            entry_title = algo_id.replace('_', ' ').capitalize()
            
            for filename in group_files:
                fpath = os.path.join(section_path, filename)
                ext = os.path.splitext(filename)[1].lower()

                if ext == '.md':
                    try:
                        with open(fpath, 'r', encoding='utf-8') as f:
                            md_content = f.read()
                            # 尝试从 MD 中提取目录标题
                            title_match = re.search(r'^#+\s+(.*)$', md_content, re.M)
                            if title_match: entry_title = title_match.group(1).strip()
                            
                            full_tex += f"\\subsection*{{{sanitize_latex(entry_title)}}}\n"
                            full_tex += f"\\addcontentsline{{toc}}{{subsection}}{{{sanitize_latex(entry_title)}}}\n"
                            full_tex += convert_md_to_latex(md_content) + "\n"
                    except: pass
                elif ext in ['.cpp', '.cc', '.py', '.h']:
                    # 如果这组没有 MD，在此处补上目录标题
                    if not any(f.lower().endswith('.md') for f in group_files):
                        full_tex += f"\\subsection*{{{sanitize_latex(entry_title)}}}\n"
                        full_tex += f"\\addcontentsline{{toc}}{{subsection}}{{{sanitize_latex(entry_title)}}}\n"
                    
                    try:
                        with open(fpath, 'r', encoding='utf-8') as f:
                            code = f.read()
                        # 解决 f-string 中无法包含反斜杠的问题
                        safe_fname = filename.replace('_', r'\_')
                        full_tex += f"\\noindent\\texttt{{\\scriptsize File: {safe_fname}}}\n"
                        full_tex += f"\\begin{{lstlisting}}\n{code}\n\\end{{lstlisting}}\n"
                    except: pass

    full_tex += r"\end{multicols*} \end{document}"
    with open(OUTPUT_TEX, "w", encoding='utf-8') as f: f.write(full_tex)
    
    print("2. Compiling...")
    os.chdir(CURRENT_DIR)
    for e in ['.aux', '.log', '.toc']:
        try: os.remove("notebook" + e)
        except: pass
    try:
        subprocess.run(["xelatex", "-interaction=nonstopmode", "notebook.tex"], check=True)
        subprocess.run(["xelatex", "-interaction=nonstopmode", "notebook.tex"], check=True)
        print("\n[SUCCESS] PDF generated!")
    except:
        print("\n[ERROR] Compilation failed. See notebook.log.")

if __name__ == "__main__":
    generate_latex()
