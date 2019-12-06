#include "bluetooth_module.h"
#include <QtBluetooth/qbluetoothdeviceinfo.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtBluetooth/qbluetoothuuid.h>

static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");

bluetooth_module::bluetooth_module()
{

    localAdapters = QBluetoothLocalDevice::allDevices();





}
