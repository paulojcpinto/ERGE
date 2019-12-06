#ifndef CGSM_H
#define CGSM_H

#include <QSerialPort>
#include <QDialog>
#include "dialog.h"


class CGSM : public QDialog
{
  private:
    QSerialPort *serial;
  public:
    CGSM();
    CGSM( uint8_t productID, uint8_t vendorID, uint8_t baudrate );
    void confirm_module();
};

#endif // CGSM_H
