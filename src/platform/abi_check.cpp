module;

#define UNICODE 1
#define _UNICODE 1
#include <windows.h>

module DirectUI.platform;

import std;
import DirectUI.core;

static_assert(DirectUI::kWmSize == WM_SIZE);
static_assert(DirectUI::kWmActivate == WM_ACTIVATE);
static_assert(DirectUI::kWmClose == WM_CLOSE);
static_assert(DirectUI::kWmKeyDown == WM_KEYDOWN);
static_assert(DirectUI::kWmKeyUp == WM_KEYUP);
static_assert(DirectUI::kWmSysKeyDown == WM_SYSKEYDOWN);
static_assert(DirectUI::kWmSysKeyUp == WM_SYSKEYUP);
static_assert(DirectUI::kWmMouseMove == WM_MOUSEMOVE);
static_assert(DirectUI::kWmLeftButtonDown == WM_LBUTTONDOWN);
static_assert(DirectUI::kWmLeftButtonUp == WM_LBUTTONUP);
static_assert(DirectUI::kWmRightButtonDown == WM_RBUTTONDOWN);
static_assert(DirectUI::kWmRightButtonUp == WM_RBUTTONUP);
static_assert(DirectUI::kWmMiddleButtonDown == WM_MBUTTONDOWN);
static_assert(DirectUI::kWmMiddleButtonUp == WM_MBUTTONUP);
static_assert(DirectUI::kWaInactive == WA_INACTIVE);
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kBackspace) == VK_BACK);
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kTab) == VK_TAB);
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kReturn) == VK_RETURN);
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kShift) == VK_SHIFT);
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kControl) == VK_CONTROL);
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kAlt) == VK_MENU);
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kEscape) == VK_ESCAPE);
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kSpace) == VK_SPACE);
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kPageUp) == VK_PRIOR);
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kPageDown) == VK_NEXT);
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kEnd) == VK_END);
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kHome) == VK_HOME);
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kLeft) == VK_LEFT);
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kUp) == VK_UP);
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kRight) == VK_RIGHT);
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kDown) == VK_DOWN);
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kInsert) == VK_INSERT);
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kDelete) == VK_DELETE);
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::k0) == '0');
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::k1) == '1');
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::k9) == '9');
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kA) == 'A');
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kB) == 'B');
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kY) == 'Y');
static_assert(static_cast<std::uint32_t>(DirectUI::KeyCode::kZ) == 'Z');
