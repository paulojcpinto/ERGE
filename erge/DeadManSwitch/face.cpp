#include "face.h"

Face::Face(string nickname):UserDataset(nickname),userRecognizer(&UserDataset)
{



}

DataSet* Face::getDataset()
{
    return &UserDataset;
}
FRecognizer* Face::getRecognizer()
{
    return &userRecognizer;
}
