#ifndef SCHEDULER_H
#define SCHEDULER_H

#define _GNU_SOURCE
#include <sys/wait.h>
#include "process.h"

static int lastTime;
static int curTime;
static int runningID;
static int finishedNum;

int cmp(const void* a, const void* b) {
	return ((Job*)a)->ready - ((Job*)b)->ready;
}

int cmpSJF(const void* a, const void* b) {
	Job* j1 = (Job*)a;
	Job* j2 = (Job*)b;
	if (j1->ready == j2->ready)
		return j1->remain - j2->remain;
	else
		return j1->ready - j2->ready;
}

int selectFIFO(Job* jobs, int jobNum) {
	int next = -1;
	for (int i = 0; i < jobNum; ++i) {
		if (!jobs[i].isReady) break;
		else if (jobs[i].remain == 0) continue;
		else {
			next = i;
			break;
		}
	}
	return next;
}

int selectRR(Job* jobs, int jobNum) {
	int next = -1;
	if (runningID == -1) {
		for (int i = 0; i < jobNum; ++i) {
			if (jobs[i].isReady && jobs[i].remain > 0){
				next = i;
				break;
			}
		}
	}
	else if ((curTime - lastTime) % 500 == 0) {
		next = (runningID + 1) % jobNum;
		while (!jobs[next].isReady || jobs[next].remain == 0)
			next = (next + 1) % jobNum;
	}
	else
		next = runningID;
	return next;
}

int selectSJF(Job* jobs, int jobNum) {
	int next = -1;
	for (int i = 0; i < jobNum; i++) {
		if (!jobs[i].isReady || jobs[i].remain == 0)
			continue;
		else if (next == -1 || jobs[i].remain < jobs[next].remain)
			next = i;
	}
	return next;
}

int selectNext(Job* jobs, int jobNum, int policy) {

	if (runningID != -1 && (policy == SJF || policy == FIFO))
		return runningID;

	int next;
	switch (policy) {
		case(FIFO):
			next = selectFIFO(jobs, jobNum);
			break;
		case(RR):
			next = selectRR(jobs, jobNum);
			break;
		case(SJF):
		case(PSJF):
			next = selectSJF(jobs, jobNum);
			break;
	}
	return next;
}

void init(Job *jobs, int jobNum, Policy policy) {
	if (policy == SJF || policy == PSJF)
		qsort(jobs, jobNum, sizeof(Job), cmpSJF);
	else
		qsort(jobs, jobNum, sizeof(Job), cmp);

	changeCPU(getpid(), PARENT_CPU);
	continueJob(getpid(), policy);
	
	curTime = 0;
	lastTime = 0;
	runningID = -1;
	finishedNum = 0;
}

int jobsAreDone(Job *jobs, int jobNum) {
	if (runningID != -1 && jobs[runningID].remain == 0) {
		#ifdef DEBUG
		fprintf(stderr, "%s finished at time %d.\n", jobs[runningID].name, curTime);
		#endif

		waitpid(jobs[runningID].pid, NULL, 0);
		runningID = -1;
		finishedNum++;

		if (finishedNum == jobNum) return 1;
	}
	return 0;
}

void checkJobsArrival(Job* jobs, int jobNum) {
	for (int i = 0; i < jobNum; ++i) {
		if (jobs[i].ready == curTime) {
			jobs[i].isReady = 1;
			#ifdef DEBUG
			fprintf(stderr, "%s came at time %d.\n", jobs[i].name, curTime);
			#endif
		}
	}
}

void scheduleNext(Job* jobs, int jobNum, Policy policy) {
	int next = selectNext(jobs, jobNum, policy);
	if (next != -1 && runningID != next) {
		if (jobs[next].pid == -1){
			jobs[next].pid = startJob(jobs[next]);
			blockJob(jobs[next].pid, policy);	
			#ifdef DEBUG
			fprintf(stderr, "%s forked at time %d.\n", jobs[next].name, curTime);
			#endif
		}
		continueJob(jobs[next].pid, policy);
		if (runningID != -1)
			blockJob(jobs[runningID].pid, policy);
		runningID = next;
		lastTime = curTime;
	}
}

int scheduling(Job *jobs, int jobNum, Policy policy) {
	init(jobs, jobNum, policy);
	
	while(1) {
		if (jobsAreDone(jobs, jobNum)) break;

		checkJobsArrival(jobs, jobNum);

		scheduleNext(jobs, jobNum, policy);

		unitTime();
		if (runningID != -1)
			jobs[runningID].remain--;

		curTime++;
	}

	for (int i = 0; i < jobNum; ++i)
		printf("%s %d\n", jobs[i].name, jobs[i].pid);

	return 0;
}

#endif