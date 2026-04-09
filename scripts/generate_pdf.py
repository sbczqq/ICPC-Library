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
            
        # 处理标题 (加入了对 ### 的支持，并转义标题中的 & 和 #)
        if line.startswith('# '):
            title = line[2:].replace('&', r'\&').replace('#', r'\#')
            latex_lines.append(r"\subsection*{" + title + "}")
        elif line.startswith('## '):
            title = line[3:].replace('&', r'\&').replace('#', r'\#')
            latex_lines.append(r"\subsubsection*{" + title + "}")
        elif line.startswith('### '):
            title = line[4:].replace('&', r'\&').replace('#', r'\#')
            # LaTeX article 默认没有 subsubsubsection，使用 paragraph 替代，显示为加粗标题
            latex_lines.append(r"\paragraph*{" + title + "}")
            
        # 处理列表
        elif line.startswith('- '):
            content = line[2:].replace('&', r'\&').replace('#', r'\#')
            latex_lines.append(r"\begin{itemize}\item " + content + r"\end{itemize}")
            
        else:
            # 简单处理粗体和行内代码
            line = re.sub(r'\*\*(.*?)\*\*', r'\\textbf{\1}', line)
            line = re.sub(r'`(.*?)`', r'\\texttt{\1}', line)
            
            # 转义正文中的 LaTeX 特殊字符 (避免编译报错)
            line = line.replace('&', r'\&')
            line = line.replace('#', r'\#')
            
            latex_lines.append(line + r" \\") 
            
    return "\n".join(latex_lines)

def generate_latex():
    print("1. Scanning for content...")
    
    latex_content = r"""
    \documentclass[10pt, landscape, a4paper]{article}
    \usepackage[UTF8]{ctex}  % 中文支持
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
        commentstyle=\itshape\color{teal!80!black},
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
        
        # 获取该分类下所有文件
        all_files = []
        for root, dirs, files in os.walk(section_path):
            for file in files:
                if file.startswith('.'): continue
                rel_dir = os.path.relpath(root, section_path)
                if rel_dir == ".":
                    display_name = file
                else:
                    display_name = os.path.join(rel_dir, file).replace("\\", "/")
                all_files.append((display_name, os.path.join(root, file)))

        # ================= 核心优化点：智能排序规则 =================
        def custom_sort(item):
            display_name = item[0]
            base, ext = os.path.splitext(display_name)
            ext = ext.lower()
            
            clean_base = base
            if ext == '.md':
                # 智能去除常见的 Markdown 后缀，以便与代码文件同名对齐
                for suf in ['_doc', '-doc', '文档', '说明', 'doc']:
                    if clean_base.lower().endswith(suf):
                        clean_base = clean_base[:-len(suf)]
                        break
                clean_base = clean_base.strip()
            
            # 同名情况下，强制 .md 的优先级最高 (0)，其他代码文件排在后面 (1)
            priority = 0 if ext == '.md' else 1
            return (clean_base.lower(), priority, ext)

        # 按照自定义规则排序
        all_files.sort(key=custom_sort)

        # 顺序渲染到 LaTeX
        for display_name, filepath in all_files:
            base, ext = os.path.splitext(display_name)
            ext = ext.lower()

            if ext == '.md':
                try:
                    with open(filepath, 'r', encoding='utf-8') as f:
                        md_text = f.read()
                        latex_text = convert_md_to_latex(md_text)
                        latex_content += f"\n{latex_text}\n"
                except Exception as e:
                    print(f"Error parsing MD {display_name}: {e}")
                    
            elif ext in ['.cpp', '.cc', '.py', '.java']:
                try:
                    with open(filepath, 'r', encoding='utf-8') as f:
                        code = f.read()
                    
                    safe_filename = display_name.replace("_", "\\_")
                    # 加上一个小标题，标明代码文件名
                    latex_content += f"\\vspace{{0.2em}}\n\\noindent\\textbf{{{safe_filename}}}\n"
                    
                    lang_opt = "language=C++"
                    if ext == ".py": lang_opt = "language=Python"
                    elif ext == ".java": lang_opt = "language=Java"
                    
                    latex_content += f"\\begin{{lstlisting}}[{lang_opt}]\n"
                    latex_content += code
                    latex_content += "\\end{lstlisting}\n"
                    
                    # 每一个代码块结束时，增加物理留白，防止和下一个算法挤在一起
                    latex_content += "\\vspace{1.5em}\n"
                except Exception as e:
                    print(f"Error parsing Code {display_name}: {e}")

    latex_content += r"\end{multicols*} \end{document}"

    with open(OUTPUT_TEX, "w", encoding='utf-8') as f:
        f.write(latex_content)
    
    print("2. Compiling PDF using XeLaTeX...")
    os.chdir(CURRENT_DIR)
    
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