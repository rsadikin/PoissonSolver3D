cuda_add_executable(poissonsolvertest 
	PoissonSolver3DGPUTest.C
	
)
target_link_libraries(poissonsolvertest PoissonSolver3DGPU)

