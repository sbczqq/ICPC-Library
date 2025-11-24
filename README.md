# 🧊 ICPC Algorithm Library (Team Reference Document)

> 一套基于 **VS Code + Git + LaTeX** 构建的现代化、工程化 ICPC/ACM 算法模板库。

## 📖 项目简介

本项目旨在解决 competitive programming (CP) 训练中的代码管理与文档生成问题。它采用 **“文件即数据库”** 的理念：
* **日常训练**：算法以独立 `.cpp` 文件分类存储，利用 VS Code 插件实现侧边栏可视化检索，点击即用。
* **版本控制**：基于 Git 管理，完美支持 Windows/Linux 跨平台同步。
* **赛场输出**：内置自动化 Python 脚本，一键将代码库编译为符合 ICPC 标准的**三栏排版 PDF**（无需安装 Pygments 等复杂依赖）。

## 📂 目录结构

```text
ICPC-Library/
├── content/               # [核心] 算法代码库
│   ├── data-structures/   # 数据结构 (如: SegmentTree, DSU)
│   ├── graph/             # 图论 (如: Dijkstra, MaxFlow)
│   ├── math/              # 数学 (如: FFT, Geometry)
│   ├── string/            # 字符串 (如: KMP, SAM)
│   └── ...
├── scripts/               # [工具] 构建脚本
│   ├── generate_pdf.py    # PDF 生成器 (Python)
│   ├── notebook.tex       # LaTeX 中间文件 (自动生成)
│   └── notebook.pdf       # 最终生成的电子书 (自动生成)
├── .gitattributes         # [配置] 强制 LF 换行符 (跨平台关键)
├── .gitignore             # [配置] 忽略临时文件
└── README.md              # 项目说明
