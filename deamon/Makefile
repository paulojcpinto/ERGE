CC = arm-buildroot-linux-uclibcgnueabihf-g++
RM = /bin/rm -f
all: daemon

daemon:
	$(CC) -o daemon daemon.cpp Database.cpp -lpthread -lsqlite3 -DSQLITE_DEFAULT_FOREIGN_KEYS=1

	cp ~/git/ERGE/daemon ~/ficheiros_para_rasp
	scp ~/git/ERGE/daemon root@10.42.0.116:/
clean:
	${RM} daemon
