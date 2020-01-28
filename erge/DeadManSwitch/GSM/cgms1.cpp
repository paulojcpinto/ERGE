#include "cgms1.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <QThread>

using namespace std;


pthread_cond_t sGsmConfigured;
pthread_mutex_t count_mutex;
static bool sending = true;



typedef struct release
{
    char * message;
    char * number;
}release;

static vector <release> vRelease;


release messagesRelease[100];

enum responseGSM { ERROR = false, OK = true } responseGSM;

volatile uint8_t stateConfiguration = 0;

volatile bool GsmConfigured = ERROR;


CGSM1::CGSM1(QWidget *parent) :
    QDialog(parent)
{
  serial = new QSerialPort(this);
     /*
      *
      *  Testing code, prints the description, vendor id, and product id of all ports.
      *  Used it to determine the values for the arduino uno.
      *
      *
     qDebug() << "Number of ports: " << QSerialPortInfo::availablePorts().length() << "\n";
     foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
         qDebug() << "Description: " << serialPortInfo.description() << "\n";
         qDebug() << "Has vendor id?: " << serialPortInfo.hasVendorIdentifier() << "\n";
         qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier() << "\n";
         qDebug() << "Has product id?: " << serialPortInfo.hasProductIdentifier() << "\n";
         qDebug() << "Product ID: " << serialPortInfo.productIdentifier() << "\n";
     }
     */


     /*
      *   Identify the port the arduino uno is on.
      */
     bool arduino_is_available = false;
     QString arduino_uno_port_name;
     //
     //  For each available serial port
     foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
         //  check if the serialport has both a product identifier and a vendor identifier
         if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier()){
             //  check if the product ID and the vendor ID match those of the arduino uno
             if((serialPortInfo.productIdentifier() == arduino_uno_product_id)
                     && (serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id)){

                 arduino_is_available = true; //    arduino uno is available on this port
                 arduino_uno_port_name = serialPortInfo.portName();
             }
         }
     }


     /*
      *  Open and configure the arduino port if available
      */
     if(arduino_is_available){
          qDebug() <<arduino_uno_port_name;
         qDebug() << "Found the arduino port...\n";
         serial->setPortName(arduino_uno_port_name);
         serial->open(QSerialPort::ReadWrite);
         serial->setBaudRate(QSerialPort::Baud9600);
         serial->setDataBits(QSerialPort::Data8);
         serial->setFlowControl(QSerialPort::NoFlowControl);
         serial->setParity(QSerialPort::NoParity);
         serial->setStopBits(QSerialPort::OneStop);
         QObject::connect(serial, SIGNAL(readyRead()), this, SLOT(readSerial()));
         QObject::connect(this, SIGNAL(init2()), this, SLOT(initGSM1()));

     }else{
         qDebug() << "Couldn't find the correct port for the serial.\n";
         QMessageBox::information(this, "Serial Port Error", "Couldn't open serial port to serial.");
     }    //serial->write("AT+CMEE=2\r\n");


     //serial->write("AT+CPIN=7484\r\n");
  //serial->write("AT\r\n");//
     //serial->write("AT+CRES\r\n");
 //serial->write("AT+ATZ=0");
 //serial->write("AT+CSCS=?\r\n!");
     //serial->write("AT+CMGF=1\r\n");
   //serial->write("AT+CMGS=\"+351933288042\"\r\n");
 //serial->write("AT+CCID\r\n");
     //serial->write("AT\r\n");
    // serial->write("AT+CMGF=1\r\n");
 //serial->putChar(27);
 //serial->write("AT+CMEE=2\r\n");
   //  serial->write("ATE0\r\n");
 //serial->write("AT+CSMINS\r\n");
     //serial->putChar(27);
    //serial->write("paulo\r\n");
  // serial->putChar(26);
    // serial->write("AT+CGATT=1\r\n");
    // serial->write("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n");
    // serial->write("AT+SAPBR=3,1,\"APN\",\"RCMNET\"\r\n" );
     //serial->write("AT\r\n");


}

CGSM1::~CGSM1()
{
    if(serial->isOpen()){
        serial->close(); //    Close the serial port if it's open.
    }
    delete ui;
}

