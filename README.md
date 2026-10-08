# IVS · 本地智能视频监控系统

基于 **C++17、Qt Widgets、OpenCV DNN 与 MySQL** 的 Windows 桌面项目，将视频采集、YOLOv8 目标检测、事件录像、截图和历史查询集成在同一应用中。

项目重点是视觉模型与桌面应用的工程集成：分离采集、检测、录像和界面刷新逻辑，利用本地文件保存媒体、数据库保存索引，形成可回看的监控记录。

**[演示视频](doc/demo.mp4) · [用户手册](doc/用户使用手册.docx) · [源码入口](src/IVS/main.cpp) · [构建配置](src/IVS/IVS.pro)**

## 核心功能

| 模块 | 已实现能力 |
| --- | --- |
| 监控画面 | 单画面 / 四画面切换；本机摄像头与本地视频演示源 |
| 目标检测 | 加载 YOLOv8n ONNX 模型，使用 OpenCV DNN 在 CPU 上推理并绘制目标框 |
| 视频记录 | 普通分段录像；开启检测后，根据目标检测结果触发事件录像 |
| 图像留存 | 手动截图；事件录像中自动采集特征截图并关联记录 |
| 历史查询 | 普通与事件录像回看、相关图像查看、操作日志查询 |
| 用户与配置 | 用户注册 / 登录、通道及录像参数设置、本地配置文件读取 |

> 界面中的“移动目标检测”目前基于目标检测结果触发记录，不等同于运动估计、多目标跟踪或异常行为识别。四画面布局也不代表已验证四台独立物理摄像头接入。

## 系统结构

代码采用 `view / service / model / util` 分层，通过 Qt 信号与槽连接界面和后台任务。

| 层级 | 目录 | 主要职责 |
| --- | --- | --- |
| 界面 | [`view/`](src/IVS/view) | 监控、登录、设置、回放、日志页面 |
| 任务与业务 | [`service/`](src/IVS/service) | 视频采集、检测、录像、回放及用户业务 |
| 数据访问 | [`model/`](src/IVS/model) | 用户、通道、视频、图片及日志数据操作 |
| 公共组件 | [`util/`](src/IVS/util) | 数据库连接管理、窗口管理 |
| 模型推理 | [`yolo/`](src/IVS/yolo) | ONNX 加载、预处理、输出解析和绘制 |

### 并发与界面刷新

- **任务拆分**：每个监控通道分别创建采集与检测工作线程；登录后启用录像任务，界面更新留在主线程。
- **检测抽帧**：`DetectTask` 每 10 帧执行一次模型推理，其余帧复用已有检测结果，降低推理调用频率。
- **定时渲染**：缓存最新帧，使用 33ms 定时器拉取并显示，避免每个到达帧都立即触发完整绘制。
- **先缩放再转换**：在主线程渲染阶段，先缩放到控件大小，再完成颜色转换及 `QImage/QPixmap` 构建。
- **连接管理**：`DbConnectionPool` 按线程 ID 管理 MySQL 连接，并执行连接有效性检查。

33ms 是定时器配置值，不是端到端帧率实测。当前不将 CPU 占用、推理延迟或检测准确率作为统一性能结论；比较性能时需固定设备、分辨率、通道数量和采样方法。

### 事件记录

`RecordTask` 包含 `IDLE / NORMAL / EXCEPTION` 状态，按检测开关切换普通录像和事件录像。事件模式下检测到目标后开始记录，并在录像中保存最多 3 张关联截图。MySQL 保存媒体路径、通道、时间和关联信息。

当前录像时长和部分事件计数采用固定 30 帧/秒假设，处理速度变化时，帧计数不能直接视为精确的真实时间。

## 目录导航

```text
IVS/
├── README.md
├── src/IVS/
│   ├── IVS.pro           # qmake 工程配置
│   ├── main.cpp          # 程序入口
│   ├── view/             # Qt Widgets 界面
│   ├── service/          # 后台任务与业务逻辑
│   ├── model/            # 实体与数据访问
│   ├── util/             # 数据库及窗口管理
│   ├── yolo/             # OpenCV DNN 推理封装
│   └── bin/              # 源码构建输出与运行资源
├── bin/                  # 已打包 Windows 程序与依赖
├── records/              # 录像与截图
└── doc/                  # 演示、用户手册、SQL 和 README 截图
```

