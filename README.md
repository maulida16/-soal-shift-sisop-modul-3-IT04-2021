### Kelompok IT-04

* Maulida Meirisdiana 05311940000005
* Dava Aditya Jauhar 05311940000030
* Gerry Putra Fresnando 05311940000031

### Pembahasan Soal

## Soal 1

Keverk adalah orang yang cukup ambisius dan terkenal di angkatannya. Sebelum dia menjadi ketua departemen di HMTC, dia pernah mengerjakan suatu proyek dimana keverk tersebut meminta untuk membuat server database buku. Proyek ini diminta agar dapat digunakan oleh pemilik aplikasi dan diharapkan bantuannya dari pengguna aplikasi ini. Di dalam proyek itu, Keverk diminta: 

#### a. Pada saat client tersambung dengan server, terdapat dua pilihan pertama, yaitu register dan login. Jika memilih register, client akan diminta input id dan passwordnya untuk dikirimkan ke server. User juga dapat melakukan login. Login berhasil jika id dan password yang dikirim dari aplikasi client sesuai dengan list akun yang ada didalam aplikasi server. Sistem ini juga dapat menerima multi-connections. Koneksi terhitung ketika aplikasi client tersambung dengan server. Jika terdapat 2 koneksi atau lebih maka harus menunggu sampai client pertama keluar untuk bisa melakukan login dan mengakses aplikasinya. Keverk menginginkan lokasi penyimpanan id dan password pada file bernama akun.txt dengan format :
	
	akun.txt
	id:password
	id2:password2

###### Server Side

			#include <stdio.h>
			#include <sys/socket.h>
			#include <stdlib.h>
			#include <netinet/in.h>
			#include <string.h>
			#include <unistd.h>
			#include <dirent.h>
			#include <sys/types.h>
			#include <sys/stat.h>
			#define BUFFER_MAX_LENGTH 1024
			#define PORT 8080

			int file_created = 0, session = 0;
			char *trade, kosongan[512];
			int registration();
			int login();
			int foldermaker();
			int tsvadmin();
			
* Pertama kami menginputkan beberapa library untuk digunakan dan mendeklarasikan beberapa variabel integer, variabel array, beberapa fungsi dan sebuah pointer.

			int main(int argc, char const *argv[]) {
			    int server_fd, new_socket, valread, status = 1;
			    struct sockaddr_in address;
			    int opt = 1;
			    int addrlen = sizeof(address);
			    char buffer[1024] = {0}, buffer2[1024] = {0};
			    char *hello = "You are connected\n";

* Di dalam fungsi main kami mendeklarasikan beberapa variabel dan fungsi untuk menjalankan fungsi sokcet client-server.

			    foldermaker();

			    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
				perror("socket failed");
				exit(EXIT_FAILURE);
			    }

			    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
				perror("setsockopt");
				exit(EXIT_FAILURE);
			    }

			    address.sin_family = AF_INET;
			    address.sin_addr.s_addr = INADDR_ANY;
			    address.sin_port = htons( PORT );

			    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
				perror("bind failed");
				exit(EXIT_FAILURE);
			    }

			    int listening = listen(server_fd, 3);
			    if (listening < 0) {
				perror("listen");
				exit(EXIT_FAILURE);
			    }

