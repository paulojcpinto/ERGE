#include "mcamera.h"

MCamera::MCamera():m_device(0)
{
    m_cap.open(m_device);

}

MCamera::MCamera(int device):m_device(device)
{
    m_cap.open(device);

}

bool MCamera::open()
{
    /* open video device*/
    return m_cap.open(m_device);
}

void MCamera::shutdown()
{
   m_cap.release();
}

bool MCamera::captureFrame(cv::Mat& image)
{
    Mat frame;

    if( m_cap.isOpened())
    {

        m_cap.read(frame);
        image = frame.clone();
        return true;
    }
    else
    {
        if(open())
        {
            m_cap.read(frame);
            image = frame.clone();
            return true;
        }
        return false;
    }
    return false;
}

bool MCamera::isActive(void)
{
    return m_cap.isOpened();
}




