cuda_add_executable(poissonsolvertest 
	PoissonSolver3DGPUTest.cpp
	
)
target_link_libraries(poissonsolvertest PoissonSolver3DGPU)

