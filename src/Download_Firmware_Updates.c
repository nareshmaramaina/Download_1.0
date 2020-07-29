#include<header.h>
char *Server_Firmware_release_file="/etc/Firmware_release";
char *Device_Firmware_release_file="/etc/visiontek_Firmware_release";
int Firmware_updates( int Total_Current_Server_Apps);
int Get_Server_Firmwares_Count();
int Download_firmwares(int Update_count,struct RHMSFirmware DownloadFirmware[Update_count],char *);
int Download_Firmware_Updates(void)
{
	int Total_Current_Server_Apps=0;
	Total_Current_Server_Apps = Get_Server_Firmwares_Count();
	fprintf(stdout,"Total Current Firmware Server Apps = %d\n",Total_Current_Server_Apps);
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
	FILE *fp = NULL;
	char *line=NULL,*str=NULL;
	int Update_count,F_check=0;
	size_t len=0;
	int i,check=0, ret ,  Total_Server_Apps=0;
	memset(ServerFirmware,0,sizeof(ServerFirmware));
	memset(DownloadFirmware,0,sizeof(DownloadFirmware));
	fp = fopen(Server_Firmware_release_file,"r");
	if(fp == NULL)
	{
		fprintf(stdout," %s  file not found \n",Server_Firmware_release_file);
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
				fprintf(stdout,"FirmwareName check Error: Wrong Firmware Response in %s\n",Server_Firmware_release_file);
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
				fprintf(stdout,"Version check Error: Wrong Firmware Response in %s\n",Server_Firmware_release_file);
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
				fprintf(stdout,"FirmwareURL check Error: Wrong Firmware Response in %s\n",Server_Firmware_release_file);
				return 0;
			}
		}

		else fprintf(stdout," Line = %s \n",line);


	}

	Total_Server_Apps = i;
	fprintf(stdout," Total_Server_Apps = %d\n", Total_Server_Apps);
	if ( Total_Server_Apps == 0 )
	{
	fprintf(stdout,"Something Went Wrong with Firmware parse/request call , Empty Response came\n");
	return -1;
	}
	free(line);
	line=NULL;
	fclose(fp);

	//	for(i=0;i<Total_Server_Apps;i++)
	//		fprintf(stdout,"FirmwareName=%s,FirmwareVersion= %.1f FirmwareURL = %s\n",ServerFirmwareName,ServerFirmware[i].Version,ServerFirmware[i].URL);


	fp = fopen(Device_Firmware_release_file,"r");
	if(fp == NULL)
	{
		fprintf(stdout," %s  file not found \n",Device_Firmware_release_file);
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
					fprintf(stdout,"FirmwareName Error : Wrong Firmware Response in %s\n",Device_Firmware_release_file);
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
		fprintf(stdout,"Firmware Name Newly Added or Got Changed by server\n");
		//	system("cat %s >> /etc/.visiontek_Firmware_release_Changed");
		//		remove(Device_Firmware_release_file);	
	}

	if( ServerFirmware[0].Version > DeviceFirmwareVersion )
	{

		for (Update_count=0, i=0; i < Total_Server_Apps ; i++ )
		{

			if( ServerFirmware[i].Version > DeviceFirmwareVersion )
			{
				if ( strlen(ServerFirmware[i].URL) < 12 )
				{
					fprintf(stdout,"URL Not found / Not Correct,  FirmwareName = %s ServerFirmwareVersion = %f \n",ServerFirmwareName,ServerFirmware[i].Version );
					continue;
				}
				if ( strlen(ServerFirmwareName) <= 0 || ServerFirmware[i].Version <= 0.0 )
				{
					fprintf(stdout,"URL Not found / Not Correct,  FirmwareName = %s ServerFirmwareVersion = %f \n",ServerFirmwareName,ServerFirmware[i].Version );
					continue;
				}
				strcpy(DownloadFirmware[Update_count].URL,ServerFirmware[i].URL);
				DownloadFirmware[Update_count].Version=ServerFirmware[i].Version;
				fprintf(stdout,"Update Found,FirmwareName = %s ServerFirmwareVersion = %f,DeviceFirmwareVersion = %f\n",DeviceFirmwareName,ServerFirmware[i].Version , DeviceFirmwareVersion);

				Update_count++;
			}

		}
		Download_firmwares(Update_count,DownloadFirmware,ServerFirmwareName);
	}

	else 
		fprintf(stdout,"No Update Found,  FirmwareName = %s, Current DeviceFirmwareVersion = %f\n",DeviceFirmwareName, DeviceFirmwareVersion);


	return Update_count;
}
int Get_Server_Firmwares_Count()
{
	FILE *fp = NULL;

	char *line=NULL;
	int i;
	size_t len;
	fp = fopen(Server_Firmware_release_file,"r");
	if(fp == NULL)
	{
		fprintf(stdout," %s  file not found \n",Server_Firmware_release_file);
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
int Download_firmwares(int Update_count,struct RHMSFirmware DownloadFirmware[Update_count],char *FirmwareName)
{
	char FileName_with_path[320];
	char path[310];
	char cmd[320];
	int i,ret;

	fprintf(stdout,"Firmware Update count %d \n",Update_count);
	//for(i=0;i<Update_count;i++)
	for(i=Update_count-1;i>=0;i--)
	{
		memset(path,0,sizeof(path));
		memset(cmd,0,sizeof(cmd));
		memset(FileName_with_path,0,sizeof(FileName_with_path));
		sprintf(path,"/mnt/sysuser/Software-Upgrade/Firmware_Downloads/%s",FirmwareName);
		sprintf(cmd,"mkdir -p %s",path);

		system(cmd);

		sprintf(FileName_with_path,"%s/firmware-%.1f.zip",path,DownloadFirmware[i].Version);

		ret = Download_Update(DownloadFirmware[i].URL,FileName_with_path);

		if ( ret == 0 )
		{
			fprintf(stdout,"%s File Download Success\n",FileName_with_path);
			Add_to_installation(FileName_with_path,1);
		}
		else
			fprintf(stdout,"%s File Download Failure\n",FileName_with_path);

		//fprintf(stdout,"DownloadFirmware[%d].URL = %s,DownloadFirmware[%d].Version = %f\n",i,DownloadFirmware[i].URL,i,DownloadFirmware[i].Version);
	}
	return 0;
}
