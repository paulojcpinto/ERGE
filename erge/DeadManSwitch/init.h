#ifndef INIT_H
#define INIT_H
#include <QThread>
#include "GSM/cgms1.h"
#include "programscheduler.h"

extern pthread_mutex_t condition_mutex;
extern pthread_cond_t condition_cond;
extern CGSM1 *w;
extern ProgramScheduler p;


#endif // INIT_H
