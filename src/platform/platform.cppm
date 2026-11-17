export module DirectUI.platform;

import std;

export namespace DirectUI {

struct Error {
  std::uint32_t uint4_code{};
  std::string str_msg{};
};

struct Device {
  std::uint64_t uint8_id{};
};

struct SwapChain {
  std::uint64_t uint8_id{};
};

[[nodiscard]] auto create_device() -> std::expected<Device, Error>;

[[nodiscard]] auto create_swap_chain(Device const& dev, std::uintptr_t uint8_hwnd) -> std::expected<SwapChain, Error>;

[[nodiscard]] auto render_clear_color(SwapChain& sc, std::array<float, 3> const& arr_rgb) -> std::expected<void, Error>;

[[nodiscard]] auto native_handle(Device const& dev) -> void*;

[[nodiscard]] auto native_handle(SwapChain const& sc) -> void*;

auto destroy_swap_chain(SwapChain& sc) -> void;

auto destroy_device(Device& dev) -> void;

}
