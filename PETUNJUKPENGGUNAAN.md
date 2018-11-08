# Petunjuk Penggunaan
```
pembuat : Rifki Sadikin (rifki.sadikin@lipi.go.id)
tanggal : 6 November 2018
```

Petunjuk penggunaan ini berisi:

 - Cara **membuat** dan **menginstal** Pustaka Pemercepat Komputasi  berbasis GPU CUDA untuk Penyelesaian Persamaan *Poisson* dalam Koordinat Silindrikal. 
 - Cara **memakai** Pustaka Pemercepat Komputasi  berbasis GPU CUDA untuk Penyelesaian Persamaan *Poisson* dalam Koordinat Silindrikal dalam C++.

## Struktur Berkas
Struktur berkas  pustaka Pemercepat Komputasi  berbasis GPU CUDA untuk Penyelesaian Persamaan *Poisson* dalam Koordinat Silindrikal adalah sebagai berikut:

``` bash
|-- CMakeLists.txt
|-- docs
|   `-- Doxyfile.in
|-- example
|   |-- CMakeLists.txt
|   |-- PoissonSolver3DGPUTest.cpp
|   `-- PoissonSolver3DGPUTest.h
|-- interface
|   |-- PoissonSolver3DCylindricalGPU.cxx
|   `-- PoissonSolver3DCylindricalGPU.h
|-- kernel
|   |-- PoissonSolver3DGPU.cu
|   `-- PoissonSolver3DGPU.h
|-- PETUNJUKPENGGUNAAN.md
`-- README.md
```
## Modul Program yang Dibutuhkan
Untuk menginstall pustaka dibutuhkan program pengembang di lingkungan sistem operasi Linux (saat ini masih di Linux) yaitu:

 1. CMAKE versi  minimal 3.10.3
 2. GCC versi minimal 6.2.0
 3. CUDA versi minimal 8.0
 4. GIT versi minimal 1.8.3.1

Jika disediakan module program pada lingkungan sistem HPC maka dapat menjalankan perintah berikut ini:


```console
module load cmake/3.10.3
module load gcc/6.2.0
module load cuda
```
## Instalasi
Untuk menginstall pustaka PoissonSolver3D lakukan langkah-langkah berikut:

 1. Jalankan git clone untuk menyalin kode sumber ke direktori local. Perintah ini akan menyalin kode sumber  ke direktori **PoissonSolver3D**
```console
$ git clone http://github.com/rsadikin/PoissonSolver3D.git
```
 2. Pindah ke direktori PoissonSolver3D, dan lihat daftar direktori
 ```console
$ cd PoissonSolver3D
$ tree .
.
|-- CMakeLists.txt
|-- docs
|   `-- Doxyfile.in
|-- example
|   |-- CMakeLists.txt
|   |-- PoissonSolver3DGPUTest.cpp
|   `-- PoissonSolver3DGPUTest.h
|-- interface
|   |-- PoissonSolver3DCylindricalGPU.cxx
|   `-- PoissonSolver3DCylindricalGPU.h
|-- kernel
|   |-- PoissonSolver3DGPU.cu
|   `-- PoissonSolver3DGPU.h
|-- PETUNJUKPENGGUNAAN.md
`-- README.md
```
 3.  Buatlah folder **buildpoissonsolver** di luar direktori kode sumber
 ```console
 $ mkdir $HOME/buildpoissonsolver
 ```
 4.  Pindah ke folder **buildpoissonsolver** 

 ```console
 $ cd $HOME/buildpoissonsolver
 ```
