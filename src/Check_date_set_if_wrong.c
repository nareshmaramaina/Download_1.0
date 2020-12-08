#include<header.h>

struct net_time
{
	int date;
	char month[10];
	int year;
	char time[50];
	char zone[10];

}net_time_t;

int get_month(char *month)
{
	int i = 0;

	char mon[12][12]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

	for ( i = 0; i < 12 ; i++ )
	{
		if ( strstr(month,mon[i]) != NULL )
		{
			return i;
		}

	}

	return -1;
}

int  curl_date()
{
	FILE *fp  = NULL ;
	struct timeval tv;
	unsigned long t=0;
	struct tm rtc_tm;

	if (system("curl google.co.in -I  | grep Date | cut -d ' ' -f3- > /tmp/.DM_Downloader_gmt_time") == 0)
	{
		fp = fopen("/tmp/.DM_Downloader_gmt_time","r");
		if ( fp == NULL )
		{
			return -1;
		}
		fscanf(fp,"%d %s %d %s %s",&net_time_t.date,net_time_t.month,&net_time_t.year,net_time_t.time,net_time_t.zone);
		fclose(fp);

		if ( strstr(net_time_t.zone,"GMT") == NULL )
		{
			return -1;
		}
		memset(&rtc_tm,0x0,sizeof(rtc_tm));
		sscanf(net_time_t.time,"%d : %d : %d",&rtc_tm.tm_hour,&rtc_tm.tm_min,&rtc_tm.tm_sec);
		rtc_tm.tm_year = net_time_t.year-1900;
		rtc_tm.tm_mon  = get_month(net_time_t.month);
		rtc_tm.tm_mday = net_time_t.date;
		rtc_tm.tm_hour = rtc_tm.tm_hour + 11;    // GMT 
		t = mktime(&rtc_tm);
		tv.tv_sec = t - 19800;  // IST -5.30 hours
		tv.tv_usec = 0;
		settimeofday(&tv, NULL);

		return 0;
	}
	return -1;
}
int Check_date_set_if_wrong(int Set_direct)
{
	int YYYY;
	char Date_time[64];
	Update_Current_Date_with_Time(Date_time);
	sscanf(Date_time,"%04d-",&YYYY);
	if( Set_direct == 1 || YYYY < 2020 ||  YYYY > 2030 )
	{
		fprintf(stdout,"Year = %d, Wrong Date, need to update\n",YYYY);
		if ( system("rdate -s time-a.nist.gov") == 0 || system("rdate -s time-b.nist.gov")  == 0 || system("rdate -s time-c.nist.gov") == 0 || system("rdate -s time-d.nist.gov") == 0 || curl_date() == 0)
		{

			printf("Successfully Updated Date and Time \n"); 
			return 0;
		}

		else
		{

			printf("Failure to Update date and time\n");
			return -1;
		}
	}
	else return 0;

}
