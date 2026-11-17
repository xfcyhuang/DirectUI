# 公开 API 零 COM 泄漏,COM 头收口于 DirectUI.platform

D3D11/DXGI/DWrite 皆为 COM 接口,但库的公开模块(DirectUI.render、DirectUI.text 等)签名中一律不出现任何 COM 类型:对外只暴露自有 RAII 聚合 struct、值类型与自由函数;原始 `ID3D11Device*` 等仅经 `native_handle()` 逃生舱函数获取。全部 COM/Win32 头收口在边界垫层模块 `DirectUI.platform` 内,与代码规范「第三方头不穿越 import 边界」条款一致。

## Consequences

- HRESULT 语义在垫层内一次性翻译为 `std::expected<T, Error>`,全库错误处理风格统一。
- 未来更换后端(如 D3D12)时,公开 API 面不变,垫层以下整体替换。
