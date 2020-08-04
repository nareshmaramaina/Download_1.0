#include<header.h>
char *Install_Applications_file="/mnt/sysuser/Software-Upgrade/Applications_Downloads/Install_Applications.info";
char *Install_Firmwares_file="/mnt/sysuser/Software-Upgrade/Firmware_Downloads/Install_Firmwares.info";
char *Standard_Apps_path="/mnt/sysuser/Software-Upgrade/Applications_Downloads/";
char *Standard_Firmwares_path="/mnt/sysuser/Software-Upgrade/Firmware_Downloads/";
int main()
{

	Delete_From_installation("/mnt/sysuser/Software-Upgrade/Firmware_Downloads/testing/firmware-1.1.zip",1);
	return 0;
}
int Delete_From_installation(char *patch_file,int type)
{
	int Total_Count;
	Get_Total_Downloaded_Updates(type);
	Total_Count = Get_Total_Downloaded_Updates(type);
	Delete_patch(Total_Count,patch_file, type);
	return 0;
}
int Delete_patch(int Total_PatchCount,char *patch_file,int type)
{
	char patch[Total_PatchCount][330];
	FILE *fp=NULL;
	char filename[128];
	char *line=NULL;
	size_t len=0;
	int Found=0,PatchCount=0,i;
	char path[128];
	memset(filename,0,sizeof(memset));
	memset(patch,0,sizeof(patch));
	memset(path,0,sizeof(path));

	if ( type == FIRMWARE )
	{
		strcpy(path,Standard_Firmwares_path);
		strcpy(filename,Install_Firmwares_file);
	}
	else if ( type == APPLICATION )
	{
		strcpy(path,Standard_Apps_path);
		strcpy(filename,Install_Applications_file);
	}
	else
	{
		fprintf(stdout,"Unknown type Requested\n");
		return -1;
	}

	if ( type == FIRMWARE )
		strcpy(filename,Install_Firmwares_file);	
	else if ( type == APPLICATION )
		strcpy(filename,Install_Applications_file);
	else 
	{
		fprintf(stdout,"Unknown type\n");
		return -1;
	}
	fp = fopen(filename,"r");
	if ( fp == NULL )
	{
		fprintf(stdout," Open error = %s\n",filename);
		return -1;
	}

	while( (getline(&line, &len, fp) > 0) )
	{
		if ( strstr(line,path) != NULL )
		{
			if ( line[strlen(line)-1] == '\n' )
				line[strlen(line)-1] = '\0';

			if((strcmp(line,patch_file)) == 0)
			{
				fprintf(stdout,"Found For Deletion\n");
				Found=1;
			}
			else strcpy(patch[PatchCount++],line);
		}
	}
	free(line);
	line=NULL;
	fclose(fp);

	if ( Found == 1 )
	{

		fp = fopen(filename,"w");
		if ( fp == NULL )
		{
			fprintf(stdout," write error = %s\n",filename);
			return -1;
		}
		else 
		{
			for(i=0;i<PatchCount;i++)
				fprintf(fp,"%s\n",patch[i]);
			fclose(fp);
			remove(patch_file);
			fprintf(stdout,"Successfully Deleted From Installation and Delted patch %s \n",patch_file);
		}

	}
	else fprintf(stdout,"Notttttt Found %s",patch_file);

	return -1;
}
int Get_Total_Downloaded_Updates(int type)
{
	FILE *fp=NULL;
	char filename[128];
	char *line=NULL;
	char path[128];
	size_t len=0;
	int Updates=0;
	memset(filename,0,sizeof(memset));
	memset(path,0,sizeof(path));

	if ( type == FIRMWARE )
	{
		strcpy(path,Standard_Firmwares_path);
		strcpy(filename,Install_Firmwares_file);	
	}
	else if ( type == APPLICATION )
	{
		strcpy(path,Standard_Apps_path);
		strcpy(filename,Install_Applications_file);
	}
	else 
	{
		fprintf(stdout,"Unknown type Requested\n");
		return -1;
	}
	fp = fopen(filename,"r");
	if ( fp == NULL )
	{
		fprintf(stdout," Open error = %s\n",filename);
		return -1;
	}

	while( (getline(&line, &len, fp) > 0) )
		if((strstr(line,path)) != NULL)
			Updates++;
	free(line);
	line=NULL;
	fclose(fp);
	return Updates;
}

