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
Install the project...
-- Install configuration: ""
-- Installing: /home/usertest/buildpoissonsolver/lib/libpoissonsolvergpu.so
-- Installing: /home/usertest/buildpoissonsolver/include/PoissonSolver3DCylindricalGPU.h
-- Installing: /home/usertest/buildpoissonsolver/include/PoissonSolver3DGPU.h
```

