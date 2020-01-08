#include "loghandler.h"
#include <sstream>
#include <mqueue.h>
#include <unistd.h>


LogHandler::LogHandler(string programName)
{

    if ((fd =  ::open(LOG_NAME,	O_CREAT | O_WRONLY | O_APPEND, 0600)) < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }
    name=programName;

}

void LogHandler::writeToLog(string message)
{
    string MessageToLog;
    MessageToLog= name;
    MessageToLog.append(message);
    MessageToLog.append("\n\n");
    write(fd,MessageToLog.c_str(),MessageToLog.length());
}
