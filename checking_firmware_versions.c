#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/*int Firmware_Updates()
{
	FILE *fp = NULL;

	char *line=NULL,*str=NULL;

	size_t len=20;
	char buff[128]="";
	fp = fopen("/etc/Firmware_release","r");
	if(fp == NULL)
	{
		fprintf(stdout,"Firmware Details Unavailable\n");
		strcpy(module.FirmwareVersion,"Not Found");
		strcpy(module.FirmwareName,"Not Found");
		return -1;
	}
	while((getline(&line, &len, fp)) > 0)
	{
		if((str = (char *)strstr(line,"FirmwareName")) != NULL)
		{
			memset(buff,0,sizeof(buff));	
			strcpy(buff,str+12);
			if(buff[strlen(buff)-1] == '\n')
				buff[strlen(buff)-1]='\0';

			strcpy(module.FirmwareName,buff);
		}
		else if((str = (char *)strstr(line,"Version")) != NULL)
		{
			memset(buff,0,sizeof(buff));	
			strcpy(buff,str+7);
			if(buff[strlen(buff)-1] == '\n')
				buff[strlen(buff)-1]='\0';

			strcpy(module.FirmwareVersion,buff);
		}

	}
	free(line);
	line=NULL;
	str=NULL;
	fclose(fp);
	return 0;
}


int main()
{
 	Firmware_Updates();
        return 0;
} */


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int Firmware_Updates();
struct Firmware
{
	int  Total_Apps; 
	char FirmwareType[50][128];
	char FirmwareName[50][128];
	char FirmwareURL[50][512];
	float FirmwareVersion[50];
}F_Download;


int main()
{
	Firmware_Updates();	
	return 0;
}

