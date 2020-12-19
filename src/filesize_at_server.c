#include<header.h>
int filesize_at_server(char *URL,int Env_flag)
{
	char cmd[1024]="";
	int size=0;
	memset(cmd,0,sizeof(cmd));

	if ( Env_flag == 1 )
		sprintf(cmd,"/vision/DeviceManagement/system/usr/bin/DM_curl --head --cacert  /vision/DeviceManagement/certs/curl-ca-bundle.crt %s | grep -i Content-Length | cut -d':' -f2",URL);
	else 
		sprintf(cmd,"curl --head  --cacert  /vision/DeviceManagement/certs/curl-ca-bundle.crt %s | grep -i Content-Length | cut -d':' -f2",URL);
	puts(cmd);
	FILE *fp=NULL;
	fp = popen(cmd,"r");
	if ( fp != NULL )
	{
		fscanf(fp,"%d",&size);
		fprintf(stdout,"HEAD Size of URL file = %d\n",size);
		pclose(fp);
		return size;
	}
	else return -1; 
}
int size_of_file(char *file)
{
	struct stat buf;
	if(stat(file,&buf) != 0)
	{
		perror("error:");
		return -1;
	}

	else
		return buf.st_size;

}

