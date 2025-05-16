#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_SEATS 5
#define NUM_STUDENTS 10

sem_t taWakeup;
sem_t waitingStudents;
pthread_mutex_t mutex;

int waiting = 0;
int helped =0;

void* ta_thread(void* arg) {
	while (1){
		sem_wait(&taWakeup);
		while (1){
			pthread_mutex_lock(&mutex);
			if (waiting == 0){
				pthread_mutex_unlock(&mutex);
				break;}
			waiting--;
			helped++;
			printf("TA is helping. \n");
			sleep(2);
			sem_post(&waitingStudents);
			pthread_mutex_unlock(&mutex);
			if (helped >= NUM_STUDENTS){
			exit(0);}}
		printf("TA goes to sleep.\n");}
}

void* student_thread(void* arg){
	int id = *(int*)arg;
	while (1){
		sleep(rand() % 5 + 1);
		pthread_mutex_lock(&mutex);
		if (waiting < NUM_SEATS){
			printf("student %d is waiting. waiting students: %d \n", id, waiting);
			waiting++;
			if (waiting == 1){
				sem_post(&taWakeup);}
			pthread_mutex_unlock(&mutex);
			sem_wait(&waitingStudents);
			printf("student %d got help. \n", id);
			}
		else {
			printf("student %d found no emphty chair. \n", id);
			pthread_mutex_unlock(&mutex);}
	}
	free(arg);
	pthread_exit(NULL);

}


int main() {
	pthread_t ta;
	pthread_t students[NUM_STUDENTS];

	srand(time(NULL));


	sem_init(&taWakeup, 0, 0);
	sem_init(&waitingStudents, 0, 0);
	pthread_mutex_init(&mutex, NULL);
	pthread_create(&ta, NULL, ta_thread, NULL);
	int i =0;
	for (i ; i < NUM_STUDENTS; i++){
		int* id = malloc(sizeof(int));
		*id = i + 1;
		pthread_create(&students[i], NULL, student_thread, id);}

	pthread_join(ta, NULL);
	for (int j = 0; j < NUM_STUDENTS; j++){
		pthread_join(students[i], NULL);}

	return 0;}


