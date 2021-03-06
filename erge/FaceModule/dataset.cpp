#include "dataset.h"
#include <string.h>
#include <vector>
#include <mqueue.h>
#include <unistd.h>
#include <stdlib.h>

#define PROGRAM_NAME "DATASET_MODULE: "
#define DEFAULT_PATH "/datasets/teste1/"



DataSet::DataSet()
{
 m_dataset_path = DEFAULT_PATH;

 if ((fd = open("/var/log/deadman.log",	O_CREAT | O_WRONLY | O_APPEND, 0600)) < 0) {
             perror("open");
             exit(EXIT_FAILURE);
         }
}


DataSet::DataSet(string Nickname)
{
    stringstream ss;
    ss <<"/datasets/"<<Nickname<<"/";
    m_dataset_path = ss.str();
    ss.clear();
    ss<<"mkdir /datasets";
    system(ss.str().c_str());
    ss.clear();
    ss<<"mkdir "<<m_dataset_path;
    system(ss.str().c_str());

    if ((fd = open("/var/log/deadman.log",	O_CREAT | O_WRONLY | O_APPEND, 0600)) < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }

}



bool DataSet::createDataset()
{
    CascadeClassifier face_cascade;
     vector<Mat> images;
     String face_cascade_name = "/opt/haarcascade_frontalface_alt.xml";
     Mat frame;
     if( !face_cascade.load( face_cascade_name ) )
     {
         writeToLog("ERROR opening CascadeClassifier");
         return false;
     }
     while(!addFace(images,0))
     {
     if(cam.captureFrame(frame) == false)
        {
            writeToLog("Error getting frame!");
        }
          else
     {
         Mat frame_gray;
         cvtColor( frame, frame_gray, COLOR_BGR2GRAY );

         std::vector<Rect> faces;
         face_cascade.detectMultiScale( frame_gray, faces, 1.1, 3, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50));
         if(faces.size()==1)
         {
             writeToLog("Face was found");
             Mat croppedImage = frame(faces[0]);
             images.push_back(croppedImage);
             //TODO: send to bluetooth the signal that a face was found!

         }
         else if(faces.size()>1) writeToLog("Founded more than one face!");
         else writeToLog("Face was not found!");
     }
     }
     writeToLog("Created dataset successfuly");
    return true;
}



void DataSet::writeToLog(string Message)
{
    string MessageToLog;
    MessageToLog= PROGRAM_NAME;
    MessageToLog.append(Message);
    MessageToLog.append("\n\n");
    write(fd,MessageToLog.c_str(),MessageToLog.length());

}



bool DataSet::addFace(vector<cv::Mat> images, unsigned int lengh)
{
    if(images.size() == lengh)
    {
        for(unsigned int i = 1;i <= lengh; i++)
        {
            string path = m_dataset_path + "user0"+ "_" + to_string(i) + ".jpg";
            cv::imwrite(path,images.back());
            images.pop_back();
        }
        lastLengh= lengh;
        writeToLog("Created Dataset successfuly");
        return true;
    }
    writeToLog("Number of images invalid!");
    return false;
}




bool DataSet::appendFrame(vector<cv::Mat> images)
{
 if(images.empty())
 {
     writeToLog(" AppendFrames: Images vector was empty");
     return false;
 }
 for(int i= lastLengh; i<images.size()+lastLengh; i++ )
 {
     string path = m_dataset_path + "user0"+ "_" + to_string(i) + ".jpg";
     cv::imwrite(path,images.back());
     images.pop_back();
 }
 lastLengh += images.size();
 writeToLog("Appended to Dataset successfuly");
 return true;

}



vector<cv::Mat> DataSet::readFace(int lengh)
{
    vector<cv::Mat> images;
       for(int i = 1; i <= lengh; i++)
          {
                string path = m_dataset_path +"user0" + "_" +to_string(i) + ".jpg";
                images.push_back(cv::imread(path,CV_LOAD_IMAGE_GRAYSCALE ));
          }
    writeToLog("Readed images successfully");
    return images;
}



void DataSet::removeFace()
{
    for(int i = 1; i <= 15; i++)
    {
        string path = m_dataset_path +"user0"+ "_" +to_string(i) + ".jpg";
        remove(path.c_str());
    }
}



string DataSet::getDatasetPath()
{
    return m_dataset_path;
}
