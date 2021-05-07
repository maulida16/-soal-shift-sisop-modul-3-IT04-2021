#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<dirent.h>

pthread_t tid[10000];
char cwd[1000];
char folder[1000];

void *pindah(void *arg);

int main(int argc, char *argv[]) {
	getcwd(cwd, sizeof(cwd));
	int a;
	memset(folder, '\0', sizeof(folder));

	if(strcmp(argv[1], "-f") == 0) {
		if(argc < 2) {
			printf("argumen invalid");
		}
		for(int i=2; i<argc; i++) {
			pthread_create(&tid[i], NULL, pindah, (void *)argv[i]);
		}
		for(int j=2; j<argc; j++) {
			a = pthread_join(tid[j], NULL);
			if(a == 0) {
				printf("\nFile %d: Berhasil Dikategorikan", j-1);
			}
			else {
				printf("\nFile %d: Sad, gagal :(", j-1);
			}
		}
	}
	else {
		DIR *dir;
		struct dirent *tmp;
		int i=0;
		if(strcmp(argv[1], "-d") == 0) {
            		dir = opendir(argv[2]);
			strcpy(folder, argv[2]);
		}
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
			printf("\nDirektori sukses disimpan!");
		}
		else {
			printf("\nYah, gagal disimpan :(");
		}
        closedir(dir);

	}
	return 0;
}

void *pindah(void *arg) {
	char *fpath = (char *)arg;
	char *extensi = NULL;
	extensi = strrchr(fpath, '.');

	char ext[1000];
	memset(ext, '\0', sizeof(ext));
	if(extensi) {
		extensi++;
        	for(int i=0; i<strlen(extensi); i++) {
               	ext[i] = tolower(extensi[i]);
    		}
	}
	else strcpy(ext, "Unknown");

    	char *fname = NULL;
    	fname = strrchr(fpath, '/');
    	if(fname) 
		fname++;
	else fname = fpath;

	char dirpath[1000];
	strcpy(dirpath, cwd);
	strcat(dirpath, "/");
	strcat(dirpath, ext);

	mkdir(dirpath, S_IRWXU);

	if(strlen(folder) > 1) {
		char fullname[1000];
		strcpy(fullname, folder);
		strcat(fullname, "/");
		strcat(fullname, fname);

		strcat(dirpath, "/");
        	strcat(dirpath, fname);

        	rename(fullname, dirpath);
	}
	else {					
	    	strcat(dirpath, "/");
        	strcat(dirpath, fname);

		rename(fpath, dirpath);
	}
}
