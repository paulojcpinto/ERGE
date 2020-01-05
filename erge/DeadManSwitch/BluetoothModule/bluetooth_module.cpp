#include "bluetooth_module.h"
#include <QtBluetooth/qbluetoothdeviceinfo.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtBluetooth/qbluetoothuuid.h>
#include "bluetooth_server.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <mqueue.h>
#include <unistd.h>


#define  PROGRAM_NAME "BluetoothModule: "

int imagesTaked;
bool endedFace,endedFInger;
ProgramScheduler *data2;
bluetooth_module* test;

static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");
using namespace  std;
bluetooth_module::bluetooth_module(ProgramScheduler* scheduler,QWidget *parent ): QDialog(parent)
{


    data=scheduler;
    data2=data;
    localAdapters = QBluetoothLocalDevice::allDevices();
    test= this;
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
    if ((fd =  ::open("/var/log/deadman.log",	O_CREAT | O_WRONLY | O_APPEND, 0600)) < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }




}


void bluetooth_module::writeToLog(string Message)
{
    string MessageToLog;
    MessageToLog= PROGRAM_NAME;
    MessageToLog.append(Message);
    MessageToLog.append("\n\n");
    write(fd,MessageToLog.c_str(),MessageToLog.length());

}
void bluetooth_module::connected(const QString &name)
{

}
void bluetooth_module::clientConnected(const QString &name)
{

    stringstream ss;
    ss<<"Client connected: "<<name.toStdString();
    writeToLog(ss.str());

}


void bluetooth_module::clientDisconnected()
{

    stringstream ss;
    ss<<"Somoene disoconneted: ";
    writeToLog(ss.str());

}
void bluetooth_module::clientDisconnected(const QString &name)
{
    stringstream ss;
    ss<<"Client disconnected: "<<name.toStdString();
    writeToLog(ss.str());

}

int parsingNumber(string aux)
{
  int auxInt;
  stringstream toint(aux);
  toint >> auxInt;
  return auxInt;

}


void* createDatasetFunc(void *threadid)
{
     data2->createUser(&imagesTaked,&endedFace,&endedFInger);
     pthread_exit(NULL);

}

void* datasetHandler(void *threadid)
{
    imagesTaked=0;
    endedFace=false;
    endedFInger=false;
    pthread_t doDataSet;
    int rTask= pthread_create(&doDataSet,NULL,createDatasetFunc,NULL);
    while(!endedFInger){};
    test->sendMessage("<F>");
    int imagesAux=0;
    while(!endedFace)
    {
        while (imagesAux < imagesTaked)
        {
           test->sendMessage("<I>");
           imagesAux++;
        }
    }
    pthread_join(doDataSet,NULL);
    stringstream ss;
    ss<<"Created user successfully!!";
    pthread_exit(NULL);
}

void bluetooth_module::receibedMessage(const QString &sender, const QString &message)
{
    stringstream ss;
    ss <<"Message Receibed: "<<message.toStdString() ;
    writeToLog(ss.str());
    ss.clear();
    string message_receibed = message.toStdString();
    if(message.contains("<L>"))
    {
        sendMessage("RASP");
    }else
    if(message.contains("<")&& message.contains(">"))
    {
        char c;
        c=message.at(1).toLatin1();
        string parser;

        switch (c) {
        case 'S':
             parsing(message,user.nickName,'S');
             ss<<"User Nickname:"<<user.nickName<<"\n\n";
            break;
        case 'P':
            parsing(message,user.pinCode,'P');
            ss<<"User Pincode: "<<user.pinCode<<"\n\n";
            break;
         case 'M':
            parsing(message,user.email,'M');
            ss<<"User Email: "<<user.email<<"\n\n";
            break;
         case 'X':
            parsing(message,user.emailPassword,'X');
            ss<<"User Email Password: "<< user.emailPassword<<"\n\n";
            break;
         case 'T':
            parsing(message,user.phoneNumber,'T');
            ss<<"User Phone Number: "<<user.phoneNumber<<"\n\n";
            break;
         case 'R':
            parsing(message,user.messageToRelease,'R');
            ss<<"User Message to Release: "<<user.messageToRelease<<"\n\n";
            break;
        case 'O':
            parsing(message,parser,'O');
            user.repeatTime = parsingNumber(parser);
            ss<<"User Repeat Time: "<<user.repeatTime<<"\n\n";
            break;
        case 'D':
            parsing(message,user.dateToStart,'D');
            ss<<"User Date to Start: "<<user.dateToStart<<"\n\n";
            break;
        case 'A':
            parsing(message,user.platformToRelease,'A');
            ss<<"User Platform to Release: "<<user.platformToRelease<<"\n\n";
            break;
         case 'C':
            sendResult(data->addUser(user),'C');
            ss<<"added User?";
            break;
         case 'Q':
           sendResult(data->login(user.nickName,user.pinCode),'Q');
           ss<<"Login from user: "<<user.nickName<<" with pincode: "<<user.pinCode;
            break;
         case 'Y':
            pthread_t create;
            int rThread;
            rThread=pthread_create(&create, NULL, datasetHandler, NULL);
            if(rThread)
            {
                stringstream ss;
                ss<<"ERROR: return code from pthread_create() is: "<<rThread;
                writeToLog(ss.str());
            }

            break;
        default:
            ss<<"1 character:"<<c<<"\n";

            break;
        }
        writeToLog(ss.str());
    }


}





void bluetooth_module::sendResult(int result,char c)
{
     QString answer = "<";
     answer.append(c);
     answer.append(QString::number(result));
     answer.append('>');
     stringstream ss;
     ss<<"Message sended: "<<answer.toStdString() ;
     writeToLog(ss.str());
     sendMessage(answer);
}

void bluetooth_module::parsing(QString input, string &output, char answer)
{
    output="";
    for(int i = 2; i<input.length()-1;i++)
       output += input.at(i).toLatin1();
    QString answer_back = "<";
    answer_back.append(answer);
    answer_back.append(QString::number(output.length()));
    answer_back.append(">");
    stringstream ss;
    ss<<"Message sended: "<<answer_back.toStdString()<< "\n";

    writeToLog(ss.str());
    sendMessage(answer_back);
}
void bluetooth_module::reactOnSocketError(const QString &error)
{

}
