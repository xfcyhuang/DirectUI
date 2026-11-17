#define BOOST_TEST_MODULE dui_core_window_event
#include <boost/test/included/unit_test.hpp>

import std;
import DirectUI.core;

BOOST_AUTO_TEST_CASE(close_message_yields_close_event) {
  auto const opt_ev = DirectUI::translate_message(DirectUI::kWmClose, std::uintptr_t{0}, std::intptr_t{0});
  BOOST_TEST(opt_ev.has_value());
  BOOST_TEST((std::holds_alternative<DirectUI::CloseEvent>(*opt_ev)));
}

BOOST_AUTO_TEST_CASE(size_message_yields_resize_with_loword_hiword_dimensions) {
  auto const int8_lparam = static_cast<std::intptr_t>(static_cast<unsigned long long>(480) << 16 | 640);
  auto const opt_ev = DirectUI::translate_message(DirectUI::kWmSize, std::uintptr_t{0}, int8_lparam);
  BOOST_TEST(opt_ev.has_value());
  auto const* ptr_resize = std::get_if<DirectUI::ResizeEvent>(&*opt_ev);
  BOOST_TEST(ptr_resize != nullptr);
  BOOST_CHECK_EQUAL(ptr_resize->uint4_width, 640u);
  BOOST_CHECK_EQUAL(ptr_resize->uint4_height, 480u);
}

BOOST_AUTO_TEST_CASE(activate_message_yields_focus_gained) {
  auto const opt_ev = DirectUI::translate_message(DirectUI::kWmActivate, std::uintptr_t{1}, std::intptr_t{0});
  auto const* ptr_focus = std::get_if<DirectUI::FocusEvent>(&*opt_ev);
  BOOST_TEST(ptr_focus != nullptr);
  BOOST_TEST(ptr_focus->b_focused);
}

BOOST_AUTO_TEST_CASE(deactivate_message_yields_focus_lost) {
  auto const opt_ev = DirectUI::translate_message(DirectUI::kWmActivate, std::uintptr_t{DirectUI::kWaInactive}, std::intptr_t{0});
  auto const* ptr_focus = std::get_if<DirectUI::FocusEvent>(&*opt_ev);
  BOOST_TEST(ptr_focus != nullptr);
  BOOST_TEST(!ptr_focus->b_focused);
}

BOOST_AUTO_TEST_CASE(unhandled_message_yields_nullopt) {
  auto const opt_ev = DirectUI::translate_message(0xFFFFu, std::uintptr_t{0}, std::intptr_t{0});
  BOOST_TEST(!opt_ev.has_value());
}
