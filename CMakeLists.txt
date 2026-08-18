cmake_minimum_required(VERSION 3.16)
project(pci2321 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(pci2321 SHARED src/pci2321.cpp src/io_backend.cpp)
target_include_directories(pci2321 PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
set_target_properties(pci2321 PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 1)

add_executable(sweep_example examples/sweep_example.cpp)
target_link_libraries(sweep_example PRIVATE pci2321)

include(GNUInstallDirs)
install(TARGETS pci2321 sweep_example
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
