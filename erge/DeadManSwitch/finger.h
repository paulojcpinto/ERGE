#ifndef FINGER_H
#define FINGER_H

#include <string>
#include "SqlModule/sqlquery.h"
using namespace std;

class Finger
{
    private:

        string fingerName;
        unsigned int fingerID;


    public:

        Finger ( void ) {}
        Finger ( unsigned int ID );
        Finger(fingerPrintQuery fingerInfo );
        bool CompareFinger ( unsigned int ID );
        ~Finger ( void ) {}

};

#endif // FINGER_H
