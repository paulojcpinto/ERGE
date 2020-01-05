#include "face.h"

Face::Face(string nickname):UserDataset(nickname),userRecognizer(&UserDataset)
{



}
Face::Face(faceQuery faceInfo):UserDataset(faceInfo.PathDataset),userRecognizer(&UserDataset)
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
