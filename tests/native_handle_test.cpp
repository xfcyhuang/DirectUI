#define BOOST_TEST_MODULE dui_platform_native_handle
#include <boost/test/included/unit_test.hpp>

import std;
import DirectUI.platform;

BOOST_AUTO_TEST_CASE(native_handle_of_empty_window_is_null) {
  auto const win = DirectUI::Window{};
  BOOST_TEST(DirectUI::native_handle(win) == nullptr);
}

BOOST_AUTO_TEST_CASE(native_handle_of_empty_device_is_null) {
  auto const dev = DirectUI::Device{};
  BOOST_TEST(DirectUI::native_handle(dev) == nullptr);
}

BOOST_AUTO_TEST_CASE(native_handle_of_empty_swap_chain_is_null) {
  auto const sc = DirectUI::SwapChain{};
  BOOST_TEST(DirectUI::native_handle(sc) == nullptr);
}

BOOST_AUTO_TEST_CASE(native_handle_returns_void_pointer) {
  static_assert(std::is_same_v<decltype(DirectUI::native_handle(std::declval<DirectUI::Window const&>())), void*>);
  BOOST_TEST(true);
}
