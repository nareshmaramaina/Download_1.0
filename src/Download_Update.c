#include<header.h>

int Download_Update(char *URL, char *Filename_with_Download_Location)
{

	char Download_cmd[1024];

	int ret=0;

	int i=0;


	memset(Download_cmd,0x00,sizeof(Download_cmd));

	sprintf(Download_cmd,"curl --cacert /vision/DeviceManagement/certs/curl-ca-bundle.crt -C -  %s -o %s",URL,Filename_with_Download_Location);

	puts(Download_cmd);

	for (  i= 0 ; i < 20; i++)
	{
		Wait_for_internet(); //Block until internet 

		ret = system(Download_cmd);

		if (ret == 0)
			break;
		else 
			fprintf(stdout,"Error in server response, retrying %d\n",i);

		sleep(30);
	}	
	return ret;
}



