#ifndef PROCESS_H
#define PROCESS_H

#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#define PRINTK 336

#define CHILD_CPU 1
#define PARENT_CPU 0

typedef struct job {
	char name[32];
	int ready;
	int remain;
	pid_t pid;
	int isReady;
}Job;

typedef enum policy {
	FIFO,
	RR,
	SJF,
	PSJF
} Policy;

void unitTime() {volatile unsigned long i; for (i = 0; i < 1000000UL; i++);}

void changeCPU(int pid, int core) {
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(core, &mask);
	sched_setaffinity(pid, sizeof(mask), &mask);
}

void blockJob(int pid, Policy policy) {
	struct sched_param param;
	
	if (policy == FIFO) {
		param.sched_priority = sched_get_priority_min(SCHED_FIFO);
		sched_setscheduler(pid, SCHED_FIFO, &param);
	}
    else {
		param.sched_priority = 0;
		sched_setscheduler(pid, SCHED_IDLE, &param);
	}
}

void continueJob(int pid, Policy policy) {
	struct sched_param param;
	
	if (policy == FIFO) {
		param.sched_priority = sched_get_priority_max(SCHED_FIFO);
		sched_setscheduler(pid, SCHED_FIFO, &param);
	}
    else {
		param.sched_priority = 0;
		sched_setscheduler(pid, SCHED_OTHER, &param);
	}
}

int startJob(Job job) {
	int pid = fork();

	if (pid < 0) {
		perror("fork");
		return -1;
	}

	if (pid == 0) {

        #ifdef DEBUG
        fprintf(stderr, "%s in child process.\n", job.name);
        #endif
        
        struct timespec startTime = {0, 0};
        struct timespec endTime = {0, 0};

		clock_gettime(CLOCK_MONOTONIC, &startTime);
		for (int i = 0; i < job.remain; i++)
			unitTime();
		clock_gettime(CLOCK_MONOTONIC, &endTime);

        long pid = (long)getpid();

		syscall(PRINTK, pid, startTime.tv_sec, startTime.tv_nsec,
                            endTime.tv_sec, endTime.tv_nsec);
		exit(0);
	}
	else {
		changeCPU(pid, CHILD_CPU);
		return pid;
	}
}

#endif