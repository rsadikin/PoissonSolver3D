#ifndef POISSONSOLVER3DCYLINDRICALGPU_H
#define POISSONSOLVER3DCYLINDRICALGPU_H

/// \file PoissonSolver3DCylindricalGPU.h
/// \class PoissonSolver3DCylindricalGPU
/// \brief Kelas ini merupakan interface PoissonSolver 3D dalam koordinat silindrikal 
/// yang diterapkan pada NVDIA Cuda
///
/// \author Rifki Sadikin <rifki.sadikin@cern.ch>, Indonesian Institute of Sciences
/// \date Nov 20, 2017
#include "PoissonSolver3DGPU.h"



class PoissonSolver3DCylindricalGPU  {
public:
  ///< Enumeration of Poisson Solver Strategy Type
  enum StrategyType {
    kRelaxation = 0, ///< S.O.R Cascaded MultiGrid
    kMultiGrid = 1,  ///< Geometric MG
    kFastRelaxation = 2       ///< Spectral (TODO)
  };

  ///< Enumeration of Cycles Type
  enum CycleType {
    kVCycle = 0,    ///< V Cycle
    kWCycle = 1,    ///< W Cycle (TODO)
    kFCycle = 2      ///< Full Cycle
  };

  ///< Fine -> Coarse Grid transfer operator types
  enum GridTransferType {
    kHalf = 0, ///< Half weighting
    kFull = 1, ///< Full weighting
  };

  ///< Smoothing (Relax) operator types
  enum RelaxType {
    kJacobi = 0,      ///< Jacobi (5 Stencil 2D, 7 Stencil 3D_
    kWeightedJacobi = 1, ///< (TODO)
    kGaussSeidel = 2  ///< Gauss Seidel 2D (2 Color, 5 Stencil), 3D (7 Stencil)
  };

  ///< Coarse -> fine  operator types (TODO: Interp and Restrict in one packet, just one enumeration)
  enum InterpType {
    kHalfInterp = 0,    ///< Half bi linear interpolation
    kFullInterp = 1    ///< Full bi linear interpolation
  };

  ///< Parameters choice for MultiGrid 	algorithm
  struct MGParameters {
    bool isFull3D;    ///<  TRUE: full coarsening, FALSE: semi coarsening
    CycleType cycleType;  ///< cycleType follow  CycleType
    GridTransferType gtType; ///< gtType grid transfer type follow GridTransferType
    RelaxType relaxType; ///< relaxType follow RelaxType
    int gamma;  ///< number of iteration at coarsest level
    int nPre;    ///< number of iteration for pre smoothing
    int nPost;  ///< number of iteration for post smoothing
    int nMGCycle; ///< number of multi grid cycle (V type)
    int maxLoop;  ///< the number of tree-deep of multi grid


    // default values
    MGParameters() {
      isFull3D = false;
      cycleType = kFCycle;
      gtType = kFull; // default full
      relaxType = kGaussSeidel; // default relaxation method
      nPre = 2;
      nPost = 2;
      nMGCycle = 200;
      maxLoop = 6;

    }
  };



  static const float fgkTPCZ0;       ///< nominal gating grid position
  static const float fgkIFCRadius;   ///< Mean Radius of the Inner Field Cage ( 82.43 min,  83.70 max) (cm)
  static const float fgkOFCRadius;   ///< Mean Radius of the Outer Field Cage (252.55 min, 256.45 max) (cm)
  static const float fgkZOffSet;     ///< Offset from CE: calculate all distortions closer to CE as if at this point
  static const float fgkCathodeV;    ///< Cathode Voltage (volts)
  static const float fgkGG;          ///< Gating Grid voltage (volts)
  static const float fgkdvdE;        ///< [cm/V] drift velocity dependency on the E field (from Magboltz for NeCO2N2 at standard environment)
  static const float fgkEM;          ///< charge/mass in [C/kg]
  static const float fgke0;          ///< vacuum permittivity [A·s/(V·m)]

  static float fgExactErr; ///< Error tolerated
  static float fgConvergenceError; ///< Error tolerated
  int fIterations; ///< number of maximum iteration
  MGParameters fMgParameters;   ///< parameters multi grid

  void SetExactSolution(float *exactSolution, const int fPhiSlices);
  void SetCycleType(PoissonSolver3DCylindricalGPU::CycleType cycleType) {
    fMgParameters.cycleType = cycleType;
  }

  StrategyType fStrategy;  ///< strategy used default multiGrid
  PoissonSolver3DCylindricalGPU();
  PoissonSolver3DCylindricalGPU(int nRRow, int nZColumn, int nPhiSlice);
  PoissonSolver3DCylindricalGPU(const char *name, const char *title);
  virtual ~PoissonSolver3DCylindricalGPU();
  void PoissonSolver3D(float *matricesV, float *matricesChargeDensities, int nRRow, int nZColumn,
                       int phiSlice, int maxIterations, int symmetry);


  // setter and getter
  void SetStrategy(StrategyType strategy) {fStrategy = strategy;}
  StrategyType GetStrategy() { return fStrategy; }
  void SetExactSolution(float *exactSolution,int nRRow, int nZColumn, int phiSlice);
  float *fExactSolutionF;

  float GetErrorConv(int iteration) {return fErrorConvF[iteration];	}
  float GetErrorExact(int iteration) {return fErrorExactF[iteration];	}
private:
  PoissonSolver3DCylindricalGPU(const PoissonSolver3DCylindricalGPU &);               // not implemented
  PoissonSolver3DCylindricalGPU &operator=(const PoissonSolver3DCylindricalGPU &);    // not implemented
  void PoissonMultiGrid3D2D(float *VPotential, float *RhoChargeDensities, int nRRow,
                            int nZColumn, int phiSlice, int symmetry);



  // store potential and charge
  float * fVPotential; //-> hold potential in an object of tmatrixd 
  float * fRhoCharge; //-> pointer to an object of tmatrixd for storing charge 
  int fNRRow;
  int fNZColumn;
  int fPhiSlice;
  // error single precision for cuda-based
  float *fErrorConvF;
  float *fErrorExactF;
  bool fExactPresent;
  float fMaxExact;
};

#endif
