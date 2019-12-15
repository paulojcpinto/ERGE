#include "sqlquery.h"
#include <sstream>
#include <iostream>

#define  PROGRAM_NAME "SqlQuerry: "

using namespace std;


SqlQuery::SqlQuery()
{
    msgq_query = "/database_query";
    msgq_callback = "/daetabase_callback";
    if ((fd = open("/var/log/database.log",	O_CREAT | O_WRONLY | O_APPEND, 0600)) < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }

}

SqlQuery::~SqlQuery()
{
    closeQueryQueue();
    closeCallbackQueue();
}

int parsingNumber(string aux)
{
  int auxInt;
  stringstream strToInt(aux);
  strToInt >> auxInt;
  return auxInt;

}
bool SqlQuery::openQueryQueue()
{
    msgq_id_query = mq_open(msgq_query, O_RDWR | O_EXCL, S_IRWXU | S_IRWXG, NULL);
    if (msgq_id_query == (mqd_t) - 1)
    {
        messageLog = PROGRAM_NAME;
        messageLog.append("Error opening Query Queue!!\n\n");
        write(fd,messageLog.c_str(),messageLog.length());
        perror("In query queue mq_open()");
        return false;

    }
    messageLog = PROGRAM_NAME;
    messageLog.append("Opened Query Queue successfully!!\n\n");
    write(fd,messageLog.c_str(),messageLog.length());
    return true;
}

bool SqlQuery::openCallbackQueue()
{
    msgq_id_callback = mq_open(msgq_callback, O_RDONLY);
     if (msgq_id_callback == (mqd_t) - 1)
    {
        messageLog = PROGRAM_NAME;
        messageLog.append("Error opening Query callback Queue!!\n\n");
        write(fd,messageLog.c_str(),messageLog.length());
        perror("In callback queue mq_open()");
        return false;
    }
     messageLog = PROGRAM_NAME;
     messageLog.append("Opened Query CallBack Queue successfully!!\n\n");
     write(fd,messageLog.c_str(),messageLog.length());
     return true;
}

bool SqlQuery::closeQueryQueue()
{
    if(mq_close(msgq_id_query) == 0)
    {
        messageLog = PROGRAM_NAME;
        messageLog.append("Closed Query Queue successfully!!\n\n");
        write(fd,messageLog.c_str(),messageLog.length());
        return true;
    }
    return false;
}

bool SqlQuery::closeCallbackQueue()
{
    if(mq_close(msgq_id_callback) == 0)
    {
         messageLog = PROGRAM_NAME;
         messageLog.append("Opened Query CallBack Queue successfully!!\n\n");
         write(fd,messageLog.c_str(),messageLog.length());
         return true;
    }
    return false;
}

bool SqlQuery::sendQuery(string custom_query) // change from const char *
{
    int msgsz;
    if(openQueryQueue())
    {
        memset(query, 0, sizeof(query));
        sprintf(query, custom_query.c_str());
        msgsz = mq_send(msgq_id_query, query, strlen(query)+1, 1);
        if (msgsz == -1)
        {
            messageLog = PROGRAM_NAME;
            messageLog.append("Error sending query to Queue!!\n\n");
            write(fd,messageLog.c_str(),messageLog.length());
            perror("In query queue mq_send()");
            return false;
        }
        messageLog = PROGRAM_NAME;
        messageLog.append("Sended Querry successfully: ");
        messageLog.append(custom_query);
        messageLog.append("\n\n");
        write(fd,messageLog.c_str(),messageLog.length());
        return closeQueryQueue();
    }
    return false;
}

bool SqlQuery::receiveQuery()
{
    int msgsz;
    unsigned int sender;
    if(openCallbackQueue())
    {
        memset(result, 0, sizeof(result));
        msgsz = mq_receive(msgq_id_callback, result, MAX_MSG_LEN, &sender);
        if (msgsz == -1)
        {
            messageLog = PROGRAM_NAME;
            messageLog.append("Error receibing query from Queue!!\n");
            write(fd,messageLog.c_str(),messageLog.length());
            perror("In callback queue mq_receive()");
            return false;
        }
        messageLog = PROGRAM_NAME;
        messageLog.append("Receibed from queue successfully: ");
        messageLog.append(result);
        messageLog.append("\n\n");
        write(fd,messageLog.c_str(),messageLog.length());
        return closeCallbackQueue();
    }
    return false;
}



