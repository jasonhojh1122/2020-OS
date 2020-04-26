#define _GNU_SOURCE

#include <string.h>
#include <memory.h>
#include "process.h"
#include "scheduler.h"

int main() {

	char policyStr[32];
	int jobNum;
	Policy policy;
	Job *jobs;

	scanf("%s%d", policyStr, &jobNum);
	
	jobs = (Job*)malloc(jobNum * sizeof(Job));

	for (int i = 0; i < jobNum; ++i) {
		scanf("%s%d%d", jobs[i].name, &jobs[i].ready, &jobs[i].remain);
		jobs[i].pid = -1;
		jobs[i].isReady = 0;
	}

	if (strcmp(policyStr, "FIFO") == 0)
		policy = FIFO;
	else if (strcmp(policyStr, "RR") == 0)
		policy = RR;
	else if (strcmp(policyStr, "SJF") == 0)
		policy = SJF;
	else if (strcmp(policyStr, "PSJF") == 0)
		policy = PSJF;

	scheduling(jobs, jobNum, policy);
	return 0;
}