# MakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)
project (3DPoissonSolverGPUTest)

find_package(CUDA)
if(NOT CUDA_FOUND)
    message( FATAL_ERROR "NVIDIA CUDA package not found" )
else()
    find_library(LIBCUDA_SO_PATH libcuda.so)
    string(FIND ${LIBCUDA_SO_PATH} "-NOTFOUND" LIBCUDA_SO_PATH_NOTFOUND )
endif(NOT CUDA_FOUND)
message( STATUS "Building Poisson Solver  with CUDA support" )

if(LIBCUDA_SO_PATH_NOTFOUND GREATER -1)
  message( FATAL_ERROR "NVIDIA CUDA libcuda.so not found" )
endif(LIBCUDA_SO_PATH_NOTFOUND GREATER -1)


#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/cuda" ${CMAKE_MODULE_PATH})
#find_package(CUDA QUIET REQUIRED)
find_library(PSLIB libPoissonSolver3DGPU.so)
string(FIND ${PSLIB} "-NOTFOUND" PSLIB_NOTFOUND )

if(PSLIB_NOTFOUND GREATER -1)
  message( FATAL_ERROR "Poisson Solver Cuda Library libPoissonSolver3DGPU.so not found" )
endif(PSLIB_NOTFOUND GREATER -1)
#set(CUDA_ATTACH_VS_BUILD_RULE_TO_CUDA_FILE OFF)
# set(BUILD_SHARED_LIBS ON)
#list(APPEND CUDA_NVCC_FLAGS)


#cuda_add_executable(poissonsolvertest 
#	PoissonSolver3DGPUTest.cpp
#i)
# add_executable(poissonsolvergputest PoissonSolver3DGPUTest.cpp ${PSLIB})
# install(TARGETS poissonsolvergputest  DESTINATION bin)



set(CPP_SOURCE PoissonSolver3DGPUTest.cpp)
set(HEADERS PoissonSolver3DGPUTest.h)

add_executable(poissonsolvergputest  ${CPP_SOURCE} ${PSLIB})

install(TARGETS poissonsolvergputest  DESTINATION bin)