string SqlQuery::getLastQueryResult()
{
    string str(result);
    return str;
}


bool SqlQuery::selectQuery(const char* column, const char* table, const char* cond_column, int cond)
{
    stringstream ss;
    ss << "select " << column << " from " << table << " where " << cond_column << "=" << cond;
    return sendQuery(ss.str());
}

bool SqlQuery::selectQuery(const char* column, const char* table, const char* cond_column, const char* cond)
{
    stringstream ss;
    ss << "select " << column << " from " << table << " where " << cond_column << "='" << cond << "'";
    return sendQuery(ss.str());
}

bool SqlQuery::selectQuery(const char* column, const char* table)
{
    stringstream ss;
    ss << "select " << column << " from " << table;
    return sendQuery(ss.str());
}

bool SqlQuery::selectQuery(const char *table)
{
    stringstream ss;
    ss <<"select * from "<<table;
    return sendQuery(ss.str());
}




/* Unsafe select functions */
string SqlQuery::selectQueryGetResponse(const char* column, const char* table, const char* cond_column, const char* cond)
{
    selectQuery(column, table, cond_column, cond);
    receiveQuery();
    return getLastQueryResult();
}

string SqlQuery::selectQueryGetResponse(const char* column, const char* table)
{
    selectQuery(column, table);
    receiveQuery();
    return getLastQueryResult();
}

bool SqlQuery::insertUser(fullUser muser)
{

    if(insertUser(muser.user))
    {
        if(insertFace(muser.user.NickName ,muser.faceInfo))
          if(insertFingerPrint(muser.user.NickName,muser.fingerInfo))
              if(insertMessageInfo(muser.user.NickName,muser.messageInfo))
                  return true;


    }
    return false;
}

bool SqlQuery::insertUser(const userQuery user)
{
    stringstream ss;
    ss<<"INSERT INTO MYUSER VALUES('"<<user.NickName<<"', '"<<user.PinCode << "', '"<<user.Email<<"', '"<<user.PhoneNumber<<"', '"<<user.EmailPassword<<"');";
    if(sendQuery(ss.str()))
    {
        return receiveQuery();
    }
    return false;
}

bool SqlQuery::insertFace(const string Nickname, const faceQuery faceInfo)
{
    stringstream ss;
    ss<<"INSERT INTO FACE(DataSetPath,NumberOfImages,Nickname) VALUES('"<<faceInfo.PathDataset<<"',"<<faceInfo.NumberOfImages<<", '"<<Nickname<<"');";
    if(sendQuery(ss.str()))
    {
        return receiveQuery();
    }
    return false;
}


bool SqlQuery::insertFingerPrint(const string Nickname, const fingerPrintQuery fingerInfo)
{
    stringstream ss;
    ss<<"INSERT INTO FINGERPRINT VALUES("<<fingerInfo.FingerprintID<<", '"<<fingerInfo.FingerprintName<<"', '"<<Nickname<<"');";
    if(sendQuery(ss.str()))
    {
        return receiveQuery();
    }
    return false;
}

bool SqlQuery::insertMessageInfo(const string Nickname,const messageInfoQuery messageInfo)
{
    stringstream ss;
    ss<<"INSERT INTO MESSAGEINFO(userMessage,targetPlatform,dateToStart,jumpTime,NickName) VALUES( '"<<messageInfo.UserMessage<<"','"<<messageInfo.TargetPlatform<<"','"<<messageInfo.dateToStart<<"',"<<messageInfo.JumpTime<<",'"<<Nickname <<"');";
    if(sendQuery(ss.str()))
    {
        return receiveQuery();
    }
    return false;
}

