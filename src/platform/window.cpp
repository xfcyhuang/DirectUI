module;

#define UNICODE 1
#define _UNICODE 1
#include <windows.h>

module DirectUI.platform;

import std;

namespace DirectUI {

namespace {

struct WindowImpl {
  HWND hwnd{};
  EventReceiver fn_on_event{};
  std::wstring wstr_class{};
};

auto impl_registry() -> std::deque<WindowImpl>& {
  static std::deque<WindowImpl> deq_reg;
  return deq_reg;
}

auto next_id() -> std::uint64_t& {
  static std::uint64_t uint8_next{0};
  return uint8_next;
}

auto utf8_to_wide(std::string_view str_src) -> std::wstring {
  auto const int4_len = MultiByteToWideChar(CP_UTF8, 0, str_src.data(), static_cast<int>(str_src.size()), nullptr, 0);
  auto wstr_out = std::wstring(static_cast<std::size_t>(int4_len), L'\0');
  MultiByteToWideChar(CP_UTF8, 0, str_src.data(), static_cast<int>(str_src.size()), wstr_out.data(), int4_len);
  return wstr_out;
}

auto class_name_from_id(std::uint64_t uint8_id) -> std::wstring {
  auto buf_id = std::array<char, 32>{};
  auto const res_chars = std::to_chars(buf_id.data(), buf_id.data() + buf_id.size(), uint8_id);
  auto const str_narrow = std::string_view{buf_id.data(), static_cast<std::size_t>(res_chars.ptr - buf_id.data())};
  return std::wstring{L"DirectUIWindow_"} + utf8_to_wide(str_narrow);
}

auto CALLBACK wnd_proc(HWND hwnd, UINT uint4_msg, WPARAM uint8_wparam, LPARAM int8_lparam) -> LRESULT {
  if (uint4_msg == WM_NCCREATE) {
    auto const* ptr_cs = reinterpret_cast<CREATESTRUCTW*>(int8_lparam);
    SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(ptr_cs->lpCreateParams));
    return DefWindowProcW(hwnd, uint4_msg, uint8_wparam, int8_lparam);
  }
  auto* ptr_impl = reinterpret_cast<WindowImpl*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
  if (ptr_impl != nullptr && ptr_impl->fn_on_event) {
    auto const opt_ev = translate_message(static_cast<std::uint32_t>(uint4_msg), static_cast<std::uintptr_t>(uint8_wparam), static_cast<std::intptr_t>(int8_lparam));
    if (opt_ev.has_value()) {
      ptr_impl->fn_on_event(*opt_ev);
    }
  }
  if (uint4_msg == WM_CLOSE) {
    DestroyWindow(hwnd);
    return 0;
  }
  if (uint4_msg == WM_DESTROY) {
    if (ptr_impl != nullptr) {
      ptr_impl->hwnd = nullptr;
    }
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProcW(hwnd, uint4_msg, uint8_wparam, int8_lparam);
}

}

auto create_window(WindowDesc const& desc, EventReceiver fn_on_event) -> std::expected<Window, Error> {
  auto const uint8_id = next_id()++;
  auto const wstr_class = class_name_from_id(uint8_id);
  auto const h_instance = GetModuleHandleW(nullptr);
  auto wc = WNDCLASSEXW{};
  wc.cbSize = sizeof(WNDCLASSEXW);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = wnd_proc;
  wc.hInstance = h_instance;
  wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
  wc.lpszClassName = wstr_class.c_str();
  if (RegisterClassExW(&wc) == 0) {
    return std::unexpected{Error{static_cast<std::uint32_t>(GetLastError()), "RegisterClassExW"}};
  }
  auto rc = RECT{0, 0, static_cast<LONG>(desc.uint4_width), static_cast<LONG>(desc.uint4_height)};
  AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
  auto const wstr_title = utf8_to_wide(desc.str_title);
  auto& impl = impl_registry().emplace_back(WindowImpl{nullptr, std::move(fn_on_event), wstr_class});
  auto const hwnd = CreateWindowExW(0, wstr_class.c_str(), wstr_title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, h_instance, reinterpret_cast<LPVOID>(&impl));
  if (hwnd == nullptr) {
    auto const uint4_err = static_cast<std::uint32_t>(GetLastError());
    UnregisterClassW(wstr_class.c_str(), h_instance);
    impl_registry().pop_back();
    return std::unexpected{Error{uint4_err, "CreateWindowExW"}};
  }
  impl.hwnd = hwnd;
  ShowWindow(hwnd, SW_SHOW);
  UpdateWindow(hwnd);
  return Window{uint8_id};
}

auto run_message_loop() -> void {
  auto msg = MSG{};
  while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }
}

auto destroy_window(Window& win) -> void {
  auto& deq = impl_registry();
  if (win.uint8_id >= deq.size()) {
    return;
  }
  auto& impl = deq[win.uint8_id];
  if (impl.hwnd != nullptr) {
    DestroyWindow(impl.hwnd);
    impl.hwnd = nullptr;
  }
  UnregisterClassW(impl.wstr_class.c_str(), GetModuleHandleW(nullptr));
  impl.fn_on_event = nullptr;
}

}
