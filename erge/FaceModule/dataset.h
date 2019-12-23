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
    DataSet();
    DataSet(string NickName);
    ~DataSet();
    bool createDataset();
    bool addFace(vector<cv::Mat>images, unsigned int lengh = 15);
    bool appendFrame(vector<cv::Mat>images);
    vector<cv::Mat> readFace(int lengh=15);
    void removeFace();
    string getDatasetPath();
private:
    MCamera cam;
    void writeToLog(string message);
    string m_dataset_path;
    int fd;
    int lastLengh;
};

#endif // DATASET_H
