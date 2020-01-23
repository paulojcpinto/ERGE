#include "fingerprintthread.h"
#include <fcntl.h>
#include <stdio.h>
#include "programscheduler.h"

#define BUTTONS "/dev/buttons"

typedef struct fingerprintStatus
{
    bool needCreateFingerPrint;
    bool createSuccess;
    unsigned int fingerID;
} fingerprintStatus;

static volatile fingerprintStatus fingerPrintStatus;


void* fingerPrintThread(void * para)
{

    ProgramScheduler * scheduler = (ProgramScheduler *) para;

QThread::sleep(1);
Fingerprint f;

uint8_t Image = 0;
char device[13];
int ID=1;
char buffer[1]={0};
FILE* fd ;
fingerPrintStatus.createSuccess = false;
fingerPrintStatus.needCreateFingerPrint = false;

strcpy(device,"/dev/ttyUSB0");
int x= f.uart->open(device);
if (x != 0)
  {
    strcpy(device,"/dev/ttyUSB1");
    f.uart->open(device);
  }


f.finger->setHardwareSerial(f.uart);
f.finger->begin(57600);


if(f.finger->verifyPassword())
{
        printf("Found fingerprint sensor! \n");
        qDebug ()<<"Found fingerprint sensor! \n";
}
else{
  printf("Did Not Found fingerprint sensor! \n");
                    qDebug ()<<"DID NOT Found fingerprint sensor! \n";
                    return 0;
}
int aux = 0;

while (1)
  {
    fd = fopen (BUTTONS, "r" );
        if(fd == NULL){
            printf ("Device doesn't exist\n");
            return 0;
        }
        //read (fd,buffer,3,NULL);
        fread(buffer,sizeof(int),3,fd);
       // qDebug ()<<"Values on buffer: "<<buffer[0]<<" \n";

        //printf ("Read it");
        fclose (fd);
     if (buffer[0] == '0')
     {
         if (!fingerPrintStatus.needCreateFingerPrint)
         {
        uint8_t fingerID=0;
     f.finger->getImage();
     f.finger->image2Tz(1);

     fingerID = f.searchFingerprint ();
     if (fingerID)
       {
          qDebug ()<<"Finger Print ID =" << fingerID <<"\n\n";
        printf("\n%d -> Finger Print ID =  %d\n",aux++, fingerID);
       pthread_cond_signal (&condition_cond);
       scheduler->doPresenceCheck ( fingerID );
         //sleep (10);
       }
     else
     {
       printf("\n%d -> That Finger Print is not stored\n",aux++);
      qDebug ()<<" That Finger Print is not stored \n";
     }

    }
         else if (fingerPrintStatus.needCreateFingerPrint)
         {
             qDebug()<< "init";
             uint8_t p;
             p = FINGERPRINT_INVALIDIMAGE;
             while (p != FINGERPRINT_OK)
             {
                 p = f.finger->getImage();
                 //printf("\n\n1\n\n");
                 //return -1;
                 switch (p) {
                     case FINGERPRINT_OK:
                       qDebug() <<("\nImageTaken\n");
                       break;
                     case FINGERPRINT_NOFINGER:
                         qDebug()<<("\nnoFinger");
                       break;
                     case FINGERPRINT_PACKETRECIEVEERR:
                       qDebug()<<("\nCommunication error");
                       break;
                     case FINGERPRINT_IMAGEFAIL:
                       qDebug()<<("\nImaging error");
                       break;
                     default:
                       qDebug()<<("\nUnknown error");
                       break;
                     }
             }
            p=FINGERPRINT_INVALIDIMAGE;

             while (p != FINGERPRINT_OK)
             {
               p = f.finger->image2Tz(1);
               switch (p) {
                   case FINGERPRINT_OK:
                     qDebug()<<("\n\nImage converted");
                     break;
                   case FINGERPRINT_IMAGEMESS:
                     qDebug()<<("\n\nImage too messy");

               }
               }

            p = f.finger->fingerFastSearch();

               qDebug()<<("\n\nFingerPrintID = %d\n\n", f.finger->fingerID);
             qDebug()<<("\n\Confidence = %d\n\n", f.finger->confidence);


         p=FINGERPRINT_INVALIDIMAGE;
                 while (p != FINGERPRINT_OK)
                 {
                     p = f.finger->getImage();
                     switch (p) {
                         case FINGERPRINT_OK:
                           qDebug()<<("\nImageTaken\n");
                           break;
                         case FINGERPRINT_NOFINGER:
                             qDebug()<<("\nnoFinger");
                           break;
                         case FINGERPRINT_PACKETRECIEVEERR:
                           qDebug()<<("\nCommunication error");
                           break;
                         case FINGERPRINT_IMAGEFAIL:
                           qDebug()<<("\nImaging error");
                           break;
                         default:
                           qDebug()<<("\nUnknown error");
                           break;
                         }
                 }


                 p=FINGERPRINT_INVALIDIMAGE;

                     while (p != FINGERPRINT_OK)
                     {
                     p = f.finger->image2Tz(2);
                     switch (p) {
                         case FINGERPRINT_OK:
                           qDebug()<<("\n\nImage converted");
                           break;
                         case FINGERPRINT_IMAGEMESS:
                           qDebug()<<("\n\nImage too messy");

                     }
                     }
             p=FINGERPRINT_INVALIDIMAGE;
             p = f.finger->createModel();
             if (p == FINGERPRINT_OK) {
               qDebug()<<("\nPrints matched!\n");
             } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
               qDebug()<<("\nCommunication error");

             }
             fingerPrintStatus.fingerID = scheduler->getNewFingerID ();
             fingerPrintStatus.createSuccess = false;
             p = f.finger->storeModel(fingerPrintStatus.fingerID);
             if (p == FINGERPRINT_OK) {
                 qDebug()<<("\nStored!");
                 fingerPrintStatus.createSuccess = true;
                 fingerPrintStatus.needCreateFingerPrint = false;
               } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
                 fingerPrintStatus.createSuccess = false;
                 fingerPrintStatus.needCreateFingerPrint = false;
                 qDebug()<<("\nCommunication error");
                 qDebug() <<"end";
             }else
             {
                 fingerPrintStatus.createSuccess = false;
                 fingerPrintStatus.needCreateFingerPrint = false;
             }


         }
  }
     sleep (1);
}

}


unsigned int createFingerPrint ( void )
{
    fingerPrintStatus.createSuccess = false;
    fingerPrintStatus.needCreateFingerPrint = true;
    while (fingerPrintStatus.needCreateFingerPrint) {
        ;

    }
    if (fingerPrintStatus.createSuccess)
        return fingerPrintStatus.fingerID;
    return  0;

}
