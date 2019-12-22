#ifndef FINGER_H
#define FINGER_H

#include <string>

using namespace std;

class Finger
{
    private:

        string fingerName;
        unsigned int fingerID;


    public:

        Finger ( void ) {}
        Finger ( unsigned int ID );
        void operator=(Finger finger);
        bool CompareFinger ( unsigned int ID );
        ~Finger ( void ) {}

};

#endif // FINGER_H
