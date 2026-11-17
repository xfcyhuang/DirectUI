add_library(dui_core STATIC event.cpp)
target_sources(dui_core PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES core.cppm)
target_link_libraries(dui_core PUBLIC dui_std)
