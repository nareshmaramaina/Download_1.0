#include<header.h>

int Download_Update(char *URL, char *Filename_with_Download_Location)
{

	char Download_cmd[1024];
	
	char *ptr=NULL;

	int ret=0;

	short int  i=0,Env_flag=0;

	char curl_name[64]="/vision/DeviceManagement/system/usr/bin/DM_curl";
	if ( access(curl_name,F_OK) != 0 )
	{
		fprintf(stdout,"%s not found \n",curl_name);
		memset(curl_name,0,sizeof(curl_name));
		strcpy(curl_name,"curl");
	}
	else
	{
		setenv("LD_LIBRARY_PATH","/vision/DeviceManagement/system/usr/lib/",1);
		Env_flag=1;

	}


	memset(Download_cmd,0x00,sizeof(Download_cmd));

	sprintf(Download_cmd,"%s --cacert  /vision/DeviceManagement/certs/curl-ca-bundle.crt -C -  %s -o %s",curl_name,URL,Filename_with_Download_Location);

	puts(Download_cmd);

	for (  i= 0 ; i < 20; i++)
	{
		Wait_for_internet(); //Block until internet 

		Check_date_set_if_wrong(0);

		ret = system(Download_cmd);
		if (ret == 0)
			break;
		else if ( ret == 8448 )
		{
			fprintf(stdout,"byte position Error, Need to Delete %s, and start without -C - mode, Retrying Count %d, Cmd ret = %d, shell curl understand ret value  = %d\n",Filename_with_Download_Location,i,ret,ret/255);
			remove(Filename_with_Download_Location);
			ptr = strstr(Download_cmd,"-C -");
			if ( ptr != NULL )
			{
				ptr[0]=ptr[1]=ptr[2]=ptr[3]=' ';
				i--;
			}

			fprintf(stdout,"No -C - Download mode, Download CMD = %s\n",Download_cmd);
			continue;
		}
		else  
			fprintf(stdout,"Error in server response, retrying %d, Cmd ret = %d, shell curl understand ret value  = %d \nDownload: Sleep 30secs\n",i,ret,ret/255);

		sleep(30);
	}	
	if ( Env_flag == 1 )
		unsetenv("LD_LIBRARY_PATH");
	return ret;
}



