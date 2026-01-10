# Installation Instructions

## Linux (Debian/Ubuntu)

### Prerequisites

#### 1. Kaitai Struct Compiler (ksc)
The project requires `kaitai-struct-compiler` v0.11 or newer.
Download the latest `.deb` package from [GitHub Releases](https://github.com/kaitai-io/kaitai_struct_compiler/releases) and install it:

```bash
wget https://github.com/kaitai-io/kaitai_struct_compiler/releases/download/0.11/kaitai-struct-compiler_0.11_all.deb
sudo dpkg -i kaitai-struct-compiler_0.11_all.deb
rm kaitai-struct-compiler_0.11_all.deb
```

#### 2. System Dependencies
Install wxWidgets, OpenGL, and other required libraries:

```bash
sudo apt-get install libwxgtk3.2-dev libssl-dev freeglut3-dev zlib1g-dev default-jre
```

#### 3. Build Tools
Ensure you have a C++14 capable compiler (GCC/Clang), CMake, and Ninja installed:

```bash
sudo apt-get install build-essential cmake ninja-build
```

### Build Steps

1.  **Clone the repository** (if you haven't already):
    ```bash
    git clone --recursive https://github.com/yourusername/wxMMDViewer.git
    cd wxMMDViewer
    ```
    *Note: The `--recursive` flag is important to fetch the `kaitai_runtime` submodule.*

2.  **Build using the script**:
    ```bash
    ./ninja_build.sh
    ```

3.  **Run**:
    ```bash
    ./build/src/wxmmdviewer
    ```

### Debug Build (with backward-cpp)

To enable better stack traces on crash (using backward-cpp), use the `ENABLE_BACKWARD` option:

```bash
mkdir build
cd build
cmake -G "Ninja" -DENABLE_BACKWARD=ON ..
ninja
```

