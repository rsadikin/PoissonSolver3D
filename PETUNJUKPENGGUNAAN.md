# PETUNJUK PENGGUNAAN


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
 3. CUDA versi 9.1

Jika disediakan module program pada lingkungan sistem HPC maka dapat menjalankan perintah berikut ini:


```console
module load cmake/3.10.3
module load gcc/6.2.0
module load cuda
```
## Instalasi


