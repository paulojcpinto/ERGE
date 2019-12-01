CC = arm-buildroot-linux-uclibcgnueabihf-g++
RM = /bin/rm -f
all: daemon

daemon:
	$(CC) -o daemon daemon.cpp Database.cpp -lpthread -lsqlite3 -DSQLITE_DEFAULT_FOREIGN_KEYS=1

	cp ~/git/ERGE/deamon/daemon ~/ficheiros_para_rasp
clean:
	${RM} daemon
