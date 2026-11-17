import std;
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

auto print_error(std::string_view str_op, DirectUI::Error const& err) -> void {
  std::println("{} failed: {} (0x{:08X})", str_op, err.str_msg, err.uint4_code);
}

}

auto main() -> int {
  auto const exp_window = DirectUI::create_window(DirectUI::WindowDesc{}, [](DirectUI::EventView const& ev) {
    using enum DirectUI::EventKind;
    if (ev.kind == kClose) {
      std::println("event: close");
    } else if (ev.kind == kResize) {
      std::println("event: resize {}x{}", ev.uint4_width, ev.uint4_height);
    } else if (ev.kind == kFocus) {
      std::println("event: focus {}", ev.b_focused);
    }
  });
  if (!exp_window) {
    print_error("create_window", exp_window.error());
    return 1;
  }
  auto window = *exp_window;

  auto const exp_device = DirectUI::create_device();
  if (!exp_device) {
    print_error("create_device", exp_device.error());
    return 1;
  }
  auto device = *exp_device;

  auto const exp_swap = DirectUI::create_swap_chain(device, window);
  if (!exp_swap) {
    print_error("create_swap_chain", exp_swap.error());
    return 1;
  }
  auto swap_chain = *exp_swap;

  auto const time_start = std::chrono::steady_clock::now();
  while (DirectUI::pump_events()) {
    auto const fp8_ms = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - time_start).count());
    auto const fp8_hue = std::fmod(fp8_ms / 6000.0, 1.0);
    auto const res_clear = DirectUI::render_clear_color(swap_chain, hue_rgb(fp8_hue));
    if (!res_clear) {
      print_error("render_clear_color", res_clear.error());
      break;
    }
  }

  DirectUI::destroy_swap_chain(swap_chain);
  DirectUI::destroy_device(device);
  DirectUI::destroy_window(window);
  return 0;
}
