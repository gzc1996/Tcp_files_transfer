#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#pragma pack(1)
//文件描述信息
typedef struct _file_info
{
	char name[51];
	unsigned int size;

}file_info;

#pragma pack()


int main(int argc, char **argv)
{
/*	if(3 != argc)
	{
		fprintf(stderr, "参数错误!");
	}
*/
	int sock;
	int count = 1;

	for(int i=0; i<count; i++){
	//第一步:创建套接字
	    sock = socket(AF_INET, SOCK_STREAM, 0);

	//第two步:连接服务器
	//指定服务器地址
	    struct sockaddr_in srvaddr;
	    srvaddr.sin_family = AF_INET;
	    srvaddr.sin_addr.s_addr = inet_addr(argv[1]);
	    srvaddr.sin_port = htons(10000);

	//连接服务器地址
	    if(connect(sock, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) == -1)
	    {
		perror("connect error");
		exit(1);
	    }

	    recv(sock, &count, sizeof(count), 0);

            printf("there are %d files for transfer.\n", count);	
	
	    char str[100];
	    recv(sock, &str, sizeof(str), 0);
	    printf("%s\n", str);
	    printf("continue?y/n:");
	
	    char flag[10];
	    gets(flag);
	    send(sock, &flag, sizeof(flag), 0);
	

	    int ret = 0, ret1 = 0;
	
	    file_info fi;

	    ret = recv(sock, &fi, sizeof(fi), 0);	
	

	    char *buff = (char *)malloc(1024 * 1024); 

	    if(ret > 0)
	    {	
		FILE *fp = NULL;

		fp = fopen(fi.name, "wb");
	
		if(NULL == fp)
		{
			perror("fopen");
			exit(1);
		}	
		
		while(ret1 < fi.size)		
		{	
			ret = recv(sock, buff, 1024 * 1024, 0);
			fwrite(buff, 1, ret, fp);
			ret1 += ret;
			printf("reciving>>>>>>%.2f%\n", ((float)ret1 / (float)fi.size) * 100);
			printf("######################################\n");
		}
		fclose(fp);
	    } 
	
	    free(buff);
	    close(sock);
       }
	return 0;
}
