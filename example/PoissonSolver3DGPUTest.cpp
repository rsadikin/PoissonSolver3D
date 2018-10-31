#include <iostream>
#include <math.h>
#include "PoissonSolver3DGPUTest.h"
#include "PoissonSolver3DGPU.h"
///
/// DoPoissonSolverExperiments
///
void DoPoissonSolverExperiment(const int kRows, const int kColumns, const int kPhiSlices,  const int kIterations, const int kSymmetry)   {
		
	
	int kPhiSlicesPerSector = kPhiSlices/18;
  
  	const float gridSizeR   =  (fgkOFCRadius-fgkIFCRadius) / (kRows-1) ;
  	const float gridSizeZ   =  fgkTPCZ0 / (kColumns-1) ;
	
 	const float gridSizePhi =  (M_PI * 2)/ ( 18.0 * kPhiSlicesPerSector);

	int size = kRows * kColumns * kPhiSlices;

	float * VPotential = new float[size];
	float * VPotentialExact  = new float[size]; 	
	float * RhoCharge  = new float[size]; 	
	float * errorConv  = new float[200]; 	
	float * errorExact  = new float[200]; 	
	
	InitVoltandCharge3D(VPotentialExact,VPotential,RhoCharge,kRows,kColumns,kPhiSlices,gridSizeR,gridSizeZ,gridSizePhi);


 	const float  ratioPhi    =  gridSizeR*gridSizeR / (gridSizePhi*gridSizePhi) ;  // ratio_{phi} = gridsize_{r} / gridsize_{phi}
  	const float  ratioZ      =  gridSizeR*gridSizeR / (gridSizeZ*gridSizeZ) ; // ratio_{Z} = gridsize_{r} / gridsize_{z}
	const float  convErr 	   =  fgConvergenceError;
	const float  IFCRadius   =  fgkIFCRadius;

	const int  fparamsize = 8;
	float * fparam = new float[fparamsize];

	fparam[0] = gridSizeR;
  	fparam[1] = gridSizePhi;
	fparam[2] = gridSizeZ;
	fparam[3] = ratioPhi;
	fparam[4] = ratioZ;
	fparam[5] = convErr;
	fparam[6] = IFCRadius;

	int  iparamsize = 4;
	int  * iparam = new int[iparamsize];

	iparam[0] = 2;//nPre	
	iparam[1] = 2;//nPost;			
	iparam[2] = 6;//maxLoop;
	iparam[3] = 200; //nMGCycle;
	for (int k=0;k<kPhiSlices;k++) {
	for (int i=0;i<kRows;i++) {
			for (int j=0;j< kColumns;j++) printf("%.3f\t",RhoCharge[k * (kRows * kColumns) + i * kColumns + j]);
			printf("\n");
	}
	printf("\n");
	}
	// VCycle
	PoissonMultigrid3DSemiCoarseningGPUError(VPotential, RhoCharge, kRows, kColumns ,kPhiSlices, 0 , fparam, iparam, true, errorConv,errorExact, VPotentialExact);
  	// Call poisson solver

	
	for (int k=0;k<kPhiSlices;k++) {
	for (int i=0;i<kRows;i++) {
			for (int j=0;j< kColumns;j++) printf("%.3f\t",VPotential[k * (kRows * kColumns) + i * kColumns + j]  - VPotentialExact[k* (kRows * kColumns) + i * kColumns + j]);
			printf("\n");
	}
	printf("\n");
	}

	/**
	
	TVectorD *error[5];
	TVectorD *errorConv[5];
	int iterations[5];
	
	// memory allocation 
  	TMatrixD *arrayofArrayV[kPhiSlices], *arrayofCharge[kPhiSlices] ;
	TMatrixD *arrayofArrayVGrid[kPhiSlices], *arrayofChargeGrid[kPhiSlices] ;
	TMatrixD *arrayofArrayVExact[kPhiSlices];
  
  	for ( int k = 0 ; k < kPhiSlices ; k++ ) {
    		arrayofArrayV[k]     =   new TMatrixD(kRows,kColumns) ;
		arrayofArrayVExact[k]     =   new TMatrixD(kRows,kColumns) ;
    		arrayofCharge[k]     =   new TMatrixD(kRows,kColumns) ;    
		arrayofArrayVGrid[k]     =   new TMatrixD(kRows,kColumns) ;
    		arrayofChargeGrid[k]     =   new TMatrixD(kRows,kColumns) ;    
  	}


	// side in TPC chamber
	int side = 0;
	
	/// Generate exact problems -- solutios pair	
	InitVoltandCharge3D(arrayofArrayVExact,arrayofChargeGrid,kRows,kColumns,kPhiSlices,side,gridSizeR,gridSizeZ,gridSizePhi,1);
	
	
	/// zeroing potential for inital guess
	for ( int k = 0 ; k < kPhiSlices ; k++ ) {		
    *arrayofArrayVGrid[k]     =   *arrayofArrayVExact[k];
		*arrayofArrayV[k] =  *arrayofArrayVExact[k];
		for ( int i = 1 ; i < kRows-1 ; i++ ) {
			for ( int j = 1 ; j < kColumns-1 ; j++ ) {
			(*arrayofArrayVGrid[k])(i,j) = 0.0;
			(*arrayofArrayV[k])(i,j) = 0.0;
			}
		}		
	}
	
  
	// create poissonSolver
	AliTPCPoissonSolverCuda *poissonSolver = new AliTPCPoissonSolverCuda();
	
	AliTPCPoissonSolverCuda::fgConvergenceError = 1e-8;
	// zeroring array of error
	poissonSolver->SetExactSolution(arrayofArrayVExact,kRows,kColumns, kPhiSlices);
	
	
	
	// Case 1. Set the strategy as multigrid, fullmultigrid, and full 3d
	poissonSolver->SetStrategy(kMultiGrid);	
	poissonSolver->SetCycleType(kFCycle);

	TStopwatch w; 
	w.Start();
	poissonSolver->PoissonSolver3D(arrayofArrayVGrid,arrayofChargeGrid,kRows,kColumns,kPhiSlices, kIterations,kSymmetry) ;
	w.Stop();

	TMatrixD vError(kRows,kColumns);
	arrayofArrayVGrid[0]->Print();

	
	::Info("testAliTPCPoissonSolverMem3D_Consistency",Form("Time Poisson Multigrid F-Cycle 3D: = %f \n",w.CpuTime()));

	delete poissonSolver;

  	for ( int k = 0 ; k < kPhiSlices ; k++ ) {
    		delete arrayofArrayV[k];
		delete arrayofArrayVExact[k];
    		delete arrayofCharge[k];    
		delete arrayofArrayVGrid[k];
    		delete arrayofChargeGrid[k];    
  	}
	**/
	delete VPotential;
	delete VPotentialExact;
	delete RhoCharge;
	delete[] iparam;
	delete[] fparam;
}



