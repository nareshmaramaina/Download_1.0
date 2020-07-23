#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int Firmware_updates();
struct RHMSFirmware
{
	char URL[512];
	float Version;
};

int main()
{
	int Total_Current_Server_Apps=0;
	Total_Current_Server_Apps = Get_Server_Firmwares_Count();
	fprintf(stdout,"Total_Current_Server_Apps = %d\n",Total_Current_Server_Apps);
	if ( Total_Current_Server_Apps < 0)
		return -1;
	Firmware_updates(Total_Current_Server_Apps);	
	return 0;
}

int Firmware_updates( int Total_Current_Server_Apps)
{
	float DeviceFirmwareVersion=0.0;
	char ServerFirmwareName[128]="";
	char DeviceFirmwareName[128]="";
	struct RHMSFirmware ServerFirmware[Total_Current_Server_Apps];
	struct RHMSFirmware DownloadFirmware[Total_Current_Server_Apps];
//	struct POSFirmware DeviceFirmware[Total_Current_Server_Apps];
	FILE *fp = NULL;
	char *line=NULL,*str=NULL;
	int Update_count,F_check=0;
	size_t len=0;
	int i,check=0, ret ,  Total_Server_Apps=0;
	fprintf(stdout,"RHMS Firmware size = %ld\n",sizeof(struct RHMSFirmware));
	memset(ServerFirmware,0,sizeof(ServerFirmware));
	memset(DownloadFirmware,0,sizeof(DownloadFirmware));
	fp = fopen("/etc/Firmware_release","r");
	if(fp == NULL)
	{
		fprintf(stdout," /etc/Firmware_release  file not found \n");
		return -1;
	}

	for(i=0,F_check=0,check=0;getline(&line, &len, fp) > 0;)
	{
		if((str = (char *)strstr(line,"FirmwareName:")) != NULL)
		{
			if ( F_check == 0 )
			{
				strcpy(ServerFirmwareName,str+13);
				if(ServerFirmwareName[strlen(ServerFirmwareName)-1] == '\n')
					ServerFirmwareName[strlen(ServerFirmwareName)-1]='\0';
				F_check = 1;
			}
			else 
			{
				fprintf(stdout,"FirmwareName check Error: Wrong Firmware Response in /etc/Firmware_release\n");
				return -1;
			}

		}
		else if((str = (char *)strstr(line,"Version:")) != NULL)
		{
			if( F_check == 1 && (i==0 || check == 2) )
			{

				ServerFirmware[i].Version  = atof(str+8);
				check=1;
			}
			else 
			{	
				fprintf(stdout,"Version check Error: Wrong Firmware Response in /etc/Firmware_release\n");
				return 0;
			}
		}

		else if((str = (char *)strstr(line,"FirmwareURL:")) != NULL)
		{
			if( check == 1  )
			{
				strcpy(ServerFirmware[i].URL,str+12);
				if(ServerFirmware[i].URL[strlen(ServerFirmware[i].URL)-1] == '\n')
					ServerFirmware[i].URL[strlen(ServerFirmware[i].URL)-1]='\0';
				check =2;
				i++;
			}
			else
			{
				fprintf(stdout,"FirmwareURL check Error: Wrong Firmware Response in /etc/Firmware_release\n");
				return 0;
			}
		}

		else fprintf(stdout," Line = %s \n",line);


	}

	Total_Server_Apps = i;
	fprintf(stdout," Total_Server_Apps = %d\n", Total_Server_Apps);
	free(line);
	line=NULL;
	fclose(fp);

	for(i=0;i<Total_Server_Apps;i++)
		fprintf(stdout,"FirmwareName=%s,FirmwareVersion= %.1f FirmwareURL = %s\n",ServerFirmwareName,ServerFirmware[i].Version,ServerFirmware[i].URL);


	fp = fopen("/etc/visiontek_Firmware_release","r");
	if(fp == NULL)
	{
		fprintf(stdout," /etc/visiontek_Firmware_release  file not found \n");
	}
	else 
	{
		for(i=0,F_check=0;getline(&line, &len, fp) > 0;)
		{
			if((str = (char *)strstr(line,"FirmwareName:")) != NULL)
			{
				strcpy(DeviceFirmwareName,str+13);
				if(DeviceFirmwareName[strlen(DeviceFirmwareName)-1] == '\n')
					DeviceFirmwareName[strlen(DeviceFirmwareName)-1]='\0';
				F_check=1;
			}
			else if((str = (char *)strstr(line,"Version:")) != NULL)
			{
				if ( F_check == 1 )
				{
					DeviceFirmwareVersion  = atof(str+8);
					break;
				}
				else 
				{	
					fprintf(stdout,"FirmwareName Error : Wrong Firmware Response in /etc/visiontek_Firmware_release\n");
					return 0;
				}
			}


			else fprintf(stdout,"Line = %s \n",line);


		}
		free(line);
		line=NULL;
		fclose(fp);
	}
	ret = strcmp(ServerFirmwareName,DeviceFirmwareName);
	if ( ret != 0) 
	{

		fprintf(stdout,"Firmware Name Got Changed\n");
		system("cat /etc/visiontek_Firmware_release >> /etc/.visiontek_Firmware_release_Changed");
		remove("/etc/visiontek_Firmware_release");	
	}

	if( ServerFirmware[0].Version > DeviceFirmwareVersion )
	{

		for (Update_count=0, i=0; i < Total_Server_Apps ; i++ )
		{

			if( ServerFirmware[i].Version > DeviceFirmwareVersion )
			{
				if ( strlen(ServerFirmware[i].URL) < 12 )
				{
					fprintf(stdout,"URL Not found,  FirmwareName = %s ServerFirmwareVersion = %f \n",ServerFirmwareName,ServerFirmware[i].Version );
					continue;
				}
				strcpy(DownloadFirmware[Update_count].URL,ServerFirmware[i].URL);
				DownloadFirmware[Update_count].Version=ServerFirmware[i].Version;
				fprintf(stdout,"Update Found,FirmwareName = %s ServerFirmwareVersion = %f,DeviceFirmwareVersion = %f\n",DeviceFirmwareName,ServerFirmware[i].Version , DeviceFirmwareVersion);

				Update_count++;
			}

		}
		Download_firmwares(Update_count,DownloadFirmware);
	}

	else 
		fprintf(stdout,"No Update Found,  FirmwareName = %s, Current DeviceFirmwareVersion = %f\n",DeviceFirmwareName, DeviceFirmwareVersion);


	return 0;
}
int Get_Server_Firmwares_Count()
{
	FILE *fp = NULL;

	char *line=NULL;
	int i;
	size_t len;
	fp = fopen("/etc/Firmware_release","r");
	if(fp == NULL)
	{
		fprintf(stdout," /etc/Firmware_release  file not found \n");
		return -1;
	}
	else 
	{
		for(i=0;getline(&line, &len, fp) > 0;)
			if(	strstr(line,"Version:") != NULL)
				i++;
		fclose(fp);

	}
	return i;
}
int Download_firmwares(int Update_count,struct RHMSFirmware DownloadFirmware[Update_count])
{
	fprintf(stdout,"Update_count %d \n",Update_count);
	int i;
	for(i=0;i<Update_count;i++)
		fprintf(stdout,"DownloadFirmware[%d].URL = %s,DownloadFirmware[%d].Version = %f\n",i,DownloadFirmware[i].URL,i,DownloadFirmware[i].Version);

	return 0;
}
