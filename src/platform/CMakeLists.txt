add_library(directui STATIC event.cpp window.cpp render.cpp)
target_sources(directui PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES platform.cppm)
target_link_libraries(directui PUBLIC dui_std user32 gdi32 d3d11 dxgi)
