import os
import subprocess
import re

# 路径配置
CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
CONTENT_DIR = os.path.join(CURRENT_DIR, "..", "content")
OUTPUT_TEX = os.path.join(CURRENT_DIR, "notebook.tex")

def convert_md_to_latex(md_text):
    """
    将简单的 Markdown 转换为 LaTeX
    """
    lines = md_text.split('\n')
    latex_lines = []
    
    for line in lines:
        line = line.strip()
        if not line:
            latex_lines.append("")
            continue
            
        # 处理标题
        if line.startswith('# '):
            latex_lines.append(r"\subsection*{" + line[2:] + "}")
        elif line.startswith('## '):
            latex_lines.append(r"\subsubsection*{" + line[3:] + "}")
        # 处理列表
        elif line.startswith('- '):
            latex_lines.append(r"\begin{itemize}\item " + line[2:] + r"\end{itemize}")
        else:
            # 简单处理粗体和行内代码
            line = re.sub(r'\*\*(.*?)\*\*', r'\\textbf{\1}', line)
            line = re.sub(r'`(.*?)`', r'\\texttt{\1}', line)
            latex_lines.append(line + r" \\") 
            
    return "\n".join(latex_lines)

def generate_latex():
    print("1. Scanning for content...")
    
    # === 修改点 1: 引入 ctex 宏包支持中文，移除 inputenc ===
    latex_content = r"""
    \documentclass[10pt, landscape, a4paper]{article}
    \usepackage[UTF8]{ctex}  % 关键：中文支持
    \usepackage{multicol}
    \usepackage{listings} 
    \usepackage{xcolor}
    \usepackage{geometry}
    \usepackage{fancyhdr}
    \usepackage{amsmath}
    \usepackage{amssymb}
    \usepackage{enumitem}

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
        commentstyle=\itshape\color{teal!80!black}, % 注释颜色调深一点
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
    \setlist{nosep}

    \begin{document}
    \begin{multicols*}{3} 
    \tableofcontents
    """

    if not os.path.exists(CONTENT_DIR):
        print(f"Error: Content directory not found at {CONTENT_DIR}")
        return

    # 遍历一级目录
    for section in sorted(os.listdir(CONTENT_DIR)):
        section_path = os.path.join(CONTENT_DIR, section)
        if not os.path.isdir(section_path) or section.startswith('.'):
            continue

        latex_content += f"\\section*{{{section.capitalize()}}}\n"
        
        # 递归获取文件
        all_files = []
        for root, dirs, files in os.walk(section_path):
            for file in files:
                rel_dir = os.path.relpath(root, section_path)
                if rel_dir == ".":
                    all_files.append((file, os.path.join(root, file)))
                else:
                    display_name = os.path.join(rel_dir, file).replace("\\", "/")
                    all_files.append((display_name, os.path.join(root, file)))

        all_files.sort(key=lambda x: x[0])

        # 分离 md 和 代码
        md_files = [item for item in all_files if item[0].endswith('.md')]
        code_files = [item for item in all_files if item[0].endswith(('.cpp', '.cc', '.py', '.java'))]
        
        # 处理 Markdown
        for display_name, filepath in md_files:
            try:
                with open(filepath, 'r', encoding='utf-8') as f:
                    md_text = f.read()
                    latex_text = convert_md_to_latex(md_text)
                    latex_content += f"\n{latex_text}\n"
            except Exception as e:
                print(f"Error parsing MD {display_name}: {e}")

        # 处理代码
        for display_name, filepath in code_files:
            try:
                with open(filepath, 'r', encoding='utf-8') as f:
                    code = f.read()
                
                safe_filename = display_name.replace("_", "\\_")
                latex_content += f"\\subsection*{{{safe_filename}}}\n"
                
                lang_opt = "language=C++"
                if display_name.endswith(".py"): lang_opt = "language=Python"
                
                latex_content += f"\\begin{{lstlisting}}[{lang_opt}]\n"
                latex_content += code
                latex_content += "\\end{lstlisting}\n"
            except Exception as e:
                print(f"Error parsing Code {display_name}: {e}")

    latex_content += r"\end{multicols*} \end{document}"

    with open(OUTPUT_TEX, "w", encoding='utf-8') as f:
        f.write(latex_content)
    
    print("2. Compiling PDF using XeLaTeX...")
    os.chdir(CURRENT_DIR)
    
    # === 修改点 2: 使用 xelatex 编译器 ===
    # xelatex 对 UTF-8 和中文字体的支持远好于 pdflatex
    cmd = ["xelatex", "-interaction=nonstopmode", "notebook.tex"]
    
    try:
        # 运行两次以生成目录
        subprocess.run(cmd, check=True)
        subprocess.run(cmd, check=True)
        print("\n[SUCCESS] PDF generated successfully with Chinese support!")
    except subprocess.CalledProcessError:
        print("\n[ERROR] Compilation failed.")
        print("请检查是否安装了 texlive-lang-chinese 和 texlive-xetex。")
    except FileNotFoundError:
        print("\n[ERROR] 找不到 xelatex 命令。")
        print("请运行: sudo apt install texlive-xetex")

if __name__ == "__main__":
    generate_latex()