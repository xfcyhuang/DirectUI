import std;
import DirectUI.platform;

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
    std::println("create_window failed: {} (0x{:08X})", exp_window.error().str_msg, exp_window.error().uint4_code);
    return 1;
  }
  auto window = *exp_window;
  DirectUI::run_message_loop();
  DirectUI::destroy_window(window);
  return 0;
}
