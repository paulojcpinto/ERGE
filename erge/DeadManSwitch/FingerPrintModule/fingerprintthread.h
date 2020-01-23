#include <QApplication>
#include <pthread.h>
#include <QDebug>
#include <QThread>
#include "FingerprintReader.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include "init.h"


extern void* fingerPrintThread(void * para);
