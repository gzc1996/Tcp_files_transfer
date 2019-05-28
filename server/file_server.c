#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <malloc.h>

#pragma pack(1)

//文件描述信息
typedef struct _file_info
{
	char name[51];
	unsigned int size;

} file_info;

#pragma pack()


char *path[100];
char *file_name[100];
int count = 0;
int count0 = 1;
file_info fi[100];
char* buffer[100];
int bytes[100];//memorize valid buffer's bytes 
const char* file_path;

void* send_thr(void* arg, int num);
int readFileList(char* arg);

int main(int argc, char** argv)
{
/*	if(argc != 3)
	{
		fprintf(stderr, "Usage error!\n");
		exit(1);
	}

	struct stat st;
	if(stat(argv[2], &st) == -1)
	{
		perror("Get file info fail");
		exit(1);
	}

	if(!S_ISREG(st.st_mode))
	{
		fprintf(stderr, "Sended file is not a regular file!\n");
		exit(1);
	}

*/

//	fi.size = st.st_size;
/*******************************recive************************************/
     printf("waiting send files client for sending...\n");
     for(int i=0; i<count0; i++){
	int sock0_listen = socket(AF_INET, SOCK_STREAM, 0);

	int optval0 = 1;
	setsockopt(sock0_listen, SOL_SOCKET, SO_REUSEADDR, &optval0, sizeof(optval0));

	struct sockaddr_in myaddr0;
	myaddr0.sin_family = AF_INET;
	myaddr0.sin_addr.s_addr = INADDR_ANY;
	myaddr0.sin_port = htons(10001);

	if(bind(sock0_listen, (struct sockaddr*)&myaddr0, sizeof(myaddr0)) == -1){
		perror("bind error");
		exit(1);
	}

	listen(sock0_listen, 5);

	struct sockaddr_in clnaddr0;
	socklen_t len0;

	int sock0_conn;

	len0 = sizeof(clnaddr0);
	sock0_conn = accept(sock0_listen, (struct sockaddr*)&clnaddr0, &len0);

	if(sock0_conn == -1){
		perror("accept error");
		continue;
	}

	//recv start
	int ret = 0, ret1 = 0;

	ret = recv(sock0_conn, &fi[i], sizeof(fi[i]), 0);

	recv(sock0_conn, &count0, sizeof(count0), 0);
	printf("There are %d files for recived.\n",count0);

	char str[100];
	recv(sock0_conn, &str, sizeof(str), 0);
	printf("%s\n", str);
	
	char *buff = (char *)malloc(1024 * 1024);
	buffer[i] = (char *)malloc(1024 *1024);

	if(ret > 0){
/*		FILE *fp = NULL;
		char pathname[100];

		strcpy(pathname, "./file/");
		strcat(pathname, fi.name);

		fp =fopen(pathname, "wb");

		if(NULL == fp){
			perror("fopen");
			exit(1);
		}
*/           
		while(ret1 < fi[i].size){
			ret = recv(sock0_conn, buff, 1024 * 1024, 0);
			bytes[i] = ret;
//			fwrite(buff, 1, ret, fp);
			strcpy(buffer[i], buff);
			printf("%s\n",buffer[i]);
			ret1 += ret;
			printf("sending >>>>>>>%.2f%\n", ((float)ret1 / (float)fi[i].size) * 100);
			printf("############################################\n");
		}

//		fclose(fp);
	}
	free(buff);
	close(sock0_conn);
	close(sock0_listen);
    }
/********************************send*************************************/
	printf("waiting recive client for connecting...\n");	
/*	char* file1 = "file";
        char basepath[100];   
        memset(basepath,'\0',sizeof(basepath));
        strcpy(basepath,"./");
        strcat(basepath,file1);
        readFileList(basepath);
*/
       	int sock_listen;

/*	char* p = NULL;
	p = strrchr(argv[2], '/');
	if(p == NULL)
		strcpy(fi.name, argv[2]);
	else
		strcpy(fi.name, p + 1);

	file_path = argv[2];
	

	signal(SIGPIPE, SIG_IGN);

*/
        printf("there are %d files for transfer.\n", count0);

        for(int i = 0; i < count0; i++){
//	   strcpy(fi.name, file_name[i]);
	   printf("the %dth file is %s.\n",i+1,fi[i].name);

	   char str[100];
	   sprintf(str,"the %dth file is %s.\n",i+1,fi[i].name);
           
/*	   file_path = path[i];
           
	   struct stat st;
	   if(stat(file_path, &st) == -1)
	    {
		perror("Get file info fail");
		exit(1);
	    }
	   fi.size = st.st_size;
*/
	   sock_listen = socket(AF_INET, SOCK_STREAM, 0);

	//setsockopt函数：设置套接字属性
	//将套接字的SO_REUSEADDR属性设置为1，即允许地址复用
	   int optval = 1;
	   setsockopt(sock_listen, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));


	   struct sockaddr_in myaddr;
	   myaddr.sin_family = AF_INET; //指定地址家族为Internet地址家族
	   myaddr.sin_addr.s_addr = INADDR_ANY; //指定IP地址为本机任意IP
	   myaddr.sin_port = htons(10000); //指定端口号

	   if(bind(sock_listen, (struct sockaddr*)&myaddr, sizeof(myaddr)) == -1)
	   {
	   	  perror("bind error");
		  exit(1);
	   }

	   listen(sock_listen, 5);

	   struct sockaddr_in clnaddr;
	   socklen_t len;

//	   while(1)
//	   {	
		int sock_conn; //连接套接字，用于和相应的客户端通信

		len = sizeof(clnaddr);
		sock_conn = accept(sock_listen, (struct sockaddr*)&clnaddr, &len);

		if(sock_conn == -1)
		{
			perror("accept error");
			continue;
		}

	//	pthread_t tid;
	//	if(pthread_create(&tid, NULL, send_thr, (void*)(long)sock_conn))
	//	{
	//		perror("create new thread fail");
	//		close(sock_conn);
	//	}
	 	send(sock_conn, &count0, sizeof(count0), 0);

	        send(sock_conn, &str, sizeof(str), 0);
		
		char flag;
		recv(sock_conn, &flag, sizeof(flag), 0);
		
		if(flag == 'y'){
	           send_thr((long)sock_conn, i);
                   printf("##################################\n\n");
		}
//	   }
                close(sock_conn);  
	        close(sock_listen);
	  }

	return 0;
}


