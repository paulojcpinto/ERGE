#ifndef FACERECOGNIZER_H
#define FACERECOGNIZER_H
#include <string.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include "dataset.h"

#define ERROR_CASCADE -1
#define ERROR_MODEL -2

using namespace std;

class FRecognizer
{
public:
    FRecognizer();
    FRecognizer(DataSet* dataset);
    void changeDataSet(String Nickname);
    bool findFace(Mat *frameP);
    int recognizeFace(Mat frame);
    int loadRecognizer(int numberOfImages=15);

    const string face_cascade_name = "/opt/haarcascade_frontalface_alt.xml";

private:
    void loadLog();
    void writeToLog(string message);
    bool loadCascade();
    DataSet *dataset;
    CascadeClassifier face_cascade;
    Ptr<FaceRecognizer> model;

    int fd;

};

#endif // FACERECOGNIZER_H
