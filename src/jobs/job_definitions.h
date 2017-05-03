#ifndef JOB_DEFINITIONS_H_INCLUDED
#define JOB_DEFINITIONS_H_INCLUDED

#include "job.h"

enum class JobWantTo
{
	Nothing,
	RunNext,
};

struct JobTicket
{
	vsedit::Job * pJob;
	JobWantTo whenDone;
};

#endif // JOB_DEFINITIONS_H_INCLUDED
