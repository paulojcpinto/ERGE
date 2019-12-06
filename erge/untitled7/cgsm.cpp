#include "dialog.h"
#include "ui_dialog.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <string>
#include <QDebug>
#include <QMessageBox>

#include "cgsm.h"

CGSM::CGSM()
{

}

CGSM::CGSM( uint8_t productID, uint8_t vendorID, uint8_t baudrate )
{
  bool serialIsAvailable = false;
  QString serialPortName;


  //
  //  For each available serial port
  foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
      //  check if the serialport has both a product identifier and a vendor identifier
      if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier()){
          //  check if the product ID and the vendor ID match those of the arduino uno
          if((serialPortInfo.productIdentifier() == 8963)
                  && (serialPortInfo.vendorIdentifier() == 1659)){
              serialIsAvailable = true; //    arduino uno is available on this port
              serialPortName = serialPortInfo.portName();
          }
      }
  }
  if(serialIsAvailable){
      qDebug() << "Found the ... port...\n";
      serial->setPortName(serialPortName);
      serial->open(QSerialPort::ReadWrite);
      serial->setBaudRate(baudrate);
      serial->setDataBits(QSerialPort::Data8);
      serial->setFlowControl(QSerialPort::NoFlowControl);
      serial->setParity(QSerialPort::NoParity);
      serial->setStopBits(QSerialPort::OneStop);
     QObject::connect(serial, SIGNAL(readyRead()), this, SLOT(readSerial()));
  }else{
      qDebug() << "Couldn't find the correct port for the serial.\n";

  }
}

void CGSM::confirm_module()
{
  serial->write("AT\r\n");
}

