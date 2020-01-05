#include "face.h"

Face::Face(string nickname,MCamera* cameraPointer):UserDataset(nickname,cameraPointer),userRecognizer(&UserDataset)
{



}
Face::Face(MCamera* cameraPointer,faceQuery faceInfo):UserDataset(faceInfo.PathDataset,cameraPointer),userRecognizer(&UserDataset)
{
    numberOfImages=faceInfo.NumberOfImages;
}

DataSet* Face::getDataset()
{
    return &UserDataset;
}
FRecognizer* Face::getRecognizer()
{
    return &userRecognizer;
}
bool Face::createDataset(int *imagesTaked, bool *ended)
{
    return UserDataset.createDataset(imagesTaked,ended);
}
void Face::operator=(Face face)
{
  this->UserDataset= face.UserDataset;
  this->userRecognizer=face.userRecognizer;
}
