#ifndef DDRIVERS_H
#define DDRIVERS_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>
#include <iostream>

using namespace std;

class dDrivers{
	public:
		dDrivers();
		int LockStyleSolenoidWrite(const char buf[]);
		int ReedSwitchRead(void);
};

#endif // CDRIVER_H
