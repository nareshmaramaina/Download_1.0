#include<header.h>

void Send_Update_singal_to_app()
{
	pid_t pid=0;
	FILE *fp=NULL;

	fp = fopen("/tmp/.App_pid","r");
	if ( fp == NULL )
	{
		fprintf(stderr,"/tmp/.App_pid file not found\n");
		return;
	}
	fscanf(fp,"%d",&pid);

	fclose(fp);

	if ( pid != 0 )
	{
		fprintf(stderr,"App PID number not found in /tmp/.App_pid\n");
		return;
	}

	if ( kill(pid,10) < 0 )
		fprintf(stdout,"Failed to Send signal 10 to PID = %d\n",pid);

	else fprintf(stdout,"Successfully Sent Signal 10 to  PID = %d\n",pid);

	return;
}

