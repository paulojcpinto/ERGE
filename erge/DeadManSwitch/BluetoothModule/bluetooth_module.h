#ifndef BLUETOOTH_MODULE_H
#define BLUETOOTH_MODULE_H


#include <QtBluetooth/qbluetoothhostinfo.h>
#include <QtWidgets/qdialog.h>
#include "bluetooth_server.h"
#include <string>
#include "programscheduler.h"

QT_USE_NAMESPACE



using namespace std;


class bluetooth_module : public QDialog
{
    Q_OBJECT

public:
        bluetooth_module(ProgramScheduler* scheduler, QWidget *parent = nullptr);
        void sendFingerSucces (string );

signals:
     void sendMessage(const QString &message);

private slots:
    void receibedMessage(const QString &sender, const QString &message);
    void clientDisconnected(const QString &name);
    void clientDisconnected();
    void connected(const QString &name);
    void reactOnSocketError(const QString &error);
    void clientConnected(const QString &name);



private:
    user_parsing user;
    int adapterFromUserSelection() const;
    int currentAdapterIndex = 0;
    int fd;
    ProgramScheduler *data;
    bluetooth_server *server;
    QList<QBluetoothHostInfo> localAdapters;
    QString localName;
    void parsing(QString input,string &output,char answer);
    void writeToLog(string Message);
    void sendResult(int result,char c);
    void clear();

};

#endif // BLUETOOTH_MODULE_H
