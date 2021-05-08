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
	else {
		DIR *dir;
		struct dirent *tmp;
		int i=0;
		if(strcmp(argv[1], "-d") == 0) {
            		dir = opendir(argv[2]);
			strcpy(dirpath, argv[2]);
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
			printf("Direktori sukses disimpan!\n");
		}
		else {
			printf("Yah, gagal disimpan :(\n");
		}
        closedir(dir);
	}
	return 0;
}

void *pindah(void *arg) {
	char *fpath = (char *)arg;
	char *hid = NULL;
	hid = strchr(fpath, '.');
	char *ext = NULL;
	ext = strrchr(fpath, '.');

	char temp[1000];
	if(ext) {
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
