#include "Database.h"
#include <mqueue.h>
#include <sys/syslog.h>
#include <sys/types.h> 
#include <signal.h>
#include <iostream>


#define MSGOBJ_NAME "/database_query"
#define MSGOBJ_NAME_CALLBACK "/daetabase_callback"
#define MAX_MSG_LEN 10000
using namespace std;
mqd_t msgq_id;
mqd_t msgq_id_callback;

void handler(int signum)
{
	if(signum == SIGTERM)
	{
		mq_close(msgq_id);
		mq_close(msgq_id_callback);
		exit(0);
	}
}

int main(int argc, char *argv[])
{

	// Daemon variables	
	pid_t pid, sid;

	//syslog(LOG_INFO, "%s\n", "starting daemon");
	cout<<"Starting daemon \n";	
	pid = fork();

	//syslog(LOG_INFO, "%s\n", "forked");	
	cout<<"Daemon Forked \n";
	if(pid > 0){
		printf("Deamon PID: %d\n", pid);
		syslog(LOG_INFO, "%s\n", "parent exiting");	
		exit(EXIT_SUCCESS); // parent process (exit)
	}

	sid = setsid(); // create a new session

	if (sid < 0) { // on error exit
		perror("setsid");		
		exit(EXIT_FAILURE);
	}

	if (chdir("/") < 0) { // on error exit
		//syslog(LOG_ERR, "%s\n", "chdir");
		perror("chdir");
		exit(EXIT_FAILURE);
	}

	umask(0);   // Set file permissions
	close(STDIN_FILENO);  // close standard input file descriptor
	close(STDOUT_FILENO); // close standard output file descriptor
	close(STDERR_FILENO); // close standard error file descriptor

	// Message queue variables
	char msgcontent[MAX_MSG_LEN];
	char msgcontent_callback[MAX_MSG_LEN];
	int msgsz;
	int len, fd;
	unsigned int sender;
    if ((fd = open("/var/log/deadman.log",	O_CREAT | O_WRONLY | O_APPEND, 0600)) < 0) {
			perror("open");
			exit(EXIT_FAILURE);
		}
	//write(fd,"Deu ate aqui\n",13);	
	// Open query queue
	msgq_id = mq_open(MSGOBJ_NAME, O_RDWR | O_CREAT);
	if(msgq_id == (mqd_t) - 1)
	{
		write(fd,"Daemon: Error openeing queue!\n\n",32);
		//syslog(LOG_DAEMON|LOG_ERR, "%s\n", "error opening query queue");
		exit(1);
	}	
	write(fd,"Daemon: Created queue succesfully!\n\n",37);
	//syslog(LOG_DAEMON|LOG_INFO, "%s\n", "queue query created/opened");
	sleep(1);
	// Open callback queue
	msgq_id_callback = mq_open(MSGOBJ_NAME_CALLBACK, O_RDWR | O_CREAT);
	if(msgq_id_callback  == (mqd_t) - 1)
	{
		write(fd,"Daemon: Error creating the callback!\n\n",38);
		//syslog(LOG_DAEMON|LOG_ERR, "%s\n", "error opening callback queue");
		exit(1);
	}
	//syslog(LOG_DAEMON|LOG_INFO, "%s\n", "queue callback created/opened");
	write(fd,"Daemon: queue callback created/opened\n\n",40);

	//Database object
	CDatabase database;

	while(1){
		// Wait for message on queue
		msgsz = mq_receive(msgq_id, msgcontent, MAX_MSG_LEN, &sender);
		if(msgsz == -1)
		{
			syslog(LOG_DAEMON|LOG_ERR, "%s:%s\n", "error in queue receive", strerror(errno));
			exit(1);
		}
		syslog(LOG_DAEMON|LOG_INFO, "Query: %s\n", msgcontent);
		string message("Daemon: Query: ");
		message.append(msgcontent);
		message.append("\n\n");
		
		write(fd,message.c_str(),message.size());
		// Execute database query
		database.executeQuery(msgcontent);
		// Clear message buffer
		memset(msgcontent, 0, sizeof(msgcontent));
		// Clear callback buffer
		memset(msgcontent_callback, 0, sizeof(msgcontent_callback));
		// Get query result
		database.getResult(msgcontent_callback);
		//syslog(LOG_DAEMON|LOG_INFO, "Query result: %s\n", msgcontent_callback);
		message = "Daemon: Query result = ";
		message.append(msgcontent_callback);
		message.append("\n\n");
        write(fd,message.c_str(),message.size());
		// Send message
		mq_send(msgq_id_callback, msgcontent_callback, strlen(msgcontent_callback)+1, 1);

	}
	exit(EXIT_SUCCESS);		
}
