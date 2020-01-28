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
/*
void *theadtry (void *arg)
{
  Fingerprint f;

  uint8_t Image = 0;
  char device[13];
  int ID=1;
  strcpy(device,"/dev/ttyUSB0");
  f.uart->open(device);
  f.finger->setHardwareSerial(f.uart);
  f.finger->begin(57600);


  if(f.finger->verifyPassword())
          printf("Found fingerprint sensor! \n");
  else{
          printf("Did not Find fingerprint sensor!\n");
  }
  int aux = 0;
  while (1)
    {
       uint8_t fingerID=0;
       f.finger->getImage();
       f.finger->image2Tz(1);

       fingerID = f.searchFingerprint ();
       if (fingerID)
          printf("\n%d -> Finger Print ID =  %d\n",aux++, fingerID);
       else
         printf("\n%d -> That Finger Print is not stored\n",aux++);
       sleep (3);
    }

 // printf("\n\n\paulo\nfingerprint ID = %d", fingerID);

}
*/
//CGSM1 *w;
static void sendPeriodicUpdate(int signo)
{


    if(signo==SIGALRM)
    {
        time_t rawtime;
        struct tm * timeinfo;

        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        qDebug() <<"\n\n"<<asctime (timeinfo)<<"\n\n";
        p.verifyReleaseTime ();
        // w->releaseSMS ("916201643", "pilinha minha" );
    }

}



  QSerialPort *serialptr;
void* cc(void * para)
{

QThread::sleep(1);

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
    startThreads();
      w=  &ola;//l=&w;
      w->releaseSMS ("916201643", "aaaa1" );
      w->releaseSMS ("916201643", "aaaa2" );

    struct itimerval itv;

            signal(SIGALRM,sendPeriodicUpdate);

            //ualarm(300,300);
            itv.it_interval.tv_sec = 60;
            itv.it_interval.tv_usec = 0;//4*10000;
            itv.it_value.tv_sec = 1;
            itv.it_value.tv_usec = 0;//4*10000;
            setitimer (ITIMER_REAL, &itv, NULL);

   // pthread_create (&fingerPrintID, NULL, fingerPrintThread, (void *) &p);
    pthread_create(&sumID, NULL, cc, NULL);
    vector<int> d;
    d.push_back (3);
    d.push_back (5);
    qDebug()<<"\n\n\n" << d.at (1);//<<d.at (1)<<"\n\n\n";

    int x =d.at (0);

/*

  time_t raw_time;
  struct tm *ptr_ts;
  pthread_t pps;
  int p;
  pthread_attr_t attr;
  time ( &raw_time );
  ptr_ts = gmtime ( &raw_time );
  ProgramScheduler pp;
  cin >> p;
pthread_create(&pps, NULL,theadtry, NULL);
pthread_join(pps, NULL);
  pp.addUser ();

  pp.verifyReleaseTime ();

  printf ("\n\n\n\n\nTime Los Angeles: %2d:%02d\n",
       ptr_ts->tm_hour, ptr_ts->tm_min);
  printf ("Time Amsterdam: %2d:%02d\n",
       ptr_ts->tm_year+1900, ptr_ts->tm_mday);
  return 0;
  */
    return  a.exec();

}

