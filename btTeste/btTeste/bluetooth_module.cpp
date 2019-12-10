#include "bluetooth_module.h"
#include <QtBluetooth/qbluetoothdeviceinfo.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtBluetooth/qbluetoothuuid.h>
#include "bluetooth_server.h"
#include <iostream>
#include <sstream>

static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");
using namespace  std;
bluetooth_module::bluetooth_module(QWidget *parent ): QDialog(parent)
{

    localAdapters = QBluetoothLocalDevice::allDevices();
    QBluetoothLocalDevice adapter(localAdapters.at(0).address());
    adapter.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    server = new bluetooth_server(this);
    cout<<"Started\n";
    connect(server, QOverload<const QString &>::of(&bluetooth_server::clientConnected),
            this, &bluetooth_module::clientConnected);
    connect(server, QOverload<const QString &>::of(&bluetooth_server::clientDisconnected),
            this,  QOverload<const QString &>::of(&bluetooth_module::clientDisconnected));
    connect(server, &bluetooth_server::messageReceived,
            this,  &bluetooth_module::receibedMessage);

    connect(this, &bluetooth_module::sendMessage, server, &bluetooth_server::sendMessage);

    server->startServer();
    //! [Create Chat Server]

    //! [Get local device name]
    localName = QBluetoothLocalDevice().name();




}
void bluetooth_module::connected(const QString &name)
{

}
void bluetooth_module::clientConnected(const QString &name)
{

    cout << "Connected to :"<<name.toStdString()<<"  \n";
    //sendMessage("Coisa");
}


void bluetooth_module::clientDisconnected()
{

}
void bluetooth_module::clientDisconnected(const QString &name)
{

}

int parsingNumber(string aux)
{
  int auxInt;
  stringstream toint(aux);
  toint >> auxInt;
  return auxInt;

}

void bluetooth_module::receibedMessage(const QString &sender, const QString &message)
{
    cout <<"Message Receibed: "<<message.toStdString()<<"\n" ;
    string message_receibed = message.toStdString();
    if(message.contains("<L>"))
    {
        sendMessage("RASP");
    }
    if(message.contains("<")&& message.contains(">"))
    {
        char c;
        c=message.at(1).toLatin1();
        string parser;

        switch (c) {
        case 'S':
             parsing(message,user.nickName,'S');
             cout<<"User Nickname:"<<user.nickName<<"\n\n";
            break;
        case 'P':
            parsing(message,user.pinCode,'P');
            cout<<"User Pincode: "<<user.pinCode<<"\n\n";
            break;
         case 'M':
            parsing(message,user.email,'M');
            cout<<"User Email: "<<user.email<<"\n\n";
            break;
         case 'X':
            parsing(message,user.emailPassword,'X');
            cout<<"User Email Password: "<< user.emailPassword<<"\n\n";
            break;
         case 'T':
            parsing(message,user.phoneNumber,'T');
            cout<<"User Phone Number: "<<user.phoneNumber<<"\n\n";
            break;
         case 'R':
            parsing(message,user.messageToRelease,'R');
            cout<<"User Message to Release: "<<user.messageToRelease<<"\n\n";
            break;
        case 'O':
            parsing(message,parser,'O');
            user.repeatTime = parsingNumber(parser);
            cout<<"User Repeat Time: "<<user.repeatTime<<"\n\n";
            break;
        case 'D':
            parsing(message,user.dateToStart,'D');
            cout<<"User Date to Start: "<<user.dateToStart<<"\n\n";
            break;
        case 'A':
            parsing(message,user.platformToRelease,'A');
            cout<<"User Platform to Release: "<<user.platformToRelease<<"\n\n";
            break;
        default:
            cout<<"1 character:"<<c<<"\n";

            break;
        }
    }


}
void bluetooth_module::parsing(QString input, string &output, char answer)
{
    for(int i = 2; i<input.length()-1;i++)
       output += input.at(i).toLatin1();
    cout<<"\n" << "Pasring of: "<<answer<<" Result: "<<output<<"\n";
    QString answer_back = "<";
    answer_back.append(answer);
    answer_back.append(QString::number(output.length()));
    answer_back.append(">");
    cout<<"Message sended: "<<answer_back.toStdString()<< "\n";

    sendMessage(answer_back);
}
void bluetooth_module::reactOnSocketError(const QString &error)
{

}
