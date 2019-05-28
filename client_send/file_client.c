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
file_info fi;
const char* file_path;

void* send_thr(void* arg);
int readFileList(char* arg);

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
    else if(ptr->d_type == 8){//means it's a file not a folder or others
      char *ingpath = (char *)malloc(sizeof(char)*50);
      strcpy(ingpath,basepath);
      strcat(ingpath,"/");
      strcat(ingpath,ptr->d_name);
      path[count] = ingpath;
      file_name[count++] = ptr->d_name;
    } 
    else if(ptr->d_type == 4){//means it's a file folder, traverse all files recursively
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

int main(int argc, char** argv)
{
	char* file1 = "file";
        char basepath[100];   
        memset(basepath,'\0',sizeof(basepath));
        strcpy(basepath,"./");
        strcat(basepath,file1);
        readFileList(basepath);

       	int sock;
        printf("there are %d files for transfer.\n", count);

        for(int i = 0; i < count; i++){
	   strcpy(fi.name, file_name[i]);
	   printf("the %dth file path is %s.\n",i+1,path[i]);

	   char str[100];
	   sprintf(str,"the %dth file path is %s.\n",i+1,path[i]);
           
	   file_path = path[i];
           
	   struct stat st;
	   if(stat(file_path, &st) == -1)
	    {
		perror("Get file info fail");
		exit(1);
	    }
	   fi.size = st.st_size;

	   sock = socket(AF_INET, SOCK_STREAM, 0);

	   struct sockaddr_in srvaddr;
	   srvaddr.sin_family = AF_INET; //指定地址家族为Internet地址家族
	   srvaddr.sin_addr.s_addr = inet_addr(argv[1]);
	   srvaddr.sin_port = htons(10001); //指定端口号

	   if(connect(sock, (struct sockaddr*)&srvaddr, sizeof(srvaddr)) == -1){
		   perror("connect error");
		   exit(1);
	   }

	   if(-1 == send(sock, &fi, sizeof(fi), 0)){
		   perror("send");
		   close(sock);
		   return NULL;
	   }

	   if(-1 == send(sock, &count, sizeof(count), 0)){
		   perror("send");
		   close(sock);
		   return NULL;
	   }

	   if(-1 == send(sock, &str, sizeof(str), 0)){
		   perror("send");
		   close(sock);
		   return NULL;
	   }

	   FILE* fp = NULL;
	   int ret;
	   char buff[1024];

	   fp = fopen(file_path, "rb");

	   if(fp == NULL){
		   perror("Open sended file fail");
		   exit(1);
	   }

	   while(!feof(fp)){
		   ret = fread(buff, 1, sizeof(buff), fp);
		   if(-1 == send(sock, buff, ret, 0))
			   break;
	   }

	   fclose(fp);
	   close(sock);
	}

	return 0;
}
