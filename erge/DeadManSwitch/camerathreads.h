#ifndef CAMERATHREADS_H
#define CAMERATHREADS_H
#include "programscheduler.h"
#include"BluetoothModule/bluetooth_module.h"
extern pthread_cond_t startReco;
extern pthread_cond_t startFrame;

void  initThread(ProgramScheduler* programData,bluetooth_module* bluetooth);
void initRecognizer(string nickname);
void startThreads();



#endif // CAMERATHREADS_H
