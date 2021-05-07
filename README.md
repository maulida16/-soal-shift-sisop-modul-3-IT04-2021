### Kelompok IT-04

* Maulida Meirisdiana 05311940000005
* Dava Aditya Jauhar 05311940000030
* Gerry Putra Fresnando 05311940000031

### Pembahasan Soal

## Soal 2

#### a. Membuat program perkalian matrix (4x3 dengan 3x6) dan menampilkan hasilnya. Matriks nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka).

    #include <stdio.h>
    #include <sys/ipc.h>
    #include <sys/shm.h>
    #include <unistd.h>
    #define col1 4
    #define col2 3
    #define col3 6
    #define zero 0

    int arr1[col1][col2];
    int arr2[col2][col3];
    int temp[col1][col3];

* Pertama kami menginputkan library yang akan digunakan, setelah itu men-define beberapa variabel konstan untuk membuat matriks sesuai ukuran yang dibutuhkan.
* Selanjutnya kami membuat array dua dimensi untuk membuat matriks1, matriks2, dan hasil perkalian dari matriks1 dan matriks2.

      void matrixprep(){

      int a = 0, rem;
      if(a == 0){
        a++;
        rem = col1*col2;
        for(int i = 0; i < col1; i++){
          for(int j = 0; j < col2; j++){
            printf("Number to input to arr1 remaining: %d\n", rem-(i*col2+j));
            scanf("%d", &arr1[i][j]);
            printf("\n");
          }
        } 
      }
      if (a != 0){
        rem = col2*col3;
        for(int i = 0; i < col2; i++){
          for(int j = 0; j < col3; j++){
            printf("Number to input to arr2 remaining: %d\n", rem-(i*col3+j));
            scanf("%d", &arr2[i][j]);
            printf("\n");
          }
        }
      }
      printf("\n");

    }
      
* Fungsi `matrixprep()` digunakan untuk menginputkan angka yang akan dimasukkan ke dalam matriks. 
* Pertama dilakukan inisiasi variabel `a = 0` sebagai parameter untuk menjalankan iterasi pemasukan nilai pada matriks 1. Setelah itu dideklarasikan ukuran dari matriks 1 di dalam variabel `rem` dan selanjutnya adalah menjalankan perulangan for untuk memasukkan nilai pada baris dan kolom matriks.
* Pemasukan nilai untuk matriks oleh user dilakukan dengan cara mengeluarkan output berisi perintah untuk memasukkan nilai pada matriks dengan posisi `rem-(i*col2+j)` dan setelah itu inputan nilai akan disimpan dengan fungsi `scanf`.
* 
