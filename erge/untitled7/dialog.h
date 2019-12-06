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
#ifndef DIALOG_H
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
    ~Dialog();

private slots:
    void readSerial();
    void updateTemperature(QString);

private:
    Ui::Dialog *ui;

    QSerialPort *serial;
    static const quint16 arduino_uno_vendor_id = 1659;
    static const quint16 arduino_uno_product_id = 8963;
    QByteArray serialData;
    QString serialBuffer;
    QString parsed_data;
    double temperature_value;
};

#endif // DIALOG_H
