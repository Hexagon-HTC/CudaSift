cmake_minimum_required(VERSION 3.22)

project(cudaSift LANGUAGES CXX CUDA)
set(cudaSift_VERSION_MAJOR 2)
set(cudaSift_VERSION_MINOR 0)
set(cudaSift_VERSION_PATCH 0)

find_package(OpenCV REQUIRED)
find_package(CUDA )

set(CMAKE_CUDA_ARCHITECTURES "60;70;75;80;86")

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
  matching.cu  
  cudaSiftD.h  
  cudaSift.h  
  cudautils.h
)  

set(cpp_sources
  geomFuncs.cpp  
)

# Create the library
add_library(cudaSift STATIC ${cuda_sources} ${cpp_sources})

# Set library properties
set_target_properties(cudaSift PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
    COMPILE_FLAGS "${EXTRA_CXX_FLAGS}"
    POSITION_INDEPENDENT_CODE ON
)

# Include directories
target_include_directories(cudaSift PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Link libraries
target_link_libraries(cudaSift PUBLIC ${OpenCV_LIBS})
target_link_libraries(cudaSift PRIVATE ${CUDA_LIBRARIES})

# Installation
install(TARGETS cudaSift
    EXPORT cudaSiftTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES
    cudaSift.h
    cudaSiftH.h
    cudaSiftD.h
    cudaImage.h
    cudautils.h
    DESTINATION include
)

install(EXPORT cudaSiftTargets
    FILE cudaSiftTargets.cmake
    DESTINATION lib/cmake/cudaSift
)

# Create config files for find_package
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    cudaSiftConfigVersion.cmake
    VERSION ${cudaSift_VERSION_MAJOR}.${cudaSift_VERSION_MINOR}.${cudaSift_VERSION_PATCH}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cudaSiftConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cudaSiftConfig.cmake"
    INSTALL_DESTINATION lib/cmake/cudaSift
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/cudaSiftConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/cudaSiftConfigVersion.cmake"
    DESTINATION lib/cmake/cudaSift
)
