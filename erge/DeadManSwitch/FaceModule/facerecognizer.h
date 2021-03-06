#ifndef FACERECOGNIZER_H
#define FACERECOGNIZER_H
#include <string.h>
//#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
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
    FRecognizer(DataSet* dataset);
    void changeDataSet(String Nickname);
    static bool findFace(Mat *frameP);
    int recognizeFace(Mat frame);
    int loadRecognizer(string nickName, int numberOfImages=15);
     string face_cascade_name = "/opt/haarcascade_frontalface_alt.xml";
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
