#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "ccamera.h">
using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame );

/** Global variables */
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
int aux=0;
int i =0;

void imadjust(const Mat1b& src, Mat1b& dst, int tol = 1, Vec2i in = Vec2i(0, 255), Vec2i out = Vec2i(0, 255))
{
    // src : input CV_8UC1 image
    // dst : output CV_8UC1 imge
    // tol : tolerance, from 0 to 100.
    // in  : src image bounds
    // out : dst image buonds

    dst = src.clone();

    tol = max(0, min(100, tol));

    if (tol > 0)
    {
        // Compute in and out limits

        // Histogram
        vector<int> hist(256, 0);
        for (int r = 0; r < src.rows; ++r) {
            for (int c = 0; c < src.cols; ++c) {
                hist[src(r, c)]++;
            }
        }

        // Cumulative histogram
        vector<int> cum = hist;
        for (int i = 1; i < hist.size(); ++i) {
            cum[i] = cum[i - 1] + hist[i];
        }

        // Compute bounds
        int total = src.rows * src.cols;
        int low_bound = total * tol / 100;
        int upp_bound = total * (100 - tol) / 100;
        in[0] = distance(cum.begin(), lower_bound(cum.begin(), cum.end(), low_bound));
        in[1] = distance(cum.begin(), lower_bound(cum.begin(), cum.end(), upp_bound));

    }

    // Stretching
    float scale = float(out[1] - out[0]) / float(in[1] - in[0]);
    for (int r = 0; r < dst.rows; ++r)
    {
        for (int c = 0; c < dst.cols; ++c)
        {
            int vs = max(src(r, c) - in[0], 0);
            int vd = min(int(vs * scale + 0.5f) + out[0], out[1]);
            dst(r, c) = saturate_cast<uchar>(vd);
        }
    }
}


int main( int argc, const char** argv )
{


    cout<< "OLA\r\n";
  //String face_cascade_name = "/opt/haarcascade_frontalface_default.xml";
    String face_cascade_name = "/opt/haarcascade_frontalface_alt.xml";

   //String face_cascade_name = "/home/andre/haarcascade_frontalface_alt.xml";

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

    //while (aux==0) {
   for(i; i<1000; i++)

   {
       if(cam.captureFrame(frame) == false)
          {
              cout << " error\n";
          }
            else
       {
           cout <<"image\n";
           Mat framie;
           //imadjust(frame,framie);
           detectAndDisplay( frame );

       }
   }


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
    Mat frame_gray, frame_gray_resized,fame_resized;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
   // equalizeHist( frame_gray, frame_gray );
           // cv :: resize(frame, frame_resized , Size(200,200), 1.0, 1.0, INTER_CUBIC);



    //-- Detect faces
    std::vector<Rect> faces;
    face_cascade.detectMultiScale(frame_gray_resized,faces);
    //face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50));

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
    if(faces.size()>=1)
    {
        cout<<"Face found";

        Mat new_image = Mat::zeros( frame.size(), frame.type() );
          double alpha = 1.0; /*< Simple contrast control */
          int beta = 0;       /*< Simple brightness control */
          cout << " Basic Linear Transforms " << endl;
          cout << "-------------------------" << endl;
          cout << "* Enter the alpha value [1.0-3.0]: "; cin >> alpha;
          cout << "* Enter the beta value [0-100]: ";    cin >> beta;
          for( int y = 0; y < frame.rows; y++ ) {
              for( int x = 0; x < frame.cols; x++ ) {
                  for( int c = 0; c < frame.channels(); c++ ) {
                      new_image.at<Vec3b>(y,x)[c] =
                        saturate_cast<uchar>( alpha*frame.at<Vec3b>(y,x)[c] + beta );
                  }
              }
          }
        imwrite("/opt/photos/coisalinda.jpeg",new_image);

        //cvtColor(frame,frame,cv::COLOR_BGR2RGB);
        imwrite("/opt/photos/coisalinda1.jpeg",frame_gray);

        imwrite("/opt/photos/coisalinda4.jpeg",frame);

        //imwrite("/opt/coisalinda2.jpeg",frame);
        Mat croppedImage = frame(faces[0]);
        imwrite("/opt/photos/coisalinda3.jpeg",croppedImage);




        aux=1;
        i=11000;

}
    else
    {cout << "face not found";
       // imwrite("/opt/coisalinda.jpeg",frame);

    }
}