void CGSM1::readSerial()
{
    /*
     * readyRead() doesn't guarantee that the entire message will be received all at once.
     * The message can arrive split into parts.  Need to buffer the serial data and then parse for the temperature value.
     *
     */

  //  QStringList buffer_split = serialBuffer.split(","); //  split the serialBuffer string, parsing with ',' as the separator

    //  Check to see if there less than 3 tokens in buffer_split.
    //  If there are at least 3 then this means there were 2 commas,
    //  means there is a parsed temperature value as the second token (between 2 commas)
    //if(buffer_split.length() < 3){
        // no parsed value yet so continue accumulating bytes from serial in the buffer.
 // qDebug () << serial->size();
  serialData += serial->readAll();
        qDebug() <<serialData;
        if(serialData.contains("\r\n"))
        {
            prepareGSM();
        }

      //  serialBuffer = serialBuffer + QString::fromStdString(serialData.toStdString());
        //serialData.clear();
  //  }else{
        // the second element of buffer_split is parsed correctly, update the temperature value on temp_lcdNumber
     //   serialBuffer = "";
  //      qDebug() << buffer_split << "\n";
    //    parsed_data = buffer_split[1];

  //  }

}


void CGSM1::prepareGSM ()
{
  switch (stateConfiguration) {
    case 0:
      {
        if(serialData.contains("OK\r\n"))
          {
            stateConfiguration ++ ;
            serialData.clear();
            serial->write("AT+CPIN=7484\r\n");
          }
         else if (serialData.contains("ERROR\r\n"))
          {
            serialData.clear();
            serial->write("AT\r\n");
          }
      } break;

    case 1 :
      {
        if (serialData.contains("SMS Ready\r\n")              ||
            serialData.contains("operation not allowed\r\n")) /* This means that the card was already unlocked */
          {
            if (vRelease.empty ())
            {
                         stateConfiguration = 100 ;
            serialData.clear();
            //serial->write("AT+CMGF=1\r\n");
          }
            else
            {
                stateConfiguration =2;

                serial->write("AT+CMGF=1\r\n");
            }
}
      }break;

    case 2:
      {
        if(serialData.contains("OK\r\n"))
          {
            stateConfiguration ++ ;
            serialData.clear();
 //           release aux;
   //         aux.number = "933288042";
     //       aux.message = "as";
       //     vRelease.push_back (aux);
            serial->write("AT+CMGS=\"+351");
            serial->write (vRelease[0].number);
            serial->write ("\"\r\n");
          }
         else if (serialData.contains("ERROR\r\n"))
          {
            serialData.clear();
            serial->write("AT+CMGF=1\r\n");          }
      }break;
    case 3:
      {
         if(serialData.contains("\n>"))
           {
             stateConfiguration ++ ;
             serialData.clear();
             serial->write (vRelease[0].message);
 //            serial->write("****   ****\n"
   //                        "*    *     *\n"
     //                      "*    *     *\n"
       //                    "****      *\n"
         //                  "*    *     *\n"
           //                "*    *     *\n"
             //              "*    *     *");
             /*serial->write("                ****\n"
                           "                     *\n"
                           "Grupo:     ***\n"
                           "                      *\n"
                           "                ****");*/
           //  QThread::sleep(1);
             serial->putChar(26);
             pthread_cond_signal(&sGsmConfigured);
             GsmConfigured = true;
           }
      }break;
    case 4:
    {
       if(serialData.contains("OK\r\n"))
       {
           vRelease.erase (vRelease.begin ());
           if (!vRelease.empty ())
           {
               stateConfiguration = 2;
               serial->write("AT+CMGF=1\r\n");
           }
           else
           {
               stateConfiguration = 100;
               sending =false;
           }
       }
       else if (serialData.contains("ERROR\r\n"))
       {
           stateConfiguration = 2;
           serial->write("AT+CMGF=1\r\n");
       }
     }

    }

}

void CGSM1::updateTemperature(QString sensor_reading)
{
serial->write("AT\r\n");
}

void CGSM1::initGSM()
{
  qDebug() << "ok";
  init2();
//serial->write("AT\r\n");
}

void CGSM1::releaseSMS(char* number, char* message)
{

    release aux;
    qDebug()<<"\n\n\n" <<  "release ok?"<<"\n\n\n";
    aux.number = "933288042";
    aux.message = message;
    vRelease.push_back (aux);
    if (sending == false)
    {
        serialData.clear();
        serial->write("AT+CMGF=1\r\n");
        stateConfiguration=2;
    }
}

