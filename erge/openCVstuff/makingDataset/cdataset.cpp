#include "cdataset.h"

CDataSet::CDataSet()
{
    m_dataset_path = "/datasets/teste1/";
}

CDataSet::~CDataSet()
{

}
bool CDataSet::addFace(vector<cv::Mat> images,int label)
{
    if(images.size() == 15)
    {
        for(int i = 1;i <= 15; i++)
        {
            string path = m_dataset_path + "user" + to_string(label)+ "_" + to_string(i) + ".jpg";
            cv::imwrite(path,images.back());
            images.pop_back();
        }
        return true;
    }
    return false;
}

vector<cv::Mat> CDataSet::readFace(vector<int> labels)
{
    vector<cv::Mat> images;
    int current_label = -1, temp_label = -1;
    int size_labels = labels.size();
    for (int i = 0; i < size_labels; i++)
    {
        current_label = labels.back();
        if(current_label != temp_label)
        {
            for(int i = 1; i <= 15; i++)
            {
                string path = m_dataset_path +"user" + to_string(current_label)+ "_" +to_string(i) + ".jpg";
                images.push_back(cv::imread(path,CV_LOAD_IMAGE_GRAYSCALE ));
            }
            temp_label = current_label;

        }
        labels.pop_back();
    }
    return images;
}

void CDataSet::removeFace(int label)
{
    for(int i = 1; i <= 15; i++)
    {
        string path = m_dataset_path +"user" + to_string(label)+ "_" +to_string(i) + ".jpg";
        remove(path.c_str());
    }
}

string CDataSet::getDatasetPath()
{
    return m_dataset_path;
}
