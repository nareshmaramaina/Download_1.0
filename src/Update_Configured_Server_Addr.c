#include <header.h>
char *Default_RHMS_Server_Address="https://rhms2.visiontek.co.in";
char Server_Addr[512];
int  Update_Configured_Server_Addr()
{
	short int Index=0;
	FILE *fp;
	size_t len=0,sizeofBuffer;
	char *str=NULL; 


	fp = fopen("/etc/.RHMS_Server.config","r");

	if (fp == NULL)
	{
		strcpy(Server_Addr,Default_RHMS_Server_Address);
		fprintf(stderr," /etc/.RHMS_Server.config file not found, Proceeding with default address = %s\n",Server_Addr);
		return -1;
	}

	memset(Server_Addr,0,sizeof(Server_Addr));
	while((getline(&str,&len,fp)) != -1)
	{
		if  (strstr(str,"ServerAddress:") != NULL )
		{
			sizeofBuffer = sizeof(Server_Addr); 
			if( strlen(str+14) > sizeofBuffer ) 
			{
				fprintf(stderr,"Invalid: Server_Addr Length More than %d bytes \n",sizeofBuffer);
				continue;
			}
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
		strcpy(Server_Addr,Default_RHMS_Server_Address);
	}
	for(Index=0;Server_Addr[Index];Index++)
	{
		if(Server_Addr[Index]==' '||Server_Addr[Index]=='\n'||Server_Addr[Index]=='\t')
		{
			fprintf(stdout,"Space/Tab/Newline Found in Server_Addr = %s\n",Server_Addr);
			memmove(Server_Addr+Index,Server_Addr+Index+1,strlen(Server_Addr+Index+1)+1);
			Index--;
		}
	}

	fprintf(stdout," Server Address is %s\n", Server_Addr);
	return 0;
}