void* send_thr(void* arg, int num)
{
	int sock_conn = (long)arg;

//        pthread_detach(pthread_self());

	struct sockaddr_in clnaddr;
	socklen_t len = sizeof(clnaddr);
	getpeername(sock_conn, (struct sockaddr*)&clnaddr, &len);

	//接收客户端连接请求成功
	printf("\nclient%s:%d is online!\n", inet_ntoa(clnaddr.sin_addr), ntohs(clnaddr.sin_port));

	//设置send和recv超时时间都为2S
	struct timeval timeout = {2, 0};
	setsockopt(sock_conn, SOL_SOCKET,SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval));
	setsockopt(sock_conn, SOL_SOCKET,SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

	if(-1 == send(sock_conn, &fi[num], sizeof(fi[num]), 0))
	{
		printf("\nclient%s:%d is offline!\n", inet_ntoa(clnaddr.sin_addr), ntohs(clnaddr.sin_port));
		close(sock_conn);
		return NULL;
	}


//	FILE* fp = NULL;
	int ret;

/*	fp = fopen(file_path, "rb");	

	if(fp == NULL)
	{
		perror("Open sended file fail");
		exit(1);
	}

	while(!feof(fp))
	{
		ret = fread(buff, 1, sizeof(buff), fp);
		if(-1 == send(sock_conn, buff, ret, 0))
			break;
	}

	fclose(fp);
*/
	send(sock_conn, buffer[num], bytes[num], 0);

	printf("\nclient%s:%d is offline!\n", inet_ntoa(clnaddr.sin_addr), ntohs(clnaddr.sin_port));
	close(sock_conn);	

	return NULL;
}


int readFileList(char *basepath){
  DIR *dir;
  struct dirent *ptr;
  char base[1000];

  if((dir = opendir(basepath)) == NULL){
    perror("open dir error");
    exit(1);
  }

  while((ptr = readdir(dir)) != NULL){
    if(strcmp(ptr->d_name,".") == 0 || strcmp(ptr->d_name,"..") == 0)
      continue;
    else if(ptr->d_type == 8){
      char *ingpath = (char *)malloc(sizeof(char)*50);
      strcpy(ingpath,basepath);
      strcat(ingpath,"/");
      strcat(ingpath,ptr->d_name);
      path[count] = ingpath;
      file_name[count++] = ptr->d_name;
    } 
    else if(ptr->d_type == 4){
      memset(base,'\0',sizeof(base));
      strcpy(base,basepath);
      strcat(base,"/");
      strcat(base,ptr->d_name);
      readFileList(base);
    }
  }
  closedir(dir);
  return 1;
}

