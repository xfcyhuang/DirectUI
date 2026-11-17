module;

#define UNICODE 1
#define _UNICODE 1
#include <windows.h>

module DirectUI.platform;

import std;

namespace DirectUI {

auto translate_message(std::uint32_t uint4_msg, std::uintptr_t uint8_wparam, std::intptr_t int8_lparam) -> std::optional<EventView> {
  if (uint4_msg == WM_CLOSE) {
    return EventView{.kind = EventKind::kClose, .uint4_width = 0, .uint4_height = 0, .b_focused = false};
  }
  if (uint4_msg == WM_SIZE) {
    auto const uint4_width = static_cast<std::uint32_t>(LOWORD(int8_lparam));
    auto const uint4_height = static_cast<std::uint32_t>(HIWORD(int8_lparam));
    return EventView{.kind = EventKind::kResize, .uint4_width = uint4_width, .uint4_height = uint4_height, .b_focused = false};
  }
  if (uint4_msg == WM_ACTIVATE) {
    auto const b_focused = static_cast<std::uint16_t>(uint8_wparam) != WA_INACTIVE;
    return EventView{.kind = EventKind::kFocus, .uint4_width = 0, .uint4_height = 0, .b_focused = b_focused};
  }
  return std::nullopt;
}

}
