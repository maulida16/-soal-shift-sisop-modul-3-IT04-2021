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

* Pertama kami menginputkan library yang akan digunakan, setelah itu men-define beberapa variabel konstan untuk membuat ukuran matriks sesuai dengan yang dibutuhkan.
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
* Setelah iterasi pemasukan nilai pada matriks 1 terselesaikan, akan dilanjutkan dengan matriks 2 karena setelah menjalankan proses pertama, nilai dari variabel a akan bertambah.

		void matrixprint(int param){

			if (param == 0){

				for(int i = 0; i < col1; i++){
					for(int j = 0; j < col3; j++){
						printf("%d ", temp[i][j]);
					}
					printf("\n");
				}

				printf("\n");

			}

			else if (param == 1){

				for(int i = 0; i < col1; i++){
					for(int j = 0; j < col2; j++){
						// printf("Halo ");
						printf("%d ", arr1[i][j]);
					}
					printf("\n");
				}

				printf("\n");
			}

			else if (param == 2){

				for(int i = 0; i < col2; i++){
					for(int j = 0; j < col3; j++){
						printf("%d ", arr2[i][j]);
					}

					printf("\n");
				}

				printf("\n");
			}

			else {
				matrixprint(1); matrixprint(2); matrixprint(0);
			}

		}

* Fungsi `matrixprint` digunakan untuk mengeluarkan hasil perkalian antar matriks.
* Fungsi ini memerlukan sebuah parameter yang digunakan sebagai percabangan kondisi untuk nilai matriks apa yang akan diprint.
* Kondisi pertama dimana parameter bernilai 0 akan mengeluarkan output dari matriks `temp` yang menyimpan nilai matriks berukuran 4x6.
* Kondisi kedua dimana parameter bernilai 1 akan mengeluarkan output dari matriks `arr1` yang menyimpan nilai matriks berukuran 4x3.
* Kondisi ketiga dimana parameter bernilai 2 akan mengeluarkan output dari matriks `arr2` yang menyimpan nilai matriks berukuran 3x6.
* Kondisi lain dimana parameter bernilai selain 0,1,2 akan mengeluarkan output dari ketiga parameter di atasnya.

		void multiply(int mat1[col1][col2], int mat2[col2][col3], int res[col1][col3]){

		    int i, j, k;
		    for (i = 0; i < col1; i++) {
			for (j = 0; j < col3; j++) {
			    temp[i][j] = 0;
						// printf("%d ", arr1);
			    for (k = 0; k < col2; k++){
				printf("%d += %d * %d", temp[i][j], mat1[i][k], mat2[k][j]);
						if (k < col2) printf(" = ");
				temp[i][j] += mat1[i][k] * mat2[k][j];
						printf("%d", temp[i][j]);
						if (k < col2-1) printf("\n");
						}
					printf("\n");
			}
				printf("\n");
		    }
			printf("\n");
		}

* Fungsi `multiply` ini digunakan untuk melakukan perkalian dari matriks 1 dan matriks 2.
* Perkalian dilakukan dengan iterasi baris dari matriks 1 dan kolom dari matriks 2 lalu dikalikan. Hasil perkalian akan dimasukkan ke variabel `temp[i][j]` dan selanjutnya ditambahkan dengan perkalian dari baris dan kolom yang sama. Setelah perkalian dari baris dan kolom yang sama selesai, program akan melanjutkan iterasi ke baris dan kolom selanjutnya.

		void main()
		{
		    key_t key = 1234;
		    int	*value, *andri;
			*andri = 0;

		    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
		    value = shmat(shmid, NULL, 0);
			andri = shmat(shmid, NULL, 0);

		    matrixprep();
			multiply(arr1, arr2, temp);
			matrixprint(3);

		    for (int i = 0; i < col1; i++){
			for (int j = 0; j < col3; j++){
			    value[i*col3+j] = temp[i][j];
			}
		    }

			printf("Value: \n");
			for (int i = 0; i < col1; i++){
				for (int j = 0; j < col3; j++){
			    		printf("%d ", value[i*col3+j]);
			}
				printf("\n");
		    }
			printf("\n");

			printf("\nhampir jalan\n");

			while(1){
				if (*andri == 5) {printf("andri received = %d\n", *andri); *andri = 0; break;}
				printf("waiting for andri...\n");
				sleep(1);
			}


		    shmdt(value);
			shmdt(andri);
		    shmctl(shmid, IPC_RMID, NULL);
		}
		
* Pada fungsi main yang pertama kami lakukan adalah mendeklarasikan sebuah key yang dapat digunakan untuk meminta resource shared memory. Kami juga membuat pointer `andri` dan `value` untuk shared memory nantinya.
* `int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);` digunakan untuk shared memory identifier yang memuat beberapa parameter untuk membuat shared memory. 
*  Berikutnya baik pada pointer `value` dan `andri` dideklarasikan dengan perintah `shmat(shmid, NULL, 0)` untuk menuju ke alamat shared memory yang sudah diindentifikasi oleh `shmid`.
*  Dilanjutkan dengan memanggil fungsi `matrixprep`, `multiply`, dan `matrixprint` dengan parameter 3.
*  Selanjutnya hasil perkalian kedua matriks yang disimpan pada variabel `temp` akan dimasukkan ke pointer `value`.
*  Langkah selanjutnya adalah melakukan perulangan `while` untuk menunggu terkoneksi dengan shared memory pada soal 2b. Jika masih menunggu untuk terkoneksi maka program akan mengeluarkan output `printf("waiting for andri...\n");` yang akan berulang setiap 1 detik.
*  `shmdt(value)` dan  `shmdt(andri)` digunakan untuk melepaskan segmen shared memory yang digunakan.
