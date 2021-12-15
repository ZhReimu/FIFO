#ifndef __CS_H__
	#define __CS_H__
	#include <unistd.h>
	#include <sys/file.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	void checkLock(int fd);
	void lock_set(int fd, int type);
	int isProducerRunning();
	int getFileSize(int fd);
#endif