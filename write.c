#include "cs.h"

int main(int argc, char* argv[]) {
	int sleepTime = 1;
	int maxProduct = 10;
	int productCount = 0;
	int fd;
	char buff[] = "@";
	char* file = "FIFO.txt";
	if(argc > 1) {
		printf("参数个数为 %d\n",argc);
		switch(argc) {
			case 2:
				sleepTime = atoi(argv[1]);
				break;
			case 3:
				sleepTime = atoi(argv[1]);
				maxProduct = atoi(argv[2]);
		}
	}
	printf("sleepTime = %d, maxProduct = %d\n",sleepTime,maxProduct);

	while(productCount < maxProduct) {
		// 首先打开文件
		fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0666);
		if (fd < 0) {
			perror("open");
			exit(1);
		}
		puts("正在等待 锁 ");
		// 给文件上写入锁
		lock_set(fd, F_WRLCK);
		buff[0] = 'A' + productCount++ % 26;
		printf("文件上锁，开始写入文件，内容为 %s\n", buff);
		write(fd,buff,sizeof(buff)-1);
		printf("睡 %d s\n",sleepTime);
		sleep(sleepTime);
		// 给文件解锁
		lock_set(fd, F_UNLCK);
		puts("文件解锁，停止写入文件");
		close(fd);
	}
	return 0;
}

