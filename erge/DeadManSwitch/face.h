#ifndef FACE_H
#define FACE_H
#include "FaceModule/facerecognizer.h"

class Face
{
  private:

    DataSet UserDataset;
    FRecognizer userRecognizer;


  public:    
    Face(string nickname);
    DataSet* getDataset();
    FRecognizer* getRecognizer();


};

#endif // FACE_H
