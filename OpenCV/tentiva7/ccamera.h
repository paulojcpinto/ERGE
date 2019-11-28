#ifndef CCAMERA_H
#define CCAMERA_H
#include <opencv2/opencv.hpp>
#include <queue>
#include <string>
using namespace cv;
using namespace std;

class CCamera
{
public:
    CCamera();
    CCamera(int);
    ~CCamera();
    bool captureFrame(Mat&);
    bool isActive(void);
    bool open(void);
    void shutdown(void);
private:
    VideoCapture m_cap;
    int m_device;
};


#endif
