#ifndef DATASET_H
#define DATASET_H
#include <string.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include "mcamera.h"
using namespace cv;
using namespace std;


class DataSet
{
public:
   // DataSet();
    DataSet(string NickName, MCamera* cameraPointer,int lastIndex);
    ~DataSet();
    bool createDataset(int *imagesTaked,bool *ended);
    bool addFace(vector<cv::Mat>images, unsigned int lengh = 15);
    bool appendFrame(vector<cv::Mat>images,int newLengh);
    string* getPath(){return &m_dataset_path;};
    bool readFace(vector<cv::Mat>* output,int lengh=15);
    void removeFace();
    string getDatasetPath();
private:
    MCamera* cam;
    void writeToLog(string message);
    string m_dataset_path;
    int fd;
    int lastLengh;
};

#endif // DATASET_H
