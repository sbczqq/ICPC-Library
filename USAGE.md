

### 2. 详细使用手册 (USAGE.md)

**文件名：** `USAGE.md`
**存放位置：** `ICPC-Library` 根目录（或者你可以打印出来给队友看）

```markdown
# 📘 ICPC 模板库使用指南

本手册详细说明了如何维护代码库、进行多端同步以及生成比赛用书。

## 1. 日常维护流程 (Adding Code)

不要将代码保存在零散的地方，所有经过验证的算法模板都应归档到 `content/` 目录。

### 命名规范
* **文件名**：使用全小写 + 下划线，例如 `segment_tree.cpp`, `min_cost_max_flow.cpp`。
* **文件内容**：
    * 代码应包含必要的头文件。
    * **不仅是函数**：建议包含一个简短的 `main` 函数示例或结构体封装，确保代码不仅能看，还能直接通过编译。
    * **注释**：在文件顶部用注释写明时间复杂度、空间复杂度或特殊注意事项。

### 目录分类
如果这是一个新领域的算法（例如“计算几何”），请在 `content` 下新建文件夹 `geometry`，然后将代码放入其中。脚本会自动识别新文件夹并将其作为 PDF 的一级标题。

---

## 2. 跨设备同步 (Win <-> Linux)

由于我们配置了 `.gitattributes`，你不需要担心 Windows (CRLF) 和 Linux (LF) 的换行符冲突问题。

### 在 Windows 上写代码后：
1.  打开 VS Code 终端。
2.  提交更改：
    ```bash
    git add .
    git commit -m "Add: 增加了网络流 Dinic 算法模板"
    git push
    ```

### 在 Linux 设备上开始训练前：
1.  打开终端，进入项目目录。
2.  拉取最新代码：
    ```bash
    git pull
    ```
    *此时，你的 Linux 本地文件已经是最新的，且换行符已被自动转换为 Linux 标准的 LF。*

---

## 3. 生成比赛 PDF (Generating PDF)

在比赛前或需要打印时，执行生成操作。

1.  **进入脚本目录**：
    ```bash
    cd scripts
    ```
2.  **运行脚本**：
    ```bash
    python generate_pdf.py
    ```
3.  **查收结果**：
    脚本运行完毕后，在当前目录下打开 `notebook.pdf`。
    * **检查点**：确认目录是否生成？确认长代码是否正确换行？确认中文注释是否乱码？

---

## 4. 比赛/训练时的使用技巧 (VS Code)

为了达到“点击即用”的最快速度，推荐在 VS Code 中这样操作：

1.  安装插件 **Snippet Explorer**。
2.  在插件设置中，将 `ICPC-Library/content` 文件夹添加为代码片段源。
3.  **实战操作**：
    * 当你在写题目代码 `A.cpp` 时，发现需要用到“并查集”。
    * 点击左侧侧边栏的小图标（Snippet Explorer）。
    * 展开 `data-structures` 文件夹。
    * 点击 `union_find.cpp`。
    * 代码会自动插入到你当前的光标位置。

---

## ❓ 常见问题 (FAQ)

**Q: 生成 PDF 时报错 `LaTeX Error: File 'listings.sty' not found`?**
A: 你的 LaTeX 发行版缺少宏包。如果是 MiKTeX，请确保在设置中开启了 "Install missing packages on-the-fly"。如果是 Linux，请运行 `sudo apt install texlive-latex-recommended`。

**Q: 生成的 PDF 中文是乱码？**
A: 确保你的 `.cpp` 文件是 **UTF-8** 编码（VS Code 默认即为 UTF-8）。

**Q: 如何忽略编译产生的 `.exe` 文件？**
A: 确保根目录下的 `.gitignore` 文件包含 `*.exe`, `*.o`, `*.log` 等规则。