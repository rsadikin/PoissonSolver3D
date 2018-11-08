#include <iostream>
#include <math.h>

#include <iostream>
#include <cstdio>
#include <ctime>

#include "PoissonSolver3DGPUTest.h"
/// \file berkas PoissonSolver3DGPUTest.cpp berisi implementasi pengujian pustaka PoissonSolver3DCylindricalGPU
///
/// dibuat oleh: Rifki Sadikin (rifki.sadikin@lipi.go.id)
/// tanggal: 7 November 2018

/// Lakukan eksperimen poisson solver
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


 /**
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
**/
	/**
	for (int k=0;k<1;k++) {
	for (int i=0;i<kRows;i++) {
			for (int j=0;j< kColumns;j++) printf("%.3f\t",VPotentialExact[k* (kRows * kColumns) + i * kColumns + j]);
			printf("\n");
	}
	printf("\n");
	}
 **/

  
	// create poissonSolver
	PoissonSolver3DCylindricalGPU *poissonSolver = new PoissonSolver3DCylindricalGPU();
	
	PoissonSolver3DCylindricalGPU::fgConvergenceError = 1e-6;
	// zeroring array of error
	poissonSolver->SetExactSolution(VPotentialExact,kRows,kColumns, kPhiSlices);
	
	
	
	// Case 1. Set the strategy as multigrid, fullmultigrid, and full 3d
	poissonSolver->SetStrategy(PoissonSolver3DCylindricalGPU::kMultiGrid);	
	poissonSolver->SetCycleType(PoissonSolver3DCylindricalGPU::kFCycle);

	// TStopwatch w;
	std::clock_t start,stop;
	double duration;

    	start = std::clock();
 
	//w.Start();
	poissonSolver->PoissonSolver3D(VPotential,RhoCharge,kRows,kColumns,kPhiSlices, kIterations,kSymmetry) ;
	//w.Stop();
	stop = std::clock();
	/**
	for (int k=0;k<1;k++) {
	for (int i=0;i<kRows;i++) {
			for (int j=0;j< kColumns;j++) printf("%.3f\t",VPotential[k* (kRows * kColumns) + i * kColumns + j]);
			printf("\n");
	}
	printf("\n");
	}**/
	
    	duration = ( stop - start ) / (double) CLOCKS_PER_SEC;

	std::cout<<"Poisson Solver 3D Cylindrical GPU test" << '\n';
	std::cout<<"Ukuran grid (r,phi,z) = (" << kRows << "," << kColumns << "," << kPhiSlices <<") \n";
    	std::cout<<"Waktu komputasi: \t\t\t"<< duration <<" s \n";
	std::cout<<"Jumlah iterasi siklus multigrid: \t"<< poissonSolver->fIterations << '\n';
	std::cout<<"Iterasi\tError Convergen\t\tError Absolut \n";
	std::cout<< std::scientific;
	for (int i=0;i<poissonSolver->fIterations;i++) {
		std::cout<<"[" <<  i  << "]:\t" << poissonSolver->GetErrorConv(i) << "\t" << poissonSolver->GetErrorExact(i) << '\n' << std::scientific;
	}
	delete poissonSolver;
	delete VPotential;
	delete VPotentialExact;
	delete RhoCharge;
}



/// Inisisasi nilai volt dan charge
void InitVoltandCharge3D(float * VPotentialExact,float *VPotential,float * RhoCharge,const int kRows, const int kColumns,const int kPhiSlices,float gridSizeR,float gridSizeZ,float gridSizePhi) {

  //TFormula vTestFunction1("f1", "[0]*(x^4 - 338.0 *x^3 + 21250.75 * x^2)*cos([1]* y)^2*exp(-1* [2] * z^2)");
  //TFormula rhoTestFunction1("ff1", "[0]*(((16.0 * x^2 - 9.0 * 338.0 * x + 4.0*21250.75) *cos([1] * y)^2 * exp(-1 *[2]*z^2)) - ((x^2 -  338.0 * x + 21250.75) * 2 * [1]^2 * cos(2 * [1] * y) * exp(-1 *[2]*z^2)) + ((x^4 -  338.0 * x^3 + 21250.75 * x^2) * cos([1] * y)^2 * (4*[2]^2*z^2 - 2 * [2]) * exp(-1 *[2]*z^2)))");
		
  double  rlist[kRows], zedlist[kColumns] , philist[kPhiSlices];
  float phi0,radius0,z0;
  double a,b,c;
a = 1e-7;
b = 0.5;
c = 1e-4; 
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
			RhoCharge[index] = -1 * TestFunction1ChargeEval(a,b,c,radius0,phi0,z0);	
			
			if (j == 0) VPotential[index] = VPotentialExact[index];
			else if (j == kColumns-1) VPotential[index] = VPotentialExact[index];
			else if (i == 0) VPotential[index] = VPotentialExact[index];
			else if (i == kRows - 1) VPotential[index] = VPotentialExact[index];
			else VPotential[index ] = 0.0;	
		} // end j
	} // end i		
			
    } // end phi  
	
}

/// analytic function untuk potensial
float TestFunction1PotentialEval(double a, double b, double  c, float radius0,float phi0,float z0) {
	
  //TFormula vTestFunction1("f1", "[0]*(x^4 - 338.0 *x^3 + 21250.75 * x^2)*cos([1]* y)^2*exp(-1* [2] * z^2)");
	float ret = a * (pow(radius0,4) - 338.0 * pow(radius0,3) + 21250.75 * pow(radius0,2)) * pow(cos(b*phi0),2) * exp ( -1 * c * z0*z0);


	return ret;
}			

/// analytic function untuk charge
float TestFunction1ChargeEval(double a, double b, double  c, float radius0,float phi0,float z0) {
	
  //TFormula rhoTestFunction1("ff1", "[0]*(((16.0 * x^2 - 9.0 * 338.0 * x + 4.0*21250.75) *cos([1] * y)^2 * exp(-1 *[2]*z^2)) - ((x^2 -  338.0 * x + 21250.75) * 2 * [1]^2 * cos(2 * [1] * y) * exp(-1 *[2]*z^2)) + ((x^4 -  338.0 * x^3 + 21250.75 * x^2) * cos([1] * y)^2 * (4*[2]^2*z^2 - 2 * [2]) * exp(-1 *[2]*z^2)))");
	
	float ret = a * (((16.0 * pow(radius0,2) - 9.0 * 338.0 * radius0 + 4.0*21250.75 ) * pow(cos (b * phi0),2) *exp(-1 * c * z0 * z0) ) - ((pow(radius0,2) - 338.0 * radius0 + 21250.75) *2 * b*b* cos(2 * b * phi0)  * exp(-1 * c *z0 * z0) ) + ((pow(radius0,4) - 338.0 * pow(radius0,3) + 21250.75 * pow(radius0,2)) * pow(cos(b * phi0),2) * (4.0 *c*c *z0*z0 - 2 *c) * exp(-1 * c * z0 * z0))) ;
	return ret;
}			
	
	
// fungsi main untuk test
int main() {

	
    	DoPoissonSolverExperiment(17, 17, 18,  200, 0);
	std::cout << "\n";
    	DoPoissonSolverExperiment(33, 33, 36,  200, 0);
	std::cout << "\n";
   	DoPoissonSolverExperiment(65, 65, 72,  200, 0);
	std::cout << "\n";
    	DoPoissonSolverExperiment(129, 129, 144,  200, 0);
	std::cout << "\n";
    	DoPoissonSolverExperiment(257, 257, 288,  200, 0);
	std::cout << "\n";
	return 0;
}
