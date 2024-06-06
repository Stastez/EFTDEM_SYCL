cmake_minimum_required(VERSION 3.28)

set(TARGET_NAME EFTDEM_SYCL)

project(${TARGET_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(AdaptiveCpp CONFIG REQUIRED)
find_package(lodepng CONFIG REQUIRED)

add_compile_definitions("EFTDEM_DEBUG=$<CONFIG:Debug>")
add_executable(${TARGET_NAME} main.cpp
        MobileMappingReader.cpp
        MobileMappingReader.h
        Sorter.cpp
        Sorter.h
        PointCloud.h
        Point.h
        Point.inl
        Rasterizer.cpp
        Rasterizer.h
        SYCLState.h
        PngWriter.cpp
        PngWriter.h
        RadialFiller.cpp
        RadialFiller.h
        ConfigReader.cpp
        ConfigReader.h
        InterpolationFiller.cpp
        InterpolationFiller.h
)

if (MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /W4)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -favor:AMD64")
else ()
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif ()

add_sycl_to_target(TARGET ${TARGET_NAME})
target_link_libraries(${TARGET_NAME} PRIVATE lodepng)