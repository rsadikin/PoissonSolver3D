# For example the framework can run without GEANT4, but ROOT is
# mandatory


# **************************************************************************
# * Copyright(c) 1998-2014, ALICE Experiment at CERN, All rights reserved. *
# *                                                                        *
# * Author: The ALICE Off-line Project.                                    *
# * Contributors are mentioned in the code where appropriate.              *
# *                                                                        *
# * Permission to use, copy, modify and distribute this software and its   *
# * documentation strictly for non-commercial purposes is hereby granted   *
# * without fee, provided that the above copyright notice appears in all   *
# * copies and that both the copyright notice and this permission notice   *
# * appear in the supporting documentation. The authors make no claims     *
# * about the suitability of this software for any purpose. It is          *
# * provided "as is" without express or implied warranty.                  *
# **************************************************************************

# Module
cmake_minimum_required (VERSION 2.8.11)
project (3DPoissonSolverGPU)

find_package(CUDA)
if(NOT CUDA_FOUND)
    message( FATAL_ERROR "NVIDIA CUDA package not found" )
else()
    find_library(LIBCUDA_SO_PATH libcuda.so)
    string(FIND ${LIBCUDA_SO_PATH} "-NOTFOUND" LIBCUDA_SO_PATH_NOTFOUND )
endif(NOT CUDA_FOUND)
message( STATUS "Building PoissonSolver3D Cylindrical Multigrid  with CUDA support" )

if(LIBCUDA_SO_PATH_NOTFOUND GREATER -1)
  message( FATAL_ERROR "NVIDIA CUDA libcuda.so not found." )
endif(LIBCUDA_SO_PATH_NOTFOUND GREATER -1)

#set nvcc flags
set(CUDA_NVCC_FLAGS -Wno-deprecated-gpu-targets --use_fast_math --maxrregcount 64 -O4 -Xcompiler -fPIC -Xptxas -O4 -gencode arch=compute_30,code=sm_30 -gencode arch=compute_35,code=sm_35 -gencode arch=compute_52,code=sm_52 -gencode arch=compute_61,code=sm_61 -gencode arch=compute_60,code=sm_60)

#for convenience


find_package(Doxygen)

if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)





# Module include folder
include_directories(.
                    interface
                    kernel
                   )


#compile CUDA object file
cuda_compile(SCGPU_O kernel/PoissonSolver3DGPU.cu)



#set it back
if(STDCXX11FOUND GREATER -1)
  string ( REPLACE "-std=c++98" "-std=c++11" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
endif()
#end of CUDA specific stuff; from here on build usual library


# Sources in alphabetical order
set(SRCS
    interface/PoissonSolver3DCylindricalGPU.cxx
    ${SCGPU_O}
   )

# Headers from sources
set(CINTHDRS
    interface/PoissonSolver3DCylindricalGPU.h
    )
  
set(HDRS
    ${CINTHDRS}
    kernel/PoissonSolver3DGPU.h
  )

enable_testing()
set(TARGET_NAME poissonsolvergpu)


add_library(${TARGET_NAME} SHARED ${SRCS})
target_link_libraries(${TARGET_NAME} ${CUDA_LIBRARIES} ${LIBCUDA_SO_PATH})

set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_FLAGS "")

# Installation
install(TARGETS ${TARGET_NAME}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)

install(FILES ${HDRS} DESTINATION include)