// set init
void InitVoltandCharge3D(float * VPotentialExact,float *VPotential,float * RhoCharge,const int kRows, const int kColumns,const int kPhiSlices,float gridSizeR,float gridSizeZ,float gridSizePhi) {

		
  double  rlist[kRows], zedlist[kColumns] , philist[kPhiSlices];
  float phi0,radius0,z0;
  double a,b,c;
  a = fgkOFCRadius*fgkOFCRadius;
  a*= (fgkOFCRadius - fgkIFCRadius); 
  a*= (fgkOFCRadius - fgkIFCRadius);
  a =  (100.0/a);
  b = 0.5;
  c = M_E / (fgkTPCZ0 * fgkTPCZ0 );
 
   int index;
	// list points on grid in cm
  for ( int k = 0 ; k < kPhiSlices ; k++ ) 
    philist[k] =  gridSizePhi * k;
  for ( int i = 0 ; i < kRows ; i++ )    
    rlist[i] = fgkIFCRadius + i*gridSizeR ;
  for ( int j = 0 ; j < kColumns ; j++ ) 
    zedlist[j]  = j * gridSizeZ ;
	
    for ( int k = 0 ; k < kPhiSlices ; k++ )  {
	phi0    = philist[k];
	for ( int i = 0 ; i < kRows ; i++ ) {
		radius0 = rlist[i] ;
		for ( int j = 0 ; j < kColumns ; j++ ) {

			index = k * kRows * kColumns + i * kColumns  + j;

			z0 = zedlist[j];

			VPotentialExact[index] = TestFunction1PotentialEval(a,b,c,radius0,phi0,z0);			
			RhoCharge[index] = TestFunction1ChargeEval(a,b,c,radius0,phi0,z0);	
			
			if (j == 0) VPotential[index] = VPotentialExact[index];
			else if (j == kColumns-1) VPotential[index] = VPotentialExact[index];
			else if (i == 0) VPotential[index] = VPotentialExact[index];
			else if (i == kRows - 1) VPotential[index] = VPotentialExact[index];
			else VPotential[index ] = 0.0;	
		} // end j
	} // end i		
			
    } // end phi  
	
}

//
float TestFunction1PotentialEval(double a, double b, double  c, float radius0,float phi0,float z0) {
	
	float ret = a * (pow(radius0,4) - 338.0 * pow(radius0,3) + 21250.75 * pow(radius0,2));
	ret *= cos(b*phi0);
	ret *= exp ( -1 * c * z0*z0);


	return ret;
}			
//
float TestFunction1ChargeEval(double a, double b, double  c, float radius0,float phi0,float z0) {
	
	
	float ret = a * (((16.0 * pow(radius0,2) - 9.0 * 338.0 * radius0 + 4.0*21250.75 ) * pow(cos (b * phi0),2.0) *exp(-1 * c * z0 * z0) ) - ((pow(radius0,2.0) - 338.0 * radius0 + 21250.75) *2 * b*b* cos(2 * b * phi0)  * exp(-1 * c *z0 * z0) ) + ((pow(radius0,4.0) - 338.0 * pow(radius0,3.0) + 21250.75 * pow(radius0,2.0)) * pow(cos(b * phi0),2.0) * (4 *c*c *z0*z0 - 2 *c) * exp(-1 * c * z0 * z0))) ;
	return ret;
}			
	
	


int main() {

	
    	DoPoissonSolverExperiment(17, 17, 18,  200, 0);
	return 0;
}
