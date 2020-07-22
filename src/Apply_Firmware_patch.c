#include<header.h>

int Apply_Firmware_Patch(float patch_toUpdate )
{

	char cmd[128];

	char Patch_File[80];

	FILE *fp=NULL;

	int ret =0;

	memset(Patch_File,0x00,sizeof(Patch_File));

	memset(cmd,0,sizeof(cmd));

	sprintf(Patch_File,"/mnt/sysuser/Firmware-Upgrade/patch-%.1f.zip",patch_toUpdate);

	if ( access(Patch_File,F_OK) != 0 )
	{
		fprintf(stderr,"%s file not found\n",Patch_File);
		return -1;
	}



	if( access("/mnt/sysuser/Firmware-Upgrade/Extract",F_OK) == 0 )	
		system ("rm -rf /mnt/sysuser/Firmware-Upgrade/Extract"); // Removing previous files  

	system("mkdir -p /mnt/sysuser/Firmware-Upgrade/Extract"); // Extract Patch in Particular Folder 

	remove("/root/.Firmware_version");


	sprintf(cmd,"unzip %s -d /mnt/sysuser/Firmware-Upgrade/Extract >> /etc/.RHMS_Patches.info",Patch_File);

	printf("Extracting %s\n",cmd);  

	ret = system(cmd); // Unzipping 

	sleep(2);



	if(ret == 0)
	{
		chdir("/mnt/sysuser/Firmware-Upgrade/Extract");// Changing directory to firmware
		ret = system("md5sum -c patch.md5"); // Md5sum verification

		if( ret == 0)
		{

			ret = system("tar -xvf patch.tar.bz2 >> /etc/.RHMS_Patches.info"); // Extraction for tar
			if ( ret != 0)
			{
				fprintf(stderr," Extration Failed retrying with xvjf flag\n");
				ret = system("tar -xvjf patch.tar.bz2"); // Extraction for tar
			}
			if(ret == 0)
			{

				fp = fopen("/root/.Firmware_version","w");  // Writing version file to /root/version file

				if (fp == NULL)
					return -1;

				fprintf(fp,"%.1f",patch_toUpdate);

				fclose(fp);	

				ret = system("grep -q \"#VISIONTEK#\" /vision/update.sh");

				if ( ret != 0)
					ret = Update_script();


				if ( ret == 0)
				{


					fp = fopen ("/mnt/sysuser/Firmware-Upgrade/download_complete", "w");
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

			}
			else
			{
				fprintf(stderr,"patch.tar.bz2 extraction failed\n");
			}			
		}
		else 
			fprintf(stderr,"patch.md5 Checksum failed\n");
	}

	else 	fprintf(stderr,"Unzip failed %s\n",Patch_File);	

	if( ret != 0 )
		system ("rm -rf /mnt/sysuser/Firmware-Upgrade/Extract"); // Removing previous files  

	remove(Patch_File); // Removing patch file

	return ret;

}


