cmake_minimum_required(VERSION 3.21)
project(smvk LANGUAGES CXX VERSION 0.1.0)

# The full Vulkan backend is still being extracted. This intentionally
# builds only the already-independent portable headers. It is NOT an
# executable RHI backend or a substitute for Tourmaline's current engine.
add_subdirectory(core)

add_library(smvk_portable INTERFACE)
add_library(smvk::portable ALIAS smvk_portable)
target_compile_features(smvk_portable INTERFACE cxx_std_20)
target_link_libraries(smvk_portable INTERFACE tourmaline_core)
target_include_directories(smvk_portable INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    # Compatibility forwarding headers for staged legacy source includes.
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/compat>)

# Don't silently introduce the library's tests into an embedding project.
option(SMVK_BUILD_TESTS "Build portable subset smoke tests" ${PROJECT_IS_TOP_LEVEL})
if(SMVK_BUILD_TESTS)
    enable_testing()
    add_executable(smvk_portable_smoke tests/portable_smoke.cpp)
    target_link_libraries(smvk_portable_smoke PRIVATE smvk::portable)
    add_test(NAME smvk_portable_smoke COMMAND smvk_portable_smoke)
endif()
