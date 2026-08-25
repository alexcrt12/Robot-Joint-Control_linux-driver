cmake_minimum_required(VERSION 3.12)
project(hybrid)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. FIXED TYPO: Now explicitly pybind11
set(PYBIND11_FINDPYTHON ON)

find_package(pybind11 REQUIRED)

# Define where your external library lives on your PC
set(EXTERNAL_INC_DIR "/home/rh-robot/Desktop/pci2321/include")
set(EXTERNAL_LIB_DIR "/home/rh-robot/Desktop/pci2321/build")

# 2. Create the Python module (compiling ONLY the wrapper)
# (Deleted the add_executable line that was here)
pybind11_add_module(pci2321 bindings/wrap.cpp)

# 3. Tell CMake where to find the external headers
target_include_directories(pci2321 PRIVATE ${EXTERNAL_INC_DIR})

# 4. Find the external compiled library (e.g., libpci2321.so)
find_library(EXTERNAL_LIB
        NAMES pci2321
        PATHS ${EXTERNAL_LIB_DIR}
        REQUIRED
)

# 5. Link the external library to your Python module
target_link_libraries(pci2321 PRIVATE ${EXTERNAL_LIB})

# 6. (Crucial for Linux) Bake the external library path into the Python module
# so Python knows where to find the .so file at runtime.
set_target_properties(pci2321 PROPERTIES
        INSTALL_RPATH "${EXTERNAL_LIB_DIR}"
        BUILD_WITH_INSTALL_RPATH TRUE
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)