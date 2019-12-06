#include <QCoreApplication>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "ccamera.h"
#include <vector>
#include "cdataset.h"
#include <iostream>

using namespace std;
CascadeClassifier face_cascade;

int main(int argc, char *argv[])
{
    vector<Mat> images;
     int labels= 1;
     CDataSet newData;
     vector<int> i;
     i.clear();
     i.push_back(0);
     //i.push_back(1);
     cout << i.size() <<"value"<<i.at(0)<<"\n";
     //for(int j=1; j<15;+j++)i.push_back(j);
     //i.clear();
     //i.push_back(15);
     String face_cascade_name = "/opt/haarcascade_frontalface_alt.xml";
     //String face_cascade_name = "/opt/haarcascade_frontalface_default.xml";


   //    //-- 1. Load the cascades
       if( !face_cascade.load( face_cascade_name ) )
       {
           cout << "--(!)Error loading face cascade\r\n";
           return -1;
       }
       else cout <<"loaded images";


     images = newData.readFace(i);
     Ptr<FaceRecognizer> model = createLBPHFaceRecognizer();
   //  i.clear();
     //i.push_back(15);
      for(int j=1; j<15;j++)i.push_back(j);
      int predicted_label = -1;
      double predicted_confidence = 0.0;
      images.pop_back();
      i.pop_back();

     model->train(images,i);
    // Mat testSample = images[images.size() - 1];
     cout<<"treino feito";
     CCamera cam; Mat frame;

     while(1)

      {
          if(cam.captureFrame(frame) == false)
             {
                 cout << " error\n";
             }
               else
          {
              cout <<"image\n";
              Mat frame_gray;
              cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
              Mat face_resized;

                std::vector<Rect> faces;

             face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50));
             if(faces.size()>=1)
             {

                 face_resized = frame_gray(faces[0]);


                 model->predict(face_resized, predicted_label, predicted_confidence);
             //   int predictedLabel = model->predict(face_resized);

               // string result_message = format("Predicted class = %d / Actual class = %d.", predicted_label, predicted_confidence);
                cout << "Predict classs =" << predicted_label << "Confidence = "<< predicted_confidence << "\n";

             } else
             {
                // resize(frame_gray, face_resized, Size(260, 260), 1.0, 1.0, INTER_CUBIC);

                 cout << "face not found\n";
                 //model->predict(face_resized, predicted_label, predicted_confidence);
                 //cout << "Predict classs =" << predicted_label << "Confidence = "<< predicted_confidence << "\n";


             }





          }
      }








}
