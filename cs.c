#include "cs.h"

/**
* @param fd 文件描述符
* 获取指定文件描述符的锁的情况，手动阻塞，直到锁被释放，已不再使用
*/
void checkLock(int fd) {
	struct flock lock;
	lock.l_whence = SEEK_SET;//赋值lock结构体
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_type = F_WRLCK;
	while(1) {
		fcntl(fd, F_GETLK, &lock);
		if (lock.l_type == F_UNLCK) {
			return;
		}
	}
}

/**
*	@param fd 要操作的文件描述符
*	@param type 要进行的操作
* 对指定文件描述符进行指定操作
*/
void lock_set(int fd, int type) {
	// 定义并初始化 lock 结构体
	struct flock lock;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_type = type;
	lock.l_pid = getpid();
	// 根据不同的 type 值给文件上锁或解锁
	// 阻塞方式当该文件上有锁时，程序会一直等待锁被释放
	if ((fcntl(fd, F_SETLKW, &lock)) == 0) {
		if (lock.l_type == F_RDLCK) {
			printf("%d 设置读取锁\n", lock.l_pid);
		} else if (lock.l_type == F_WRLCK) {
			printf("%d 设置写入锁\n", lock.l_pid);
		} else if (lock.l_type == F_UNLCK) {
			printf("%d 释放锁\n", lock.l_pid);
		}
	} else {
		puts("上锁失败！");
		exit(1);
	}
}

/**
* 判断生产者进程是否在执行
*/
int isProducerRunning(){
	return 1;
	FILE * fp;
	char ch = -1;
	fp = popen("ps -el | grep producer", "r");
	if(fp == NULL){
		puts("popen 失败，未检测到 生产者程序，请先运行生产者程序！");
		exit(1);
	}
	ch = fgetc(fp);
	pclose(fp);
	return ch != -1;
}

/**
*	@param fd 要操作的文件描述符
*	获取文件大小 (字节)
*/
int getFileSize(int fd){
	// lseek 会返回本次移动的距离 (字节)
	int size = lseek(fd, 0, SEEK_END);
	// 重置文件指针
	lseek(fd, 0, SEEK_SET);
	return size;
}
