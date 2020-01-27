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
#include "loghandler.h"


#define  PROGRAM_NAME "BluetoothModule: "

int imagesTaked, imagesToAppend;
bool endedFace,endedFInger;
ProgramScheduler *data2;
bluetooth_module* test;
String currentUser;

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

    server->startServer(QBluetoothAddress());
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
     LogHandler log("CreateDatasetThread: ");
     log.writeToLog("thread created");
     data2->createUser(&imagesTaked,&endedFace,&endedFInger);
     log.writeToLog("Thread ended");
     pthread_exit(NULL);

}

void* appendImages(void *threadid)
{
    LogHandler log("AppendImages: ");
    log.writeToLog("thread created");
    //data2->appendImageDataset(imagesTaked,&endedFace,&endedFInger,imagesToAppend);
    data2->appendImageDataset(currentUser,imagesToAppend,&imagesTaked,&endedFace);
    log.writeToLog("Thread ended");
    pthread_exit(NULL);


}

void* appendHandler(void *threadid)
{
    imagesTaked=0;
    endedFace=false;
    pthread_t doAppend;
    LogHandler log("ThreadAppendHandler: ");
    log.writeToLog("Inicialized successfully");
    int rTask= pthread_create(&doAppend,NULL,appendImages,NULL);
    if(rTask)
    {
        stringstream ss;
        ss<<"ERROR: return code from pthread_create() is: "<<rTask;
        log.writeToLog(ss.str());
        pthread_exit(NULL);
    }
    int imagesAux=0;
    while(!endedFace)
    {
        while (imagesAux < imagesTaked)
        {
           test->sendMessage("<I>");
           imagesAux++;
        }
    }
    pthread_join(doAppend,NULL);
    stringstream ss;
    ss<<"Created user successfully!!";
    log.writeToLog(ss.str());
    pthread_exit(NULL);

}


void* datasetHandler(void *threadid)
{
    imagesTaked=0;
    endedFace=false;
    endedFInger=false;
    pthread_t doDataSet;
    LogHandler log("ThreadDataSetHandel: ");
    log.writeToLog("Inicialized successfully");
    int rTask= pthread_create(&doDataSet,NULL,createDatasetFunc,NULL);
    if(rTask)
    {
        stringstream ss;
        ss<<"ERROR: return code from pthread_create() is: "<<rTask;
        log.writeToLog(ss.str());
        pthread_exit(NULL);
    }
    while(!endedFInger){

        usleep(100);

           };
    log.writeToLog("Fingerprint created successfuly");
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
        c=message.at(message.indexOf('<')+1).toLatin1();
        string parser;
        QString auxMessage;
        switch (c) {
        case 'S':
            clear();
             parsing(message,user.nickName,'S');
             currentUser = user.nickName;
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
        case 'G':
           clear();
           for(int i = 2; i<message.length()-1;i++)
              user.nickName += message.at(i).toLatin1();
            //parsing(message,user.nickName,'G');
           currentUser = user.nickName;

            ss<<"Request to send info about user: "<<user.nickName<<"\n";
            if(data->getUserForUpdate(user.nickName,&user))
            {
                QString phoneNumber ="";
                phoneNumber.append("<t");
                phoneNumber.append(user.phoneNumber.c_str()) ;
                phoneNumber.append(">");
                sendMessage(phoneNumber);
                writeToLog("sended phone number to update:"+phoneNumber.toStdString());
            }
               else sendMessage("<G-1>");

            break;
        case'o':
            if(user.repeatTime!=0)
            {
                 QString repeatTime="";
                 repeatTime.append("<o");
                 repeatTime.append(QString::number(user.repeatTime));
                 repeatTime.append(">");
                 sendMessage(repeatTime);
                 ss<<"Sending email: "<<user.email<<", to mobile app";

            }

        break;
        case'm':
            if(!user.email.empty())
            {
                QString email="";
                email.append("<m");
                email.append(user.email.c_str());
                email.append(">");
                sendMessage(email);
            }
            break;
        case 'x':
            if(!user.emailPassword.empty())
            {
                QString emailPass="";
                emailPass.append("<x");
                emailPass.append(user.emailPassword.c_str());
                emailPass.append(">");
                sendMessage(emailPass);
                ss<<"Sending email password: "<<user.emailPassword<<", to mobile app";

            }
            break;
          case 'r':
            if(!user.messageToRelease.empty())
            {
                QString message="";
                message.append("<r");
                message.append(user.messageToRelease.c_str());
                message.append(">");
                sendMessage(message);
                ss<<"Sending message to release: "<<user.messageToRelease<<", to mobile app";
            }
            break;
        case 'a':
            if(!user.platformToRelease.empty())
            {
                QString platform ="";
                platform.append("<a");
                platform.append(user.platformToRelease.c_str());
                platform.append(">");
                sendMessage(platform);
                ss<<"Sending platform to release: "<<user.platformToRelease<<" to mobile app";
            }
            break;
        case 'U':
            ss<<"Updatring user:"<< user.nickName;
            data->updateFullUser(user,user.nickName);
        break;

        case 'F':
            pthread_t append;
            int rAppend;
            rAppend=pthread_create(&append, NULL, appendHandler, NULL);
            if(rAppend)
            {
                stringstream ss;
                ss<<"ERROR: return code from pthread_create() is: "<<rThread;
                writeToLog(ss.str());

            }
            auxMessage = "<F"+QString::number(data->getNumberofImages(currentUser))+">";
            sendMessage(auxMessage);

            break;
        case 'J':
            parsing(message,parser,'J');
            imagesToAppend = parsingNumber(parser);
            ss<<"Images to Append: "<<imagesToAppend<<"\n\n";
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
    for(int i = 2; i<input.indexOf('>');i++)
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
void bluetooth_module::clear()
{
    user.email="";
    user.pinCode="";
    user.nickName="";
    user.repeatTime=0;
    user.dateToStart="";
    user.phoneNumber="";
    user.emailPassword="";
    user.messageToRelease="";
    user.platformToRelease="";
}
