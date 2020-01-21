#ifndef FINGERPRINTREADER_H
#define FINGERPRINTREADER_H

#include <string>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <ctime>
#include <mqueue.h>

#include "uart.h"
#include "Adafruit_Fingerprint.h"

using namespace std;

class Fingerprint{
public:
    HardwareSerial* uart;
    Adafruit_Fingerprint* finger;
    Fingerprint();
    ~Fingerprint();
    int8_t FingerprintRead(uint8_t);
    void noFinger(void);
    int8_t FingerprintEnroll(int);
    int8_t Enroll(int);
    uint8_t searchFingerprint(void);
};

#endif // CDRIVER_H
