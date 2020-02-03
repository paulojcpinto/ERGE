#ifndef DATAINFO_H
#define DATAINFO_H

#include <string>
#include "SqlModule/sqlquery.h"

using namespace std;

typedef struct dataInfo
{

    string message;
    string platform;
    bool deleteAfterRelease;

} releaseInfo;


class DataInfo
{
    private:
        releaseInfo dataInfo;
    public:
        DataInfo();
        DataInfo( string message, string platform, bool deleteAfterRelease = true );
        DataInfo(messageInfoQuery messageInfo);
        void updateMessage ( string message );
        void updateplatform ( string platform );
        void updateAfterRelease( bool deleteAfterRelease );
        releaseInfo getReleaseInfo ( void );
        ~DataInfo( void ){}
};

#endif // DATAINFO_H