5. Jalankan  **cmake** di folder **buildpoissonsolver** dengan menyatakan di mana pustaka akan diinstal dengan perintah sebagai berikut
```console
$ export  PSLIB=/home/usertest/trypoissonsolver/PoissonSolver3D
$ export PSLIB_INSTALL=/home/usertest/buildpoissonsolver
$ cmake $PSLIB/ -DCMAKE_INSTALL_PREFIX=$PSLIB_INSTALL
-- The C compiler identification is GNU 6.2.0
-- The CXX compiler identification is GNU 6.2.0
-- Check for working C compiler: /apps/tools/gcc-6.2.0/bin/gcc
-- Check for working C compiler: /apps/tools/gcc-6.2.0/bin/gcc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /apps/tools/gcc-6.2.0/bin/c++
-- Check for working CXX compiler: /apps/tools/gcc-6.2.0/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Looking for pthread_create
-- Looking for pthread_create - not found
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - found
-- Found Threads: TRUE
-- Found CUDA: /apps/tools/cuda-9.1 (found version "9.1")
-- Building SpaceCharge distortion framework with CUDA support
-- Found Doxygen: /usr/bin/doxygen (found version "1.8.5") found components:  doxygen dot
Doxygen build started
-- Configuring done
-- Generating done
-- Build files have been written to: /home/usertest/buildpoissonsolver
```
6. Kompail kode sumbernya
```console
$ make
```
7. Install modul PoissonSolver3D di folder **buildpoissonsolver** 
```console
$ make install
..
-- Install configuration: ""
-- Installing: /home/usertest/trypoissonsolver/buildpoissonsolver/lib/libPoissonSolver3DCylindricalGPU.so
-- Installing: /home/usertest/trypoissonsolver/buildpoissonsolver/include/PoissonSolver3DCylindricalGPU.h
-- Installing: /home/usertest/trypoissonsolver/buildpoissonsolver/include/PoissonSolver3DGPU.h
```

Hasil instalasi adalah sebuah pustaka yang dapat digunakan (**shared library**) yaitu libPoissonSolver3DCylindricalGPU.so pada direktori **lib** dan 2 berkas header yang mengandung definisi kelas /fungsi sehingga pengguna pustaka dapat menggunakannya.

## Penggunaan Pustaka 

### Penggunaan Dalam CMAKE 

Dalam folder **example** di struktur direktori sumber terdapat contoh penggunaan pustaka libpoissonsolvergpu.so dengan menggunakan **cmake**. Berikut ini langkah-langkahnya:

1. Buat file **CMakeLists.txt** dengan struktur sebagai berikut untuk menambahkan pustaka cuda (**libcuda.so**) dan pustaka PoissonSolver3DCylindricalGPU (**libPoissonSolver3DCylindricalGPU.so**) pada proyek cmake:



```cmake
cmake_minimum_required (VERSION 2.8.11)
project (3DPoissonSolverGPUTest)

find_package(CUDA)
if(NOT CUDA_FOUND)
    message( FATAL_ERROR "NVIDIA CUDA package not found" )
else()
    find_library(LIBCUDA_SO_PATH libcuda.so)
    string(FIND ${LIBCUDA_SO_PATH} "-NOTFOUND" LIBCUDA_SO_PATH_NOTFOUND )
endif(NOT CUDA_FOUND)
message( STATUS "Building Poisson Solver  with CUDA support" )

if(LIBCUDA_SO_PATH_NOTFOUND GREATER -1)
  message( FATAL_ERROR "NVIDIA CUDA libcuda.so not found" )
endif(LIBCUDA_SO_PATH_NOTFOUND GREATER -1)


#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/cuda" ${CMAKE_MODULE_PATH})
#find_package(CUDA QUIET REQUIRED)
set(PSLIBNAME libPoissonSolver3DCylindricalGPU.so)

find_library(PSLIB ${PSLIBNAME})
string(FIND ${PSLIB} "-NOTFOUND" PSLIB_NOTFOUND )

if(PSLIB_NOTFOUND GREATER -1)
  message( FATAL_ERROR "Poisson Solver Cuda Library libPoissonSolver3DCylindricalGPU.o not found" )
endif(PSLIB_NOTFOUND GREATER -1)

```
Setelah itu, baru tambahkan perintah pada CMakeLists.txt (dilanjutkan) untuk mengikut sertakan kode sumber user yaitu:

```
# tambah disini kode sumber user
set(CPP_SOURCE PoissonSolver3DGPUTest.cpp)
set(HEADERS PoissonSolver3DGPUTest.h)

set(TARGET_NAME poissonsolvergputest)

add_executable(${TARGET_NAME}
  PoissonSolver3DGPUTest.cpp
)


# ikut sertakan shared library cuda dan poisson solver
target_link_libraries(${TARGET_NAME} ${PSLIB} ${LIBCUDA_SO_PATH})
```

2. Pada kode sumber include header file sehingga definisi fungsi dan kelas dapat dipanggil di badan kode.


```c++
#include "PoissonSolver3DCylindricalGPU.h"

...
// create poissonSolver
PoissonSolver3DCylindricalGPU *poissonSolver = new PoissonSolver3DCylindricalGPU();
PoissonSolver3DCylindricalGPU::fgConvergenceError = 1e-8;
poissonSolver->SetExactSolution(VPotentialExact,kRows,kColumns, kPhiSlices);
poissonSolver->SetStrategy(PoissonSolver3DCylindricalGPU::kMultiGrid);
poissonSolver->SetCycleType(PoissonSolver3DCylindricalGPU::kFCycle);
poissonSolver->PoissonSolver3D(VPotential,RhoCharge,kRows,kColumns,kPhiSlices, kIterations,kSymmetry) ;
```


