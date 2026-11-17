#define BOOST_TEST_MODULE dui_core_input_event
#include <boost/test/included/unit_test.hpp>

import std;
import DirectUI.core;

BOOST_AUTO_TEST_CASE(keydown_message_yields_key_down_event) {
  auto const opt_ev = DirectUI::translate_message(DirectUI::kWmKeyDown, std::uintptr_t{0x41}, std::intptr_t{0});
  auto const* ptr_key = std::get_if<DirectUI::KeyEvent>(&*opt_ev);
  BOOST_TEST(ptr_key != nullptr);
  BOOST_TEST((ptr_key->phase == DirectUI::KeyPhase::kDown));
  BOOST_TEST((ptr_key->key == DirectUI::KeyCode::kA));
}

BOOST_AUTO_TEST_CASE(keyup_message_yields_key_up_event) {
  auto const opt_ev = DirectUI::translate_message(DirectUI::kWmKeyUp, std::uintptr_t{0x1B}, std::intptr_t{0});
  auto const* ptr_key = std::get_if<DirectUI::KeyEvent>(&*opt_ev);
  BOOST_TEST(ptr_key != nullptr);
  BOOST_TEST((ptr_key->phase == DirectUI::KeyPhase::kUp));
  BOOST_TEST((ptr_key->key == DirectUI::KeyCode::kEscape));
}

BOOST_AUTO_TEST_CASE(unmapped_key_message_yields_nullopt) {
  auto const opt_ev = DirectUI::translate_message(DirectUI::kWmKeyDown, std::uintptr_t{0xE7}, std::intptr_t{0});
  BOOST_TEST(!opt_ev.has_value());
}

BOOST_AUTO_TEST_CASE(mouse_move_yields_move_pointer_event_with_coordinates) {
  auto const int8_lparam = static_cast<std::intptr_t>(static_cast<unsigned long long>(300) << 16 | 400);
  auto const opt_ev = DirectUI::translate_message(DirectUI::kWmMouseMove, std::uintptr_t{0}, int8_lparam);
  auto const* ptr_ptr = std::get_if<DirectUI::PointerEvent>(&*opt_ev);
  BOOST_TEST(ptr_ptr != nullptr);
  BOOST_TEST((ptr_ptr->phase == DirectUI::PointerPhase::kMove));
  BOOST_TEST((ptr_ptr->button == DirectUI::PointerButton::kNone));
  BOOST_CHECK_EQUAL(ptr_ptr->int4_x, 400);
  BOOST_CHECK_EQUAL(ptr_ptr->int4_y, 300);
}

BOOST_AUTO_TEST_CASE(left_button_down_yields_down_event) {
  auto const int8_lparam = static_cast<std::intptr_t>(static_cast<unsigned long long>(480) << 16 | 640);
  auto const opt_ev = DirectUI::translate_message(DirectUI::kWmLeftButtonDown, std::uintptr_t{0}, int8_lparam);
  auto const* ptr_ptr = std::get_if<DirectUI::PointerEvent>(&*opt_ev);
  BOOST_TEST(ptr_ptr != nullptr);
  BOOST_TEST((ptr_ptr->phase == DirectUI::PointerPhase::kDown));
  BOOST_TEST((ptr_ptr->button == DirectUI::PointerButton::kLeft));
  BOOST_CHECK_EQUAL(ptr_ptr->int4_x, 640);
  BOOST_CHECK_EQUAL(ptr_ptr->int4_y, 480);
}

BOOST_AUTO_TEST_CASE(right_button_up_yields_up_event) {
  auto const opt_ev = DirectUI::translate_message(DirectUI::kWmRightButtonUp, std::uintptr_t{0}, std::intptr_t{0});
  auto const* ptr_ptr = std::get_if<DirectUI::PointerEvent>(&*opt_ev);
  BOOST_TEST(ptr_ptr != nullptr);
  BOOST_TEST((ptr_ptr->phase == DirectUI::PointerPhase::kUp));
  BOOST_TEST((ptr_ptr->button == DirectUI::PointerButton::kRight));
}

BOOST_AUTO_TEST_CASE(translate_key_maps_letter_digit_and_special) {
  auto const opt_z = DirectUI::translate_key(0x5Au);
  BOOST_TEST(opt_z.has_value());
  BOOST_TEST((*opt_z == DirectUI::KeyCode::kZ));
  auto const opt_9 = DirectUI::translate_key(0x39u);
  BOOST_TEST(opt_9.has_value());
  BOOST_TEST((*opt_9 == DirectUI::KeyCode::k9));
  auto const opt_left = DirectUI::translate_key(0x25u);
  BOOST_TEST(opt_left.has_value());
  BOOST_TEST((*opt_left == DirectUI::KeyCode::kLeft));
  auto const opt_shift = DirectUI::translate_key(0x10u);
  BOOST_TEST(opt_shift.has_value());
  BOOST_TEST((*opt_shift == DirectUI::KeyCode::kShift));
  BOOST_TEST(!DirectUI::translate_key(0xE7u).has_value());
}

BOOST_AUTO_TEST_CASE(syskeydown_message_yields_key_down_event) {
  auto const opt_ev = DirectUI::translate_message(0x0104u, std::uintptr_t{0x12}, std::intptr_t{0});
  auto const* ptr_key = std::get_if<DirectUI::KeyEvent>(&*opt_ev);
  BOOST_TEST(ptr_key != nullptr);
  BOOST_TEST((ptr_key->phase == DirectUI::KeyPhase::kDown));
  BOOST_TEST((ptr_key->key == DirectUI::KeyCode::kAlt));
}

BOOST_AUTO_TEST_CASE(negative_mouse_coordinate_is_sign_extended) {
  auto const int8_lparam = static_cast<std::intptr_t>(static_cast<unsigned long long>(0x0010) << 16 | 0xFF80);
  auto const opt_ev = DirectUI::translate_message(DirectUI::kWmMouseMove, std::uintptr_t{0}, int8_lparam);
  auto const* ptr_ptr = std::get_if<DirectUI::PointerEvent>(&*opt_ev);
  BOOST_TEST(ptr_ptr != nullptr);
  BOOST_CHECK_EQUAL(ptr_ptr->int4_x, -128);
  BOOST_CHECK_EQUAL(ptr_ptr->int4_y, 16);
}