## 运行环境

| 依赖 | 使用方式 |
| --- | --- |
| Windows x64 | 当前提供的二进制及开发路径面向 Windows |
| C++17 | `IVS.pro` 中声明 |
| Qt 6 | Widgets、SQL、Multimedia；兼容的 MinGW 64-bit Kit |
| OpenCV 4.10 | `.pro` 当前链接 `*4100` 库；包含 DNN 及视频编解码模块 |
| MySQL | 本地元数据存储，Qt 通过 `QMYSQL` 驱动访问 |
| ONNX | 默认模型 `data/model/yolov8n.onnx` |

Qt、OpenCV 和编译器的架构与 ABI 必须一致，不能直接混用 MSVC 与 MinGW 构建的库。模型默认使用 OpenCV 的 CPU 后端，不依赖 CUDA。

## 快速开始

### 1. 获取项目

```bash
git clone https://github.com/newbee666277/IVS.git
cd IVS
```

### 2. 初始化数据库

检查并执行 [`doc/sql.sql`](doc/sql.sql)。**脚本开头包含 `DROP DATABASE IF EXISTS monitor_db`，会删除同名数据库。请仅在独立演示环境执行，已有数据时先备份或调整脚本。**

脚本创建用户、通道、事件、图片、普通视频、操作日志和系统配置表。初始化后，通过设置界面配置通道，确保录像记录引用的通道已创建。

### 3. 配置连接

程序从**当前工作目录**读取 `db_config.ini`。`DbConnectionPool` 读取根级键，`DbConn` 读取 `[Database]` 组。为兼容现有代码，两个位置填写相同信息：

```ini
hostname=127.0.0.1
dbname=monitor_db
username=YOUR_DB_USER
password=YOUR_DB_PASSWORD
port=3306

[Database]
hostname=127.0.0.1
dbname=monitor_db
username=YOUR_DB_USER
password=YOUR_DB_PASSWORD
port=3306
```

示例账户需替换为本机信息。数据库账户与应用登录账户不是同一概念。

### 4. 使用打包程序

在仓库根目录的 PowerShell 中运行：

```powershell
Set-Location bin
.\IVS.exe
```

保留 `bin` 内的 DLL、Qt 插件、模型和演示视频。首次启动先完成设置，再进入监控界面；登录后才会创建录像任务。

### 5. 从源码构建

1. 用 Qt Creator 打开 `src/IVS/IVS.pro`，选择兼容的 Qt 6 / MinGW 64-bit Kit。
2. 将 `.pro` 中的 `OPENCV_ROOT` 改为本机路径，核对头文件、库目录与链接库名称。
3. 执行 qmake 并构建，可执行文件输出到 `src/IVS/bin`。
4. 将运行工作目录设为 `src/IVS/bin`，准备 INI、模型、演示视频及 Qt/MySQL 运行依赖。
5. 确保相对于运行目录的 `../records` 下存在 `normal_records/videos`、`normal_records/features`、`exception_records/videos`、`exception_records/features`，并可写入。

程序存在相对路径依赖，从其他目录启动可能导致配置或媒体找不到。打包版与源码版的运行目录不要混用。

## 模型与视频源

- 默认模型输入设置为 **320 × 320**；替换 ONNX 时需核对输入形状与输出解析格式。
- 演示源使用 `data/video/pg1.mp4`、`pg2.mp4`、`pg3.mp4`。
- 本机摄像头通过 `VideoCapture(0)` 打开，尚未实现任意摄像头 ID 映射或 RTSP 接入。
- 录像使用 H.264 编码，写入能力取决于 OpenCV/FFmpeg 构建与运行依赖。

## 常见问题

| 现象 | 优先检查 |
| --- | --- |
| `QMYSQL driver not loaded` | Qt SQL 插件、MySQL 客户端 DLL 及版本/架构 |
| 数据库连接失败 | 服务、账户、端口；INI 根级键和 `[Database]` 是否一致 |
| 模型加载失败 | 工作目录、模型路径及模型形状 |
| 黑屏 | 摄像头占用、设备权限或演示视频路径 |
| 可以预览但没有录像 | 登录状态、目录写权限、H.264 编码器、数据库通道配置 |
| 多通道延迟 | CPU 负载、分辨率、推理耗时；定时器不保证实际吞吐 |

