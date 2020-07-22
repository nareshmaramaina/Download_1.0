#include<header.h>
int get_updates_info(int Type)
{
	char *app_config_file="/mnt/sysuser/App-Upgrade/config-appversions";
	char *firmware_config_file="/mnt/sysuser/Firmware-Upgrade/config-patchversions";
	FILE *fp=NULL;
	char *line=NULL,*str=NULL;
	size_t len=0;
	int Total_count;
	short int i,j;
	float versions[100],version;


	if ( Type == 1)
	{
		fp = fopen(firmware_config_file,"r");
		if ( fp == NULL )
		{
			fprintf(stderr,"%s file not found error\n",firmware_config_file);
			return -1;
		}

		Total_count=0;
		while((getline(&line, &len, fp)) > 0)
		{
			if((str = (char *)strstr(line,"PATCH_VERSION=")) != NULL)
			{
				version = atof(str+14);
				if ( version > Device.FirmWarePatchVersion && version <= RHMS_Server.FirmWarePatchVersion )
					versions[Total_count++]=version;
			}

		}


	}
	else if ( Type == 2)
	{
		fp = fopen(app_config_file,"r");
		if ( fp == NULL )
		{
			fprintf(stderr,"%s file not found error\n",app_config_file);
			return -1;
		}

		Total_count=0;

		while((getline(&line, &len, fp)) > 0)
		{
			if((str = (char *)strstr(line,"APP_VERSION=")) != NULL)
			{
				version = atof(str+12);
				if ( version > Device.ApplicationVersion && version <= RHMS_Server.ApplicationVersion )
					versions[Total_count++]=version;
			}

		}
	}


	else fprintf(stderr,"In %s  Type Error \n",__func__);

	for(i=0;i<Total_count-1;i++) //Sorting for right Order
	{
		for(j=0;j<Total_count-i-1;j++)
		{
			if(versions[j]>versions[j+1])
			{
				version=versions[j+1];
				versions[j+1]=versions[j];
				versions[j]=version;
			}
		}
	}


	patch.Total_Updates_count = Total_count;

	fprintf(stdout,"Total Number of  Patch updates  count = %d\n",patch.Total_Updates_count);

	for(i=0; i < patch.Total_Updates_count; i++)
	{
		patch.Update_Version[i] = versions[i];
		fprintf(stdout," Download Patch Number %d)  Download Version  =  %.1f\n",i+1, versions[i]);
	}

	return Total_count;
}

