#include "dialog.h"
#include "ui_dialog.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <string>
#include <QDebug>
#include <QMessageBox>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->temp_lcdNumber->display("-------");
    serial = new QSerialPort(this);
    serialBuffer = "";
    parsed_data = "";
    temperature_value = 0.0;

    /*
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
        qDebug() << "Found the arduino port...\n";
        serial->setPortName(arduino_uno_port_name);
        serial->open(QSerialPort::ReadWrite);
        serial->setBaudRate(QSerialPort::Baud9600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        QObject::connect(serial, SIGNAL(readyRead()), this, SLOT(readSerial()));
    }else{
        qDebug() << "Couldn't find the correct port for the serial.\n";
        QMessageBox::information(this, "Serial Port Error", "Couldn't open serial port to serial.");
    }


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
    //serial->write("ATE0\r\n");
//serial->write("AT+CSMINS\r\n");
    //serial->putChar(27);
   //serial->write("paulo\r\n");
 // serial->putChar(26);
   // serial->write("AT+CGATT=1\r\n");
   // serial->write("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n");
   // serial->write("AT+SAPBR=3,1,\"APN\",\"RCMNET\"\r\n" );
    serial->write("AT\r\n");
}

Dialog::~Dialog()
{
    if(serial->isOpen()){
        serial->close(); //    Close the serial port if it's open.
    }
    delete ui;
}

void Dialog::readSerial()
{
    /*
     * readyRead() doesn't guarantee that the entire message will be received all at once.
     * The message can arrive split into parts.  Need to buffer the serial data and then parse for the temperature value.
     *
     */

    QStringList buffer_split = serialBuffer.split(","); //  split the serialBuffer string, parsing with ',' as the separator

    //  Check to see if there less than 3 tokens in buffer_split.
    //  If there are at least 3 then this means there were 2 commas,
    //  means there is a parsed temperature value as the second token (between 2 commas)
    if(buffer_split.length() < 3){
        // no parsed value yet so continue accumulating bytes from serial in the buffer.
        serialData = serial->readAll();
        qDebug() <<serialData;
        serialBuffer = serialBuffer + QString::fromStdString(serialData.toStdString());
        serialData.clear();
    }else{
        // the second element of buffer_split is parsed correctly, update the temperature value on temp_lcdNumber
        serialBuffer = "";
        qDebug() << buffer_split << "\n";
        parsed_data = buffer_split[1];
        temperature_value = (9/5.0) * (parsed_data.toDouble()) + 32; // convert to fahrenheit
        qDebug() << "Temperature: " << temperature_value << "\n";
        parsed_data = QString::number(temperature_value, 'g', 4); // format precision of temperature_value to 4 digits or fewer
        Dialog::updateTemperature(parsed_data);
    }

}

void Dialog::updateTemperature(QString sensor_reading)
{
    //  update the value displayed on the lcdNumber
    ui->temp_lcdNumber->display(sensor_reading);
}