* Untuk menjalankan client-server sendiri kami membuat beberapa kondisi untuk mengecek bahwa client dan server terhubung.

			    while(status == 1){
				printf("status: connected\n");
					if (session == 0) printf("Loginnya kosong lur!\n");
				if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
				    perror("accept");
				} 
				else {

				    printf("--client found (%d)\n", new_socket);                
				    send(new_socket , hello , strlen(hello) , 0 );
				    char *reply = "--Server received your message!\n";

				    while((read(new_socket , buffer, 1024)) > 0 && (printf("--now stand by...\n"))){
					if(strcmp(buffer,"break") == 0){
					    close(new_socket);
					    session = 0;
					    printf("--client %d has been disconnected\n", new_socket);
					}

					else if(strcmp(buffer,"shutdown") == 0){
					    close(new_socket);
					    printf("--client %d has been requested shutdown\n", new_socket);
					    status = 0;
					    break;
					}

* Ketika client-server sudah berhasil berjalan, maka program akan membuat dua kondisi. Kondisi yang pertama ketika client server gagal mendapatkan informasi dari client, maka akan terjadi error. Kondisi yang kedua ketika client berhasil terhubung dengan server, maka server akan mengirim beberapa pesan selamat datang. Saat server mengirim pesan selamat datang terjadi beberapa kondisi, yang pertama jika client mengirim string "break" sehingga client-server akan ter-disconnect dan akan memulai membuat socket baru lagi. Kemungkinan kedua ketika client mengirim string "shutdown", client akan terputus total dari server.

					else {
					    char *data, *id_user;
					    int i = 1;
					    if((strcmp(buffer,"register")) == 0 && session == 0){

						while(1){
						    if(i == 1){
							memset(buffer, 0, sizeof(buffer));
							strcpy(kosongan, "\n--You're just requesting [REGISTER]\nInsert ID: ");
							send(new_socket, kosongan, strlen(kosongan)+1, 0);
							memset(kosongan, 0, sizeof(kosongan));

							if((read(new_socket , buffer, 1024)) < 0) printf("buffer error");
							data = (char*)malloc(sizeof(strlen(buffer)+1));

							strncpy(data, buffer, strlen(buffer));
							strcat(data, ":");
							printf("id: %s\n", data);
							i = 2;
						    }

						    if(i == 2){
							memset(buffer, 0, sizeof(buffer));
							strcpy(kosongan, "\n--Now insert your password: ");
							send(new_socket, kosongan, strlen(kosongan)+1, 0);
							memset(kosongan, 0, sizeof(kosongan));

							if((read(new_socket , buffer, 1024)) < 0) printf("buffer error");
							printf("password: %s\n", buffer);

							data = realloc(data, (sizeof(data)+strlen(buffer)+1));
							strncat(data, buffer, strlen(buffer));
							i = 3;
						    }

						    // strncpy(id_user, buffer2, strlen(buffer2));
						    printf("\n[SUCCESS]\n--id and password: %s\n", data);

						    if(registration(data) && session == 0){
							printf("--Register success!\n");
							free(data);
							printf("--data habis di-free: \n");
							i = 0; break;
						    } else {
							printf("--Register failed\n");
							i = 1;
						    }
						    free(data);
						}
					    }

* Kemungkinan ketiga jika client mengirim string "register" dan slot client masih kosong, maka client akan dikirimkan beberapa inputan yang perlu diisi seperti user ID dan password. Data userID dan password akan disimpan ke dalam variabel `data`. Jika register berhasil maka program akan mengirimkan output "--Register success!" dan jika gagal, maka program akan mengeluarkan output "--Register failed!".  

					    else if(strcmp(buffer,"login") == 0){
						if (session == 1) {
						strcpy(kosongan, "Masih ada orang lur!\n");
						send(new_socket, kosongan, strlen(kosongan)+1, 0);
						memset(kosongan, 0, sizeof(kosongan));
						i == 0;
						}
						while(i == 1 && session == 0){
						    printf("ia: %d\n", i);
						    if(i == 1){
							printf("ib: %d\n", i);
							memset(buffer, 0, sizeof(buffer));
							char *prompt = "\n--You're just requesting [LOGIN]\nInsert ID: ";
							send(new_socket , prompt, strlen(prompt), 0);

							if((read(new_socket , buffer, 1024)) < 0) printf("buffer error");
							data = (char*)malloc(sizeof(strlen(buffer)+1));

							strncpy(data, buffer, strlen(buffer));
							strcat(data, ":");
							printf("id: %s\n", data);
							i = 2;
						    }

						    if(i == 2){
							printf("i: %d\n", i);
							memset(buffer, 0, sizeof(buffer));
							char *prompt2 = "\n--Now insert your password: ";
							// trade = (char* )malloc(strlen(prompt2)+1); strncpy(trade, prompt2, strlen(prompt2));
							send(new_socket , prompt2, strlen(prompt2), 0);

							if((read(new_socket , buffer, 1024)) < 0) printf("buffer error");
							printf("password: %s\n", buffer);

							data = realloc(data, (sizeof(data)+strlen(buffer)+1));
							strncat(data, buffer, strlen(buffer));
							// strncpy(data, buffer, strlen(buffer));
							i = 3;
						    }

						    printf("\n[SUCCESS]\n--id and password: %s\n", data);
						    printf("i: %d\n", i);
						    if(login(data) == 1 && i == 3){
							char *prompt3 = "--Login Success!\n";
							printf("--Login success!\n"); send(new_socket, prompt3, strlen(prompt3)+1, 0);
							free(data);
							printf("--data habis di-free: \n");
							session = 1;
							i = 0; break;
						    } 
						    else {
							char *prompt3 = "--Login Failed!\n";
							printf("--Login failed\n"); send(new_socket, prompt3, strlen(prompt3), 0);
							i = 1;
							printf("ic: %d\n", i);
						    }
						    free(data);
						    printf("--data habis di-free(login): \n");
						}
						// login();
					    }

* Kemungkinan keempat adalah ketika client mengirimkan string "Login" maka server akan menampilkan permintaan untuk data yang harus diisi dan mencocokkannya dengan variabel `data`. Jika login berhasil maka program akan mengirimkan output "--Login success!" dan jika gagal, maka program akan mengeluarkan output "--Login failed!".  

#### b. Sistem memiliki sebuah database yang bernama files.tsv. Isi dari files.tsv ini adalah path file saat berada di server, publisher, dan tahun publikasi. Setiap penambahan dan penghapusan file pada folder file yang bernama  FILES pada server akan memengaruhi isi dari files.tsv. Folder FILES otomatis dibuat saat server dijalankan. 

#### Tidak hanya itu, Keverk juga diminta membuat fitur agar client dapat menambah file baru ke dalam server. Direktori FILES memiliki struktur direktori di bawah ini : 

	Direktori FILES 
	File1.ekstensi
	File2.ekstensi

#### Pertama client mengirimkan input ke server dengan struktur sebagai berikut :
	
	Contoh Command Client :
		
	add	

	Output Client Console:
		
	Publisher:
	Tahun Publikasi:
	Filepath:


#### Kemudian, dari aplikasi client akan dimasukan data buku tersebut (perlu diingat bahwa Filepath ini merupakan path file yang akan dikirim ke server). Lalu client nanti akan melakukan pengiriman file ke aplikasi server dengan menggunakan socket. Ketika file diterima di server, maka row dari files.tsv akan bertambah sesuai dengan data terbaru yang ditambahkan.


						else if(strcmp(buffer,"add") == 0){
							if (session == 0) {
								strcpy(kosongan, "Login dulu lur!\n");
								send(new_socket, kosongan, strlen(kosongan)+1, 0);
								memset(kosongan, 0, sizeof(kosongan));
								// break;
							}
							send(new_socket , reply , strlen(reply) , 0);
									while(i == 1 && session == 1){
						    if(i == 1){
							memset(buffer, 0, sizeof(buffer));
							strcpy(kosongan, "\n--You're just requesting [ADD]\nInsert Publisher: ");
							send(new_socket, kosongan, strlen(kosongan)+1, 0);
							memset(kosongan, 0, sizeof(kosongan));


							if((read(new_socket , buffer, 1024)) < 0) printf("buffer error");
							data = (char*)malloc(sizeof(strlen(buffer)+1));

							strncpy(data, buffer, strlen(buffer));
							strcat(data, "\t");
							printf("publisher(%d): %s\n", i, data);
							i = 2;
						    }

						    if(i == 2){
								do{
									i++;
									memset(buffer, 0, sizeof(buffer));
									if(i == 3) strcpy(kosongan, "\n--Now insert your Detail (year): ");
									else if (i == 4) strcpy(kosongan, "\n--Now insert your Detail (path): ");
									else if (i == 5) strcpy(kosongan, "UPLOAD");
									send(new_socket, kosongan, strlen(kosongan)+1, 0);
									memset(kosongan, 0, sizeof(kosongan));

									if((read(new_socket , buffer, 1024)) < 0) printf("buffer error");
									printf("details: %s\n", buffer);

									if(i< 5){
										data = realloc(data, (strlen(data)+strlen(buffer)+1));
										strncat(data, buffer, strlen(buffer));
										if(i == 3){
											data = realloc(data, strlen(data)+1);
											strcat(data, "\t");
										}
									}
								printf("detail(%d): %s\n", i, data);
											}
											while (i < 5);
						    }

						    // strncpy(id_user, buffer2, strlen(buffer2));
						    printf("\n[SUCCESS]\n--file data: %s\n", data);

						    if(tsvadmin(data,1,new_socket) && session == 1){
							printf("--Input success!\n");
											free(data);
							printf("--data habis di-free: \n");
							i = 0; break;
						    } else {
							printf("--Input failed\n");
							i = 1;
						    }
						    free(data);
						}
					    }
					    
* Kemungkinan berikutnya adalah ketika client ingin menambah/ mengirim data client ke database files.tsv. Client tidak memiliki izin untuk input sebelum login. Di sini client menginput beberapa data seperti  `Publisher`, `Detail (year)`, `Detail (path)`. Jika input berhasil maka program akan mengirimkan output "--Input success!" dan jika gagal, maka program akan mengeluarkan output "--Input failed!". 

#### d. Client dapat mendownload file yang telah ada dalam folder FILES di server, sehingga sistem harus dapat mengirim file ke client. Server harus melihat dari files.tsv untuk melakukan pengecekan apakah file tersebut valid. Jika tidak valid, maka mengirimkan pesan error balik ke client. Jika berhasil, file akan dikirim dan akan diterima ke client di folder client tersebut. 

	Contoh Command client
	download TEMPfile.pdf


								else if(strcmp(buffer, "see") == 0){
									if (session == 0) {
										strcpy(kosongan, "Login dulu lur!\n");
										send(new_socket, kosongan, strlen(kosongan)+1, 0);
										memset(kosongan, 0, sizeof(kosongan));
										// break;
									}
							send(new_socket , reply , strlen(reply) , 0);
									while(i == 1 && session == 1){
										char *data2 = "test";
						    if(i == 1){
							memset(buffer, 0, sizeof(buffer));
							strcpy(kosongan, "\n--You're just requesting [SEE]\nInsert Publisher: ");
							send(new_socket, kosongan, strlen(kosongan)+1, 0);
							memset(kosongan, 0, sizeof(kosongan));
							i = 2;
						    }

						    if(tsvadmin(data2,0,new_socket) && session == 1){
							printf("--Input success!\n");
							printf("--data habis di-free: \n");
											// free(data2);
							i = 0; break;
						    } else {
							printf("--Input failed\n");
							i = 1;
						    }
						    // free(data);
					    }

								}
							printf("last message: [%s] \twith size of: [%ld]\n", buffer, strlen(buffer));
					memset(buffer, 0, sizeof(buffer));

							// continue;
					send(new_socket , reply , strlen(reply) , 0);
					}
				    }
				    printf("\n--Keluar loop\n");
				}

			    }

			    return 0;
			}
			
