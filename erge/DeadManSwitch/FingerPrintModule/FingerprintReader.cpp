#include "FingerprintReader.h"
#include "fingerprintthread.h"
#include <fcntl.h>
#include <stdio.h>

Fingerprint::Fingerprint() {
    uart = (HardwareSerial*)malloc(sizeof(HardwareSerial));
    finger = (Adafruit_Fingerprint*)malloc(sizeof(Adafruit_Fingerprint));
}

Fingerprint::~Fingerprint() { }

int8_t Fingerprint::FingerprintRead(uint8_t slot){
	while (finger->getImage() != FINGERPRINT_OK);
	return finger->image2Tz(slot);
}

void Fingerprint::noFinger(void){
	while (finger->getImage() != FINGERPRINT_NOFINGER);
}

int8_t Fingerprint::FingerprintEnroll(int ID){
    if ((finger->createModel() | (finger->storeModel(ID))) != FINGERPRINT_OK) return -1;
    //printf("%d",ID);
    return 0;
}

uint8_t Fingerprint::searchFingerprint(void) {
  	if (finger->fingerFastSearch() != FINGERPRINT_OK) return 0;
  	return finger->fingerID;
}

int8_t Fingerprint::Enroll(int ID){
    //printf("Waiting...");
    LogHandler log("FINGERPRINT_READER: ");
	if ((FingerprintRead(1) != FINGERPRINT_OK) || (searchFingerprint() != 0)) return -1;

    //printf("Remove finger\n");
    log.writeToLog("Remove finger");
    //qDebug()<<"Remove finger\n";
	noFinger();		
    //printf("Place same finger again\n");
    //qDebug()<<"lace same finger again\n";
    log.writeToLog("Place same finger again");
    if ((FingerprintRead(2) | FingerprintEnroll(ID)) != FINGERPRINT_OK) return -1;
	printf("Stored!");
    return 0;
}
