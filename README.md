# DirectUI

基于 Direct3D 11 的 Windows 用户界面库:纯 D3D11 自绘渲染 + retained-mode 控件树,不依赖 Direct2D。
A Direct3D 11-based user interface library for Windows: widgets drawn by a pure, self-built D3D11 pipeline on a retained-mode widget tree, with no Direct2D dependency.

## 路线图 / Roadmap

- [x] 阶段 0 设计(渲染路线、控件模型、边界策略见 `docs/adr/`) / Phase 0 design (rendering path, widget model, boundary policy in `docs/adr/`)
- [ ] 阶段 0:窗口渲染(Win32 窗口 + SwapChain + SDF 图元 + 按需帧循环) / Window rendering (Win32 window + SwapChain + SDF primitives + on-demand frame loop)
- [ ] 阶段 1:文字渲染(DirectWrite,单行 + 字形图集) / Text rendering (DirectWrite, single-line + glyph atlas)
- [ ] 阶段 2+:控件族(按钮、布局、容器,逐步扩展) / Widget family (buttons, layout, containers, growing incrementally)

## 构建 / Build

CMake + Clang 23(llvm-mingw),C++26(`import std;`),构建脚本随阶段 0 落地。
CMake + Clang 23 (llvm-mingw), C++26 (`import std;`). Build scripts land with Phase 0.

## 文档 / Docs

- [GLOSSARY.md](GLOSSARY.md) — 术语表 / Glossary
- [docs/adr/](docs/adr/) — 架构决策记录 / Architecture decision records
