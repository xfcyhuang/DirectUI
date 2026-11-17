#define UNICODE 1
#define _UNICODE 1
#define NOMINMAX 1
#include <windows.h>

import std;
import DirectUI.core;
import DirectUI.platform;

namespace {

auto hue_rgb(double fp8_hue) -> std::array<float, 3> {
  auto const fp8_wheel = fp8_hue * 6.0;
  auto const uint4_sector = static_cast<std::uint32_t>(fp8_wheel);
  auto const fp8_frac = fp8_wheel - static_cast<double>(uint4_sector);
  auto const fp8_q = 1.0 - fp8_frac;
  switch (uint4_sector) {
  case 0: return {1.0F, static_cast<float>(fp8_frac), 0.0F};
  case 1: return {static_cast<float>(fp8_q), 1.0F, 0.0F};
  case 2: return {0.0F, 1.0F, static_cast<float>(fp8_frac)};
  case 3: return {0.0F, static_cast<float>(fp8_q), 1.0F};
  case 4: return {static_cast<float>(fp8_frac), 0.0F, 1.0F};
  default: return {1.0F, 0.0F, static_cast<float>(fp8_q)};
  }
}

auto print_event(DirectUI::UiEvent const& ev) -> void {
  std::visit([](auto const& evx) {
    using T = std::remove_cvref_t<decltype(evx)>;
    if constexpr (std::is_same_v<T, DirectUI::CloseEvent>) {
      std::println("event: close");
    } else if constexpr (std::is_same_v<T, DirectUI::ResizeEvent>) {
      std::println("event: resize {}x{}", evx.uint4_width, evx.uint4_height);
    } else if constexpr (std::is_same_v<T, DirectUI::FocusEvent>) {
      std::println("event: focus {}", evx.b_focused);
    } else if constexpr (std::is_same_v<T, DirectUI::PointerEvent>) {
      std::println("event: pointer {} ({}, {})", static_cast<std::uint32_t>(evx.phase), evx.int4_x, evx.int4_y);
    } else {
      std::println("event: key {} {}", static_cast<std::uint32_t>(evx.phase), static_cast<std::uint32_t>(evx.key));
    }
  }, ev);
}

auto print_error(std::string_view str_op, DirectUI::Error const& err) -> void {
  std::println("{} failed: {} (0x{:08X})", str_op, err.str_msg, err.uint4_code);
}

auto CALLBACK wnd_proc(HWND hwnd, UINT uint4_msg, WPARAM uint8_wparam, LPARAM int8_lparam) -> LRESULT {
  auto const opt_ev = DirectUI::translate_message(static_cast<std::uint32_t>(uint4_msg), static_cast<std::uintptr_t>(uint8_wparam), static_cast<std::intptr_t>(int8_lparam));
  if (opt_ev.has_value()) {
    print_event(*opt_ev);
  }
  if (uint4_msg == WM_CLOSE) {
    DestroyWindow(hwnd);
    return 0;
  }
  if (uint4_msg == WM_DESTROY) {
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProcW(hwnd, uint4_msg, uint8_wparam, int8_lparam);
}

}

auto main() -> int {
  auto const h_instance = GetModuleHandleW(nullptr);
  auto wc = WNDCLASSEXW{};
  wc.cbSize = sizeof(WNDCLASSEXW);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = wnd_proc;
  wc.hInstance = h_instance;
  wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
  wc.lpszClassName = L"DirectUIDemoWindow";
  if (RegisterClassExW(&wc) == 0) {
    std::println("RegisterClassExW failed (0x{:08X})", static_cast<std::uint32_t>(GetLastError()));
    return 1;
  }
  auto rc = RECT{0, 0, 800, 600};
  AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
  auto const hwnd = CreateWindowExW(0, L"DirectUIDemoWindow", L"DirectUI demo", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, h_instance, nullptr);
  if (hwnd == nullptr) {
    std::println("CreateWindowExW failed (0x{:08X})", static_cast<std::uint32_t>(GetLastError()));
    return 1;
  }
  ShowWindow(hwnd, SW_SHOW);
  UpdateWindow(hwnd);

  auto const exp_device = DirectUI::create_device();
  if (!exp_device) {
    print_error("create_device", exp_device.error());
    return 1;
  }
  auto device = *exp_device;

  auto const exp_swap = DirectUI::create_swap_chain(device, reinterpret_cast<std::uintptr_t>(hwnd));
  if (!exp_swap) {
    print_error("create_swap_chain", exp_swap.error());
    return 1;
  }
  auto swap_chain = *exp_swap;

  auto const time_start = std::chrono::steady_clock::now();
  auto msg = MSG{};
  auto b_running = true;
  while (b_running) {
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) {
        b_running = false;
        break;
      }
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
    }
    if (!b_running) {
      break;
    }
    auto const fp8_ms = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - time_start).count());
    auto const fp8_hue = std::fmod(fp8_ms / 6000.0, 1.0);
    auto const res_clear = DirectUI::render_clear_color(swap_chain, hue_rgb(fp8_hue));
    if (!res_clear) {
      print_error("render_clear_color", res_clear.error());
      b_running = false;
    }
  }

  DirectUI::destroy_swap_chain(swap_chain);
  DirectUI::destroy_device(device);
  UnregisterClassW(L"DirectUIDemoWindow", h_instance);
  return 0;
}
