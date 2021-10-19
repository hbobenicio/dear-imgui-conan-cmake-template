# dear-imgui-example

Just a simple Dear ImGui + CMake + Conan setup.

## Dependencies

### Build Dependencies

- C++ compiler
- CMake
- Python (to create a venv for conan an optionally ninja)
- Conan Dependency Manager (installed via pip inside the venv)
- GLFW3 as a Window and Event Manager (conan dependency)
  - Note: this has a transitive dependency of opengl/system and conan install will probably try to install other transitive system dependencies, like libgl-dev and xorg-dev (at least on linux)
- Dear ImGui (conan dependency)

## Build

```sh
# Creating venv for conan
python3 -m venv venv
. venv/bin/activate
pip install -U pip

# Installing conan pip dependency
pip install -r dev-requirements.txt

mkdir build && cd build

# Install conan dependencies for the specified build type
conan install .. -s build_type=Debug
# conan install .. -s build_type=Release

# Generate build files for your preferred build system
cmake .. -G Ninja
# cmake .. -G "Unix Makefiles"
# cmake .. -G "Visual Studio 15 2017 Win64"

# Build it
ninja
# make
```

## Run 

```
./bin/dear-imgui-conan-cmake-template
```

## References

- https://blog.conan.io/2019/06/26/An-introduction-to-the-Dear-ImGui-library.html

