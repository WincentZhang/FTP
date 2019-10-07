//get到本地后保存文件
int storeFile(char *context,char *filename,int len){
	int fd=open(filename,O_CREAT|O_RDWR|O_APPEND,777);
	 if(fd<0)
printf("create file failed \n");
	int length=write(fd,context,len);
	if(length<=0)
	  printf("\n error occur during writing 1");
	return length;
} 
//把指定文件内容读到buffer里
char * preFile(char *filename){
	char *buf=malloc(2000);
	int fd=open(filename,O_RDWR,777);
    int flength=lseek(fd,0,SEEK_END);
    lseek(fd,0,SEEK_SET);
	read(fd,buf,flength);
	return buf;
}
