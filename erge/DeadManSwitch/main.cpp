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

/*
#include <fcntl.h>
#include <stdio.h>

#define BUTTONS "/dev/buttons"

int main (void){

    char buffer[1]={0};
    FILE* fd ;


    //read (fd,buffer,3,NULL);
    while(1){
        fd = fopen (BUTTONS, "r" );
        if(fd == NULL){
            printf ("Device doesn't exist\n");
            return -1;
        }
    fread(buffer,sizeof(int),1,fd);

    printf ("Values on buffer: %s\n", buffer);
    //printf ("Read it");
    fclose (fd);}
    return 0;
}
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
//#include <bluetooth/bluetooth.h>
//#include <bluetooth/rfcomm.h>
#include <errno.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "programscheduler.h"
#include "../HeaderFiles/FingerprintReader.h"
#include<iostream>
#include <QThread>

using namespace std;


/*int main(int argc, char **argv)
{
    struct sockaddr_rc addr = { 0 };
    int s, status;
    char dest[18] = "70:BB:E9:EF:EC:AE";

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    // send a message
    if( status == 0 ) {
        status = write(s, "hello!", 6);
    }

    if( status < 0 ) perror("uh oh");

    close(s);
    return 0;
}*/
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

int main(int argc, char **argv)
{




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

/*
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);
system("hciconfig hci0 piscan");
    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // bind socket to port 1 of the first available
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = {{0, 0, 0, 0, 0, 0}};
    loc_addr.rc_channel = (uint8_t) 1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    // put socket into listening mode
    listen(s, 1);

    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);

  //  ba2str( &rem_addr.rc_bdaddr, buf );
 //   fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));

    // read data from the client
    bytes_read = read(client, buf, sizeof(buf));
    if( bytes_read > 0 ) {
        printf("received [%s]\n", buf);
    }

    // close connection
    close(client);
    close(s);
    return 0;*/
}

/*
 *
#include <stdio.h>
#include <time.h>

#define PST (-8)
#define CET (1)

int main ()
{
        time_t raw_time;
        struct tm *ptr_ts;

        time ( &raw_time );
        ptr_ts = gmtime ( &raw_time );

        printf ("Time Los Angeles: %2d:%02d\n",
             ptr_ts->tm_hour, ptr_ts->tm_min);
        printf ("Time Amsterdam: %2d:%02d\n",
             ptr_ts->tm_year+1900, ptr_ts->tm_mday);
        return 0;
 }


 * */
