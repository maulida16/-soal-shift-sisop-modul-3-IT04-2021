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

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread, status = 1;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0}, buffer2[1024] = {0};
    char *hello = "You are connected\n";
    
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

    while(status == 1){
        printf("status: connected\n");
		if (session == 0) printf("Loginnya kosong lur!\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            // exit(EXIT_FAILURE);
        } 
        else {

            printf("--client found (%d)\n", new_socket);                
            send(new_socket , hello , strlen(hello) , 0 );
            char *reply = "--Server received your message!\n";
            
            // valread = read( new_socket , buffer, 1024);
            while((read(new_socket , buffer, 1024)) > 0 && (printf("--now stand by...\n"))){
                // ;
                
                // fflush(stdout);
                // send(new_socket , reply , strlen(reply) , 0);
                
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
                // else printf("%s\n",buffer);

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