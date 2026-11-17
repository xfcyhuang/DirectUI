#define BOOST_TEST_MODULE dui_platform_event
#include <boost/test/included/unit_test.hpp>

import DirectUI.platform;

BOOST_AUTO_TEST_CASE(close_message_yields_close_event) {
  auto const opt_ev = DirectUI::translate_message(0x0010u, std::uintptr_t{0}, std::intptr_t{0});
  BOOST_TEST(opt_ev.has_value());
  BOOST_TEST((opt_ev->kind == DirectUI::EventKind::kClose));
}

BOOST_AUTO_TEST_CASE(size_message_yields_resize_with_loword_hiword_dimensions) {
  auto const int8_lparam = static_cast<std::intptr_t>(static_cast<unsigned long long>(480) << 16 | 640);
  auto const opt_ev = DirectUI::translate_message(0x0005u, std::uintptr_t{0}, int8_lparam);
  BOOST_TEST(opt_ev.has_value());
  BOOST_TEST((opt_ev->kind == DirectUI::EventKind::kResize));
  BOOST_CHECK_EQUAL(opt_ev->uint4_width, 640u);
  BOOST_CHECK_EQUAL(opt_ev->uint4_height, 480u);
}

BOOST_AUTO_TEST_CASE(activate_message_yields_focus_gained) {
  auto const opt_ev = DirectUI::translate_message(0x0006u, std::uintptr_t{1}, std::intptr_t{0});
  BOOST_TEST(opt_ev.has_value());
  BOOST_TEST((opt_ev->kind == DirectUI::EventKind::kFocus));
  BOOST_TEST(opt_ev->b_focused);
}

BOOST_AUTO_TEST_CASE(deactivate_message_yields_focus_lost) {
  auto const opt_ev = DirectUI::translate_message(0x0006u, std::uintptr_t{0}, std::intptr_t{0});
  BOOST_TEST(opt_ev.has_value());
  BOOST_TEST((opt_ev->kind == DirectUI::EventKind::kFocus));
  BOOST_TEST(!opt_ev->b_focused);
}

BOOST_AUTO_TEST_CASE(unhandled_message_yields_nullopt) {
  auto const opt_ev = DirectUI::translate_message(0xFFFFu, std::uintptr_t{0}, std::intptr_t{0});
  BOOST_TEST(!opt_ev.has_value());
}
