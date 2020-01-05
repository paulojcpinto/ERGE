#ifndef FACE_H
#define FACE_H
#include "FaceModule/facerecognizer.h"
#include "SqlModule/sqlquery.h"
class Face
{
  private:

    DataSet UserDataset;
    FRecognizer userRecognizer;
    int numberOfImages;

  public:    
    Face(string nickname);
    Face(faceQuery faceInfo);
    DataSet* getDataset();
    bool createDataset(int *imagesTaked,bool *ended);
    FRecognizer* getRecognizer();
    void operator=(Face finger);



};

#endif // FACE_H
