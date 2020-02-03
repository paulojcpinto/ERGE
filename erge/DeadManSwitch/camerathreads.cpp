#include "camerathreads.h"
#include "semaphore.h"

ProgramScheduler* mainClass= nullptr;
ProgramScheduler* mainReco= nullptr;
bluetooth_module* mBluetooth;
string CurrentUser="";
pthread_mutex_t mutexImage = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexGetFrames = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexReco = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t hasNewImage = PTHREAD_COND_INITIALIZER;
pthread_cond_t startReco = PTHREAD_COND_INITIALIZER;
pthread_cond_t startFrame = PTHREAD_COND_INITIALIZER;
sem_t sImage;
pthread_attr_t attr;




LogHandler mLog("ThreadCameraModule: ");
bool finish=false;
vector<Mat> imagesToProcess;

void *getFrames(void* threadid);
void *recognition(void* threadid);

void  initThread(ProgramScheduler* programData,bluetooth_module* bluetooth)
{
    mBluetooth= bluetooth;
    mainClass = programData;
    mainReco= programData;
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

        pthread_attr_init (&attr);
        size_t stack;
        pthread_attr_getstacksize (&attr, &stack);
        stack*=2;
        pthread_attr_setstacksize (&attr, stack);
        int rID= pthread_create(&threadGetframes,NULL,getFrames,NULL);
        if(rID)
        {
            mLog.writeToLog("Error creating threadGetFrames!!");
            exit(-1);

        }
        mLog.writeToLog("ThreadGetFrames created with success");
        int gID = pthread_create(&threadRecognition,&attr,recognition,NULL);
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
        sem_init (&sImage,0,0);
        long int counter;
        finish = false;
        imagesToProcess.clear ();
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
                     pthread_mutex_lock(&mutexGetFrames);
                     imagesToProcess.push_back(frame);
                     pthread_mutex_unlock(&mutexGetFrames);
                     frameLog.writeToLog("Signal to start recognition");
                     sem_post (&sImage);
                }
            }
            if(counter>100)
            {
                finish=true;
                frameLog.writeToLog("Face ulnock timed out");
                mBluetooth->sendMessage("<I2>");
            }

        }
        mainClass->getCameraInstance()->shutdown();
        frameLog.writeToLog("Stopping thread...");

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
        pthread_cond_wait(&startReco,&mutexReco);

        if(CurrentUser!="")
        {
            recoLog.writeToLog("Starting Recognizing: " +CurrentUser+"\n\n");
            if(mainReco->getUserRecognizer(CurrentUser,&recog) == 1)
            {
                prediction= 200;
                recog->loadRecognizer(CurrentUser,mainReco->getNumberofImages(CurrentUser));

                while (prediction>60 && !finish) {
                     recoLog.writeToLog("Image To Process:"+ to_string(imagesToProcess.size ()));
                    sem_wait (&sImage);
                    if(imagesToProcess.size()>=1)
                    {
                    pthread_mutex_lock(&mutexGetFrames);
                    Mat frame = imagesToProcess.front ();
                    imagesToProcess.erase (imagesToProcess.begin ());
                    pthread_mutex_unlock(&mutexGetFrames);
                    prediction =recog->recognizeFace(frame);
                    recoLog.writeToLog("Recongized result:"+ to_string(prediction));
                    }
                }
                if(!finish)
                {
                finish= true;
                recoLog.writeToLog("Recognition done with success");
                system("echo G > /dev/TIAM_led ");
                mainReco->doPresenceCheck(CurrentUser);
                mBluetooth->sendMessage("<I1>");
                }

            }else recoLog.writeToLog("Error getting recognizer from User: "+CurrentUser);

        }
        else recoLog.writeToLog("Nickname wasn't inicialized");



    }



}


bool* getfinishSatet()
{
    return &finish;
}

