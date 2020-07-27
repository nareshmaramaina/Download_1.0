#include <header.h>
char *Application_response_file="/opt/Application_response.xml";
extern char SerialID[24];
extern char Server_Addr[512];

int Applications_Request_and_Response() 
{

	int ret=-1;
	FILE    *fp;
	char    cmd[1024];
	size_t len=0;
	char *str=NULL;

	memset(cmd,0,sizeof(cmd));

	sprintf(cmd,"curl --cacert /vision/curl-ca-bundle.crt %s/api/ApplicationStatus?serialNo=%s  > %s",Server_Addr,SerialID,Application_response_file);

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

	return ret;

}
