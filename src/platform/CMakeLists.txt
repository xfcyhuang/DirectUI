add_library(directui STATIC render.cpp abi_check.cpp)
target_sources(directui PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES platform.cppm)
target_link_libraries(directui PUBLIC dui_std dui_core d3d11 dxgi)
