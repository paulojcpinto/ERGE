#include <stdio.h>
#include <errno.h>
#include <string>
#include <sstream>
#include <iostream>
#include "cquery.h"

/* This is the daemon database communication driver */

using namespace std;

CQuery::CQuery()
{
	msgq_query = "/database_query";
	msgq_callback = "/database_callback";
}

CQuery::~CQuery()
{
	closeQueryQueue();
	closeCallbackQueue();
}

bool CQuery::openQueryQueue()
{
    msgq_id_query = mq_open(msgq_query, O_RDWR | O_EXCL, S_IRWXU | S_IRWXG, NULL);
    if (msgq_id_query == (mqd_t) - 1) 
    {
        perror("In query queue mq_open()");
     	return false;   
    }
    return true;
}

bool CQuery::openCallbackQueue()
{
	msgq_id_callback = mq_open(msgq_callback, O_RDONLY);
	 if (msgq_id_callback == (mqd_t) - 1) 
    {
        perror("In callback queue mq_open()");
     	return false;   
    }
    return true;
}

bool CQuery::closeQueryQueue()
{
	if(mq_close(msgq_id_query) == 0)
		return true;
	return false;
}

bool CQuery::closeCallbackQueue()
{
	if(mq_close(msgq_id_callback) == 0)
		return true;
	return false;
}

/* Insert functions */
bool CQuery::insertRFIDQuery(const char* id_rfid, const char * name_rfid, bool admin)
{
    stringstream ss;
    ss << "insert into rfid values ('" << id_rfid << "','" << name_rfid << "'," << (int)admin << ")";
    if(sendQuery(ss.str()))
    {
        return receiveQuery();
    }
    return false;
}

bool CQuery::insertFaceQuery(const char* id_rfid)
{
    stringstream ss;
    ss << "insert into face(idRFID) values ('" << id_rfid << "')";
    if(sendQuery(ss.str()))
    {
        return receiveQuery();
    }
    return false;
}

bool CQuery::insertImageQuery(int id_face, const char* face_path)
{
    stringstream ss;
    ss << "insert into image(faceID,facePATH) values (" << id_face << ",'" << face_path << "')";
    if(sendQuery(ss.str()))
    {
        return receiveQuery();
    }
    return false;
}

/* Delete functions */
bool CQuery::deleteRFID(const char* rfid)
{
    stringstream ss;
    ss << "delete from rfid where idRFID='" << rfid << "'";
    if(sendQuery(ss.str()))
    {
        return receiveQuery();
    }
    return false;
}

bool CQuery::deleteFace(int face_id)
{
    stringstream ss;
    ss << "delete from face where faceid=" << face_id;
    if(sendQuery(ss.str()))
    {
        return receiveQuery();
    }
    return false;
}

bool CQuery::deleteFacePath(int face_id)
{
    stringstream ss;
    ss << "delete from image where faceid=" << face_id;
    if(sendQuery(ss.str()))
    {
        return receiveQuery();
    }
    return false;
}

/* Select functions */
bool CQuery::selectQuery(const char* column, const char* table, const char* cond_column, int cond)
{
	stringstream ss;
	ss << "select " << column << " from " << table << " where " << cond_column << "=" << cond;
	return sendQuery(ss.str());
}

bool CQuery::selectQuery(const char* column, const char* table, const char* cond_column, const char* cond)
{
	stringstream ss;
	ss << "select " << column << " from " << table << " where " << cond_column << "='" << cond << "'";
	return sendQuery(ss.str());
}

bool CQuery::selectQuery(const char* column, const char* table)
{
	stringstream ss;
	ss << "select " << column << " from " << table;
	return sendQuery(ss.str());
}

/* Unsafe select functions */
string CQuery::selectQueryGetResponse(const char* column, const char* table, const char* cond_column, const char* cond)
{
    selectQuery(column, table, cond_column, cond);
    receiveQuery();
    return getLastQueryResult();
}

string CQuery::selectQueryGetResponse(const char* column, const char* table)
{
    selectQuery(column, table);
    receiveQuery();
    return getLastQueryResult();
}