bool SqlQuery::getUser(string Nickname,userQuery *user)
{

    string str = selectQueryGetResponse("*","MYUSER","Nickname",Nickname.c_str());
    string srtToDelete[5]= {"Nickname = ","pincode = " ,"email = ","phone = ","pass = "};
    size_t toDelete = string::npos;

    for(int i=0;i<5;i++)
    {
        if((toDelete=str.find(srtToDelete[i]))!=string::npos)
        {
            str.erase(toDelete,srtToDelete[i].length());
            toDelete = string::npos;
        }
        else return false;
    }
    stringstream ss(str);
    string ux;
    getline(ss,user->NickName,'\n');
    getline(ss,user->PinCode,'\n');
    getline(ss,user->Email,'\n');
    getline(ss,user->PhoneNumber,'\n');
    getline(ss,user->EmailPassword,'\n');
    if( !getline(ss,ux,'\n'))   //Check if there is something left to read
    return true;
    return false;


}

bool SqlQuery::getFace( string Nickname, faceQuery *faceInfo)
{

    string str = selectQueryGetResponse("*","FACE","Nickname",Nickname.c_str());
    string srtToDelete[]= {"faceID = ","DataSetPath = " ,"NumberOfImages = ","Nickname = "};
    string faceID;
    size_t toDelete = string::npos;


    for(int i=0;i<4;i++)
    {
        if((toDelete=str.find(srtToDelete[i]))!=string::npos)
        {
            str.erase(toDelete,srtToDelete[i].length());
            toDelete = string::npos;
        }
        else return false;
    }
    stringstream ss(str);
    string ux,auxNumber;
    getline(ss,faceID,'\n');
    getline(ss,faceInfo->PathDataset,'\n');
    getline(ss,auxNumber,'\n');
    faceInfo->NumberOfImages = parsingNumber(auxNumber);
   return true;
}

bool SqlQuery::getFingerprint(string Nickname, fingerPrintQuery *fingerInfo)
{

    string str = selectQueryGetResponse("*","FINGERPRINT","Nickname",Nickname.c_str());
    string srtToDelete[]= {"fingerprintID = ","nameFingerprint = " ,"Nickname = "};
    size_t toDelete = string::npos;


    for(int i=0;i<3;i++)
    {
        if((toDelete=str.find(srtToDelete[i]))!=string::npos)
        {
            str.erase(toDelete,srtToDelete[i].length());
            toDelete = string::npos;
        }
        else return false;
    }
      stringstream ss(str);
      string auxID;
    getline(ss,auxID,'\n');
    fingerInfo->FingerprintID= parsingNumber(auxID);
    getline(ss,fingerInfo->FingerprintName,'\n');
    getline(ss,Nickname,'\n');
   return true;

}

bool SqlQuery::getMessage(string Nickname, messageInfoQuery *messageInfo)
{
    string str = selectQueryGetResponse("*","MESSAGEINFO","Nickname",Nickname.c_str());
    string srtToDelete[]= {"messageInfoID = ","userMessage = " ,"targetPlatform = ","dateToStart = ","jumpTime = ","Nickname = "};
    size_t toDelete = string::npos;

    for(int i=0;i<6;i++)
    {
        if((toDelete=str.find(srtToDelete[i]))!=string::npos)
        {
            str.erase(toDelete,srtToDelete[i].length());
            toDelete = string::npos;
        }
        else return false;
    }
    stringstream ss(str);
    string auxID;
    getline(ss,auxID,'\n');
    getline(ss,messageInfo->UserMessage,'\n');
    getline(ss,messageInfo->TargetPlatform,'\n');
    getline(ss,messageInfo->dateToStart,'\n');
    string auxJumpTime;
    getline(ss,auxJumpTime,'\n');
    messageInfo->JumpTime= parsingNumber(auxJumpTime);
    getline(ss,Nickname,'\n');


    return true;
}

bool SqlQuery::getUser(string Nickname,fullUser *output)
{

    messageLog = PROGRAM_NAME;


    if(getUser(Nickname,&output->user))
    {
        if(getFace(Nickname,&output->faceInfo))
        {
            if(getFingerprint(Nickname,&output->fingerInfo))
            {
                if(getMessage(Nickname,&output->messageInfo))
                {
                    messageLog.append("Success obtaining user from database!!\n\n");
                    write(fd,messageLog.c_str(),messageLog.length());
                    return true;
                }
            }else messageLog.append("Error getting Fingerprint data from database!!\n\n");

        }else messageLog.append("Error getting Face data from database!!\n\n");

    }
    else messageLog.append("Error getting User data from database!!\n\n");

    write(fd,messageLog.c_str(),messageLog.length());
    return false;
}