* Kemungkinan terakhir adalah ketika client ingin mengecek file.tsv untuk memastikan apakah file tersebut valid atau tidak, maka client harus menginputkan `Publisher` terlebih dahulu.

			int registration(char *passing){

			int errno;

			if((strlen(passing)) < 3 || login(passing) == 1) errno = 0;

			else {
			    printf("registration for: %s\n", passing);
			    FILE *target = fopen("akun.txt", "a+");

			    fprintf(target, "%s", passing);
			    fprintf(target, "\n");
			    fclose(target);
			}

			return errno;
			}

* Di atas merupakan cuplikan program untuk menjalankan fungsi `registration` pada server.

			int login(char *passing){

			char str[512];
			int tempChar, found = 0;
			FILE *target = fopen("akun.txt", "r");

			while(fgets(str,1024,target)){

			    if(strncmp(str,passing,strlen(passing)) == 0 && strlen(passing) > 2){
				found = 1;
				break;
			    }
			    // puts(str);
			}

			fclose(target);

			return found;

			}

* Di atas merupakan cuplikan program untuk menjalankan fungsi `login` pada server.

			int foldermaker(){

			    DIR *dp;
			    struct dirent *ep;
			    char path[100];

			    printf("Enter path to list files: ");
			    getcwd(path, 100);
			    printf("%s\n", path);
			    // scanf("%s", path);

			    dp = opendir(path);

				if (dp != NULL) {

					char *FILES= "FILES";
					int foldernya = 0;
					while ((ep = readdir (dp))) {
						//   puts (ep->d_name);
						if(strncmp(ep->d_name, FILES, strlen(FILES)) == 0) printf("Ada foldernya!\n");
						else {
							foldernya++;
							// printf("Nggak ada lur!\n");
						}

					}

					if(foldernya != 0){
						mkdir("FILES", 0777);
					}

				(void) closedir (dp);
				} else perror ("Couldn't open the directory");

			    return 0;
			}

			int tsvadmin(char *passing, int mode, int new_socket){

				char str[512], daftar[512];
				char* halo;
				int errno = 0, count = 0;

				if(mode == 0){
					FILE *target = fopen("files.tsv", "r+");
					fgets(str,1024,target);
					halo = strtok(str, "\t");
					while(halo != 0){
						// puts(str);
						printf("%d. %s\n", count, halo);
						strtok(NULL, "\t");
						// strcpy((char*)daftar[count], str);
						// // daftar[count] = str;
						++count;
						// send(new_socket, str, strlen(str), 0);
					}
					// for(int i = 0; i < count; i++){

					// 	printf("%d. %s\n", i, daftar[i]);

					// }
					errno = 1;
				}
				if(mode == 1){

					if((strlen(passing)) < 3) errno = 0;

					else {
						printf("writing for: %s\n", passing);
						FILE *target = fopen("files.tsv", "a+");

						fprintf(target, "%s", passing);
						fprintf(target, "\n");
						fclose(target);
						errno = 1;
					}

				}

			return errno;
			}

