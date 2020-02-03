#include "dataset.h"
#include <string.h>
#include <vector>
#include <mqueue.h>
#include <unistd.h>
#include <stdlib.h>
#define PROGRAM_NAME "DATASET_MODULE: "
#define DEFAULT_PATH "/datasets/teste1/"

//#include <QDebug>


//DataSet::DataSet()
//{
// m_dataset_path = DEFAULT_PATH;

// if ((fd = open("/var/log/deadman.log",	O_CREAT | O_WRONLY | O_APPEND, 0600)) < 0) {
//             perror("open");
//             exit(EXIT_FAILURE);
//         }
//}


DataSet::DataSet(string Nickname,MCamera* cameraPointer, int lastIndex)
{
    lastLengh = lastIndex;
    cam = cameraPointer;
    stringstream ss;
    ss <<"mkdir /datasets/"<<Nickname<<"/";
    this->nickName = Nickname;

    system(ss.str().c_str());
    stringstream path;
    path<<"/datasets/"<<Nickname<<"/";
    m_dataset_path =path.str();
    if ((fd = open("/var/log/deadman.log",	O_CREAT | O_WRONLY | O_APPEND, 0600)) < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }

}


DataSet::~DataSet()
{

}

bool DataSet::createDataset(int *imagesTaked, bool *ended)
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
     int timeout=0;
     while(images.size()<15)
     {
     if(cam->captureFrame(frame) == false)
        {
            writeToLog("Error getting frame!");
        }
          else
     {
       //  cam->shutdown();
         Mat frame_gray;
         cvtColor( frame, frame_gray, COLOR_BGR2GRAY );

         std::vector<Rect> faces;
         face_cascade.detectMultiScale( frame_gray, faces, 1.1, 3, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50));
         if(faces.size()==1)
         {
             writeToLog("Face was found");
             Mat croppedImage = frame(faces[0]);
             images.push_back(croppedImage);
             *imagesTaked=*imagesTaked+1;

         }
         else if(faces.size()>1) writeToLog("Founded more than one face!");
         if(timeout >10000)
         {
             *ended=true;
             cam->shutdown();
             writeToLog("Dataset Creation Timout");
             return false;

         }
         timeout++;

     }
     }
     addFace(images,15);
     cam->shutdown();
     *ended=true;
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




bool DataSet::appendFrame(vector<cv::Mat> images, int newLengh)
{

 if(images.empty())
 {
     writeToLog(" AppendFrames: Images vector was empty");
     return false;
 }
 int size = images.size();
 for(int i= lastLengh+1; i<= size +lastLengh; i++ )
 {
     string path = m_dataset_path + "user0"+ "_" + to_string(i) + ".jpg";
     cv::imwrite(path,images.back());
     images.pop_back();
     writeToLog("Added image:"+path);
 }
 lastLengh = newLengh;
 writeToLog("Appended to Dataset successfuly");
 return true;

}



bool DataSet::readFace(vector<cv::Mat>* output,int lengh)
{
       for(int i = 1; i <= lengh; i++)
          {
                string path = m_dataset_path +"user0_" +to_string(i) + ".jpg";
                output->push_back(cv::imread(path,CV_LOAD_IMAGE_GRAYSCALE ));
          }
    writeToLog("Readed images successfully");
    return true;
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

string* DataSet::getPath()
{
    writeToLog("\n\n\andre : " +m_dataset_path+"\n\n\n");
       return &nickName;
}
