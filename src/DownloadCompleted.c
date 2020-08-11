#include<header.h>
void Create_Firmware_DownloadCompleted_file(char *path,float Version)
{
	char Date_time[48];
	char DownloadCompleteFile[400];
	memset(Date_time,0,sizeof(Date_time));	
	memset(DownloadCompleteFile,0,sizeof(DownloadCompleteFile));

	sprintf(DownloadCompleteFile,"%s/%.1f_DownloadCompleted",path,Version);
	FILE *fp=NULL;
	fp = fopen(DownloadCompleteFile,"w");
	if ( fp == NULL)
	{
		fprintf(stderr,"%s Write Error \n",DownloadCompleteFile);
		return;
	}
	fprintf(stdout,"%s created Successfully\n",DownloadCompleteFile);
	UpDate_time_Current_Date_with_Time(Date_time);

	fprintf(fp,"Version:%.1f\nDownloaded_DateAndTime=%s\n",Version,Date_time);

	fclose(fp);

	fprintf(stdout,"Version:%.1f\nDownloaded_DateAndTime=%s\n",Version,Date_time);

	return;
}
void Create_Application_DownloadCompleted_file(char *path,float Version)
{
	char Date_time[48];
	char DownloadCompleteFile[500];
	memset(Date_time,0,sizeof(Date_time));
	memset(DownloadCompleteFile,0,sizeof(DownloadCompleteFile));

	sprintf(DownloadCompleteFile,"%s/DownloadCompleted",path);
	FILE *fp=NULL;
	fp = fopen(DownloadCompleteFile,"w");
	if ( fp == NULL)
	{
		fprintf(stderr,"%s Write Error \n",DownloadCompleteFile);
		return;
	}
	fprintf(stdout,"%s created Successfully\n",DownloadCompleteFile);

	UpDate_time_Current_Date_with_Time(Date_time);

	fprintf(fp,"Version:%.1f\nDownloaded_DateAndTime=%s\n",Version,Date_time);

	fclose(fp);

	return;
}
