/*#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <QByteArray>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    explicit Dialog(int baudrate, quint16 arduino_uno_vendor_id, quint16 arduino_uno_product_id, QWidget *parent = 0 );
    void readSerial();
    void readSerial1();

    ~Dialog();

private slots:
    void updateTemperature(QString);

private:
    Ui::Dialog *ui;
    QSerialPort *serial;
    quint16 arduino_uno_vendor_id=1659;
    quint16 arduino_uno_product_id=8963;
    QByteArray serialData;
    QString serialBuffer;
    QString parsed_data;
    double temperature_value;
};

#endif // DIALOG_H
*/
#ifndef CGSM1_H
#define CGSM1_H

#include <QDialog>
#include <QSerialPort>
#include <QByteArray>
#include <string>

namespace Ui {
class CGSM1;
}

class CGSM1 : public QDialog
{
    Q_OBJECT

public:
    explicit CGSM1(QWidget *parent = 0);
    void prepareGSM ( void );
    void initGSM( );
    void releaseSMS( char* number, char* message );
    ~CGSM1();

signals:
    void init2();

private slots:
    void readSerial();
    void updateTemperature(QString);

    void initGSM1()
    {
      //qDebug() << "ok";
    serial->write("AT+CMEE=2\r\n");
    }
private:
    Ui::CGSM1 *ui;
    void (*funcptrs)();
    QSerialPort *serial;
    static const quint16 arduino_uno_vendor_id = 1659;
    static const quint16 arduino_uno_product_id = 8963;
    QByteArray serialData;
    QString serialBuffer;
    QString parsed_data;
    double temperature_value;

};

#endif // CGSM1_H
