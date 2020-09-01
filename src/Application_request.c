#include <header.h>
char *Application_response_file="/opt/Application_response.xml";
extern char SerialID[24];
extern char Server_Addr[512];
extern char *Error_log_filename;
int Applications_Request_and_Response()
{
	int ret = 0;
	int i=0;

	for (i=0; i<5; i++)
	{
		Wait_for_internet();

		ret = Applications_Request();
		if ( ret == 0 )
		{
			fprintf(stdout,"Applications Request  and Response done Successfully\n");
			break;
		}
		else if ( ret == -2 )
			break;
		else
		{
			fprintf(stderr,"Applications Request Failure, retrying = %d, Waiting for 60secs ****\n",i+1);
			sleep(60);
		}

	}

	return ret;

}

int Applications_Request()
{

	int ret=-1;
	FILE    *fp;
	char    cmd[1024];
	size_t len=0;
	char *str=NULL;

	memset(cmd,0,sizeof(cmd));

	sprintf(cmd,"curl --cacert /vision/DeviceManagement/certs/curl-ca-bundle.crt %s/api/ApplicationStatus?serialNo=%s 1> %s 2>%s",Server_Addr,SerialID,Application_response_file,Error_log_filename);

	system(cmd);

	puts(cmd);

	memset(cmd,0,sizeof(cmd));

	sprintf(cmd,"dos2unix %s",Application_response_file);

	system(cmd);

	fp = fopen(Application_response_file,"r");

	if (fp == NULL)
	{
		fprintf(stderr,"%s open error\n",Application_response_file);
		return -1;
	}


	while((getline(&str,&len,fp)) != -1)
	{
		if  (strstr(str,"Device is not registered") != NULL )
		{
			ret = -2;
			break;
		}
		else if  (strstr(str,"ArrayOfApplicationStatusResponse") != NULL )
			ret = 0;
	}

	free(str);
	str=NULL;
	fclose(fp);

	if( ret == 0 )
		ret = Parse_Application_response_xml();
	else  
		Check_Address_Error_and_Update_Server_Addr_If_Error_Present();

	return ret;

}
