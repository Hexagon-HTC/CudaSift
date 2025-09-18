cmake_minimum_required(VERSION 3.22)

project(cudaSift LANGUAGES CXX CUDA)
set(cudaSift_VERSION_MAJOR 2)
set(cudaSift_VERSION_MINOR 0)
set(cudaSift_VERSION_PATCH 0)

set(CMAKE_CUDA_ARCHITECTURES "60;61;70;75;80;86;89")

# Find OpenCV
find_package(OpenCV REQUIRED COMPONENTS opencv_core)


if (WIN32)
  set(EXTRA_CXX_FLAGS "/DVERBOSE /D_CRT_SECURE_NO_WARNINGS ")
elseif(UNIX)
    if(APPLE)
        set(EXTRA_CXX_FLAGS "-DVERBOSE -msse2")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -msse2")
    endif()
endif()

set(cuda_sources
  cudaImage.cu
  cudaImage.h
  cudaSiftH.cu
  cudaSiftH.h
  cudaSiftD.cu
  cudaSiftD.h
  matching.cu
  cudaSift.h
  cudautils.h
) 

set(cpp_sources
  geomFuncs.cpp  
)

# Create the library
add_library(cudaSift STATIC ${cuda_sources} ${cpp_sources})
target_compile_definitions(cudaSift PRIVATE CUDASIFT_BUILD)

# Set library properties
set_target_properties(cudaSift PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
    COMPILE_FLAGS "${EXTRA_CXX_FLAGS}"
    POSITION_INDEPENDENT_CODE ON
)

# Include directories - use target-specific instead of global
target_include_directories(cudaSift PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Link OpenCV
target_link_libraries(cudaSift PUBLIC opencv_core)
