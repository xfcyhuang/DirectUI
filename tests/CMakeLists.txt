add_executable(dui_event_test event_test.cpp)
target_link_libraries(dui_event_test PRIVATE directui)
target_include_directories(dui_event_test PRIVATE ${CMAKE_SOURCE_DIR}/../liboost)
target_compile_options(dui_event_test PRIVATE -Wno-c2y-extensions)

add_test(NAME dui_event_test COMMAND dui_event_test)
