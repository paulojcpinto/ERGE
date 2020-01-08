#ifndef MCAMERA_H
#define MCAMERA_H
#include <opencv2/opencv.hpp>

using namespace cv;

class MCamera
{
public:
    MCamera();
    MCamera(int device);
    bool captureFrame(Mat&);
    bool isActive(void);
    bool open(void);
    void shutdown(void);
private:
    VideoCapture m_cap;
    int m_device;
};

#endif // MCAMERA_H
