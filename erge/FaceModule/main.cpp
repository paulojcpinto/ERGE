#include <QCoreApplication>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "mcamera.h"

void detectAndDisplay( Mat frame );
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
int aux=0;
int i =0;
using namespace std;
int main(int argc, char *argv[])
{
    String face_cascade_name = "/opt/haarcascade_frontalface_alt.xml";
    QCoreApplication a(argc, argv);


//    //-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_name ) )
    {
        cout << "--(!)Error loading face cascade\r\n";
        return -1;
    }
    else cout <<"loaded images";
////    if( !eyes_cascade.load( eyes_cascade_name ) )
////    {
////        cout << "--(!)Error loading eyes cascade\n";
////        return -1;
////    };

   MCamera cam; Mat frame;

    //while (aux==0) {
   for(i; i<100; i++)

   {
       if(cam.captureFrame(frame) == false)
          {
              cout << " error\n";
          }
            else
       {
           cout <<"image\n";
           detectAndDisplay( frame );

       }
   }

    return 0;
}

void detectAndDisplay( Mat frame )
{
    Mat frame_gray, frame_gray_resized;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
           // cv :: resize(frame, frame_resized , Size(200,200), 1.0, 1.0, INTER_CUBIC);
           // cv :: resize(frame_gray, frame_gray_resized, Size(200,200), 1.0, 1.0, INTER_CUBIC);
    //-- Detect faces
    std::vector<Rect> faces;
    //face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50));

    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4 );
    }

    //-- Show what you got
    if(faces.size()>=1)
    {
        cout<<"Face found";


        imwrite("/opt/coisalinda.jpeg",frame);
        //cvtColor(frame,frame,cv::COLOR_BGR2RGB);
        imwrite("/opt/coisalinda1.jpeg",frame_gray);
        //imwrite("/opt/coisalinda2.jpeg",frame);
        Mat croppedImage = frame(faces[0]);
        imwrite("/opt/coisalinda3.jpeg",croppedImage);




        aux=1;
        i=101;

}
    else
    {cout << "face not found\n";
       // imwrite("/opt/coisalinda.jpeg",frame);

    }
}

