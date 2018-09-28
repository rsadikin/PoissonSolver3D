cuda_add_executable(poissonsolvertest 
	PoissonSolver3DGPUTest.cpp
	
)
target_link_libraries(poissonsolvertest PoissonSolver3DGPU)

INSTALL(FILES PoissonSolver3DGPUTest.cpp PoissonSolver3DGPUTest.h 
  DESTINATION example)
INSTALL(FILES "${CMAKE_BINARY_DIR}/gpuapp/poissonsolvertest" 
  DESTINATION example)