3. Buat folder terpisah untuk mebangun proyek yang menggunakan pustaka misal **buildexamplepoissonsolver** lalu jalankan **cmake** dengan flag spesial yaitu **-DCMAKE_PREFIX_PATH** yang ditetapkan dengan absolute path tempat direktori PoissonSolver3DCylindricalGPU dibangun.

```console
$ mkdir buildexamplepoissonsolver
$ cd buildexamplepoissonsolver
$ cmake ../PoissonSolver3D/example/ -DCMAKE_PREFIX_PATH=/home/usertest/trypoissonsolver/buildpoissonsolver
...
-- Found CUDA: /apps/tools/cuda-9.1 (found version "9.1")
-- Building Poisson Solver  with CUDA support
-- Configuring done
-- Generating done
-- Build files have been written to: /home/usertest/trypoissonsolver/buildexamplepoissonsolver
```
4. Jalankan **make**  untuk membuat *executable file* 

```console
$ make

-- Building Poisson Solver  with CUDA support
-- Configuring done
-- Generating done
-- Build files have been written to: /home/usertest/trypoissonsolver/buildexamplepoissonsolver
[ 50%] Linking CXX executable poissonsolvergputest
[100%] Built target poissonsolvergputest
```  
5. Hasil dari **make** adalah program yang dapat dieksekusi yang berjalan di GPU card
6.
```console
$ ./poissonsolvergputest
Poisson Solver 3D Cylindrical GPU test
Ukuran grid (r,phi,z) = (17,17,18)
Waktu komputasi: 			0.55 s
Jumlah iterasi siklus multigrid: 	5
Iterasi	Error Convergen		Error Absolut
[0]:	8.362587e-01	2.430797e-03
[1]:	5.126660e-02	4.434380e-04
[2]:	9.490424e-03	1.892288e-04
[3]:	2.084086e-03	1.314385e-04
[4]:	1.141812e-03	1.217592e-04

Poisson Solver 3D Cylindrical GPU test
Ukuran grid (r,phi,z) = (33,33,36)
Waktu komputasi: 			1.000000e-02 s
Jumlah iterasi siklus multigrid: 	10
Iterasi	Error Convergen		Error Absolut
[0]:	1.724319e-03	1.315177e-04
[1]:	8.652242e-05	1.266415e-04
[2]:	5.450314e-05	1.256187e-04
[3]:	5.081671e-05	1.249848e-04
[4]:	4.951065e-05	1.244460e-04
[5]:	4.904093e-05	1.241379e-04
[6]:	4.890266e-05	1.239894e-04
[7]:	4.885175e-05	1.239208e-04
[8]:	4.883604e-05	1.238959e-04
[9]:	4.883501e-05	1.238844e-04

Poisson Solver 3D Cylindrical GPU test
Ukuran grid (r,phi,z) = (65,65,72)
Waktu komputasi: 			5.000000e-02 s
Jumlah iterasi siklus multigrid: 	3
Iterasi	Error Convergen		Error Absolut
[0]:	1.084726e-04	1.291784e-04
[1]:	5.680057e-06	1.283386e-04
[2]:	1.501479e-06	1.278558e-04

Poisson Solver 3D Cylindrical GPU test
Ukuran grid (r,phi,z) = (129,129,144)
Waktu komputasi: 			1.200000e-01 s
Jumlah iterasi siklus multigrid: 	3
Iterasi	Error Convergen		Error Absolut
[0]:	1.373293e-05	1.302576e-04
[1]:	2.154564e-06	1.299626e-04
[2]:	1.048384e-06	1.297261e-04

Poisson Solver 3D Cylindrical GPU test
Ukuran grid (r,phi,z) = (257,257,288)
Waktu komputasi: 			6.900000e-01 s
Jumlah iterasi siklus multigrid: 	5
Iterasi	Error Convergen		Error Absolut
[0]:	5.469890e-06	1.388922e-04
[1]:	1.783695e-06	1.384964e-04
[2]:	1.345343e-06	1.381199e-04
[3]:	1.156555e-06	1.377524e-04
[4]:	1.050234e-06	1.373983e-04
```
