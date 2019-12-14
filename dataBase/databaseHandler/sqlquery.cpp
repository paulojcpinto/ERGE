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

    /* string sql("INSERT INTO MYUSER VALUES('Ola', '1234', 'olacoisa@gmail.com', '913753546', '12353232');"
                 "INSERT INTO FINGERPRINT VALUES(5, 'Dedo2', 'Ola');"
                 "INSERT INTO FACE VALUES(2, '/Datasets/test1', 'Ola');"
                 "INSERT INTO FACE VALUES(4, '/Datasets/test1', 'Ola');"); */
    if(insertUser(muser.user))
    {

    }else return false;

}

bool SqlQuery::insertUser(const userQuery user)
{
    stringstream ss;
    ss<<"INSERT INTO MYUSER VALUES('"<<user.NickName<<"', "<<user.PinCode << "', "<<user.Email<<"', "<<user.PhoneNumber<<"', "<<user.EmailPassword<<"');";
    if(sendQuery(ss.str()))
    {
        return receiveQuery();
    }
    return false;
}







