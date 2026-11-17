export module DirectUI.platform;

import std;

export namespace DirectUI {

enum class EventKind : std::uint8_t { kClose, kResize, kFocus };

struct Error {
  std::uint32_t uint4_code{};
  std::string str_msg{};
};

struct EventView {
  EventKind kind{};
  std::uint32_t uint4_width{};
  std::uint32_t uint4_height{};
  bool b_focused{};
};

struct WindowDesc {
  std::uint32_t uint4_width{800};
  std::uint32_t uint4_height{600};
  std::string str_title{"DirectUI demo"};
};

struct Window {
  std::uint64_t uint8_id{};
};

using EventReceiver = std::function<void(EventView const&)>;

[[nodiscard]] auto translate_message(std::uint32_t uint4_msg, std::uintptr_t uint8_wparam, std::intptr_t int8_lparam) -> std::optional<EventView>;

[[nodiscard]] auto create_window(WindowDesc const& desc, EventReceiver fn_on_event) -> std::expected<Window, Error>;

auto run_message_loop() -> void;

auto destroy_window(Window& win) -> void;

}
