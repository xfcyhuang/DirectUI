# DirectUI

基于 D3D11 的 Windows 用户界面库:以纯 Direct3D 11 自绘渲染树形控件,先窗口呈现,后逐步扩展控件族。

## 呈现(Presentation)

**Window**:
库管理的最小独立呈现单元,一个 Window 对应一个操作系统顶层窗口与其独占的呈现目标。
_Avoid_: Form、逻辑窗口、面板

**SwapChain**:
Window 的呈现目标,持有待上屏的帧并决定其上屏时机。
_Avoid_: 后台缓冲(back buffer 是其组成部件,不是同义词)

**Frame**:
一次完整的「收集脏区 → 重绘 → 呈现」的产物。
_Avoid_: 刷新、tick

**Frame Loop**:
按需产生帧的调度机制:静止时无帧,动画或交互期间临时连续。
_Avoid_: 游戏循环、消息循环(后者是操作系统概念)

**Dirty Region**:
Window 上需要重绘的区域;无脏区即无帧。
_Avoid_: invalidate rect、裁剪区

## 绘制(Drawing)

**Primitive**:
构成一切可视输出的最小视觉单元,如圆角矩形、渐变、贴图、字形串。
_Avoid_: 形状、draw call

**GlyphRun**:
排版完成后位置与字形均已确定的一段文字,是文字进入渲染管线的形态(由 DirectWrite 产出)。
_Avoid_: 文本、字符串(那是用户数据,不是绘制单元)

**Effect**:
叠加在图元或控件之上的视觉增强,如模糊、透明、阴影。当前阶段仅保留参数位,不实现。
_Avoid_: 滤镜、特效(口语)

## 控件(Widgets)

**Widget**:
控件树上持久的节点:拥有状态、身份与事件路由资格,其外观由图元组成。
_Avoid_: Control、Element、组件
