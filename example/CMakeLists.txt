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
set(PSLIBNAME libPoissonSolver3DCylindricalGPU.so)

find_library(PSLIB ${PSLIBNAME})
string(FIND ${PSLIB} "-NOTFOUND" PSLIB_NOTFOUND )

if(PSLIB_NOTFOUND GREATER -1)
  message( FATAL_ERROR "Poisson Solver Cuda Library libPoissonSolver3DCylindricalGPU.so not found" )
endif(PSLIB_NOTFOUND GREATER -1)

INCLUDE_DIRECTORIES(
	${CMAKE_PREFIX_PATH}/include
) 


# tambah disini kode sumber user
set(CPP_SOURCE PoissonSolver3DGPUTest.cpp)
set(HEADERS PoissonSolver3DGPUTest.h)

set(TARGET_NAME poissonsolvergputest)

add_executable(${TARGET_NAME}
  PoissonSolver3DGPUTest.cpp
)


# ikut sertakan shared library cuda dan poisson solver
target_link_libraries(${TARGET_NAME} ${PSLIB} ${LIBCUDA_SO_PATH})



