#include "camerathreads.h"

ProgramScheduler* mainClass= nullptr;
bluetooth_module* mBluetooth;
string CurrentUser="";
pthread_mutex_t mutexImage = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t hasNewImage = PTHREAD_COND_INITIALIZER;
pthread_cond_t startReco = PTHREAD_COND_INITIALIZER;
pthread_cond_t startFrame = PTHREAD_COND_INITIALIZER;


LogHandler mLog("ThreadCameraModule: ");
bool finish=false;
vector<Mat> imagesToProcess;

void *getFrames(void* threadid);
void *recognition(void* threadid);

void  initThread(ProgramScheduler* programData,bluetooth_module* bluetooth)
{
    mBluetooth= bluetooth;
    mainClass = programData;
}

void initRecognizer(string nickname)
{
    CurrentUser = nickname;
}

void startThreads()
{
    if(mainClass!= nullptr)
    {
        pthread_t threadGetframes, threadRecognition;

        int rID= pthread_create(&threadGetframes,NULL,getFrames,NULL);
        if(rID)
        {
            mLog.writeToLog("Error creating threadGetFrames!!");
            exit(-1);

        }
        mLog.writeToLog("ThreadGetFrames created with success");
        int gID = pthread_create(&threadRecognition,NULL,recognition,NULL);
        if(gID)
        {
            mLog.writeToLog("Error creating threadRecognition");
            exit(-1);
        }
        mLog.writeToLog("ThreadRecognition created with success");
    }
    else mLog.writeToLog("Thread was not inittialized");


}

void *getFrames(void* threadid)
{
    LogHandler frameLog("THREAD_GETFRAMES: ");
    frameLog.writeToLog("Thread created");

    for(;;)
    {
        pthread_cond_wait(&startReco,&mutexImage);
        long int counter;
        Mat frame;
        while(!finish)
        {
            if(mainClass->getCameraInstance()->captureFrame(frame)==false)
            {
                frameLog.writeToLog("Error getting frame");
            }
            else
             {
                if(FRecognizer::findFace(&frame))
                {
                    frameLog.writeToLog("face retrived with success");
                    pthread_mutex_lock(&mutexImage);
                    imagesToProcess.push_back(frame);
                    pthread_mutex_unlock(&mutexImage);

                }
            }
            if(counter>100000)
            {
                finish=true;
                frameLog.writeToLog("Face ulnock timed out");
            }

        }

    }
}


void *recognition(void* threadid)
{
    LogHandler recoLog("THREAD_RECOGNIZER: ");
    recoLog.writeToLog("Thread created");
    FRecognizer* recog;
    int prediction;
    for(;;)
    {
        pthread_cond_wait(&startFrame,&mutexImage);

        if(CurrentUser!="")
        {
            recoLog.writeToLog("Starting Recognizing: " +CurrentUser);
            if(mainClass->getUserRecognizer(CurrentUser,&recog))
            {
                prediction= 200;
                recog->loadRecognizer(mainClass->getNumberofImages(CurrentUser));
                while (prediction>60) {

                    if(imagesToProcess.size()>=1)
                    {
                    pthread_mutex_lock(&mutexImage);
                    Mat frame = imagesToProcess.back();
                    imagesToProcess.pop_back();
                    pthread_mutex_unlock(&mutexImage);
                    prediction =recog->recognizeFace(frame);
                    recoLog.writeToLog("Recongized result:"+ to_string(prediction));
                    }
                    usleep(200);


                }
                finish= true;

            }else recoLog.writeToLog("Error getting recognizer from User: "+CurrentUser);

        }
        else recoLog.writeToLog("Nickname wasn't inicialized");



    }



}


