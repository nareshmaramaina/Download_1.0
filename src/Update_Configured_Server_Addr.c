#include <header.h>
char Server_Addr[512]="https://rhms2.callippus.co.uk";
int  Update_Configured_Server_Addr()
{
	FILE *fp;
	size_t len=0;
	char *str=NULL; 


	fp = fopen("/etc/.RHMS_Server.config","r");

	if (fp == NULL)
	{
		fprintf(stderr," /etc/.RHMS_Server.config file not found, Proceeding with default address = %s\n",Server_Addr);
		return -1;
	}

	memset(Server_Addr,0,sizeof(Server_Addr));
	while((getline(&str,&len,fp)) != -1)
	{
		if  (strstr(str,"ServerAddress:") != NULL )
		{
			fprintf(stdout," Updated Configured Server Address\n");
			strcpy(Server_Addr,str+14);
			break;
		}
	}
	free(str);
	str=NULL;	
	fclose(fp);

	if(Server_Addr[strlen(Server_Addr)-1] == '\n')
		Server_Addr[strlen(Server_Addr)-1]='\0';

	if ( strlen(Server_Addr) < 12 )
	{
		fprintf(stdout,"Invalid Configured server Address in  /etc/.RHMS_Server.config\n");
		strcpy(Server_Addr,"https://rhms2.callippus.co.uk");
	}
	fprintf(stdout," Server Address is %s\n", Server_Addr);
	return 0;
}
