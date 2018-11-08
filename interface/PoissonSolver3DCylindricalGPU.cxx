/// \author Rifki Sadikin <rifki.sadikin@lipi.go.id>, Indonesian Institute of Sciences
/// \date Nov 8, 2018

#include <math.h>
#include "PoissonSolver3DCylindricalGPU.h"

const float PoissonSolver3DCylindricalGPU::fgkTPCZ0 = 249.7;     ///< nominal gating grid position
const float PoissonSolver3DCylindricalGPU::fgkIFCRadius = 83.5;     ///< radius which renders the "18 rod manifold" best -> compare calc. of Jim Thomas
const float PoissonSolver3DCylindricalGPU::fgkOFCRadius = 254.5;     ///< Mean Radius of the Outer Field Cage (252.55 min, 256.45 max) (cm)
const float PoissonSolver3DCylindricalGPU::fgkZOffSet = 0.2;     ///< Offset from CE: calculate all distortions closer to CE as if at this point
const float PoissonSolver3DCylindricalGPU::fgkCathodeV = -100000.0; ///< Cathode Voltage (volts)
const float PoissonSolver3DCylindricalGPU::fgkGG = -70.0; ///< Gating Grid voltage (volts)
const float PoissonSolver3DCylindricalGPU::fgkdvdE = 0.0024; ///< [cm/V] drift velocity dependency on the E field (from Magboltz for NeCO2N2 at standard environment)
const float PoissonSolver3DCylindricalGPU::fgkEM = -1.602176487e-19 / 9.10938215e-31; ///< charge/mass in [C/kg]
const float PoissonSolver3DCylindricalGPU::fgke0 = 8.854187817e-12;                 ///< vacuum permittivity [A·s/(V·m)]

float PoissonSolver3DCylindricalGPU::fgExactErr = 1e-4;
float PoissonSolver3DCylindricalGPU::fgConvergenceError = 1e-3;

/// constructor
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
