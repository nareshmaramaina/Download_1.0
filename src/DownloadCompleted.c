#include<header.h>
void Create_Firmware_DownloadCompleted_file(char *path,float Version)
{
	char date[48];
	char DownloadCompleteFile[400];
	memset(date,0,sizeof(date));	
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
	Update_Current_Date_with_Time(date);

	fprintf(fp,"Version:%.1f\nDownloaded_DateAndTime=%s\n",Version,date);

	fclose(fp);

	fprintf(stdout,"Version:%.1f\nDownloaded_DateAndTime=%s\n",Version,date);

	return;
}
void Create_Application_DownloadCompleted_file(char *path,float Version)
{
	char date[48];
	char DownloadCompleteFile[500];
	memset(date,0,sizeof(date));
	memset(DownloadCompleteFile,0,sizeof(DownloadCompleteFile));

	sprintf(DownloadCompleteFile,"%s/DownloadCompleted",path);
	FILE *fp=NULL;
	fp = fopen(DownloadCompleteFile,"w");
	if ( fp == NULL)
	{
		fprintf(stderr,"%s Write Error \n",DownloadCompleteFile);
		return;
	}

	Update_Current_Date_with_Time(date);

	fprintf(fp,"Version:%.1f\nDownloaded_DateAndTime=%s\n",Version,date);

	fclose(fp);

	return;
}
