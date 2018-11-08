
/// \file PoissonSolver3DGPUTest.h
/// \brief Berkas ini berisi definisi fungs untuk memakai pustaka libPoissonSolver3DCylindrialGPU.so
///
/// \author Rifki Sadikin <rifki.sadikin@lipi.go.id>, Pusat Penelitian Informatika, Lembaga Ilmu Pengetahuan Indonesia
/// \author I Wayan Aditya Swardiana  <i.wayan.aditya.swardiana@lipi.go.id>, Pusat Penelitian Informatika, Lembaga Ilmu Pengetahuan Indonesia
/// \date November 8, 2018
#ifndef POISSONSOLVERGPU3DGPUTEST_H
#define POISSONSOLVERGPU3DGPUTEST_H

#include <iostream>
#include "PoissonSolver3DCylindricalGPU.h"

const double fgkTPCZ0 = 249.7;     ///< nominal gating grid position
const double fgkIFCRadius = 83.5;     ///< radius which renders the "18 rod manifold" best -> compare calc. of Jim Thomas
const double fgkOFCRadius = 254.5;     ///< Mean Radius of the Outer Field Cage (252.55 min, 256.45 max) (cm)
const double fgConvergenceError = 1e-6;                 ///< vacuum permittivity [A·s/(V·m)]
void DoPoissonSolverExperiment(const int kRows, const int kColumns, const int kPhiSlices,  const int kIterations, const int kSymmetry);
void InitVoltandCharge3D(float * VPotentialExact,float * VPotential,float * RhoCharge,const int kRows, const int kColumns,const int kPhiSlices,float gridSizeR,float gridSizeZ,float gridSizePhi);
float TestFunction1PotentialEval(double a, double b, double c,float radius0, float phi0, float z0);
float TestFunction1ChargeEval(double a, double b, double  c, float radius0,float phi0,float z0);





#endif
