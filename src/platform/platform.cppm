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

struct Device {
  std::uint64_t uint8_id{};
};

struct SwapChain {
  std::uint64_t uint8_id{};
};

using EventReceiver = std::function<void(EventView const&)>;

[[nodiscard]] auto translate_message(std::uint32_t uint4_msg, std::uintptr_t uint8_wparam, std::intptr_t int8_lparam) -> std::optional<EventView>;

[[nodiscard]] auto create_window(WindowDesc const& desc, EventReceiver fn_on_event) -> std::expected<Window, Error>;

[[nodiscard]] auto pump_events() -> bool;

[[nodiscard]] auto create_device() -> std::expected<Device, Error>;

[[nodiscard]] auto create_swap_chain(Device const& dev, Window const& win) -> std::expected<SwapChain, Error>;

[[nodiscard]] auto render_clear_color(SwapChain& sc, std::array<float, 3> const& arr_rgb) -> std::expected<void, Error>;

[[nodiscard]] auto native_handle(Window const& win) -> void*;

[[nodiscard]] auto native_handle(Device const& dev) -> void*;

[[nodiscard]] auto native_handle(SwapChain const& sc) -> void*;

auto destroy_window(Window& win) -> void;

auto destroy_swap_chain(SwapChain& sc) -> void;

auto destroy_device(Device& dev) -> void;

}
