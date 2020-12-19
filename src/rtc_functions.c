#include <header.h>


void Get_Current_Date(char *Date)
{
	struct tm *Today=NULL;
	struct timeval tv;

	gettimeofday (&tv,NULL);

	Today = localtime (&tv.tv_sec) ;

	sprintf(Date,"%02d%02d%04d",Today->tm_mday,Today->tm_mon+1,Today->tm_year+1900);

	fprintf(stdout,"Today Date (DDMMYYYY): %s \n",Date);

	return;
}
static int diff_bw_two_dates(int firstDate, int firstMonth, int firstYear, int secondDate, int secondMonth, int secondYear)
{
	int FirstNoOfDays=0;
	int SecondNoOfDays=0;

	firstMonth = (firstMonth + 9) % 12;
	firstYear = firstYear - firstMonth / 10;
	FirstNoOfDays = 365 * firstYear + firstYear/4 - firstYear/100 + firstYear/400 + (firstMonth * 306 + 5) /10 + ( firstDate - 1 );

	secondMonth = (secondMonth + 9) % 12;
	secondYear = secondYear - secondMonth / 10;
	SecondNoOfDays = 365 * secondYear + secondYear/4 - secondYear/100 + secondYear/400 + (secondMonth * 306 + 5) /10 + ( secondDate - 1 );

	return FirstNoOfDays - SecondNoOfDays; /* uses absolute if the first date is smaller so it wont give negative number */
}

int Get_Difference_Days_of_Today_with_Last_updated_day (char *filename,int my_date,int my_month,int my_year)
{
	FILE *fp;

	char str[80]="";

	int no_days=0;

	int pre_date=0,pre_month=0,pre_year=0;

	fp = fopen(filename,"r");
	if (fp == NULL)
	{
		fprintf(stderr,"%s File not Found\n",filename);
		return -1;
	}

	memset(str,0x00,80);

	fread(str,sizeof(str),1,fp);

	fclose(fp);

	if (strlen(str) != 13)
	{
		fprintf(stderr,"Invalid date format\n");
		return -1;
	}

	sscanf(str+5,"%02d%02d%4d",&pre_date,&pre_month,&pre_year);

	no_days = diff_bw_two_dates(my_date,my_month,my_year, pre_date, pre_month,pre_year);

	fprintf(stdout,"Diffrence between Last Success Date and Today  Date is %d days\n",no_days);

	return no_days;

}

//int Check_RHMS_All_requests_run(int *Hardware_run,int *BootTime_run,int *Periodic_run)

int Check_7days_Validity(char *filename)
{
	char CurrentDate[15]="";

	int CurrentDay=0, CurrentMonth=0, CurrentYear=0,no_days;	

	char Download_Started_Day_filename[550];

	FILE *fp =NULL;

	memset(Download_Started_Day_filename,0,sizeof(Download_Started_Day_filename));

	sprintf("%s.Download_Started_Day",filename);



	Get_Current_Date(CurrentDate);

	if ( access(filename,F_OK) == 0 && access(Download_Started_Day_filename, F_OK) == 0 )
	{

		sscanf(CurrentDate,"%02d%02d%4d",&CurrentDay, &CurrentMonth, &CurrentYear);

		if ( CurrentYear < 2020 )
		{
			fprintf(stderr,"Date Wrong set - Year Error\n");
			return -1;

		}
		no_days =	Get_Difference_Days_of_Today_with_Last_updated_day (Download_Started_Day_filename,CurrentDay, CurrentMonth, CurrentYear);
		if(no_days >= 7)
			return 0;

		else if (no_days == 0)
			return -1;

		else if( no_days < 0)
		{
			fprintf(stdout,"Date Error %d days forwarded\n",-(no_days));
			return -1;
		}

	}
	else
	{
		fp = fopen(Download_Started_Day_filename,"w");

		if(fp == NULL)
		{
			fprintf(stderr,"Date  File Not created\n");
			return -1;
		}

		fprintf(fp,"Date:%s",CurrentDate);

		fclose(fp);


	}

	return -1;
}