#### Kendala: 
* Belum selesai mengerjakan

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

#### b. Membuat program perhitungan matriks baru dengan inputan matriks dari user dan inputan matris dari soal 2.a. Perhitungannya adalah setiap cel yang berasal dari matriks A menjadi angka untuk faktorial, lalu cel dari matriks B menjadi batas maksimal faktorialnya (dari paling besar ke paling kecil).

	#include <stdio.h>
	#include <sys/ipc.h>
	#include <sys/shm.h>
	#include <unistd.h>
	#include <pthread.h>
	#define col1 4
	#define col2 3
	#define col3 6
	#define zero 0
	
	pthread_mutex_t softex;
	int *value, *andri, goal1, goal3, muteks;
	int hasil[col1*col3];
	int baru[col1*col3];
	int final[col1*col3];
	int argt[col1*col3];
	pthread_t thread_id[col1*col3];
	
* Pertama kami menginputkan library yang akan digunakan, setelah itu men-define beberapa variabel konstan untuk membuat ukuran matriks sesuai dengan yang dibutuhkan.
* Selanjutnya kami membuat sebuah muteks `pthread_mutex_t softex` dikarenakan program ini akan mengakses shared memory. Lalu variabel-variabel pointer dan array lain yang sekiranya dibutuhkan untuk menjalankan program. `pthread_t thread_id[col1*col3]` digunakan untuk membuat program matriks 4x6.

		int fact(int num){
			int i = 1;
			while (num > 1){
				i *= num;
				num--;
			}
			return i;
		}

