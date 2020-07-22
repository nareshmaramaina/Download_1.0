#include<header.h>
extern int FTP_MODE;
int Download_patch(int Type)
{
	char wget_cmd[2121];

	int ret=0;

	char url[2070];

	int count=0;

	char file[32];


	memset(wget_cmd,0x00,sizeof(wget_cmd));
	memset(url,0x00,sizeof(url));
	memset(&patch,0,sizeof(struct Download));

	ret = get_updates_info(Type);
	if ( ret <= 0  )
		return -1;


	for(count=0; count < patch.Total_Updates_count ; count++)
	{

		memset(file,0,sizeof(file));
		if ( Type == 1 )
		{
			chdir("/mnt/sysuser/Firmware-Upgrade/");
			sprintf(file,"patch-%.1f.zip",patch.Update_Version[count]);
			sprintf(url,"%s%s",RHMS_Server.FTP_PATH_FIRMWARE,file);
			fprintf(stdout,"Type = 1, Firmware patch file Downloading..\n");
		}
		else if ( Type == 2 )
		{
			chdir("/mnt/sysuser/App-Upgrade/");
			sprintf(file,"app-%.1f.zip",patch.Update_Version[count]);
			sprintf(url,"%s%s",RHMS_Server.FTP_PATH_APP,file);
			fprintf(stdout,"Type = 2, Application patch file Downloading..\n");
		}
		else
		{
			fprintf(stdout," Type Error \n" );
			return -1;
		}



		ret = wget_download(url,file);

		if ( count == 0 && ret==0 )
		{


			if ( Type == 1)
			{
				fprintf(stdout,"*** UPDATING TO Firmware Version %f ***\n",patch.Update_Version[count]);
				ret = Apply_Firmware_Patch(patch.Update_Version[count]);

			}
			else 
			{
				fprintf(stdout,"*** UPDATING TO Application Version %f ***\n",patch.Update_Version[count]);
				ret =   Apply_Application_Patch(patch.Update_Version[count]);

			}

			if ( ret !=0 )
			{
				fprintf(stderr, "Unable To apply Patch\n");
				return -1;

			}
			printf("Intial Patch Applied\n");

			printf("*********Reboot the Pos for Patch Updation****************\n");
		}
		else if (ret != 0 )
		{
			printf("###############Unable to download patches###########\n");
			remove(file);
			return -1;
		}



	}




	return 0;
}



