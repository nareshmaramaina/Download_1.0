#include<header.h>
int Device_App_info_Details(char *Device_Application_release_file,char *DeviceApplicationType,char *DeviceApplicationName,float *DeviceApplicationVersion )
{
	FILE *fp = NULL;
	char *line=NULL,*str=NULL;
	size_t len=0;
	int flag=0;
	fp = fopen(Device_Application_release_file,"r");
	if(fp == NULL)
	{
		fprintf(stdout," %s  file not found \n",Device_Application_release_file);
		return -1;
	}
	else 
	{
		while( (getline(&line, &len, fp) > 0 ))
		{
			if((str = (char *)strstr(line,"ApplicationType:")) != NULL)
			{
				strcpy(DeviceApplicationType,str+16);
				if(DeviceApplicationType[ strlen(DeviceApplicationType) -1 ] == '\n')
					DeviceApplicationType[ strlen(DeviceApplicationType) - 1 ]='\0';
			}
			else if((str = (char *)strstr(line,"ApplicationName:")) != NULL)
			{
				strcpy(DeviceApplicationName,str+16);
				if(DeviceApplicationName[strlen(DeviceApplicationName)-1] == '\n')
					DeviceApplicationName[strlen(DeviceApplicationName)-1]='\0';
			}
			else if((str = (char *)strstr(line,"Version:")) != NULL)
			{
				flag=1;
				*DeviceApplicationVersion  = atof(str+8);
				break;
			}


			else fprintf(stdout,"Line = %s \n",line);

		}
		free(line);
		line=NULL;
		fclose(fp);
	}
	if ( flag == 1)
		return 0;
	else 
		return -1;
}
