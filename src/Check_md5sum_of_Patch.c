#include<header.h>
extern char *Standard_Firmwares_path;
extern char *Standard_Apps_path;
int Check_md5sum_Firmware_patch(char *FirmwarePatchFile)
{
	char cmd[500];
	int ret =0;
	char ExtractPath[430];
	char RemoveExtractPath[440];
	char FirmwareName[128];
	float Version;
	char ProjectName[128];

	memset(ProjectName,0,sizeof(ProjectName));
	memset(FirmwareName,0,sizeof(FirmwareName));
	ret = Get_Tokens_of_FirmwarePatchfile(FirmwarePatchFile,FirmwareName,&Version,ProjectName);

	printf(" FirmwareName = %s  Version= %.1f\n" ,FirmwareName,Version);
	if( ret  != 0 )
		return -1;

	if ( access(FirmwarePatchFile,F_OK) != 0 )
	{
		fprintf(stderr,"%s file not found\n",FirmwarePatchFile);
		return -1;
	}

	memset(ExtractPath,0,sizeof(ExtractPath));

	sprintf(ExtractPath,"%s/%s/%s/Extract",Standard_Firmwares_path,ProjectName,FirmwareName);
	sprintf(RemoveExtractPath,"rm -rf %s",ExtractPath);	


	if( access(ExtractPath,F_OK) == 0 )
		system (RemoveExtractPath); // Removing previous files  

	memset(cmd,0,sizeof(cmd));
	sprintf(cmd,"mkdir -p %s",ExtractPath);
	system(cmd); // Extract Patch in Particular Folder 

	memset(cmd,0,sizeof(cmd));
        sprintf(cmd,"mkdir -p /etc/vision/RHMS/Firmware/%s/%s",ProjectName,FirmwareName);
        system(cmd); 


	sprintf(cmd,"unzip %s -d %s ",FirmwarePatchFile,ExtractPath);

	printf("Extracting %s\n",cmd);  

	ret = system(cmd); // Unzipping 
	
	if(ret != 0)
	{
		system (RemoveExtractPath); // Removing previous files  
		fprintf(stderr,"Unzip failed %s\n",FirmwarePatchFile);	
		return -1;
	}
	sleep(1);
	chdir(ExtractPath);// Changing directory to firmware
	ret = system("md5sum -c patch.md5"); // Md5sum verification

	if( ret != 0)
	{
		fprintf(stderr,"patch.md5 Checksum failed %s\n",FirmwarePatchFile);
		system (RemoveExtractPath); // Removing previous files  
		return -1;
	}

	system (RemoveExtractPath); // Removing previous files  

	return 0;

}
int Check_md5sum_Application_patch(char *ApplicationPatchFile)
{
	char cmd[1600];
	int ret =0;
	char ExtractPath[530];
	char RemoveExtractPath[540];
	char ApplicationType[128];
	char ApplicationName[128];
	float Version;
	char ProjectName[128];
	memset(ApplicationType,0,sizeof(ApplicationType));
	memset(ApplicationName,0,sizeof(ApplicationName));
	memset(ProjectName,0,sizeof(ProjectName));

	ret = Get_Tokens_of_ApplicationPatchfile(ApplicationPatchFile,ApplicationType,ApplicationName,&Version,ProjectName);
	printf(" ApplicationType = %s ApplicationName = %s Version= %.1f ProjectName = %s\n" ,ApplicationType,ApplicationName,Version,ProjectName);

	if( ret  != 0 )
		return -1;

	memset(ExtractPath,0,sizeof(ExtractPath));

	sprintf(ExtractPath,"%s/%s/%s/%s/Extract",Standard_Apps_path,ProjectName,ApplicationType,ApplicationName);
	sprintf(RemoveExtractPath,"rm -rf %s",ExtractPath);	

	if ( access(ApplicationPatchFile,F_OK) != 0 )
	{
		fprintf(stderr,"%s file not found\n",ApplicationPatchFile);
		return -1;
	}


	if( access(ExtractPath,F_OK) == 0 )
		system (RemoveExtractPath); // Removing previous files  

	memset(cmd,0,sizeof(cmd));
	sprintf(cmd,"mkdir -p %s",ExtractPath);
	system(cmd); // Extract Patch in Particular Folder 

	memset(cmd,0,sizeof(cmd));
	sprintf(cmd,"mkdir -p /etc/vision/RHMS/Apps/%s/%s/%s/",ProjectName,ApplicationType,ApplicationName);
	system(cmd); 


	memset(cmd,0,sizeof(cmd));

	sprintf(cmd,"unzip %s -d %s ",ApplicationPatchFile,ExtractPath);

	printf("Extracting %s\n",cmd);  

	ret = system(cmd); // Unzipping 

	if(ret != 0)
	{
		system (RemoveExtractPath); // Removing previous files  
		fprintf(stderr,"Unzip failed %s\n",ApplicationPatchFile);	
		return -1;
	}
	sleep(1);
	chdir(ExtractPath);// Changing directory to firmware
	ret = system("md5sum -c app.md5"); // Md5sum verification

	if( ret != 0)
	{
		system (RemoveExtractPath); // Removing previous files  
		fprintf(stderr,"app.md5 Checksum failed %s\n",ApplicationPatchFile);
		return -1;
	}
	system (RemoveExtractPath); // Removing previous files  
	return 0;
}

int Check_md5sum_of_Patch(char *PatchFile )
{
	int ret=0;

	if ( strstr(PatchFile,Standard_Firmwares_path)  != NULL )
	{
		fprintf(stdout,"Checking Firmware Patch md5sum\n");
		ret = Check_md5sum_Firmware_patch(PatchFile);
	}

	else if ( strstr(PatchFile,Standard_Apps_path) != NULL )
	{
		fprintf(stdout,"Checking Application Patch md5sum\n");
		ret = Check_md5sum_Application_patch(PatchFile);
	}

	else 
		fprintf(stdout,"unknown Patch \n");
	return ret;
}
