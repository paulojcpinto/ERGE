#include "face.h"

Face::Face(string nickname,MCamera* cameraPointer):UserDataset(nickname,cameraPointer,15),userRecognizer(&UserDataset),log("Face_module: ")
{
cam = cameraPointer;


}
Face::Face(MCamera* cameraPointer,faceQuery faceInfo):UserDataset(faceInfo.PathDataset,cameraPointer,faceInfo.NumberOfImages),userRecognizer(&UserDataset),log("Face_module: ")
{
    numberOfImages=faceInfo.NumberOfImages;
    cam = cameraPointer;
}

DataSet* Face::getDataset()
{
    return &UserDataset;
}
FRecognizer* Face::getRecognizer()
{
    return &userRecognizer;
}
bool Face::createDataset(int *imagesTaked, bool *ended)
{
    return UserDataset.createDataset(imagesTaked,ended);
}

bool Face::appendImagesDataset(int *imagesTaked, bool *ended, int Amount)
{
    CascadeClassifier face_cascade;
     vector<Mat> images;
     String face_cascade_name = "/opt/haarcascade_frontalface_alt.xml";
     Mat frame;
     if( !face_cascade.load( face_cascade_name ) )
     {
         log.writeToLog("ERROR opening CascadeClassifier");
         return false;
     }
     int timeout=0;
     while(images.size()<Amount)
     {
     if(cam->captureFrame(frame) == false)
        {
            log.writeToLog("Error getting frame!");
        }
          else
     {
         Mat frame_gray;
                  cvtColor( frame, frame_gray, COLOR_BGR2GRAY );

                  std::vector<Rect> faces;
                  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 3, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50));
                  if(faces.size()==1)
                  {
                      log.writeToLog("Face was found");
                      Mat croppedImage = frame(faces[0]);
                      images.push_back(croppedImage);
                      *imagesTaked=*imagesTaked+1;

                  }
                  else if(faces.size()>1) log.writeToLog("Founded more than one face!");
                  if(timeout >10000)
                  {

                      *ended=true;
                      cam->shutdown();
                      log.writeToLog("Dataset Creation Timout");
                       return false;
                  }
                  timeout++;

              }

     }
     if(UserDataset.appendFrame(images,numberOfImages+Amount))
     {
         cam->shutdown();
         numberOfImages+= Amount;
         *ended=true;
         *imagesTaked= this->numberOfImages;
         return true;
     }else
     return false;



     }




void Face::operator=(Face face)
{
  this->UserDataset= face.UserDataset;
  this->userRecognizer=face.userRecognizer;
}
