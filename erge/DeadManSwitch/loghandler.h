#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include <iostream>

#define LOG_NAME "/var/log/deadman.log"
using namespace std;

class LogHandler
{
public:
    LogHandler(string programName);
    void writeToLog(string message);
private:
    int fd;
    string name;

};

#endif // LOGHANDLER_H