* Fungsi `fact` dibuat untuk menampung sebuah parameter bertipe integer yang akan digunakan untuk melakukan faktorial dari perhitungan matriks.

		void matrixprep(){
			int rem = col1*col3;;
			for(int i = 0; i < col1; i++){
				for(int j = 0; j < col3; j++){
					printf("Number to input remaining: %d\n", rem-(i*col3+j));
					scanf("%d", &baru[i*col3+j]);
					printf("\n");
				}
			}
		}
		
* Fungsi `matrixprep` digunakan untuk menyimpan nilai matriks yang telah diinputkan oleh user. Selanjutnya nilai-nilai inputan tadi akan dimasukkan ke dalam variabel array `baru`.

		void* factpthread3(void *z){

			muteks = 0;

			int *goal2 = (int*) z;
			printf("goal2: %d\n", *goal2);
			printf("%d vs %d\n", hasil[*goal2], baru[*goal2]);

			if (hasil[*goal2] >= baru[*goal2]){

				final[*goal2] += fact(hasil[*goal2])/fact(hasil[*goal2]-baru[*goal2]);
				printf("final %d: %d\n", *goal2, final[*goal2]);

			}

			else if (hasil[*goal2] < baru[*goal2]){

				final[*goal2] += fact(hasil[*goal2]);
				printf("final %d: %d\n", *goal2, final[*goal2]);

			}
			else final[*goal2] += 0;
			muteks = 1;
			printf("/n");

		}

* Selanjutnya adalah membuat fungsi thread `factpthread3` untuk perhitungan setiap cell dalam matriks. Selanjutnya kami menginisiasi variabel muteks yang digunakan untuk mencegah threadnya saling berebut memori.
* Dilakukan sebuah pendeklarasian variabel pointer bernama `goal2` yang digunakan untuk mengakses nilai integer per tiap cell dari matriks.
* Setelah itu dikeluarkan output untuk mengeluarkan nilai tiap cell dari matriks pertama dan matriks kedua.
* Selanjutnya diberikan sebuah kondisi dimana jika nilai cell dari matriks pertama lebih besar sama dengan matriks kedua, maka nilai variabel array `final` akan menyimpan nilai perhitungan sesuai yang diberikan di soal yaitu memfaktorialkan nilai cell dari matriks 1 dibagi dengan pengurangan nilai cell matriks 1 dan 2 di dalam fungsi `fact` kemudian hasil dari perhitungan akan dikeluarkan.
* Kondisi kedua dimana nilai cell dari matriks 1 kurang dari nilai cell matriks 2, maka perhitungan hanya akan melakukan faktorial pada nilai dari cell matriks 1.
* Kondisi terkahir adalah jika nilai cell yang diakses oleh pointer `goal2` bernilai 0, maka hasilnya juga akan bernilai 0 atau tidak terjadi perhitungan.

		void matrixprint(int param){

			if (param == 0){
				printf("Value:\n");
				for(int i = 0; i < col1; i++){
					for(int j = 0; j < col3; j++){
						printf("%d ", value[i*col3+j]);
					}
					printf("\n");
				}

				printf("\n");

			}

			else if (param == 1){
				printf("Baru:\n");
				for(int i = 0; i < col1; i++){
					for(int j = 0; j < col3; j++){
						printf("%d ", baru[i*col3+j]);
					}
					printf("\n");
				}

				printf("\n");
			}

			else if (param == 2){
				printf("Final:\n");
				for(int i = 0; i < col1; i++){
					for(int j = 0; j < col3; j++){
						// printf("Halo ");
						printf("%d ", final[i*col3+j]);
					}
					printf("\n");
				}

				printf("\n");
			}

			else {
				matrixprint(0); matrixprint(1); matrixprint(2);
			}

		}

* Fungsi `matrixprint` digunakan untuk mengeluarkan output matriks yang sudah diperhitungkan. Fungsi ini akan mengambil nilai parameter dari variabel integer bernama `param`.
* Kondisi pertama jika parameter bernilai 0, maka fungsi akan mengeluarkan nilai matriks dari variabel array `value` dimana variabel ini diambil dari soal no 2.a
* Kondisi kedua jika parameter bernilai 1, maka fungsi akan mengeluarkan nilai matriks dari variabel array `baru`.
* Kondisi ketiga jika parameter bernilai 2, maka fungsi akan mengeluarkan nilai matriks dari variabel array `final`.
* Ketika tidak mendapat input selain ketiga parameter di atas, maka fungsi akan mengeluarkan ketiga kondisi secara urut.


			void main()
		{
		    key_t key = 1234;

		    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
		    value = shmat(shmid, NULL, 0);
		    andri = shmat(shmid, NULL, 0);

			matrixprep();

		    for (int i = 0; i < col1; i++){
			for (int j = 0; j < col3; j++){
			    hasil[i*col3+j] = value[i*col3+j];
			}
		    }

			printf("\n");

			for (int i = 0; i < col1; i++){
			for (int j = 0; j < col3; j++){

					goal1 = i*col3+j;
					printf("goal1: %d\n", goal1);

					if (!(goal3 = pthread_create(&(thread_id[goal1]), NULL, &factpthread3, (void *) &goal1)))
				pthread_join(thread_id[i*col3+j], NULL);

			}
		    }

			matrixprint(3);

			*andri = 5;

		    shmdt(value);
			shmdt(andri);
		    shmctl(shmid, IPC_RMID, NULL);
		}
		
