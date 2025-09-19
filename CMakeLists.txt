cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(cudasift LANGUAGES CXX CUDA)

# Find CUDA package
find_package(CUDAToolkit REQUIRED)

if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 75 86 89)
endif()

# Define the library
add_library(cudasift SHARED
    cudaSiftH.cu
    matching.cu
)

# Set target properties
set_target_properties(cudasift PROPERTIES
    CUDA_RUNTIME_LIBRARY Shared
    POSITION_INDEPENDENT_CODE ON
)

# Include directories
target_include_directories(cudasift
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# Link CUDA libraries
target_link_libraries(cudasift
    PUBLIC
        CUDA::cudart
)

# Define export macro for Windows DLL
target_compile_definitions(cudasift PRIVATE CUDASIFT_EXPORTS)

# Install targets
install(TARGETS cudasift
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# Install headers
install(FILES
    cudaSift.h
    cudaUtils.h
    DESTINATION include
)

# Print configuration summary
message(STATUS "CudaSift Configuration Summary:")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  CUDA architectures: ${CMAKE_CUDA_ARCHITECTURES}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")