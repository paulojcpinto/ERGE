#include "ccamera.h"


CCamera::CCamera():m_device(0)
{
    /* set video frame width to 640*/
    m_cap.set(CV_CAP_PROP_FRAME_WIDTH ,640);
    /* set video frame height to 480*/
    m_cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
    m_cap.set(CV_CAP_PROP_BRIGHTNESS, 100);
    m_cap.set(CV_CAP_PROP_EXPOSURE, 20);
    m_cap.set(CV_CAP_PROP_CONTRAST, 0);
    m_cap.set(CV_CAP_PROP_SATURATION, 0);
}
CCamera::CCamera(int device): m_device(device)
{
    /* set video frame width to 640*/
    m_cap.set(CV_CAP_PROP_FRAME_WIDTH ,640);
    /* set video frame height to 480*/
    m_cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
    m_cap.set(CV_CAP_PROP_BRIGHTNESS, 100);
    m_cap.set(CV_CAP_PROP_EXPOSURE, 20);
    m_cap.set(CV_CAP_PROP_CONTRAST, 0);
    m_cap.set(CV_CAP_PROP_SATURATION, 0);
}

CCamera::~CCamera()
{
    /*realease camera*/
    m_cap.release();
}

bool CCamera::open()
{
    /* open video device*/
    return m_cap.open(m_device);
}

void CCamera::shutdown()
{
   m_cap.release();
}

bool CCamera::captureFrame(cv::Mat& image)
{
    Mat frame;
    m_cap.open(m_device);
    if( m_cap.isOpened())
    {
        m_cap.read(frame);
        image = frame.clone();
        m_cap.release();
        return true;
    }
    return false;
}

bool CCamera::isActive(void)
{
    return m_cap.isOpened();
}








