#ifndef POISSONSOLVERGPU3DGPUTEST_H
#define POISSONSOLVERGPU3DGPUTEST_H

#include <iostream>
#include "PoissonSolver3DCylindricalGPU.h"

const double fgkTPCZ0 = 249.7;     ///< nominal gating grid position
const double fgkIFCRadius = 83.5;     ///< radius which renders the "18 rod manifold" best -> compare calc. of Jim Thomas
const double fgkOFCRadius = 254.5;     ///< Mean Radius of the Outer Field Cage (252.55 min, 256.45 max) (cm)
const double fgkZOffSet = 0.2;     ///< Offset from CE: calculate all distortions closer to CE as if at this point
const double fgkCathodeV = -100000.0; ///< Cathode Voltage (volts)
const double fgkGG = -70.0; ///< Gating Grid voltage (volts)
const double fgkdvdE = 0.0024; ///< [cm/V] drift velocity dependency on the E field (from Magboltz for NeCO2N2 at standard environment)
const double fgkEM = -1.602176487e-19 / 9.10938215e-31; ///< charge/mass in [C/kg]
const double fgke0 = 8.854187817e-12;                 ///< vacuum permittivity [A·s/(V·m)]
const double fgConvergenceError = 1e-6;                 ///< vacuum permittivity [A·s/(V·m)]
void DoPoissonSolverExperiment(const int kRows, const int kColumns, const int kPhiSlices,  const int kIterations, const int kSymmetry);
void InitVoltandCharge3D(float * VPotentialExact,float * VPotential,float * RhoCharge,const int kRows, const int kColumns,const int kPhiSlices,float gridSizeR,float gridSizeZ,float gridSizePhi);
float TestFunction1PotentialEval(double a, double b, double c,float radius0, float phi0, float z0);
float TestFunction1ChargeEval(double a, double b, double  c, float radius0,float phi0,float z0);





#endif
