#ifndef BLUETOOTH_MODULE_H
#define BLUETOOTH_MODULE_H


#include <QtBluetooth/qbluetoothhostinfo.h>
#include <QtWidgets/qdialog.h>
#include "bluetooth_server.h"
#include <string>

QT_USE_NAMESPACE


using namespace std;
struct user_parsing
{
    string nickName;
    string pinCode;
    string phoneNumber;
    string email;
    string emailPassword;
    string messageToRelease;
    int repeatTime;
    string dateToStart;
    string platformToRelease;

};

class bluetooth_module : public QDialog
{
    Q_OBJECT

public:
    bluetooth_module(QWidget *parent = nullptr);

signals:
    void sendMessage(const QString &message);

private slots:
    //void connectClicked();
    //void sendClicked();

    void receibedMessage(const QString &sender, const QString &message);

    //void clientConnected(const QString &name);
    void clientDisconnected(const QString &name);
    void clientDisconnected();
    void connected(const QString &name);
    void reactOnSocketError(const QString &error);
    void clientConnected(const QString &name);


private:
    user_parsing user;
    int adapterFromUserSelection() const;
    int currentAdapterIndex = 0;
    bluetooth_server *server;
    QList<QBluetoothHostInfo> localAdapters;
    QString localName;
    void parsing(QString input,string &output,char answer);


};

#endif // BLUETOOTH_MODULE_H
