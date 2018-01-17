#include "stdlib.h"
#include "stdio.h"
#include "pthread.h"
#include "time.h"
#include <unistd.h>

#define SECONDSMAX 20
#define MINUTESMAX 2

/*
*
* Youssef NIDABRAHIM - F2
*
**/



int seconds = 0, minutes = 0, hours = 0;
int temps = 0;

pthread_cond_t conditionSecondsJob = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conditionMinutesJob = PTHREAD_COND_INITIALIZER;
pthread_cond_t conditionHoursJob = PTHREAD_COND_INITIALIZER;



void * listeningJob(void* a) {
	while(1) {
		pthread_mutex_lock(&mutex);
		time_t t;
		while(temps == time(&t));
		temps = time(&t);
		pthread_cond_signal (&conditionSecondsJob); 
		pthread_mutex_unlock(&mutex);
	}
}

void * secondsJob(void* a) {
	while(1) {
		pthread_mutex_lock (&mutex); 
		pthread_cond_wait (&conditionSecondsJob, &mutex); 
		system("clear");
		printf("%d : %d : %d\n", hours, minutes, seconds);
		if(seconds != SECONDSMAX){
			seconds++;
		}else{
			
			pthread_cond_signal (&conditionMinutesJob); 
			seconds = 0;
		}
		fflush(stdout);
		pthread_mutex_unlock (&mutex); 
	}
}

void * minutesJob(void* a) {
	while(1) {
		
		pthread_mutex_lock(&mutex); 
		pthread_cond_wait (&conditionMinutesJob, &mutex); 

		if(minutes != MINUTESMAX){
			minutes++;
		}else{
			pthread_cond_signal (&conditionHoursJob);  
			minutes = 0;
			
		}
		pthread_mutex_unlock(&mutex); 
		
	}
}

void * hoursJob(void* a) {
	while(1) {
		
		pthread_mutex_lock(&mutex); 
		pthread_cond_wait (&conditionHoursJob, &mutex);
		hours++;
		pthread_mutex_unlock(&mutex); 
		
	}
}

int  main() {

	pthread_t threads[4];
	time_t t;

	temps = time(&t);

	pthread_create(&threads[0], 0, listeningJob, 0);
	pthread_create(&threads[1], 0, secondsJob, 0);
	pthread_create(&threads[2], 0, minutesJob, 0);
	pthread_create(&threads[3], 0, hoursJob, 0);	
	
	for(int i = 0; i < 4; ++i)
		pthread_join(threads[i], 0);

	return 0;
}


