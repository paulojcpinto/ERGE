#ifndef CDATASET_H
#define CDATASET_H
#include <string.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <sstream>

using namespace std;

class CDataSet
{
public:
    CDataSet();
    ~CDataSet();
    bool addFace(vector<cv::Mat>images,int label);
    vector<cv::Mat> readFace(vector<int>);
    void removeFace(int label);
    string getDatasetPath();
private:
    string m_dataset_path;
};

#endif // CDATASET_H
