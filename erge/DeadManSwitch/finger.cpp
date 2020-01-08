#include "finger.h"


/****************************************************************************
 *
 *
 * This specific contructor Creates a finger with a specific ID
 *
 *
 * *************************************************************************/

Finger::Finger( unsigned int ID )
{
    this->fingerID = ID;
}


/****************************************************************************
 *
 *
 * This Compare the ID from a finger with the ID of the finger saved
 * return true     ->   the finger ID's are the same
 * return false    ->   the finger ID's are not the same
 *
 *
 * *************************************************************************/

bool Finger::CompareFinger( unsigned int ID )
{
    return ( this->fingerID == ID );
}


void Finger::operator=(Finger finger)
{
  this->fingerID = finger.fingerID;
}
