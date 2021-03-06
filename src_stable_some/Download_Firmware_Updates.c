#include<header.h>
char ServerProjectName[128];
extern char *Server_Firmware_release_file;
int Firmware_updates( int Total_Current_Server_Firmwares);
int Get_Server_Firmwares_Count();
int Download_firmwares(int Update_count,struct RHMSFirmware DownloadFirmware[Update_count],char *);
int Download_Firmware_Updates(void)
{
	int Total_Current_Server_Firmwares=0;
	Total_Current_Server_Firmwares = Get_Server_Firmwares_Count();
	fprintf(stdout,"\nFirmware Request and Response, Total Firmwares count = %d\n",Total_Current_Server_Firmwares);
	if ( Total_Current_Server_Firmwares <= 0)
	{
		if ( Total_Current_Server_Firmwares == 0 )
			fprintf(stdout,"\nNo intial Firmware Version in Server\n");
		else
			fprintf(stdout,"\nError: Firmware Request and Response not happened    \n");	
		return -1;
	}
	else
		return Firmware_updates(Total_Current_Server_Firmwares);	
}

int Firmware_updates( int Total_Current_Server_Firmwares)
{
	char Device_Firmware_release_file[320];
	float DeviceFirmwareVersion=0.0;
	char ServerFirmwareName[128]="";
	char DeviceFirmwareName[128]="";
	struct RHMSFirmware ServerFirmware[Total_Current_Server_Firmwares];
	struct RHMSFirmware DownloadFirmware[Total_Current_Server_Firmwares];
	FILE *fp = NULL;
	char *line=NULL,*str=NULL;
	int Update_count,F_check=0;
	size_t len=0,sizeofBuffer=0;
	int i,check=0, ret ,  Total_Server_Firmwares=0;
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

		if((str = (char *)strstr(line,"ProjectName:")) != NULL)
		{
			memset(ServerProjectName,0,sizeof(ServerProjectName));
			sizeofBuffer = sizeof(ServerProjectName);
			if( strlen(str+12) > sizeofBuffer )
			{
				fprintf(stderr,"Invalid: ServerProjectName Length More than %d bytes \n",sizeofBuffer);
				continue;
			}
			strcpy(ServerProjectName,str+12);
			if(ServerProjectName[strlen(ServerProjectName)-1] == '\n')
				ServerProjectName[strlen(ServerProjectName)-1]='\0';

		}
		else if((str = (char *)strstr(line,"FirmwareName:")) != NULL)
		{
			if ( F_check == 0 )
			{
				sizeofBuffer = sizeof(ServerFirmwareName);
				if( strlen(str+13) > sizeofBuffer )
				{
					fprintf(stderr,"Invalid: ServerFirmwareName Length More than %d bytes \n",sizeofBuffer);
					continue;
				}

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
				sizeofBuffer = sizeof(ServerFirmware[i].URL);
				if( strlen(str+16) > sizeofBuffer )
				{
					fprintf(stderr,"Invalid: ServerFirmware[%d].URL Length More than %d bytes \n",i,sizeofBuffer);
					continue;
				}

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

	}
	free(line);
	line=NULL;
	fclose(fp);

	Total_Server_Firmwares = i;
	//fprintf(stdout," Total_Server_Firmwares = %d\n", Total_Server_Firmwares);
	if ( Total_Server_Firmwares == 0 )
	{
		fprintf(stdout,"Something Went Wrong with Firmware parse/request call , Empty Response came\n");
		return -1;
	}

	//	for(i=0;i<Total_Server_Firmwares;i++)
	//		fprintf(stdout,"FirmwareName=%s,FirmwareVersion= %.1f FirmwareURL = %s\n",ServerFirmwareName,ServerFirmware[i].Version,ServerFirmware[i].URL);

	memset( Device_Firmware_release_file,0,sizeof(Device_Firmware_release_file) );
	sprintf(Device_Firmware_release_file,"/etc/vision/RHMS/Firmware/%s/%s/FirmwareUpdated.info",ServerProjectName,ServerFirmwareName);
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
				sizeofBuffer = sizeof(DeviceFirmwareName);
				if( strlen(str+13) > sizeofBuffer )
				{
					fprintf(stderr,"Invalid: DeviceFirmwareName Length More than %d bytes \n",sizeofBuffer);
					continue;
				}

				memset(DeviceFirmwareName,0,sizeof(DeviceFirmwareName));
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


		}
		free(line);
		line=NULL;
		fclose(fp);
	}
	ret = strcmp(ServerFirmwareName,DeviceFirmwareName);
	if ( ret != 0 )
	{
		if ( strlen(DeviceFirmwareName) == 0 )
			fprintf(stdout,"Intial Firmware patch\n");
		else
			fprintf(stdout,"Firmware Name/Project Name  Newly Added or Got Changed by server\n");

		DeviceFirmwareVersion=0.0;
	}

	if( ServerFirmware[0].Version > DeviceFirmwareVersion )
	{

		for (Update_count=0, i=0; i < Total_Server_Firmwares ; i++ )
		{

			if( ServerFirmware[i].Version > DeviceFirmwareVersion )
			{
				if ( strlen(ServerFirmware[i].URL) < 12 )
				{
					fprintf(stdout,"\nURL Not found / Not Correct,  FirmwareName = %s ServerFirmwareVersion = %f \n",ServerFirmwareName,ServerFirmware[i].Version );
					continue;
				}
				if ( strlen(ServerFirmwareName) <= 0 || ServerFirmware[i].Version <= 0.0 )
				{
					fprintf(stdout,"Name/Version Not Found / Not Correct,  FirmwareName = %s ServerFirmwareVersion = %f \n",ServerFirmwareName,ServerFirmware[i].Version );
					continue;
				}
				strcpy(DownloadFirmware[Update_count].URL,ServerFirmware[i].URL);
				DownloadFirmware[Update_count].Version=ServerFirmware[i].Version;
				fprintf(stdout,"\nUpdate Found,FirmwareName = %s ServerFirmwareVersion = %f,DeviceFirmwareVersion = %f\n",ServerFirmwareName,ServerFirmware[i].Version , DeviceFirmwareVersion);

				Update_count++;
			}

		}
		if( Update_count > 0 )
			Download_firmwares(Update_count,DownloadFirmware,ServerFirmwareName);
		else 
			fprintf(stdout,"No Firmware Updates Found\n");
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
	char FileName_with_path[420];
	char path[410];
	char cmd[430];
	float Version=0.0;
	int i,ret,start,end;

	fprintf(stdout,"\nNeed to Download %d Firmwares \n",Update_count);

	if ( Update_count == 1 )
	{
		start=0;
		end=Update_count-1;
	}
	else if ( Update_count > 1 )
	{
		fprintf(stdout,"Firmware Dependencies Found \n");
		start=1;
		end=Update_count;
	}
	else 
	{
		fprintf(stdout," Wrong Downloads cenerio\n");
		return -1;
	}
	//	for(i=Update_count-1;i>=0;i--)
	memset(path,0,sizeof(path));
	memset(cmd,0,sizeof(cmd));
	sprintf(path,"/mnt/sysuser/Software-Upgrade/Firmware_Downloads/%s/%s",ServerProjectName,FirmwareName);
	sprintf(cmd,"mkdir -p %s",path);
	system(cmd);
	for(i=start;i<=end;i++)  // 2 
	{
		memset(FileName_with_path,0,sizeof(FileName_with_path));
		Version=0;
		if ( i == Update_count )
		{
			fprintf(stdout,"Final Version Downloading \n");
			sprintf(FileName_with_path,"%s/firmware-%.1f.zip",path,DownloadFirmware[0].Version);
			Version = DownloadFirmware[0].Version;
			ret = check_Download_complete(FileName_with_path,FIRMWARE);	
			if ( ret == 0 )
			{
				fprintf(stdout,"%.1f Version Already Download Completed, Ready to installation\n",DownloadFirmware[0].Version);
				continue;	
			}
			else 
				ret = Download_Update(DownloadFirmware[0].URL,FileName_with_path);
		}
		else 
		{
			sprintf(FileName_with_path,"%s/firmware-%.1f.zip",path,DownloadFirmware[i].Version);
			Version = DownloadFirmware[i].Version;
			ret = check_Download_complete(FileName_with_path,FIRMWARE);	
			if ( ret == 0 )
			{
				fprintf(stdout,"%.1f Version Already DownloadCompleted, Ready to installation\n",DownloadFirmware[i].Version);
				continue;	
			}
			else 
				ret = Download_Update(DownloadFirmware[i].URL,FileName_with_path);
		}
		if ( ret == 0 )
		{
			fprintf(stdout,"%s File Download Success\n",FileName_with_path);
			ret = Add_to_installation(Version,path,FileName_with_path,FIRMWARE);
			if ( ret != 0 )
			{
				fprintf(stderr,"Error, Failed to  Add to Installation in Firmware %s ",FileName_with_path);
				if ( Update_count  > 1)
					fprintf(stdout,"Dependencies must and Should Download and Install sequencial way, So skipping remaining Downloads\n"); 
				return -1;
			}
		}
		else
		{
			fprintf(stdout,"%s File Download Failure\n",FileName_with_path);
			if ( Update_count  > 1)
				fprintf(stdout,"Dependencies must and Should Download and Install sequencial way, So skipping remaining Downloads\n"); 
			return -1;
		}
		//fprintf(stdout,"DownloadFirmware[%d].URL = %s,DownloadFirmware[%d].Version = %f\n",i,DownloadFirmware[i].URL,i,DownloadFirmware[i].Version);
	}
	return 0;
}
