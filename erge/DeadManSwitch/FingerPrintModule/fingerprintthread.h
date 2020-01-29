#include <QApplication>
#include <pthread.h>
#include <QDebug>
#include <QThread>
#include "FingerprintReader.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include "init.h"
#include "BluetoothModule/bluetooth_module.h"

extern void initBluet(bluetooth_module* );
extern void* fingerPrintThread(void * para);
extern unsigned int createFingerPrint();
