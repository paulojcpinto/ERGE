/*#include "../HeaderFiles/FingerprintReader.h"
#include <stdio.h>
#include <QThread>

Fingerprint f;			

int main(int argc, char* argv[])
{
	uint8_t Image = 0;
	char device[13];
	int ID=1;
	strcpy(device,"/dev/ttyUSB0");
	f.uart->open(device);
	f.finger->setHardwareSerial(f.uart);
	f.finger->begin(57600);

	f.finger->getTemplateCount();
	printf("\n\n%d\n\n", f.finger->templateCount);

	if(f.finger->verifyPassword())
		printf("Found fingerprint sensor! \n");
	else{
		printf("Did not Find fingerprint sensor!\n");
		return 1;
	}
f.finger->deleteModel (8);
f.finger->deleteModel (1);
int x = f.finger->templateCount;
printf("\n\n%d\n\n", x);fingerFastSearch
   // f.finger->emptyDatabase();
        uint8_t p = FINGERPRINT_INVALIDIMAGE;
        while (p != FINGERPRINT_OK)
        {
            p = f.finger->getImage();
            //printf("\n\n1\n\n");
            //return -1;
            switch (p) {
                case FINGERPRINT_OK:
                  printf("\nImageTaken\n");
                  break;
                case FINGERPRINT_NOFINGER:
                    printf("\nnoFinger");
                  break;
                case FINGERPRINT_PACKETRECIEVEERR:
                  printf("\nCommunication error");
                  break;
                case FINGERPRINT_IMAGEFAIL:
                  printf("\nImaging error");
                  break;
                default:
                  printf("\nUnknown error");
                  break;
                }
        }
    p=FINGERPRINT_INVALIDIMAGE;

        while (p != FINGERPRINT_OK)
        {
        p = f.finger->image2Tz(1);
        switch (p) {
            case FINGERPRINT_OK:
              printf("\n\nImage converted");
              break;
            case FINGERPRINT_IMAGEMESS:
              printf("\n\nImage too messy");
              return p;
            case FINGERPRINT_PACKETRECIEVEERR:
              printf("\n\nCommunication error");
              return p;
            case FINGERPRINT_FEATUREFAIL:
              printf("\nCould not find fingerprint features");
              return p;
            case FINGERPRINT_INVALIDIMAGE:
              printf("\nCould not find fingerprint features");
              return p;
            default:
              printf("\nUnknown error");
              return p;
        }
        }
        p = f.finger->fingerFastSearch();fingerFastSearch

        printf("\n\nFingerPrintID = %d\n\n", f.finger->fingerID);
        printf("\n\Confidence = %d\n\n", f.finger->confidence);
        sleep(8);


        p = FINGERPRINT_INVALIDIMAGE;
    while (p != FINGERPRINT_OK)
    {
        p = f.finger->getImage();
        //printf("\n\n1\n\n");
        //return -1;
        switch (p) {
            case FINGERPRINT_OK:
              printf("\nImageTaken\n");
              break;
            case FINGERPRINT_NOFINGER:
                printf("\nnoFinger");
              break;
            case FINGERPRINT_PACKETRECIEVEERR:
              printf("\nCommunication error");
              break;
            case FINGERPRINT_IMAGEFAIL:
              printf("\nImaging error");
              break;
            default:
              printf("\nUnknown error");
              break;
            }
    }
p=FINGERPRINT_INVALIDIMAGE;

    while (p != FINGERPRINT_OK)
    {
    p = f.finger->image2Tz(1);
    switch (p) {
        case FINGERPRINT_OK:
          printf("\n\nImage converted");
          break;
        case FINGERPRINT_IMAGEMESS:
          printf("\n\nImage too messy");
          return p;
        case FINGERPRINT_PACKETRECIEVEERR:
          printf("\n\nCommunication error");
          return p;
        case FINGERPRINT_FEATUREFAIL:
          printf("\nCould not find fingerprint features");
          return p;
        case FINGERPRINT_INVALIDIMAGE:
          printf("\nCould not find fingerprint features");
          return p;
        default:
          printf("\nUnknown error");
          return p;
    }
    }
    p = f.finger->fingerFastSearch();

    printf("\n\nFingerPrintID = %d\n\n", f.finger->fingerID);
    printf("\n\Confidence = %d\n\n", f.finger->confidence);


p=FINGERPRINT_INVALIDIMAGE;
        while (p != FINGERPRINT_OK)
        {
            p = f.finger->getImage();
            switch (p) {
                case FINGERPRINT_OK:
                  printf("\nImageTaken\n");
                  break;
                case FINGERPRINT_NOFINGER:
                    printf("\nnoFinger");
                  break;
                case FINGERPRINT_PACKETRECIEVEERR:
                  printf("\nCommunication error");
                  break;
                case FINGERPRINT_IMAGEFAIL:
                  printf("\nImaging error");
                  break;
                default:
                  printf("\nUnknown error");
                  break;
                }
        }


        p=FINGERPRINT_INVALIDIMAGE;

            while (p != FINGERPRINT_OK)
            {
            p = f.finger->image2Tz(2);
            switch (p) {
                case FINGERPRINT_OK:
                  printf("\n\nImage converted");
                  break;
                case FINGERPRINT_IMAGEMESS:
                  printf("\n\nImage too messy");
                  return p;
                case FINGERPRINT_PACKETRECIEVEERR:
                  printf("\n\nCommunication error");
                  return p;
                case FINGERPRINT_FEATUREFAIL:
                  printf("\nCould not find fingerprint features");
                  return p;
                case FINGERPRINT_INVALIDIMAGE:
                  printf("\nCould not find fingerprint features");
                  return p;
                default:
                  printf("\nUnknown error");
                  return p;
            }
            }
    p=FINGERPRINT_INVALIDIMAGE;
    p = f.finger->createModel();
    if (p == FINGERPRINT_OK) {
      printf("\nPrints matched!\n");
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      printf("\nCommunication error");
      return p;
    } else if (p == FINGERPRINT_ENROLLMISMATCH) {
      printf("\nFingerprints did not match");
      return p;
    } else {
      printf("\nUnknown error");
      return p;
    }
    p = f.finger->storeModel(8);
    if (p == FINGERPRINT_OK) {
        printf("\nStored!");
      } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
        printf("\nCommunication error");
        return p;
      } else if (p == FINGERPRINT_BADLOCATION) {
        printf("\nCould not store in that location");
        return p;
      } else if (p == FINGERPRINT_FLASHERR) {
        printf("\nError writing to flash");
        return p;
      } else {
        printf("\nUnknown error");
        return p;
      }


    p = f.finger->getTemplateCount();
    printf("\n\n%d\n\n", f.finger->templateCount);
    p = f.finger->fingerFastSearch();

    printf("\n\nFingerPrintID = %d\n\n", f.finger->fingerID);

	while ( !f.Enroll(ID) );
	ID++;
	usleep(2);
	while ( !f.Enroll(ID) );
	ID++;
	usleep(2);
	while ( !f.Enroll(ID) );
	return 0;
}

*/
