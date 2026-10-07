# 📘 ICPC 模板库使用指南

本手册详细说明了如何维护代码库、编写算法文档、进行多端同步以及生成比赛用书。

## 1. 日常维护流程 (Adding Content)

不要将代码保存在零散的地方，所有经过验证的算法模板都应归档到 `content/` 目录。我们现在支持 **“文档 + 代码”** 的混排模式。

### 目录结构与命名
如果这是一个新领域的算法（例如“快速傅里叶变换”），建议在 `content` 下新建分类文件夹 `math`，然后放入以下文件：

* **文档 (.md)**：例如 `fft_doc.md`。
    * 用于记录算法原理、公式推导（支持 LaTeX 格式，如 $O(N \log N)$）和注意事项。
    * 生成 PDF 时，文档会自动排版在代码之前。
* **代码 (.cpp)**：例如 `fft.cpp`。
    * **文件名**：使用全小写 + 下划线。
    * **内容**：应包含完整的头文件和结构体封装，确保代码可以直接通过编译。
    * **编码**：务必使用 **UTF-8** 编码（VS Code 默认设置），防止中文乱码。

---

## 2. 跨设备同步 (Win <-> Linux)

由于我们配置了 `.gitattributes`，你不需要担心 Windows (CRLF) 和 Linux (LF) 的换行符冲突问题。

仓库的主分支是 `master`。Windows 和 Linux 都可以使用以下两种流程更新主分支。

### 方式 A：将 main 的更新合并到 master

在 `main` 分支上写完代码或文档后，先提交并推送到 `main`：

```bash
git add .
git commit -m "Add: 增加了 FFT 算法及其原理解析"
git push origin main
```

将远端 `main` 的更新合并到 `master`，再推送主分支：

```bash
git fetch origin
git switch master
git pull origin master
git merge origin/main
git push origin master
```

### 方式 B：直接在 master 提交

开始修改前，切换到 `master` 并拉取最新代码：

```bash
git switch master
git pull origin master
```

写完代码或文档后，直接提交并推送到 `master`：

```bash
git add .
git commit -m "Add: 增加了 FFT 算法及其原理解析"
git push origin master
```

### 在 Linux 设备上开始训练前：
1.  打开终端，进入项目目录。
2.  拉取最新代码：
    ```bash
    git switch master
    git pull origin master
    ```
    *此时，你的 Linux 本地文件已经是最新的，且换行符已被自动转换为 Linux 标准的 LF。*

---

## 3. 生成比赛 PDF (Generating PDF)

在比赛前或需要打印时，执行生成操作。脚本会自动扫描 `.md` 和 `.cpp` 文件并生成三栏排版文档。

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
    * **检查点**：确认 Markdown 里的公式显示是否正确？目录是否生成？中文注释是否正常？

---

## 4. 比赛/训练时的使用技巧 (VS Code)

为了达到“点击即用”的最快速度，推荐两种方式：

### 方式 A：多根工作区 (推荐)
将算法库直接挂载到你的比赛工程中。
1.  打开你平时刷题的文件夹（如 `Codeforces`）。
2.  点击菜单栏 **File -> Add Folder to Workspace...**，选择 `ICPC-Library` 文件夹。
3.  保存工作区。
4.  **使用**：左侧资源管理器会直接显示算法库，展开 -> 复制 -> 粘贴。

### 方式 B：Snippet Explorer 插件
1.  安装插件 **Snippet Explorer**。
2.  在插件设置中，将 `ICPC-Library/content` 文件夹添加为代码片段源。
3.  **使用**：点击左侧侧边栏的小图标，点击算法文件，代码自动插入光标处。

---

## ❓ 常见问题 (FAQ)

**Q: `git push` 或 `git pull` 时报错 `schannel: failed to receive handshake, SSL/TLS connection failed` 怎么办？**
A: 这是 Windows 下 Git 默认加密模块与系统代理软件冲突导致的。可以通过以下三种方法解决：
* **方法一：直接关闭 SSL 验证（最简单粗暴，推荐用于个人算法库）**
  在终端运行：`git config --global http.sslVerify false`
* **方法二：切换 Git 的加密模块为 OpenSSL**
  在终端运行：`git config --global http.sslBackend openssl`
* **方法三：为 Git 显式配置本地代理端口（假设你的代理端口为 7890）**
  在终端运行：
  `git config --global http.proxy http://127.0.0.1:7890`
  `git config --global https.proxy http://127.0.0.1:7890`

**Q: 生成 PDF 时报错 `LaTeX Error: File 'listings.sty' not found`?**
A: 你的 LaTeX 发行版缺少宏包。
* **Windows (MiKTeX)**: 打开 MiKTeX Console，设置 "Always install missing packages on-the-fly"。
* **Linux**: 运行 `sudo apt install texlive-latex-recommended texlive-latex-extra`。

**Q: Markdown 里的图片能显示在 PDF 里吗？**
A: 目前脚本主要处理文本和公式。尽量使用文字和 LaTeX 公式描述算法，如果必须插图，需手动修改 `.tex` 文件。

**Q: 如何忽略编译产生的 `.exe` 文件？**
A: 确保根目录下的 `.gitignore` 文件包含 `*.exe`, `*.o`, `*.log` 等规则。
