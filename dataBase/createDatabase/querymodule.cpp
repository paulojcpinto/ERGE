
#include "querymodule.h"



#define  PROGRAM_NAME "DatabaseCreationModule: "

using namespace std;


QueryModule::QueryModule()
{
    msgq_query = "/database_query";
    msgq_callback = "/daetabase_callback";
    if ((fd = open("/var/log/database.log",	O_CREAT | O_WRONLY | O_APPEND, 0600)) < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }


}

QueryModule::~QueryModule()
{
    closeQueryQueue();
    closeCallbackQueue();
}



bool QueryModule::openQueryQueue()
{
    msgq_id_query = mq_open(msgq_query, O_RDWR | O_EXCL, S_IRWXU | S_IRWXG, NULL);
    if (msgq_id_query == (mqd_t) - 1)
    {
        messageLog = PROGRAM_NAME;
        messageLog.append("Error opening Query Queue!!\n");
        write(fd,messageLog.c_str(),messageLog.length());
        perror("In query queue mq_open()");
        return false;

    }
    messageLog = PROGRAM_NAME;
    messageLog.append("Opened Query Queue successfully!!\n");
    write(fd,messageLog.c_str(),messageLog.length());
    return true;
}

bool QueryModule::openCallbackQueue()
{
    msgq_id_callback = mq_open(msgq_callback, O_RDONLY);
     if (msgq_id_callback == (mqd_t) - 1)
    {
        messageLog = PROGRAM_NAME;
        messageLog.append("Error opening Query callback Queue!!\n");
        write(fd,messageLog.c_str(),messageLog.length());
        perror("In callback queue mq_open()");
        return false;
    }
     messageLog = PROGRAM_NAME;
     messageLog.append("Opened Query CallBack Queue successfully!!\n");
     write(fd,messageLog.c_str(),messageLog.length());
     return true;
}

bool QueryModule::closeQueryQueue()
{
    if(mq_close(msgq_id_query) == 0)
    {
        messageLog = PROGRAM_NAME;
        messageLog.append("Closed Query Queue successfully!!\n");
        write(fd,messageLog.c_str(),messageLog.length());
        return true;
    }
    return false;
}

bool QueryModule::closeCallbackQueue()
{
    if(mq_close(msgq_id_callback) == 0)
    {
         messageLog = PROGRAM_NAME;
         messageLog.append("Opened Query CallBack Queue successfully!!\n");
         write(fd,messageLog.c_str(),messageLog.length());
         return true;
    }
    return false;
}

bool QueryModule::sendQuery(string custom_query) // change from const char *
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
            messageLog.append("Error sending query to Queue!!\n");
            write(fd,messageLog.c_str(),messageLog.length());
            perror("In query queue mq_send()");
            return false;
        }
        messageLog = PROGRAM_NAME;
        messageLog.append("Sended Querry successfully: ");
        messageLog.append(custom_query);
        messageLog.append("\n");
        write(fd,messageLog.c_str(),messageLog.length());
        return closeQueryQueue();
    }
    return false;
}

bool QueryModule::receiveQuery()
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
        messageLog.append("\n");
        return closeCallbackQueue();
    }
    return false;
}

string QueryModule::getLastQueryResult()
{
    string str(result);
    return str;
}
