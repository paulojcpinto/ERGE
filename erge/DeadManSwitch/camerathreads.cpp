#include "camerathreads.h"
#include "semaphore.h"

ProgramScheduler* mainClass= nullptr;
bluetooth_module* mBluetooth;
string CurrentUser="";
pthread_mutex_t mutexImage = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexImage1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t hasNewImage = PTHREAD_COND_INITIALIZER;
pthread_cond_t startReco = PTHREAD_COND_INITIALIZER;
pthread_cond_t startFrame = PTHREAD_COND_INITIALIZER;
sem_t sImage;




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
        sem_init (&sImage,0,0);
    }
    else mLog.writeToLog("Thread was not inittialized");


}

void *getFrames(void* threadid)
{
    LogHandler frameLog("THREAD_GETFRAMES: ");
    frameLog.writeToLog("Thread created");

    for(;;)
    {
        pthread_cond_wait(&startFrame,&mutexImage);
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
                  //  pthread_mutex_lock(&mutexImage);
                    imagesToProcess.push_back(frame);
                     frameLog.writeToLog("face retrived with successo+");

                    //pthread_mutex_unlock(&mutexImage);
  sem_post (&sImage);

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
        pthread_cond_wait(&startReco,&mutexImage1);

        if(CurrentUser!="")
        {
            recoLog.writeToLog("Starting Recognizing: " +CurrentUser+"\n\n");
            if(mainClass->getUserRecognizer(CurrentUser,&recog) == 1)
            {
                recoLog.writeToLog( to_string (1)+"\n\n");

                prediction= 200;
                recog->loadRecognizer(mainClass->getNumberofImages(CurrentUser));
                while (prediction>60) {
                     recoLog.writeToLog("Image To Process:"+ to_string(imagesToProcess.size ()));
                    sem_wait (&sImage);
                    recoLog.writeToLog("sem pos:");
                    if(imagesToProcess.size()>=1)
                    {
                    //pthread_mutex_lock(&mutexImage1);
                    Mat frame = imagesToProcess.front ();
                    imagesToProcess.erase (imagesToProcess.begin ());
                    //pthread_mutex_unlock(&mutexImage1);
                    prediction =recog->recognizeFace(frame);
                    recoLog.writeToLog("Recongized result:"+ to_string(prediction));
                    }



                }
                finish= true;

            }else recoLog.writeToLog("Error getting recognizer from User: "+CurrentUser);

        }
        else recoLog.writeToLog("Nickname wasn't inicialized");



    }



}

