#include <libxml/tree.h>
#include <libxml/parser.h>
#include <header.h>
char *Server_Firmware_release_file="/etc/vision/RHMS/Firmware/ServerFirmwareRelease.info";
char *Server_Application_release_file="/etc/vision/RHMS/Apps/ServerApplicationsRelease.info";
extern char *Application_response_file;
extern char *Firmware_response_file;

int parseStory (xmlDocPtr doc, xmlNodePtr cur,char *output_file)
{
	xmlChar *key;
	FILE *fp;

	fp=fopen(output_file,"a");

	if(fp == NULL)
	{
		fprintf(stderr,"Error: %s  Not Created\n",output_file);
		return -1;
	}

	for(cur = cur->xmlChildrenNode; cur != NULL; cur = cur->next)
	{
		key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		if ( key )
			fprintf(fp,"%s:%s\n", cur->name, key);
	}
	xmlFree(key);

	fclose(fp);
	return 0;
}

int Parse_Firmware_response_xml(void)
{
	xmlDocPtr doc;
	xmlNodePtr cur;
	xmlChar *key;
	FILE *fp=NULL;
	
	doc = xmlParseFile(Firmware_response_file);
	if (doc == NULL ) 
		return -1;
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) 
	{
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return -1;
	}
	if (xmlStrcmp(cur->name, (const xmlChar *) "FirmwareStatusResponse")) {
		fprintf(stderr,"document of the wrong type, root node != story");
		xmlFreeDoc(doc);
		return -1;
	}
	
	system("mkdir -p /etc/vision/RHMS/Firmware/");	
	
	fp=fopen(Server_Firmware_release_file,"w");

	if(fp == NULL)
	{
		fprintf(stderr,"Error: %s  Not Created\n",Server_Firmware_release_file);
		return -1;
	}
	for(cur = cur->xmlChildrenNode; cur != NULL; cur = cur->next)
	{
		if (!xmlStrcmp(cur->name, (const xmlChar *) "Dependencies"))
		{
			fclose(fp);
			for(cur = cur->xmlChildrenNode; cur != NULL; cur = cur->next)
			{
				if (!xmlStrcmp(cur->name, (const xmlChar *) "Dependency"))
					parseStory (doc, cur,Server_Firmware_release_file);
			}
			break;
		}
		key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		if ( key )
			fprintf(fp,"%s:%s\n", cur->name, key);

	}
	fclose(fp);
	fprintf(stdout,"Extracted Successfully in %s\n",Server_Firmware_release_file);

	xmlFreeDoc(doc);
	return 0;
}
int Parse_Application_response_xml(void)
{
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(Application_response_file);

	if (doc == NULL ) 
		return -1;
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) 
	{
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return -1;
	}
	if (xmlStrcmp(cur->name, (const xmlChar *) "ArrayOfApplicationStatusResponse")) {
		fprintf(stderr,"document of the wrong type, root node != story");
		xmlFreeDoc(doc);
		return -1;
	}

	system("mkdir -p /etc/vision/RHMS/Apps/");	
	remove(Server_Application_release_file);
	for(cur = cur->xmlChildrenNode; cur != NULL; cur = cur->next)
	{
		if (!xmlStrcmp(cur->name, (const xmlChar *) "ApplicationStatusResponse"))
			parseStory (doc, cur,Server_Application_release_file);

	}
	fprintf(stdout,"Extracted Successfully in %s\n",Server_Application_release_file);

	xmlFreeDoc(doc);
	return 0;
}
/*int main()
  {
  Parse_Application_response_xml();
  Parse_Firmware_response_xml(); 
  return 0;
  }*/
