#include "cs.h"

int main(int argc, char* argv[]) {
	int maxProduct = 10;
	int fileSize = 0;
	int succRead = 0;
	int fd;
	char buff[1024];
	char outBuff[1024];
	char* file = "FIFO.txt";

	if(argc > 1) {
		maxProduct = atoi(argv[1]);
	}
	printf("maxProduct = %d\n",maxProduct);
	
	do{
	/*
		if (!isProducerRunning()){
			puts("未检测到生产者程序，请先运行 生产者 程序！");
		}
		*/
		// 首先打开文件
		fd = open(file, O_RDWR, 0666);
		if (fd < 0) {
			puts("文件打开失败，请先运行 生产者 程序！");
			exit(1);
		}
		fileSize = getFileSize(fd);
		// 如果 生产者 正在运行，但是文件已经消费完毕，那就等待 生产者 继续生产
		if(isProducerRunning()){
			puts("正在等待生产者程序生产");
			while( fileSize == 0 && isProducerRunning() ){
				fileSize = getFileSize(fd);
			}
		}
		if(fileSize == 0){
			puts("资源消费完毕！");
			exit(0);
		}
		// 如果文件剩余内容不足以满足一次消费
		if(fileSize < maxProduct){
			// 但是 生产者程序还在运行
			if(isProducerRunning()){
				printf("目前文件大小为 %d，正在等待生产者生产\n", fileSize);
				// 那就等待生产者程序生产
				while(fileSize < maxProduct && isProducerRunning()){
					fileSize = getFileSize(fd);
				}
				// 如果生产结束后还，fileSize 还是没有达到 maxProduct
				// 那就修改 maxProduct，以保证程序能正常结束
				if(fileSize < maxProduct){
					puts("生产者未能继续生产资源，调整 maxProduct 为当前剩下资源量");
					maxProduct = fileSize;
				}
			// 否则就修改 maxProduct 为 fileSize，以保证程序能正常执行结束
			}else{
				maxProduct = fileSize;
			}
		}
		// 消费前 maxProduct 字节的内容
		do{
			// 清空 读取 buff
			memset(buff, 0, sizeof(buff));
			// 读取 maxProduct 字节进 读取 buff
			succRead = read(fd, buff, fileSize);
			// 重置文件指针
			lseek(fd, 0, SEEK_SET);
		}while(succRead < maxProduct);
		
		// 输出前 maxProduct 字节内容
		memset(outBuff, 0, sizeof(outBuff));
		strncpy(outBuff, buff, maxProduct);
		printf("读取文件 %s\n", outBuff);
		puts("正在等待 锁 ");
		// 关闭文件
		close(fd);
		fd = open(file, O_RDWR | O_TRUNC, 0666);
		// 给文件上写入锁
		lock_set(fd, F_WRLCK);
		puts("删除读取过的文件内容\n");
		if (fd < 0) {
			puts("文件打开失败");
			exit(1);
		}
		write(fd, buff + maxProduct, fileSize - maxProduct);
		// 给文件解锁
		lock_set(fd, F_UNLCK);
		close(fd);
	}while(fileSize > 0);
	
	return 0;
}
