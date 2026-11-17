export module DirectUI.core;

import std;

export namespace DirectUI {

struct CloseEvent {};

struct ResizeEvent {
  std::uint32_t uint4_width{};
  std::uint32_t uint4_height{};
};

struct FocusEvent {
  bool b_focused{};
};

enum class PointerPhase : std::uint8_t { kDown, kUp, kMove };

enum class PointerButton : std::uint8_t { kNone, kLeft, kRight, kMiddle };

struct PointerEvent {
  PointerPhase phase{};
  PointerButton button{};
  std::int32_t int4_x{};
  std::int32_t int4_y{};
};

enum class KeyPhase : std::uint8_t { kDown, kUp };

enum class KeyCode : std::uint32_t {
  kBackspace = 0x08,
  kTab = 0x09,
  kReturn = 0x0D,
  kShift = 0x10,
  kControl = 0x11,
  kAlt = 0x12,
  kEscape = 0x1B,
  kSpace = 0x20,
  kPageUp = 0x21,
  kPageDown = 0x22,
  kEnd = 0x23,
  kHome = 0x24,
  kLeft = 0x25,
  kUp = 0x26,
  kRight = 0x27,
  kDown = 0x28,
  kInsert = 0x2D,
  kDelete = 0x2E,
  k0 = 0x30,
  k1 = 0x31,
  k2 = 0x32,
  k3 = 0x33,
  k4 = 0x34,
  k5 = 0x35,
  k6 = 0x36,
  k7 = 0x37,
  k8 = 0x38,
  k9 = 0x39,
  kA = 0x41,
  kB = 0x42,
  kC = 0x43,
  kD = 0x44,
  kE = 0x45,
  kF = 0x46,
  kG = 0x47,
  kH = 0x48,
  kI = 0x49,
  kJ = 0x4A,
  kK = 0x4B,
  kL = 0x4C,
  kM = 0x4D,
  kN = 0x4E,
  kO = 0x4F,
  kP = 0x50,
  kQ = 0x51,
  kR = 0x52,
  kS = 0x53,
  kT = 0x54,
  kU = 0x55,
  kV = 0x56,
  kW = 0x57,
  kX = 0x58,
  kY = 0x59,
  kZ = 0x5A,
};

struct KeyEvent {
  KeyPhase phase{};
  KeyCode key{};
};

using UiEvent = std::variant<CloseEvent, ResizeEvent, FocusEvent, PointerEvent, KeyEvent>;

inline constexpr std::uint32_t kWmSize{0x0005};
inline constexpr std::uint32_t kWmActivate{0x0006};
inline constexpr std::uint32_t kWmClose{0x0010};
inline constexpr std::uint32_t kWmKeyDown{0x0100};
inline constexpr std::uint32_t kWmKeyUp{0x0101};
inline constexpr std::uint32_t kWmSysKeyDown{0x0104};
inline constexpr std::uint32_t kWmSysKeyUp{0x0105};
inline constexpr std::uint32_t kWmMouseMove{0x0200};
inline constexpr std::uint32_t kWmLeftButtonDown{0x0201};
inline constexpr std::uint32_t kWmLeftButtonUp{0x0202};
inline constexpr std::uint32_t kWmRightButtonDown{0x0204};
inline constexpr std::uint32_t kWmRightButtonUp{0x0205};
inline constexpr std::uint32_t kWmMiddleButtonDown{0x0207};
inline constexpr std::uint32_t kWmMiddleButtonUp{0x0208};
inline constexpr std::uint16_t kWaInactive{0};

[[nodiscard]] auto translate_message(std::uint32_t uint4_msg, std::uintptr_t uint8_wparam, std::intptr_t int8_lparam) -> std::optional<UiEvent>;

[[nodiscard]] auto translate_key(std::uint32_t uint4_vk) -> std::optional<KeyCode>;

}
