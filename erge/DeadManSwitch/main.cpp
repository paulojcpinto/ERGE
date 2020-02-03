/*
#include <QCoreApplication>
#include<stdio.h>
#include<syscall.h>


int main(int argc, char *argv[])
{
        printf("adsa");
        system("ls \n");

       // system("cd /dev/\n");
   int x = system("echo 0 > /dev/led0\n");
   printf("\n\n %d \n\n",x);
return 0;

}
*/


#include <QtWidgets/qapplication.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "programscheduler.h"
#include<iostream>
#include <QThread>
#include "BluetoothModule/bluetooth_module.h"
#include "FingerPrintModule/fingerprintthread.h"
#include <time.h>
#include <init.h>
#include "GSM/cgms1.h"
#include "camerathreads.h"

using namespace std;

QSerialPort *serialptr;

static void sendPeriodicUpdate(int signo)
{

    LogHandler log("PERIODIC_UPDATE: ");

    if(signo==SIGALRM)
    {
        time_t rawtime;
        struct tm * timeinfo;

        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        stringstream ss;
        ss <<"Time info Aquired"<<asctime (timeinfo)<<"\n\n";
        log.writeToLog(ss.str());
        p.verifyReleaseTime ();

    }

}



void* cc(void * para)
{

//QThread::sleep(10);

w->initGSM();

}

int main(int argc, char **argv)
{

    pthread_t fingerPrintID;

    QApplication a(argc, argv);

    bluetooth_module b(&p);

    CGSM1 ola;
    pthread_t sumID;
    initThread(&p,&b);
    initBluet(&b);
    startThreads();
      w=  &ola;//l=&w;
     w->releaseSMS ("913753546", "aaaa1" );
     // w->releaseSMS ("916201643", "aaaa2" );

    struct itimerval itv;

       //

    signal(SIGALRM,sendPeriodicUpdate);

    //ualarm(300,300);
    itv.it_interval.tv_sec = 60;
    itv.it_interval.tv_usec = 0;//4*10000;
    itv.it_value.tv_sec = 1;
    itv.it_value.tv_usec = 0;//4*10000;
    setitimer (ITIMER_REAL, &itv, NULL);

    pthread_create (&fingerPrintID, NULL, fingerPrintThread, (void *) &p);

   // pthread_create(&sumID, NULL, cc, NULL);



    return  a.exec();

}

