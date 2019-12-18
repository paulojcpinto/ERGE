#ifndef QUERYMODULE_H
#define QUERYMODULE_H

#include <mqueue.h>
#include <string>
#include <string.h>
#include <vector>
#include <mqueue.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>



#define MAX_MSG_LEN 10000


using namespace std;

class QueryModule
{
public:
    QueryModule();
    ~QueryModule();
    bool openQueryQueue();
    bool openCallbackQueue();
    bool closeQueryQueue();
    bool closeCallbackQueue();
    bool sendQuery(string);
    bool receiveQuery();
    string getLastQueryResult();
private:
    char query[MAX_MSG_LEN];
    char result[MAX_MSG_LEN];
    const char * msgq_query;
    const char * msgq_callback;
    mqd_t msgq_id_query;
    mqd_t msgq_id_callback;
    int fd;
    string messageLog;

};

#endif // QUERYMODULE_H
