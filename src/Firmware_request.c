#include<header.h>
char *Firmware_response_file="/opt/Firmware_response.xml";
extern char SerialID[24];
extern char Server_Addr[512];

int Firmware_Request_and_Response() 
{

	int ret=-1;
	FILE    *fp;
	char    cmd[1024];
	size_t len=0;
	char *str=NULL;

	memset(cmd,0,sizeof(cmd));

	sprintf(cmd,"curl --cacert /vision/curl-ca-bundle.crt %s/api/FirmwareStatus?serialNo=%s  > %s",Server_Addr,SerialID,Firmware_response_file);

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
	return ret; 

}

