#include<header.h>
int Add_to_installation(char *patch,int type)
{
	FILE *fp=NULL;
	char filename[128];

	memset(filename,0,sizeof(memset));

	if ( type == 1 )
		strcpy(filename,"/mnt/sysuser/Software-Upgrade/Firmware_Downloads/Install_Firmwares");	
	else if ( type == 2 )
		strcpy(filename,"/mnt/sysuser/Software-Upgrade/Applications_Downloads/Install_Applications");
	else 
	{
		fprintf(stdout,"Unknown type\n");
		return -1;
	}
	fp = fopen(filename,"a");
	if ( fp == NULL )
	{
		fprintf(stdout," Open error = %s\n",filename);
		return -1;
	}

	else
	{	
		fprintf(fp,"%s\n",patch);
		fclose(fp);
		fprintf(stdout,"Ready for installation = %s\n",patch);

	}
	return 0;
}
