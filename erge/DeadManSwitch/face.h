#ifndef FACE_H
#define FACE_H
#include "FaceModule/facerecognizer.h"
#include "SqlModule/sqlquery.h"
#include "loghandler.h"
class Face
{
  private:

    DataSet UserDataset;
    FRecognizer userRecognizer;
    int numberOfImages;
    LogHandler log;
    MCamera * cam;

  public:    
    Face(string nickname,MCamera* cameraPointer);
    Face(MCamera* cameraPointer,faceQuery faceInfo);
    DataSet* getDataset();
    bool createDataset(int *imagesTaked,bool *ended);
    bool appendImagesDataset(int *imagesTaked,bool *ended,int Amount);
    int getNumberofImages(){return numberOfImages;};
    FRecognizer* getRecognizer();
    void operator=(Face finger);



};

#endif // FACE_H
