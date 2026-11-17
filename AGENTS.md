# DirectUI

本仓库为独立 git 仓库(issue 跟踪于 GitHub)。本文件优先于工作区级 AGENTS.md。

## 语言规范

始终使用中文思考和回复,包括子代理:向子代理派发任务时,prompt 必须用中文书写,并明确要求子代理以中文思考和回复;所有面向用户的输出(issue 正文、评论、文档)一律使用中文。

## 代码规范

本工程 C++ 代码规范以 [`../cpp26.md`](../cpp26.md) 为唯一来源,写、改、审、重构任何 C++ 代码前必读并遵循;本文件不摘录、不复制其内容。

## 宿主窗口约定

demo 与一切测试/示例程序的基础窗口一律直接使用 Win32API(`RegisterClassExW`/`CreateWindowExW`/自建 `WndProc`/`PeekMessage` 泵),不引入任何窗口抽象或第三方窗口库;库本体(DirectUI.core/DirectUI.platform)不创建窗口(ADR-0004)。

## Agent skills

### Issue tracker

Issues 跟踪于 GitHub Issues(xfcyhuang/DirectUI,通过 gh CLI)。见 `docs/agents/issue-tracker.md`。

### Triage labels

使用默认五标签:needs-triage / needs-info / ready-for-agent / ready-for-human / wontfix。见 `docs/agents/triage-labels.md`。

### Domain docs

Single-context 布局:根目录 `GLOSSARY.md` + `docs/adr/`。见 `docs/agents/domain.md`。
