#include "facerecognizer.h"
#include <string.h>
#include <vector>
#include <mqueue.h>
#include <unistd.h>
#include <stdlib.h>

#define PROGRAM_NAME "FACERECOGNIZER: "

FRecognizer::FRecognizer():face_cascade_name("/opt/haarcascade_frontalface_alt.xml")
{
 dataset = new DataSet();
 loadLog();

}

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
    if(!loadCascade())
        return ERROR_CASCADE;
     vector<Mat> images;
     images = dataset->readFace(numberOfImages);
     vector<int> labels;
     model = createLBPHFaceRecognizer();
     if(model ==nullptr) return ERROR_MODEL;
     labels.clear();
     for(int i=0; i<numberOfImages;i++)labels.push_back(i);
     model->train(images,labels);

     return 1;


}
bool FRecognizer::findFace(Mat *frameP)
{
    Mat frame,frame_gray;
    frame = *frameP;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    vector<Rect> faces;
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 3, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50));
    if(faces.size()>1)
    {
        writeToLog("Founded more than one face!");
        return false;
    }
    if(faces.size()<=0)
    {
        writeToLog("No faces founded");
        return false;
    }
    writeToLog("One face was found!");
    *frameP = frame_gray(faces[0]);
    return true;

}

int FRecognizer::recognizeFace(Mat frame) //return the confidence level
{
    int predicted_label = -1;
    double predicted_confidence = 0.0;
    model->predict(frame, predicted_label, predicted_confidence);
    return (int)predicted_confidence;
}


void FRecognizer::loadLog()
{
    if ((fd = open("/var/log/deadman.log",	O_CREAT | O_WRONLY | O_APPEND, 0600)) < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }
}
