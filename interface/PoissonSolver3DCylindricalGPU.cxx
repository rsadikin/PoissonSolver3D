/// \author Rifki Sadikin <rifki.sadikin@cern.ch>, Indonesian Institute of Sciences
/// \date Nov 20, 2017

#include <math.h>
#include "PoissonSolver3DCylindricalGPU.h"

///
PoissonSolver3DCylindricalGPU::PoissonSolver3DCylindricalGPU() {

	fErrorConvF = new float [fMgParameters.nMGCycle];
	fErrorExactF = new float [fMgParameters.nMGCycle];

}


PoissonSolver3DCylindricalGPU::PoissonSolver3DCylindricalGPU(int nRRow, int nZColumn, int nPhiSlice)  {	
	fNRRow = nRRow;
	fNZColumn = nZColumn;
 	fPhiSlice = nPhiSlice;
	fErrorConvF = new float [fMgParameters.nMGCycle];
	fErrorExactF = new float [fMgParameters.nMGCycle];
}
/// destructor
PoissonSolver3DCylindricalGPU::~PoissonSolver3DCylindricalGPU() {
	delete fErrorConvF;
	delete fErrorExactF;
	delete fExactSolutionF;
}

/// function overriding
void PoissonSolver3DCylindricalGPU::PoissonSolver3D(float *matricesV, float *matricesCharge,
                                          int nRRow, int nZColumn, int phiSlice, int maxIteration,
                                          int symmetry) {

  fNRRow = nRRow;
  fNZColumn = nZColumn;
  fPhiSlice = phiSlice;

  PoissonMultiGrid3D2D(matricesV, matricesCharge, nRRow, nZColumn, phiSlice, symmetry);
  

}


// method to do multigrid3d2d
void PoissonSolver3DCylindricalGPU::PoissonMultiGrid3D2D(float *VPotential, float * RhoChargeDensities, int nRRow,
                                               int nZColumn, int phiSlice, int symmetry) {

	const float  gridSizeR   =  (PoissonSolver3DCylindricalGPU::fgkOFCRadius-PoissonSolver3DCylindricalGPU::fgkIFCRadius) / (nRRow-1); // h_{r}
  	const float  gridSizePhi =  M_PI/phiSlice;  // h_{phi}
  	const float  gridSizeZ   =  PoissonSolver3DCylindricalGPU::fgkTPCZ0 / (nZColumn-1) ; // h_{z}
 	const float  ratioPhi    =  gridSizeR*gridSizeR / (gridSizePhi*gridSizePhi) ;  // ratio_{phi} = gridsize_{r} / gridsize_{phi}
  	const float  ratioZ      =  gridSizeR*gridSizeR / (gridSizeZ*gridSizeZ) ; // ratio_{Z} = gridsize_{r} / gridsize_{z}
	const float  convErr 	   =  PoissonSolver3DCylindricalGPU::fgConvergenceError;
	const float  IFCRadius   =  PoissonSolver3DCylindricalGPU::fgkIFCRadius;

	int fparamsize = 8;
	float * fparam = new float[fparamsize];

	fparam[0] = gridSizeR;
  	fparam[1] = gridSizePhi;
	fparam[2] = gridSizeZ;
	fparam[3] = ratioPhi;
	fparam[4] = ratioZ;
	fparam[5] = convErr;
	fparam[6] = IFCRadius;

	int iparamsize = 4;
	int * iparam = new int[iparamsize];

	iparam[0] = fMgParameters.nPre;	
	iparam[1] = fMgParameters.nPost;			
	iparam[2] = fMgParameters.maxLoop;
	iparam[3] = fMgParameters.nMGCycle;


	if (fMgParameters.cycleType == kFCycle)
	{
		if (fExactPresent == true) { 
			PoissonMultigrid3DSemiCoarseningGPUErrorFCycle(VPotential, RhoChargeDensities,nRRow, nZColumn,phiSlice,symmetry, fparam, iparam, fExactPresent, fErrorConvF, fErrorExactF, fExactSolutionF);
		} else {
			PoissonMultigrid3DSemiCoarseningGPUErrorFCycle(VPotential, RhoChargeDensities,nRRow, nZColumn,phiSlice,symmetry, fparam, iparam, fExactPresent, fErrorConvF, fErrorExactF, NULL);

		}
	} else if (fMgParameters.cycleType == kWCycle) 
	{
		PoissonMultigrid3DSemiCoarseningGPUErrorWCycle(VPotential, RhoChargeDensities,nRRow, nZColumn,phiSlice,symmetry, fparam, iparam, fErrorConvF, fErrorExactF, fExactSolutionF);
	} else 
	{
		if (fExactPresent == true) {
			PoissonMultigrid3DSemiCoarseningGPUError(VPotential, RhoChargeDensities,nRRow, nZColumn,phiSlice,symmetry, fparam, iparam, fExactPresent, fErrorConvF, fErrorExactF, fExactSolutionF);
		} else {
			PoissonMultigrid3DSemiCoarseningGPUError(VPotential, RhoChargeDensities,nRRow, nZColumn,phiSlice,symmetry, fparam, iparam, fExactPresent, fErrorConvF, fErrorExactF, NULL);
		}
	} 	
	fIterations = iparam[3];
	delete[] fparam;
	delete[] iparam;
}



void PoissonSolver3DCylindricalGPU::SetExactSolution(float*exactSolution,int nRRow, int nZColumn, int phiSlice) {
  	fNRRow = nRRow;
  	fNZColumn = nZColumn;
  	fPhiSlice = phiSlice;
	fExactSolutionF = new float[fNRRow * fPhiSlice,fNZColumn];
	fExactPresent = true;
	fMaxExact = 0.0;;
	for (int i=0;i<nRRow*nZColumn*phiSlice;i++) {
		fExactSolutionF[i] = exactSolution[i];
		if (abs(fExactSolutionF[i]) > fMaxExact) fMaxExact = abs(fExactSolutionF[i]); 
	}
}