* `int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);` digunakan untuk shared memory identifier yang memuat beberapa parameter untuk membuat shared memory. 
* Berikutnya baik pada variabel `value` dan `andri` dideklarasikan dengan perintah `shmat(shmid, NULL, 0)` untuk mengakses alamat shared memory yang sudah diindentifikasi oleh `shmid`.
* Dilanjutkan dengan memanggil fungsi `matrixprep` untuk mendapatkan inputan nilai per cell matriks dari user. 
* Selanjutnya adalah melakukan perbandingan nilai cell pada matriks lama dan matriks baru dan dijalankan perhitungannya menggunakan thread.
* Jika semua proses sudah selesai maka pointer `andri` akan bernilai lima sehingga program pada soal 1.a bisa selesai.
* `shmdt(value)` dan  `shmdt(andri)` digunakan untuk melepaskan segmen shared memory yang digunakan.
		
#### c. Membuat program untuk mengecek 5 proses teratas menggunakan pipe dengan command `ps aux | sort -nrk 3,3 | head -5`	

	#include <stdlib.h>
	#include <stdio.h>
	#include <sys/types.h>
	#include <sys/wait.h>
	#include <unistd.h>

	int pid, status;
	int pipe1[2];
	int pipe2[2];

* Pertama kami menginputkan beberapa library yang akan digunakan serta deklarasi `pipe1` dan `pipe2` yang akan digunakan sebagai pipes.

		int main() {
			printf("Halo lur!\n");
		if (pipe(pipe1) == -1){printf("Andri"); exit(1);}
		if (pipe(pipe2) == -1){printf("Andri lagi"); exit(1);}

* Di fungsi `main`, yang pertama kali kami lakukan adalah melakukan pengecekan apakah pipe berhasil dijalankan atau tidak. Jika `pipe1` gagal, maka program akan mengeluarkan output "Andri" dan jika `pipe2` gagal, maka program akan mengeluarkan output "Andri lagi".

			if ((fork()) == 0) {


				close(pipe1[0]);
				dup2(pipe1[1], STDOUT_FILENO);

				char *argv1[] = {"ps", "-aux", NULL};
				execv("/bin/ps", argv1);
			}
			
* Di child process  pertama ini dibuat file STDOUT dan eksekusi `ps`.

			else {

				while(wait(&status) > 0);

				if ((fork()) == 0) {
				close(pipe1[1]);
				close(pipe2[0]);
				// printf("---masuk child 2\n");
				dup2(pipe1[0], STDIN_FILENO);
				dup2(pipe2[1], STDOUT_FILENO);

				char *argv1[] = {"sort", "-nrk", "3,3", NULL};
				execv("/usr/bin/sort", argv1);

				} 
				
* Di child process ini dibuat file STDIN dan STDOUT dan eksekusi `sort`.

				else {

					// // input from pipe1
					close(pipe1[0]);
					close(pipe1[1]);
					close(pipe2[1]);
					while(wait(&status) > 0);
					// printf("---masuk parent");
					dup2(pipe2[0], STDIN_FILENO);
					close(pipe2[0]);

					char *argv1[] = {"head", "-5", NULL};
					execv("/usr/bin/head", argv1);

				}
			}
		}

* Masuk ke parent proses dibuat file STDIN dan eksekusi `head`.

#### Dokumentasi: 
Soal 2.a

