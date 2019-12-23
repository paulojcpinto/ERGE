#include "Database.h"

using namespace std;


CDatabase::CDatabase()
{
	databaseName = "deadman.db";

	if ((fd = open("/var/log/database.log",	O_CREAT | O_WRONLY | O_APPEND, 0600)) < 0) {
			perror("open");
			exit(EXIT_FAILURE);
	}
		
	openDB();
}

CDatabase::~CDatabase()
{
	closeDB();
}

bool CDatabase::openDB()
{
	
	sqlite3_open(databaseName, &database);
}

bool CDatabase::closeDB()
{
	sqlite3_close(database);
}

bool CDatabase::executeQuery(char* query)
{
	char * errormsg;
	int rc = sqlite3_exec(database, query, callback, (void*)this, &errormsg);
	if( rc != SQLITE_OK){

        string message("Daemon: Error Executing Query: ");
		message.append(errormsg);
		message.append("\n\n");
		write(fd,message.c_str(),message.size());

		return FALSE;
	}else return TRUE;
}

int CDatabase::callback(void* result_ptr, int argc, char** argv, char** azColName)
{
	CDatabase* obj = (CDatabase*) result_ptr;

	stringstream ss;
	for(int i = 0; i < argc; i++)
	{
		ss << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << '\n';
	}

	obj->result = obj->result + ss.str();
	return 0;
}


void CDatabase::getResult(char* memory)
{
	sprintf(memory, "%s", result.c_str());	
	result.clear();
}
