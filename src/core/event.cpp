module DirectUI.core;

import std;

namespace DirectUI {

namespace {

auto signed_loword(std::intptr_t int8_value) -> std::int32_t {
  return static_cast<std::int32_t>(static_cast<std::int16_t>(static_cast<std::uint64_t>(int8_value) & 0xFFFF));
}

auto signed_hiword(std::intptr_t int8_value) -> std::int32_t {
  return static_cast<std::int32_t>(static_cast<std::int16_t>((static_cast<std::uint64_t>(int8_value) >> 16) & 0xFFFF));
}

auto unsigned_loword(std::intptr_t int8_value) -> std::uint32_t {
  return static_cast<std::uint32_t>(static_cast<std::uint64_t>(int8_value) & 0xFFFF);
}

auto unsigned_hiword(std::intptr_t int8_value) -> std::uint32_t {
  return static_cast<std::uint32_t>((static_cast<std::uint64_t>(int8_value) >> 16) & 0xFFFF);
}

}

auto translate_key(std::uint32_t uint4_vk) -> std::optional<KeyCode> {
  if ((uint4_vk >= 0x41 && uint4_vk <= 0x5A) || (uint4_vk >= 0x30 && uint4_vk <= 0x39)) {
    return static_cast<KeyCode>(uint4_vk);
  }
  switch (uint4_vk) {
  case static_cast<std::uint32_t>(KeyCode::kBackspace): return KeyCode::kBackspace;
  case static_cast<std::uint32_t>(KeyCode::kTab): return KeyCode::kTab;
  case static_cast<std::uint32_t>(KeyCode::kReturn): return KeyCode::kReturn;
  case static_cast<std::uint32_t>(KeyCode::kShift): return KeyCode::kShift;
  case static_cast<std::uint32_t>(KeyCode::kControl): return KeyCode::kControl;
  case static_cast<std::uint32_t>(KeyCode::kAlt): return KeyCode::kAlt;
  case static_cast<std::uint32_t>(KeyCode::kEscape): return KeyCode::kEscape;
  case static_cast<std::uint32_t>(KeyCode::kSpace): return KeyCode::kSpace;
  case static_cast<std::uint32_t>(KeyCode::kPageUp): return KeyCode::kPageUp;
  case static_cast<std::uint32_t>(KeyCode::kPageDown): return KeyCode::kPageDown;
  case static_cast<std::uint32_t>(KeyCode::kEnd): return KeyCode::kEnd;
  case static_cast<std::uint32_t>(KeyCode::kHome): return KeyCode::kHome;
  case static_cast<std::uint32_t>(KeyCode::kLeft): return KeyCode::kLeft;
  case static_cast<std::uint32_t>(KeyCode::kUp): return KeyCode::kUp;
  case static_cast<std::uint32_t>(KeyCode::kRight): return KeyCode::kRight;
  case static_cast<std::uint32_t>(KeyCode::kDown): return KeyCode::kDown;
  case static_cast<std::uint32_t>(KeyCode::kInsert): return KeyCode::kInsert;
  case static_cast<std::uint32_t>(KeyCode::kDelete): return KeyCode::kDelete;
  default: return std::nullopt;
  }
}

auto translate_message(std::uint32_t uint4_msg, std::uintptr_t uint8_wparam, std::intptr_t int8_lparam) -> std::optional<UiEvent> {
  if (uint4_msg == kWmClose) {
    return UiEvent{CloseEvent{}};
  }
  if (uint4_msg == kWmSize) {
    return UiEvent{ResizeEvent{unsigned_loword(int8_lparam), unsigned_hiword(int8_lparam)}};
  }
  if (uint4_msg == kWmActivate) {
    return UiEvent{FocusEvent{static_cast<std::uint16_t>(uint8_wparam) != kWaInactive}};
  }
  if (uint4_msg == kWmKeyDown || uint4_msg == kWmKeyUp || uint4_msg == kWmSysKeyDown || uint4_msg == kWmSysKeyUp) {
    auto const opt_key = translate_key(static_cast<std::uint32_t>(uint8_wparam));
    if (!opt_key.has_value()) {
      return std::nullopt;
    }
    auto const phase = uint4_msg == kWmKeyDown || uint4_msg == kWmSysKeyDown ? KeyPhase::kDown : KeyPhase::kUp;
    return UiEvent{KeyEvent{phase, *opt_key}};
  }
  if (uint4_msg == kWmMouseMove) {
    return UiEvent{PointerEvent{PointerPhase::kMove, PointerButton::kNone, signed_loword(int8_lparam), signed_hiword(int8_lparam)}};
  }
  if (uint4_msg == kWmLeftButtonDown || uint4_msg == kWmLeftButtonUp) {
    auto const phase = uint4_msg == kWmLeftButtonDown ? PointerPhase::kDown : PointerPhase::kUp;
    return UiEvent{PointerEvent{phase, PointerButton::kLeft, signed_loword(int8_lparam), signed_hiword(int8_lparam)}};
  }
  if (uint4_msg == kWmRightButtonDown || uint4_msg == kWmRightButtonUp) {
    auto const phase = uint4_msg == kWmRightButtonDown ? PointerPhase::kDown : PointerPhase::kUp;
    return UiEvent{PointerEvent{phase, PointerButton::kRight, signed_loword(int8_lparam), signed_hiword(int8_lparam)}};
  }
  if (uint4_msg == kWmMiddleButtonDown || uint4_msg == kWmMiddleButtonUp) {
    auto const phase = uint4_msg == kWmMiddleButtonDown ? PointerPhase::kDown : PointerPhase::kUp;
    return UiEvent{PointerEvent{phase, PointerButton::kMiddle, signed_loword(int8_lparam), signed_hiword(int8_lparam)}};
  }
  return std::nullopt;
}

}
