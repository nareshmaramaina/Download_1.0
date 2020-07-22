#include<header.h>

int Apply_Application_Patch(float patch_toUpdate )
{

	char cmd[128];

	char Patch_File[80];

	FILE *fp=NULL;

	int ret =0;

	memset(Patch_File,0x00,sizeof(Patch_File));

	memset(cmd,0,sizeof(cmd));

	sprintf(Patch_File,"/mnt/sysuser/App-Upgrade/app-%.1f.zip",patch_toUpdate);

	if ( access(Patch_File,F_OK) != 0 )
	{
		fprintf(stderr,"%s file not found\n",Patch_File);
		return -1;
	}


	if( access("/mnt/sysuser/App-Upgrade/Extract",F_OK) == 0 )	
		system ("rm -rf /mnt/sysuser/App-Upgrade/Extract"); // Removing previous files  

	system("mkdir -p /mnt/sysuser/App-Upgrade/Extract"); // Extract Patch in Particular Folder 

	remove("/root/.app_version");


	sprintf(cmd,"unzip %s -d /mnt/sysuser/App-Upgrade/Extract >> /etc/.RHMS_Patches.info",Patch_File);

	printf("Extracting %s\n",cmd);  

	ret = system(cmd); // Unzipping 

	sleep(2);

	if(ret == 0)
	{
		chdir("/mnt/sysuser/App-Upgrade/Extract");// Changing directory to firmware
		ret = system("md5sum -c app.md5"); // Md5sum verification

		if( ret == 0)
		{

			ret = system("tar -xvf app.tar.bz2 >> /etc/.RHMS_Patches.info"); // Extraction for tar
			if ( ret != 0)
			{
				fprintf(stderr," Extration Failed retrying with xvjf flag\n");
				ret = system("tar -xvjf app.tar.bz2 >> /etc/.RHMS_Patches.info"); // Extraction for tar
			}
			if(ret == 0)
			{



				fp = fopen("/root/.app_version","w");  // Writing version file to /root/version file

				if (fp == NULL)
					return -1;

				fprintf(fp,"%.1f",patch_toUpdate);

				fclose(fp);	

				fp = fopen ("/mnt/sysuser/App-Upgrade/download_complete", "w");

				if (fp == NULL)
					return -1;				
				fprintf(fp,"Success");

				fclose(fp);
				fp = fopen ("/mnt/sysuser/Firmware-Upgrade/update.sh", "w");
				if (fp == NULL)
					return -1;

				fprintf(fp,"Success");

				fclose(fp);

			}
			else
			{
				fprintf(stderr,"app.tar.bz2 extraction failed\n");
			}			
		}
		else 
			fprintf(stderr,"app.md5 Checksum failed\n");
	}

	else 	fprintf(stderr,"Unzip failed %s\n",Patch_File);	

	if( ret != 0 )
		system ("rm -rf /mnt/sysuser/App-Upgrade/Extract"); // Removing previous files  

	remove(Patch_File); // Removing patch file

	return ret;

}