Menginputkan nilai matriks
![image](https://user-images.githubusercontent.com/73152464/119264807-d12ec780-bc0e-11eb-9879-1d3d1a9f2caa.png)

Perhitungan perkalian matriks
![image](https://user-images.githubusercontent.com/73152464/119264973-4ac6b580-bc0f-11eb-83b7-150bf63c4c80.png)

Hasil input kedua matriks dan hasil perkaliannya
![image](https://user-images.githubusercontent.com/73152464/119265005-63cf6680-bc0f-11eb-9e47-138ab7b9296c.png)

Menunggu program 2.b
![image](https://user-images.githubusercontent.com/73152464/119265029-73e74600-bc0f-11eb-991f-933d69fe7e21.png)

Soal 2.b

Input matriks baru
![image](https://user-images.githubusercontent.com/73152464/119265057-8b263380-bc0f-11eb-8307-628308e2c571.png)

Membandingkan nilai cell matriks baru dan matriks lama
![image](![image](https://user-images.githubusercontent.com/57520495/119266123-b874e080-bc13-11eb-8871-98207f687ca1.png)

Hasil perhitungan
![image](https://user-images.githubusercontent.com/57520495/119266043-5f0cb180-bc13-11eb-9171-e10e87a50cc0.png)

Soal 2.c
![image](https://user-images.githubusercontent.com/73152464/119265109-be68c280-bc0f-11eb-80f7-75623de0ce40.png)

#### Kendala:
* Saat tipe `int` diganti dengan `long long int` program menjadi bermasalah. (Sudah berhasil diatasi, kami akhirnya menggunakan u`nsigned long long int`)


## Soal 3

#### a. opsi -f: digunakan untuk mengkategorikan file untuk file-file tertentu sebagai argumen.
#### b. opsi -d: digunakan untuk mengkategorikan file dalam directory tertentu sebagai argumen.
#### c. opsi * : digunakan untuk mengkategorikan file untuk seluruh file dalam current working directory saat program dijalankan.
  
		#include<sys/stat.h>
		#include<stdio.h>
		#include<unistd.h>
		#include<ctype.h>
		#include<string.h>
		#include<pthread.h>
		#include<dirent.h>

		pthread_t tid[10000];
		char cwd[1000];
		char dirpath[1000];

		void *pindah(void *arg);

* Pertama kami menginputkan beberapa library yang akan digunakan. Setelah itu kami membuat thread dan beberapa variabel array, juga sebuah pointer bernama `pindah`.

		int main(int argc, char *argv[]) {
			getcwd(cwd, sizeof(cwd));
			int a;

			if(argc < 2) {
				printf("argumen invalid");
			}

			if(strcmp(argv[1], "-f") == 0) {
				for(int i=2; i<argc; i++) {
					pthread_create(&tid[i], NULL, pindah, (void *)argv[i]);
				}
				for(int j=2; j<argc; j++) {
					a = pthread_join(tid[j], NULL);
					if(a == 0) {
						printf("File %d: Berhasil Dikategorikan\n", j-1);
					}
					else {
						printf("File %d: Sad, gagal :(\n", j-1);
					}
				}
			}
			
* Di fungsi `main` diperlukan dua parameter yaitu sebuah variabel dan sebuah pointer.
* Pertama dilakukan pengambilan path dari direktori saat ini menggunakan fungsi `getcwd` yang menyimpan path direktori saat ini di dalam sebuah buffer yaitu `cwd`
* Langkah kedua adalah mengecek apakah argumen yang diberikan sudah benar atau masih salah menggunakan `if(argc < 2)` karena jumlah argumen yang diberikan seharusnya lebih dari 2.
* Selanjutnya adalah membuat sebuah perulangan `for(int i=2; i<argc; i++)` dengan opsi `-f` yang akan membuat thread untuk mengategorikan sejumlah file. Selanjutnya thread akan ditunggu hingga selesai menggunakan `a = pthread_join(tid[j], NULL);` dan jika thread pertama berhasil dijalankan maka akan mengeluarkan output "Berhasil Dikategorikan" dan melanjutkan ke perulangan thread berikutnya. Jika thread pertama tidak berhasil dijalankan, maka akan mengeluarkan output "Sad, gagal :("


			else {
				DIR *dir;
				struct dirent *tmp;
				int i=0;
				if(strcmp(argv[1], "-d") == 0) {
					dir = opendir(argv[2]);
					strcpy(dirpath, argv[2]);
				}

* Selanjutnya adalah membuat direktori baru sebagai direktori pengkategorian untuk masing-masing ekstensi file.
* Kondisi kedua adalah jika opsi `-d` diinputkan, maka program akan membuka direktori yang dimaksud pada variabel `dir` dengan fungsi `opendir(argv[2])` dan directory path yang diinputkan pada parameter ke-2 akan disimpan pada variabel array `dirpath` dengan fungsi `strcpy(dirpath, argv[2])`

		else if((argv[1][0]=='*') && (strlen(argv[1])==1)) {
							dir = opendir(cwd);
						}
						else {
							printf("argumen invalid");
						}

						while((dir!=NULL) && (tmp=readdir(dir))) {
					    if(strcmp(tmp->d_name, ".")==0 || strcmp(tmp->d_name, "..")==0 || strcmp(tmp->d_name, "soal3.c")==0 || strcmp(tmp->d_name, "soal3")==0 || tmp->d_type==DT_DIR) 
							continue;

					    pthread_create(&tid[i], NULL, pindah, tmp->d_name);
					    i++;
					}
					for(int j=0; j<i; j++)
						a = pthread_join(tid[j], NULL);
						if(a == 0) {
							printf("Direktori sukses disimpan!\n");
						}
						else {
							printf("Yah, gagal disimpan :(\n");
						}
					closedir(dir);
					}
					return 0;
				}

* Selanjutnya jika opsi yang diinputkan adalah `*` maka variabel `dir` akan membuka working directory yang telah disimpan pada variabel buffer `cwd`. Jika gagal dijalankan/ argumen yang diinputkan tidak memenuhi, maka akan mengeluarkan output "argumen invalid".
* Berikutnya dilakukan pengeliminasian direktori dan file `soal3.c` serta file `soal3` dengan menggunakan perintah berikut `if(strcmp(tmp->d_name, ".")==0 || strcmp(tmp->d_name, "..")==0 || strcmp(tmp->d_name, "soal3.c")==0 || strcmp(tmp->d_name, "soal3")==0 || tmp->d_type==DT_DIR) `. Jika pengeliminasian ini berhasil, maka dilanjutkan dengan pembuatan thread untuk mengkategorikan seluruh file yang ada di working directory.
* Selanjutnya thread akan ditunggu hingga selesai. Jika berhasil dijalankan maka akan keluar output "Direktori sukses disimpan!" dan jika gagal maka akan mengeluarkan output "Yah, gagal disimpan"


		void *pindah(void *arg) {
			char *fpath = (char *)arg;
			char *hid = NULL;
			hid = strchr(fpath, '.');
			char *ext = NULL;
			ext = strrchr(fpath, '.');

			char temp[1000];
			if(ext) {
				ext++;
				for(int i=0;i<strlen(ext);i++) {
					temp[i] = tolower(ext[i]);
				}
			}
			else if(hid) {
				strcpy(temp, "Hidden");
			}
			else strcpy(temp, "Unknown");

			char *fname = NULL;
			fname = strrchr(fpath, '/');
			if(fname) {
				fname++;
			}
			else fname = fpath;

			char dirname[1000];
			strcpy(dirname, cwd);
			strcat(dirname, "/");
			strcat(dirname, temp);

			mkdir(dirname, S_IRWXU);

			if(strlen(dirpath) > 1) {
				char fullname[1000];
				strcpy(fullname, dirpath);
				strcat(fullname, "/");
				strcat(fullname, fname);

				strcat(dirname, "/");
				strcat(dirname, fname);

				rename(fullname, dirname);
			}
			else {					
				strcat(dirname, "/");
				strcat(dirname, fname);

				rename(fpath, dirname);
			}
		}

* Fungsi `pindah` digunakan untuk memindahkan file ke direktori ekstensi masing-masing. Pertama kami melakukan pendeklarasian beberapa variabel pointer yang akan digunakan untuk menjadi parameter jalannya suatu kondisi. Pointer `hid` dan `ext` menyimpan delimiter `.` untuk memastikan bahwa argumen pada `fpath` menyimpan sebuah file. Perbedaannya hanya cara pengecekan saja, jika `ext` mengecek dari kemunculan terakhir, maka `hid` mengecek dari kemunculan terdepan.
* `ext` akan menjadi sebuah kondisi untuk mengubah huruf pada path direktori menjadi lower case menggunakan fungsi `tolower`. Kondisi lain akan memindahkan file `hidden` dan `unknown` ke variabel `temp`.
* Selanjutnya adalah membuat path dari direktori extension yang akan dibuat menggunakan fungsi `strcpy` dan `strcat`
* Lalu ada fungsi `mkdir(dirname, S_IRWXU);` untuk membuat direktori baru.
* Berikutnya ada kondisi untuk memenuhi opsi `-d`  yaitu pertama membuat array `fullname` untuk menyimpan path yang akan dibuat dan selanjutnya memindahkan file yang ada pada `fullname` ke `dirname`.
* Kondisi lain adalah untuk memnuhi opsi `*`

####  Dokumentasi:

Folder untuk testing dari asisten
![1621777785524](https://user-images.githubusercontent.com/73152464/119263477-68911c00-bc09-11eb-9ffb-e8a0b69f59a6.jpg)

Menjalankan perintah -f
![1621777934659](https://user-images.githubusercontent.com/73152464/119263538-a4c47c80-bc09-11eb-8950-1372ce7c6ea9.jpg)

![1621777953474](https://user-images.githubusercontent.com/73152464/119263559-bad23d00-bc09-11eb-9cfb-82b5847f8cdc.jpg)

Menjalankan perintah -d
![1621778021503](https://user-images.githubusercontent.com/73152464/119263590-d89fa200-bc09-11eb-9c7e-1164117b1e10.jpg)

![1621778050307](https://user-images.githubusercontent.com/73152464/119263600-eb19db80-bc09-11eb-89e8-db7882302d4a.jpg)

Menjalankan perintah *
![1621778210275](https://user-images.githubusercontent.com/73152464/119263619-fd941500-bc09-11eb-98e8-9a96b363a911.jpg)

![1621778230285](https://user-images.githubusercontent.com/73152464/119263629-0d135e00-bc0a-11eb-8c16-04d7c8622bed.jpg)

Kendala:
* Direktori hidden tidak dapat muncul





