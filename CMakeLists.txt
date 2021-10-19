# There is no need for this high cmake version number actually.
# It's just to encourage people to upgrade them in their environments. =)
cmake_minimum_required(VERSION 3.18)

project(dear-imgui-conan-cmake-template
    VERSION 0.0.1
    DESCRIPTION "Just a simple Dear ImGui + CMake + Conan setup"
    LANGUAGES CXX
)

###############
# Conan Setup #
###############

include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
conan_basic_setup(TARGETS)
# find_package(imgui CONFIG)
# find_package(glfw CONFIG)

##############
# Submodules #
##############

# add_subdirectory(foo)
# add_subdirectory(bar)

#################
# Project Setup #
#################

add_executable(${PROJECT_NAME}
    # ImGui Backends. Platform/Environment dependent
    "${CONAN_SRC_DIRS_IMGUI}/imgui_impl_glfw.cpp"
    "${CONAN_SRC_DIRS_IMGUI}/imgui_impl_opengl3.cpp"

    "${PROJECT_SOURCE_DIR}/src/app/main.cpp"
)
target_compile_options(${PROJECT_NAME} PRIVATE "-std=c++17")
target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${CONAN_SRC_DIRS_IMGUI}"
        "${PROJECT_SOURCE_DIR}/src"
)
target_link_libraries(${PROJECT_NAME} CONAN_PKG::imgui CONAN_PKG::glfw)