/* Parameter acquisition functions */
bool CQuery::getFaceIDByRFID(int* value, const char* rfid)
{
    stringstream ss;
    ss << "select faceid from face where idrfid='" << rfid << "'";
    if(sendQuery(ss.str()))
    {
        receiveQuery();
        string str(getLastQueryResult());
        string toErase("faceID = ");
        size_t pos = string::npos;
        while((pos = str.find(toErase)) != string::npos)
        {
            // If found then erase it from string
            str.erase(pos, toErase.length());
        }
        *value = atoi(str.c_str());
        return true;
    }
    else
        return false;
}

bool CQuery::getMaxID(int* value, const char* column, const char* table)
{
    stringstream ss;
    ss << "select max(" << column << ") from " << table;
    if(sendQuery(ss.str()))
    {
        receiveQuery();
        string str(getLastQueryResult());
        ss.str("");
        ss << "max(" << column << ") = ";
        string toErase(ss.str());
        size_t pos = string::npos;
        while((pos = str.find(toErase)) != string::npos)
        {
            // If found then erase it from string
            str.erase(pos, toErase.length());
        }
        *value = atoi(str.c_str());
        return true;
    }
    else
        return false;
}

bool CQuery::getMaxID(int* value)
{
    {
        stringstream ss;
        ss << "select max(faceid) from face";
        if(sendQuery(ss.str()))
        {
            receiveQuery();
            string str(getLastQueryResult());
            string toErase("max(faceid) = ");
            size_t pos = string::npos;
            while((pos = str.find(toErase)) != string::npos)
            {
                // If found then erase it from string
                str.erase(pos, toErase.length());
            }
            *value = atoi(str.c_str());
            return true;
        }
        else
            return false;
    }
}

vector<int> CQuery::getFaceLabels()
{
    string str(selectQueryGetResponse("faceID", "image"));
    size_t pos = string::npos;
    string toErase("faceID = ");
    string to;
    vector <int> label_array;
    // Search for the substring in string in a loop untill nothing is found
    while ((pos  = str.find(toErase) )!= std::string::npos)
    {
      // If found then erase it from string
      str.erase(pos, toErase.length());
    }
    stringstream ss(str);
    while(getline(ss, to, '\n'))
    {
        label_array.push_back(atoi(to.c_str()));
    }
    return label_array;
}

vector<string> CQuery::getFacePaths()
{
    string str(selectQueryGetResponse("facePATH", "image"));
    size_t pos = string::npos;
    string toErase("facePATH = ");
    string to;
    vector <string> path_array;
    // Search for the substring in string in a loop until
    while((pos = str.find(toErase)) != string::npos)
    {
        // If found then erase it from string
        str.erase(pos, toErase.length());
    }
    stringstream ss(str);
    while(getline(ss, to, '\n'))
    {
        path_array.push_back(to);
    }
    return path_array;
}

bool CQuery::checkRFIDDuplicate(string rfid)
{
    string str(selectQueryGetResponse("idRFID", "RFID"));
    if(str.find(rfid) != string::npos)
    {
        return true;
    }
    return false;
}
/* Worker functions */
bool CQuery::sendQuery(string custom_query) // change from const char *
{
    int msgsz;
    if(openQueryQueue())
    {
        memset(query, 0, sizeof(query));
        sprintf(query, custom_query.c_str());
        msgsz = mq_send(msgq_id_query, query, strlen(query)+1, 1);
        if (msgsz == -1)
        {
            perror("In query queue mq_send()");
            return false;
        }
        return closeQueryQueue();
    }
    return false;
}

bool CQuery::receiveQuery()
{
    int msgsz;
    unsigned int sender;
    if(openCallbackQueue())
    {
        memset(result, 0, sizeof(result));
        msgsz = mq_receive(msgq_id_callback, result, MAX_MSG_LEN, &sender);
        if (msgsz == -1)
        {
            perror("In callback queue mq_receive()");
            return false;
        }
        return closeCallbackQueue();
    }
    return false;
}

string CQuery::getLastQueryResult()
{
    string str(result);
    return str;
}












