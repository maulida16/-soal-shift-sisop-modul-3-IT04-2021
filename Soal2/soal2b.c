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
int	*value, *andri, goal1, goal3, muteks;
int hasil[col1*col3];
int baru[col1*col3];
int final[col1*col3];
int argt[col1*col3];
pthread_t thread_id[col1*col3];

int fact(int num){
	int i = 1;
	while (num > 1){
		i *= num;
		num--;
	}
	return i;
}

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

}

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