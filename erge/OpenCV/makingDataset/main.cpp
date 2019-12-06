#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "ccamera.h">
#include <vector>
#include "cdataset.h"

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame );

/** Global variables */
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
vector<cv::Mat> images;
int aux=0;
int i =0;
/** @function main */
int main( int argc, const char** argv )
{


    cout<< "OLA\r\n";
    String face_cascade_name = "/opt/haarcascade_frontalface_alt.xml";
     //String face_cascade_name = "/opt/haarcascade_frontalface_default.xml";
// //   String eyes_cascade_name = "../opencv/data/haarcascades/ha´arcascade_eye_tree_eyeglasses.xml";

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

   CCamera cam; Mat frame;
   CDataSet newData;


    //while (aux==0) {
  while(   !newData.addFace(images,0))

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
  cout<<"created successfully";


//    //-- 2. Read the video stream
//    if(cam.captureFrame(frame) == false)
//    {
//        cout << " error\n";
//    }
//      else cout <<"image\n";

//        //-- 3. Apply the classifier to the frame
//       // detectAndDisplay( frame );


//}
    return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{
    Mat frame_gray;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    std::vector<Rect> faces;
   // face_cascade.detectMultiScale(frame_gray,faces);

    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50));

   for ( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4 );
    }

//        Mat faceROI = frame_gray( faces[i] );

        //-- In each face, detect eyes
//        std::vector<Rect> eyes;
//        eyes_cascade.detectMultiScale( faceROI, eyes );

//        for ( size_t j = 0; j < eyes.size(); j++ )
//        {
//            Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
//            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
//            circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4 );
//        }
    //}

    //-- Show what you got
    if(faces.size()==1)
    {
        cout<<"Face found";
        Mat croppedImage = frame_gray(faces[0]);
        images.push_back(croppedImage);
        //imwrite("/opt/coisalinda.jpeg",frame);
        aux=1;
        i=11;

}
    else if(faces.size()>1) cout<<"found "<< faces.size() <<"SO error";
    else
    {cout << "face not found";
        //imwrite("/opt/coisalinda.jpeg",frame);

    }
}
