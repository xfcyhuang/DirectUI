# 库不做窗口宿主:窗口与消息循环归应用,库为核心控件库

架构分两层:`DirectUI.core`(控件树、自有事件结构、布局、绘制记录)零 windows.h 依赖,纯 C++26、可移植可单测;`DirectUI.platform` 垫层保留 Win32/D3D11(HWND 以 `uintptr_t` 接入、SwapChain、管线执行)。窗口创建、消息循环、WndProc 完全归应用(复用 Win32API);应用在自己的 WndProc 中调用 core 的纯整数签名 `translate_message` 得到库事件(`UiEvent` variant:Close/Resize/Focus/Pointer/Key,全部自有类型)。库不子类化窗口、不装消息钩子。Win32 消息值与虚拟键码作为 ABI 协议常量以字面量定义在 core,垫层以 `static_assert` 对拍一致性。

## Considered Options

- **整窗方案**(库 `create_window` + 阻塞消息循环,票 1 实现):被否——库侵入应用消息回路,控件层被 windows.h 污染,移植性归零。
- **全库零 Win32**(渲染器也归应用):被否——每个应用重复实现 D3D11 管线,ADR-0001(纯 D3D11 自绘)失去落点。

## Consequences

- core 理论上可跨平台(替换渲染垫层即可),控件层单测无需任何窗口。
- 消息值/键码漂移风险由垫层 `static_assert` 编译期拦截。
- demo 升格为「应用样本」,承担 Win32 宿主职责(RegisterClass/CreateWindow/WndProc/消息泵)。
- 原 `Window`/`create_window`/`pump_events` 等库 API 移除,等价逻辑并入 demo。
