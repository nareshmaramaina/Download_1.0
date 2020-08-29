#include<header.h>
char *Firmware_response_file="/opt/Firmware_response.xml";
extern char SerialID[24];
extern char *Default_RHMS_Server_Address;
char *Error_log_filename="/opt/.DownloaderError.log";
extern char Server_Addr[512];
int Firmware_Request_and_Response()
{
	int ret = 0;
	int i=0;

	for (i=0; i<5; i++)
	{
		Wait_for_internet();

		ret = Firmware_Request();
		if ( ret == 0 )
		{
			fprintf(stdout,"Firmware Request  and Response done Successfully\n");
			break;
		}
		else if ( ret == -2 )
			break;
		else
		{
			fprintf(stderr,"Firmware Request Failure, retrying = %d, Waiting for 60secs ****\n",i+1);
			sleep(60);
		}

	}

	return ret;

}
int Firmware_Request() 
{

	int ret=-1;
	FILE    *fp;
	char    cmd[1024];
	size_t len=0;
	char *str=NULL;

	memset(cmd,0,sizeof(cmd));
	sprintf(cmd,"curl --cacert /vision/curl-ca-bundle.crt %s/api/FirmwareStatus?serialNo=%s 1> %s 2>%s",Server_Addr,SerialID,Firmware_response_file,Error_log_filename);

	system(cmd);

	puts(cmd);
	memset(cmd,0,sizeof(cmd));

	sprintf(cmd,"dos2unix %s",Firmware_response_file);

	system(cmd);

	fp = fopen(Firmware_response_file,"r");

	if (fp == NULL)
	{
		fprintf(stderr,"%s open error\n",Firmware_response_file);
		return -1;
	}


	while((getline(&str,&len,fp)) != -1)
	{
		if  (strstr(str,"Device is not registered") != NULL )
		{
			ret = -2;
			break;
		}
		else if  (strstr(str,"FirmwareStatusResponse") != NULL )
			ret = 0;
	}

	free(str);
	str=NULL;
	fclose(fp);

	if ( ret == 0 )
		ret = Parse_Firmware_response_xml(); 
	else  
		Check_Address_Error_and_Update_Server_Addr_If_Error_Present();

	return ret;
}
int Check_Address_Error_and_Update_Server_Addr_If_Error_Present()
{
	FILE    *fp;
	size_t len=0;
	char *str=NULL;

	fp = fopen(Error_log_filename,"r");

	if (fp == NULL)
	{
		fprintf(stderr,"%s open error\n",Error_log_filename);
		return -1;
	}


	while((getline(&str,&len,fp)) != -1)
	{
		if ( strstr(str,"Average Speed   Time" ) != NULL || strstr(str,"Dload  Upload") != NULL || strstr(str,"--:--:--")!= NULL)
			continue;
		if ( strstr(str,"Couldn't resolve host") != NULL )
		{
			memset(Server_Addr,0,sizeof(Server_Addr));
			strcpy(Server_Addr,Default_RHMS_Server_Address);
			fprintf(stderr,"ServerAddress Error: %s so Proceeding with Default Server Address = %s\n",str,Server_Addr);
			break;
		}

		fprintf(stderr,"Curl Commad Error Log: %s\n",str);
	}
	if ( strstr(str,"curl") != NULL )
		fprintf(stderr,"curl related issue\n");
	else
		fprintf(stderr,"\033[1;31m RHMS Server Network Issue\033[0m\n");

	free(str);
	str=NULL;
	fclose(fp);

	return 0;
}
