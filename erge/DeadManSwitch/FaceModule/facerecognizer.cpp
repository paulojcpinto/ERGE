#include "facerecognizer.h"
#include <string.h>
#include <vector>
#include <mqueue.h>
#include <unistd.h>
#include <stdlib.h>
#include "loghandler.h"
#define PROGRAM_NAME "FACERECOGNIZER: "

//FRecognizer::FRecognizer():face_cascade_name("/opt/haarcascade_frontalface_alt.xml")
//{
// //dataset = new DataSet();
// loadLog();

//}

FRecognizer::FRecognizer(DataSet* dataset)
{
    this->dataset= dataset;
    loadLog();


}

void FRecognizer::writeToLog(string Message)
{
    string MessageToLog;
    MessageToLog= PROGRAM_NAME;
    MessageToLog.append(Message);
    MessageToLog.append("\n\n");
    write(fd,MessageToLog.c_str(),MessageToLog.length());

}

bool FRecognizer::loadCascade()
{
    if( !face_cascade.load( face_cascade_name ) )
    {
        writeToLog("Error loading cascade");
        return false;
    }
    writeToLog("Loaded cascade successfuly");
    return true;


}

int FRecognizer::loadRecognizer(int numberOfImages)
{
     vector<Mat> images;
     images.clear ();
     vector<int> labels;
      //dataset->readFace(&images,numberOfImages);
     string*  m_dataset_path = dataset->getPath();
      for(int i = 1; i <= numberOfImages; i++)
         {
               string path =*m_dataset_path +"user0_" +to_string(i) + ".jpg";
               images.push_back(cv::imread(path,CV_LOAD_IMAGE_GRAYSCALE ));
         }
      labels.clear();
      writeToLog ("module initialized"+ to_string (numberOfImages));
      for(int i=1; i<=numberOfImages;i++)labels.push_back(i);
      model = createLBPHFaceRecognizer();
      model->train(images ,labels);
      writeToLog ("train done with success");
     return 1;


}
bool FRecognizer::findFace(Mat *frameP)
{
    Mat frame,frame_gray;
    frame = *frameP;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    LogHandler mLog("FRECOGINZER_STATIC: ");
    CascadeClassifier face_cascade;
    if( !face_cascade.load( "/opt/haarcascade_frontalface_alt.xml" ) )
    {
        mLog.writeToLog("Error loading cascade");
        return false;
    }
    //mLog.writeToLog("Loaded cascade successfuly");
    vector<Rect> faces;

    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 3, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50));
    if(faces.size()>1)
    {
        mLog.writeToLog("Founded more than one face!");
        return false;
    }
    if(faces.size()<=0)
    {
        //mLog.writeToLog("No faces founded");
        return false;
    }
    mLog.writeToLog("One face was found!");
    *frameP = frame_gray(faces[0]);
    return true;

}

int FRecognizer::recognizeFace(Mat frame) //return the confidence level
{
    int predicted_label = -1;
    double predicted_confidence = 0.0;
    writeToLog("two face was found!");
    model->predict(frame, predicted_label, predicted_confidence);
    writeToLog("Onthreee face was found!");
    return (int)predicted_confidence;
}


void FRecognizer::loadLog()
{
    if ((fd = open("/var/log/deadman.log",	O_CREAT | O_WRONLY | O_APPEND, 0600)) < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }
}