int Firmware_Updates()
{
	FILE *fp = NULL;

	char *line=NULL,*str=NULL;
	int Update_count,Update_flag;
	size_t len=20;
	int i,j,check=0,  Total_Device_Apps=0,  Total_Server_Apps=0;
	char ServerFirmwareType[50][128];
	char ServerFirmwareName[50][128];
	char ServerFirmwareURL[50][512];
	float ServerFirmwareVersion[50];

	char DeviceFirmwareType[50][128];
	char DeviceFirmwareName[50][128];
	float DeviceFirmwareVersion[50];


	memset(ServerFirmwareType,0,sizeof(ServerFirmwareType));
	memset(ServerFirmwareName,0,sizeof(ServerFirmwareName));
	memset(ServerFirmwareURL,0,sizeof(ServerFirmwareURL));
	memset(ServerFirmwareVersion,0,sizeof(ServerFirmwareVersion));
	memset(DeviceFirmwareType,0,sizeof(DeviceFirmwareType));
	memset(DeviceFirmwareName,0,sizeof(DeviceFirmwareName));
	memset(DeviceFirmwareVersion,0,sizeof(DeviceFirmwareVersion));
	fp = fopen("/etc/Firmware_release","r");
	if(fp == NULL)
	{
		fprintf(stdout," /etc/Firmware_release  file not found \n");
		return -1;
	}

	for(i=0,j=0,check=0;getline(&line, &len, fp) > 0;)
	{
		if((str = (char *)strstr(line,"FirmwareType:")) != NULL)
		{
			if( check == 0 || check == 2 || check == 4 )
			{

				strcpy(ServerFirmwareType[i],str+16);
				if(ServerFirmwareType[i][ strlen(ServerFirmwareType[i]) -1 ] == '\n')
					ServerFirmwareType[i][ strlen(ServerFirmwareType[i]) - 1 ]='\0';
				i++;
				check=1;
			}
			else 
			{
				fprintf(stdout,"FirmwareType Error: Wrong Firmware Response in /etc/Firmware_release\n");
				return -1;
			}
		}
		else if((str = (char *)strstr(line,"FirmwareName:")) != NULL)
		{
			if ( check == 1 )
			{
				strcpy(ServerFirmwareName[j],str+16);
				if(ServerFirmwareName[j][strlen(ServerFirmwareName[j])-1] == '\n')
					ServerFirmwareName[j][strlen(ServerFirmwareName[j])-1]='\0';
				j++;
				check = 2;
			}
			else 
			{
				fprintf(stdout,"FirmwareName Error: Wrong Firmware Response in /etc/Firmware_release\n");
				return -1;
			}

		}
		else if((str = (char *)strstr(line,"Version:")) != NULL)
		{
			if( check == 2  && i == j )
			{
				ServerFirmwareVersion[j-1]  = atof(str+8);
				check=3;
			}
			else 
			{	
				fprintf(stdout," %d  %d \n", i,j);
				fprintf(stdout,"Version Error: Wrong Firmware Response in /etc/Firmware_release\n");
				return 0;
			}
		}

		else if((str = (char *)strstr(line,"FirmwareURL:")) != NULL)
		{
			if( check == 3  && i == j )
			{
				strcpy(ServerFirmwareURL[j-1],str+15);
				if(ServerFirmwareURL[j-1][strlen(ServerFirmwareURL[j-1])-1] == '\n')
					ServerFirmwareURL[j-1][strlen(ServerFirmwareURL[j-1])-1]='\0';
				check=4;
			}
			else
			{
				fprintf(stdout," %d  %d \n", i,j);
				fprintf(stdout,"FirmwareURL Error: Wrong Firmware Response in /etc/Firmware_release\n");
				return 0;
			}
		}

		else fprintf(stdout," Line = %s \n",line);

		if ( i == 49  )
		{
			fprintf(stdout,"Caution: Maximum 50 applications are servered, skipping remains"); 
			break;
		}


	}

	Total_Server_Apps = i;
	fprintf(stdout," Total_Server_Apps = %d\n", Total_Server_Apps);
	free(line);
	line=NULL;
	fclose(fp);

	if ( check == 3 )
	{
		fprintf(stdout,"FirmwareURL Error: Wrong Firmware Response in /etc/Firmware_release\n");
		return -1;
	}
	for(i=0;i<j;i++)
		fprintf(stdout,"FirmwareType =%s ,FirmwareName=%s,FirmwareVersion= %.1f FirmwareURL = %s\n",ServerFirmwareType[i],ServerFirmwareName[i],ServerFirmwareVersion[i],ServerFirmwareURL[i]);


	fp = fopen("/etc/visiontek_Firmware_release","r");
	if(fp == NULL)
	{
		fprintf(stdout," /etc/visiontek_Firmware_release  file not found \n");
		return -1;
	}

	for(i=0,j=0;getline(&line, &len, fp) > 0;)
	{
		if((str = (char *)strstr(line,"FirmwareType:")) != NULL)
		{
			strcpy(DeviceFirmwareType[i],str+16);
			if(DeviceFirmwareType[i][ strlen(DeviceFirmwareType[i]) -1 ] == '\n')
				DeviceFirmwareType[i][ strlen(DeviceFirmwareType[i]) - 1 ]='\0';
			i++;
		}
		else if((str = (char *)strstr(line,"FirmwareName:")) != NULL)
		{
			strcpy(DeviceFirmwareName[j],str+16);
			if(DeviceFirmwareName[j][strlen(DeviceFirmwareName[j])-1] == '\n')
				DeviceFirmwareName[j][strlen(DeviceFirmwareName[j])-1]='\0';
			j++;
		}
		else if((str = (char *)strstr(line,"Version:")) != NULL)
		{
			if( i > 0 && i == j )
				DeviceFirmwareVersion[j-1]  = atof(str+8);

			else 
			{	
				fprintf(stdout," %d  %d \n", i,j);
				fprintf(stdout,"Wrong Firmware Response in /etc/visiontek_Firmware_release\n");
				return 0;
			}
		}


		else fprintf(stdout,"Line = %s \n",line);

		if ( i == 49  )
		{
			fprintf(stdout,"Caution: Maximum 50 applications are servered, skipping remains");
			break;
		}


	}
	Total_Device_Apps = i;
	fprintf(stdout," Total_Device_Apps = %d\n", Total_Device_Apps);
	fprintf(stdout," %d  %d \n", i,j);
	free(line);
	line=NULL;
	fclose(fp);

	for(i=0,Update_count=0;i<Total_Server_Apps;i++)
	{

		for ( Update_flag=0,j=0; j < Total_Device_Apps ; j++ )
		{
			if (  strcmp(ServerFirmwareType[i],DeviceFirmwareType[j]) == 0 && strcmp(ServerFirmwareName[i],DeviceFirmwareName[j]) ==  0  )
			{

				if( ServerFirmwareVersion[i] > DeviceFirmwareVersion[j] )
				{
					fprintf(stdout,"Update Found,FirmwareType = %s, FirmwareName = %s ServerFirmwareVersion = %f,DeviceFirmwareVersion = %f\n",DeviceFirmwareType[j],DeviceFirmwareName[j],ServerFirmwareVersion[i] , DeviceFirmwareVersion[i]);

					Update_flag=1;		
				}

				else 
					fprintf(stdout,"No Update Found, FirmwareType = %s, FirmwareName = %s, DeviceFirmwareVersion = %f\n",DeviceFirmwareType[j],DeviceFirmwareName[j], DeviceFirmwareVersion[i]);
				break;

			}
		}

		if ( j == Total_Device_Apps || Update_flag == 1)
		{
			if ( j == Total_Device_Apps )
				fprintf(stdout,"Update Found,Newly Adding  FirmwareType = %s, FirmwareName = %s ServerFirmwareVersion = %f \n",ServerFirmwareType[i],ServerFirmwareName[i],ServerFirmwareVersion[i] );
			strcpy(F_Download.FirmwareType[Update_count],ServerFirmwareType[i]);
			strcpy(F_Download.FirmwareName[Update_count],ServerFirmwareName[i]);
			strcpy(F_Download.FirmwareURL[Update_count],ServerFirmwareURL[i]);
			F_Download.FirmwareVersion[Update_count]=ServerFirmwareVersion[i];
			Update_count++;
		}
	}
	F_Download.Total_Apps=Update_count;

	fprintf(stdout,"Firmware Update Count = %d \n",F_Download.Total_Apps );

	//		fprintf(stdout,"FirmwareType =%s ,FirmwareName=%s,FirmwareVersion= %.1f FirmwareURL = %s\n",ServerFirmwareType[i],ServerFirmwareName[i],ServerFirmwareVersion[i],ServerFirmwareURL[i]);
	//		fprintf(stdout,"FirmwareType =%s ,FirmwareName=%s,FirmwareVersion= %.1f \n",DeviceFirmwareType[i],DeviceFirmwareName[i],DeviceFirmwareVersion[i]);


	return 0;
}
