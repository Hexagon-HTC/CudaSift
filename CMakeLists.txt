cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(cudasift LANGUAGES CXX CUDA)

# -------------------------------------------------------------
# Options
# -------------------------------------------------------------
option(BUILD_SHARED_LIBS "Build CudaSift as a shared library" ON)
set(CUDASIFT_CUDA_RUNTIME "Shared" CACHE STRING "Select CUDA runtime linkage (Shared|Static)")
set_property(CACHE CUDASIFT_CUDA_RUNTIME PROPERTY STRINGS Shared Static)

# Allow user override of architectures; provide a modern default set if unset
if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 75 86 89 CACHE STRING "CUDA arch list" FORCE)
endif()

# Find CUDA toolkit (modern CUDAToolkit package)
find_package(CUDAToolkit REQUIRED)

# -------------------------------------------------------------
# Library target
# -------------------------------------------------------------
add_library(cudasift
    cudaSiftH.cu
    matching.cu
)

set_target_properties(cudasift PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CUDA_RUNTIME_LIBRARY ${CUDASIFT_CUDA_RUNTIME}
)

# Public interface include dirs
target_include_directories(cudasift
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# Link CUDA runtime (cudart) explicitly
target_link_libraries(cudasift PUBLIC CUDA::cudart)

# Export macro logic: only export/import when building shared
if(BUILD_SHARED_LIBS)
    target_compile_definitions(cudasift
        PUBLIC CUDASIFT_SHARED
        PRIVATE CUDASIFT_EXPORTS
    )
else()
    target_compile_definitions(cudasift PUBLIC CUDASIFT_STATIC)
endif()

# -------------------------------------------------------------
# Installation
# -------------------------------------------------------------
install(TARGETS cudasift
    EXPORT CudaSiftTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(FILES
    cudaSift.h
    cudaUtils.h
    DESTINATION include
)

# Print configuration summary
message(STATUS "CudaSift Configuration Summary:")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Library type: ${BUILD_SHARED_LIBS}")
message(STATUS "  CUDA runtime linkage: ${CUDASIFT_CUDA_RUNTIME}")
message(STATUS "  CUDA architectures: ${CMAKE_CUDA_ARCHITECTURES}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")