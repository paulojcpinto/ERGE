#include "datainfo.h"


DataInfo::DataInfo()
{

}

DataInfo::DataInfo( string message, string platform, bool deleteAfterRelease )
{
    this->dataInfo.message = message;
    this->dataInfo.platform = platform;
    this->dataInfo.deleteAfterRelease = deleteAfterRelease;
}


/****************************************************************************************
 *
 *
 * This specific function will update the message that the user wants to send
 *
 *
 ***************************************************************************************/

void DataInfo::updateMessage( string message )
{
    this->dataInfo.message = message;
}


/****************************************************************************************
 *
 *
 * This specific function will update the platform where the information will be sharesd
 *
 *
 ***************************************************************************************/

void DataInfo::updateplatform( string platform )
{
    this->dataInfo.platform = platform;
}


/****************************************************************************************
 *
 *
 * This specific function will update the what to do after release the information
 *
 *
 ***************************************************************************************/

void DataInfo::updateAfterRelease( bool deleteAfterRelease )
{
    this->dataInfo.deleteAfterRelease = deleteAfterRelease;
}


/****************************************************************************************
 *
 *
 * This specific function will return the information that the user wants to release
 *
 *
 ***************************************************************************************/

releaseInfo DataInfo::getReleaseInfo( void )
{
    return dataInfo;
}
