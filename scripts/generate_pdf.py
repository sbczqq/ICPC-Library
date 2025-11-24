import os
import subprocess

# 路径配置
CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
CONTENT_DIR = os.path.join(CURRENT_DIR, "..", "content")
OUTPUT_TEX = os.path.join(CURRENT_DIR, "notebook.tex")

def generate_latex():
    print("1. Scanning for code files...")
    
    # LaTeX 头部：使用 listings 替代 minted
    # 定义了 C++ 的高亮样式：蓝色关键字，绿色注释，自动换行
    latex_content = r"""
    \documentclass[10pt, landscape, a4paper]{article}
    \usepackage{multicol}
    \usepackage{listings} 
    \usepackage{xcolor}
    \usepackage{geometry}
    \usepackage{fancyhdr}
    \usepackage[utf8]{inputenc} 
    \usepackage[T1]{fontenc}
    
    % 页面边距设置
    \geometry{top=1cm, bottom=1cm, left=0.5cm, right=0.5cm} 
    \setlength{\columnsep}{0.5cm}
    
    % 页眉设置
    \pagestyle{fancy}
    \fancyhf{}
    \lhead{\textbf{ICPC Template Library}}
    \rhead{\thepage}
    
    % 代码高亮配置 (Listings)
    \lstset{
        language=C++,
        basicstyle=\footnotesize\ttfamily, % 字体大小
        keywordstyle=\bfseries\color{blue}, % 关键字颜色
        commentstyle=\itshape\color{teal},  % 注释颜色
        stringstyle=\color{red},            % 字符串颜色
        numbers=left,                       % 行号在左侧
        numberstyle=\tiny\color{gray},      % 行号样式
        stepnumber=1,                       % 每行都显示行号
        breaklines=true,                    % 自动换行
        frame=lines,                        % 代码框上下加线
        tabsize=2,                          % Tab 宽度
        showstringspaces=false,             % 不显示字符串中的空格符号
        breakatwhitespace=false,            
        escapeinside={(*@}{@*)}             % 允许在代码中写 LaTeX
    }

    \begin{document}
    \begin{multicols*}{3} 
    \tableofcontents
    """

    if not os.path.exists(CONTENT_DIR):
        print(f"Error: Content directory not found at {CONTENT_DIR}")
        return

    file_count = 0
    # 遍历 content 目录
    for dirpath, dirnames, filenames in os.walk(CONTENT_DIR):
        section_name = os.path.basename(dirpath)
        # 跳过隐藏文件夹和 content 根目录
        if section_name == "content" or section_name.startswith("."): 
            continue
        
        if not filenames: continue 

        latex_content += f"\\section*{{{section_name.capitalize()}}}\n"
        
        for filename in sorted(filenames):
            if filename.endswith((".cpp", ".cc", ".py", ".java", ".sh")):
                file_count += 1
                filepath = os.path.join(dirpath, filename)
                
                # 读取文件内容
                try:
                    with open(filepath, 'r', encoding='utf-8') as f:
                        code = f.read()
                except Exception as e:
                    print(f"Skipping {filename}: {e}")
                    continue
                
                # 处理文件名显示
                safe_filename = filename.replace("_", "\\_")
                
                # 根据扩展名调整语言 (虽然默认是 C++，但对 Python 也能做基本高亮)
                lang_opt = "language=C++"
                if filename.endswith(".py"): lang_opt = "language=Python"
                elif filename.endswith(".java"): lang_opt = "language=Java"
                
                latex_content += f"\\subsection*{{{safe_filename}}}\n"
                # 写入代码块
                latex_content += f"\\begin{{lstlisting}}[{lang_opt}]\n"
                latex_content += code
                latex_content += "\\end{lstlisting}\n"

    latex_content += r"\end{multicols*} \end{document}"

    # 写入 tex 文件
    with open(OUTPUT_TEX, "w", encoding='utf-8') as f:
        f.write(latex_content)
    
    print(f"2. LaTeX generated ({file_count} files). Compiling PDF...")

    # 切换工作目录，避免路径问题
    os.chdir(CURRENT_DIR)
    
    # 编译命令：去掉了 -shell-escape，因为 listings 不需要它
    cmd = ["pdflatex", "-interaction=nonstopmode", "notebook.tex"]
    
    try:
        # 运行两次以生成目录
        subprocess.run(cmd, check=True)
        subprocess.run(cmd, check=True)
        print("\n[SUCCESS] 任务完成！PDF 已生成：")
        print(os.path.join(CURRENT_DIR, "notebook.pdf"))
    except subprocess.CalledProcessError:
        print("\n[ERROR] 编译失败。")
        print("请查看 scripts 目录下的 notebook.log 获取详细信息。")

if __name__ == "__main__":
    generate_latex()