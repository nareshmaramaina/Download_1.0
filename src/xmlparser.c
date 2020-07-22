#include<header.h>

int parseStory (xmlDocPtr doc, xmlNodePtr cur) 
{
	xmlChar *key;
	cur = cur->xmlChildrenNode;
	FILE *fp;
	fp=fopen("/etc/rtm_response","w+");
	if(fp==NULL)
	{
		fprintf(stderr,"File Not Created\n");	
		return -1;
	}



	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"RTMServerIP"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

			fprintf(fp,"RTMServerIP:%s\n", key);
			if(key)
				strcpy(RHMS_Server.RTMServerIP,(char*)key);

			xmlFree(key);
		}
		else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"PortNo"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			fprintf(fp,"PortNo:%s\n", key);
//			if(key)
//				strcpy(RHMS_Server.PortNo,(char*)key);
			xmlFree(key);
		}

		else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"UserID"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			fprintf(fp,"UserID:%s\n", key);
			xmlFree(key);
		}
		else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"Password"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			fprintf(fp,"Password:%s\n", key);
			xmlFree(key);
		}

		else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"FTP_PATH_APP"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			fprintf(fp,"FTP_PATH_APP:%s\n", key);
			if(key)
				strcpy(RHMS_Server.FTP_PATH_APP,(char*)key);
			xmlFree(key);
		}

		else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"ApplicationImageName"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			fprintf(fp,"ApplicationImageName:%s\n", key);
			if(key)
				strcpy(RHMS_Server.ApplicationImageName,(char*)key);
			xmlFree(key);
		}




		else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"ApplicationVersion"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			fprintf(fp,"ApplicationVersion:%s\n", key);
			if(key)
				RHMS_Server.ApplicationVersion = atof((char*)key);
			xmlFree(key);
		}


		else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"FTP_PATH_FIRMWARE"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			fprintf(fp,"FTP_PATH_FIRMWARE:%s\n", key);
			if(key)
				strcpy(RHMS_Server.FTP_PATH_FIRMWARE,(char*)key);
			xmlFree(key);
		}


		else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"FirmWarePatchName"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			fprintf(fp,"FirmWarePatchName:%s\n", key);
			xmlFree(key);
		}

		else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"FirmWarePatchVersion"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			fprintf(fp,"FirmWarePatchVersion:%s\n", key);
			if(key)
				RHMS_Server.FirmWarePatchVersion = atof((char*)key);
			xmlFree(key);
		}

		else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"ProjectName")))
		{
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			fprintf(fp,"ProjectName:%s\n", key);
			if(key)
				strcpy(RHMS_Server.ProjectName,(char*)key);
			xmlFree(key);
		}
		else  if ((!xmlStrcmp(cur->name, (const xmlChar *)"health_updation")))
		{
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			fprintf(fp,"health_updation:%s\n", key);
			xmlFree(key);
		}
		cur = cur->next;
	}

	fclose(fp);
	return 0;
}
static int parseDoc(char *docname) {
	xmlDocPtr doc;
	xmlNodePtr cur;



	doc = xmlParseFile(docname);


	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return -1;
	}
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return -1;
	}
	if (xmlStrcmp(cur->name, (const xmlChar *) "GeneralSettings")) {
		fprintf(stderr,"document of the wrong type, root node != story");
		xmlFreeDoc(doc);
		return -1;
	}
	parseStory (doc, cur);
	xmlFreeDoc(doc);
	return 0;
}

int Parsing_ACK_from_Server() 
{
	int ret=0;
	ret=parseDoc(RHMS_Response_file);
	return ret;
}
