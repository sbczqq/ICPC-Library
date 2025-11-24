import os
import subprocess
import re

# 路径配置
CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
CONTENT_DIR = os.path.join(CURRENT_DIR, "..", "content")
OUTPUT_TEX = os.path.join(CURRENT_DIR, "notebook.tex")

def tex_escape(text):
    """
    转义 LaTeX 特殊字符，但保留数学公式 ($...$) 和转义符
    这是一个简化的处理，假设用户在 MD 里写的公式是标准的 $...$
    """
    # 暂时只转义最关键的字符，防止编译炸裂
    # 如果你的 MD 里有大量复杂公式，建议直接在 MD 里写 LaTeX 语法
    chars = {
        '&': r'\&',
        '%': r'\%',
        '#': r'\#',
        '_': r'\_',
        '{': r'\{',
        '}': r'\}',
    }
    # 简单的转义逻辑：如果不在 $...$ 内部，则转义
    # 这里为了脚本健壮性，我们做一个简单的全局替换
    # 注意：这可能会破坏 MD 里的复杂 LaTeX 公式，
    # 如果你是重度数学公式用户，建议 MD 内容直接写 LaTeX
    for key, value in chars.items():
        text = text.replace(key, value)
    return text

def convert_md_to_latex(md_text):
    """
    将简单的 Markdown 转换为 LaTeX
    支持：# 标题, **粗体**, `代码`, 列表
    """
    lines = md_text.split('\n')
    latex_lines = []
    
    for line in lines:
        line = line.strip()
        if not line:
            latex_lines.append("")
            continue
            
        # 处理标题 # -> \subsection*
        if line.startswith('# '):
            latex_lines.append(r"\subsection*{" + line[2:] + "}")
        elif line.startswith('## '):
            latex_lines.append(r"\subsubsection*{" + line[3:] + "}")
        # 处理列表 - -> itemize
        elif line.startswith('- '):
            latex_lines.append(r"\begin{itemize}\item " + line[2:] + r"\end{itemize}")
        else:
            # 处理行内格式
            # 粗体 **text** -> \textbf{text}
            line = re.sub(r'\*\*(.*?)\*\*', r'\\textbf{\1}', line)
            # 行内代码 `text` -> \texttt{text}
            line = re.sub(r'`(.*?)`', r'\\texttt{\1}', line)
            
            latex_lines.append(line + r" \\") # 加上换行符
            
    return "\n".join(latex_lines)

def generate_latex():
    print("1. Scanning for content...")
    
    latex_content = r"""
    \documentclass[10pt, landscape, a4paper]{article}
    \usepackage{multicol}
    \usepackage{listings} 
    \usepackage{xcolor}
    \usepackage{geometry}
    \usepackage{fancyhdr}
    \usepackage[utf8]{inputenc} 
    \usepackage[T1]{fontenc}
    \usepackage{amsmath}  % 支持数学公式
    \usepackage{amssymb}
    
    % 页面设置
    \geometry{top=1cm, bottom=1cm, left=0.5cm, right=0.5cm} 
    \setlength{\columnsep}{0.5cm}
    
    % 页眉
    \pagestyle{fancy}
    \fancyhf{}
    \lhead{\textbf{ICPC Template Library}}
    \rhead{\thepage}
    
    % 代码高亮设置
    \lstset{
        language=C++,
        basicstyle=\footnotesize\ttfamily,
        keywordstyle=\bfseries\color{blue},
        commentstyle=\itshape\color{teal},
        stringstyle=\color{red},
        numbers=left,
        numberstyle=\tiny\color{gray},
        stepnumber=1,
        breaklines=true,
        frame=lines,
        tabsize=2,
        showstringspaces=false,
        breakatwhitespace=false,
        escapeinside={(*@}{@*)}
    }
    
    % 调整列表间距
    \usepackage{enumitem}
    \setlist{nosep}

    \begin{document}
    \begin{multicols*}{3} 
    \tableofcontents
    """

    if not os.path.exists(CONTENT_DIR):
        print(f"Error: Content directory not found at {CONTENT_DIR}")
        return

    # 遍历一级目录 (Section)
    for section in sorted(os.listdir(CONTENT_DIR)):
        section_path = os.path.join(CONTENT_DIR, section)
        if not os.path.isdir(section_path) or section.startswith('.'):
            continue

        latex_content += f"\\section*{{{section.capitalize()}}}\n"
        
        # 获取该目录下所有文件
        files = sorted(os.listdir(section_path))
        
        # 分离 md 和 代码文件
        md_files = [f for f in files if f.endswith('.md')]
        code_files = [f for f in files if f.endswith(('.cpp', '.cc', '.py', '.java'))]
        
        # 1. 先处理 Markdown (作为文档说明)
        for filename in md_files:
            filepath = os.path.join(section_path, filename)
            try:
                with open(filepath, 'r', encoding='utf-8') as f:
                    md_text = f.read()
                    # 转换 MD 到 LaTeX
                    latex_text = convert_md_to_latex(md_text)
                    latex_content += f"\n{latex_text}\n"
            except Exception as e:
                print(f"Error parsing MD {filename}: {e}")

        # 2. 后处理代码文件
        for filename in code_files:
            filepath = os.path.join(section_path, filename)
            try:
                with open(filepath, 'r', encoding='utf-8') as f:
                    code = f.read()
                
                safe_filename = filename.replace("_", "\\_")
                latex_content += f"\\subsection*{{{safe_filename}}}\n"
                
                lang_opt = "language=C++"
                if filename.endswith(".py"): lang_opt = "language=Python"
                
                latex_content += f"\\begin{{lstlisting}}[{lang_opt}]\n"
                latex_content += code
                latex_content += "\\end{lstlisting}\n"
            except Exception as e:
                print(f"Error parsing Code {filename}: {e}")

    latex_content += r"\end{multicols*} \end{document}"

    with open(OUTPUT_TEX, "w", encoding='utf-8') as f:
        f.write(latex_content)
    
    print("2. Compiling PDF...")
    os.chdir(CURRENT_DIR)
    cmd = ["pdflatex", "-interaction=nonstopmode", "notebook.tex"]
    
    try:
        subprocess.run(cmd, check=True)
        subprocess.run(cmd, check=True)
        print("\n[SUCCESS] PDF with Markdown generated!")
    except subprocess.CalledProcessError:
        print("\n[ERROR] Compilation failed.")

if __name__ == "__main__":
    generate_latex()