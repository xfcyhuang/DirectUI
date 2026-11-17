add_library(dui_std)
target_sources(dui_std PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES std.cppm)
target_compile_features(dui_std PUBLIC cxx_std_26)
target_compile_options(dui_std PRIVATE -Wno-reserved-module-identifier)

add_subdirectory(platform)
