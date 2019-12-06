#ifndef BLUETOOTH_MODULE_H
#define BLUETOOTH_MODULE_H


#include <QtBluetooth/qbluetoothhostinfo.h>
#include <QtWidgets/qdialog.h>

QT_USE_NAMESPACE


class bluetooth_module
{

public:
    bluetooth_module();

private:
    QList<QBluetoothHostInfo> localAdapters;

};

#endif // BLUETOOTH_MODULE_H
