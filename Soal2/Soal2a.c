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


void matrixprep(){

	for(int i = 0; i < col1; i++){
		for(int j = 0; j < col2; j++){
			scanf("%d", &arr1[i][j]);
		}
	}

	for(int i = 0; i < col2; i++){
		for(int j = 0; j < col3; j++){
			//input angka 32 kali (12 + 20)
			scanf("%d", &arr2[i][j]);
		}
	}
	printf("\n");

}

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

void multiply(int mat1[col1][col2], int mat2[col2][col3], int res[col1][col3]){

    int i, j, k;
    for (i = 0; i < col1; i++) {
        for (j = 0; j < col3; j++) {
            temp[i][j] = 0;
				// printf("%d ", arr1);
            for (k = 0; k < col2; k++){
                printf("%d =+ %d * %d", temp[i][j], mat1[i][k], mat2[k][j]);
				if (k < col2) printf(" = ");
                temp[i][j] =+ mat1[i][k] * mat2[k][j];
				printf("%d", temp[i][j]);
				if (k < col2-1) printf("\n");
				}
			printf("\n");
        }
		printf("\n");
    }
	printf("\n");
}

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
	// printf("hampir jalan y\n");

    for (int i = 0; i < col1; i++){
        for (int j = 0; j < col3; j++){
            value[i*col3+j] = temp[i][j];
        }
    }

	// printf("hampir jalan x\n");

	for (int i = 0; i < col1; i++){
        for (int j = 0; j < col3; j++){
            printf("%d", value[i*col3+j]);
        }
    }

	printf("\nhampir jalan\n");

	while(1){
		if (*andri == 5) {printf("andri received = %d\n", *andri); break;}
		printf("waiting for andri...\n");
		sleep(1);
	}
	

    shmdt(value);
	shmdt(andri);
    shmctl(shmid, IPC_RMID, NULL);
}