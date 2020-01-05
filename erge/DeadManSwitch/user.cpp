#include "user.h"


/****************************************************************************
 *
 *
 * This specific contructor Creates a User without a finger configurated
 *
 *
 * *************************************************************************/

/****************************************************************************
 *
 *
 * This specific contructor Creates a User with a finger configurated
 *
 *
 * *************************************************************************/

User::User( string nickName,string pinCode, string phoneNumber, string email, string password, unsigned int IDFinger, string message, string platform, bool deleteAfterRelease): finger (IDFinger),face(nickName,nullptr), dataInfo (message, platform, deleteAfterRelease),log("UserClass:")
{
    this->nickName = nickName;
    this->phoneNumber = phoneNumber;
    this->email = email;
    this->password = password;
    this->pinCode=pinCode;
}

User::User(MCamera* cameraPointer,fullUser Data):finger(Data.fingerInfo),face(cameraPointer,Data.faceInfo),dataInfo(Data.messageInfo),log("UserClass: ")
{
    nickName= Data.user.NickName;
    phoneNumber=Data.user.PhoneNumber;
    pinCode=Data.user.PinCode;
    email=Data.user.Email;
    password=Data.user.EmailPassword;
}

bool User::login(string pincode)
{
    stringstream ss;
    ss<<"Login Attempt On user: "<<nickName<<", Gived Pincode: "<<pincode<<" and user Pincode: "<<pinCode;
    log.writeToLog(ss.str());
    return pinCode == pincode;
}

/****************************************************************************
 *
 *
 * This function will update the email of the user
 *
 *
 * *************************************************************************/

void User::updateEMail( string email )
{
    this->email = email;
}


/****************************************************************************
 *
 *
 * This sfunction will update the password of the user
 *
 *
 * *************************************************************************/

void User::updatePassword( string password )
{
    this->password = password;
}


/****************************************************************************
 *
 *
 * This will update the Phone Number of the user
 *
 *
 * *************************************************************************/

void User::updatePhoneNumeber( string phoneNumber )
{
    this->phoneNumber = phoneNumber;
}


/****************************************************************************
 *
 *
 * This Compare the ID from a finger with the ID of the finger of the user
 * return true     ->   the finger ID's are the same
 * return false    ->   the finger ID's are not the same
 *
 *
 * *************************************************************************/

bool User::compareFinger( unsigned int id )
{

    return ( this->finger.CompareFinger( id ) );
}


/****************************************************************************
 *
 *
 * This Compare the nick Name of the user with the one received
 * return true   ->  the nick Names match
 * return false  ->  the nick Names does not match
 *
 *
 * *************************************************************************/


bool User::compareNickName( string nickName )
{

   // if(this->nickName.compare(nickName)==0)
        //return true;
    //else return false;
    return (this->nickName  == nickName );
}

bool User::createDataset(int *imagesTaked, bool *ended)
{
    return face.createDataset(imagesTaked,ended);
}
