#include <libxml/tree.h>
#include <libxml/parser.h>
//#include <header.h>

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
	cur = cur->xmlChildrenNode;

	while( cur != NULL)
	{
		key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		if ( key )
			fprintf(fp,"%s:%s\n", cur->name, key);
		cur = cur->next;
	}
	xmlFree(key);

	fclose(fp);
	return 0;
}

int Parse_Firmware_response_xml(void)
{
	xmlDocPtr doc;
	xmlNodePtr cur;
	char *docname="/opt/Firmware_response.xml";
	char *Output_file="/etc/Firmware_release";
	doc = xmlParseFile(docname);

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
	remove(Output_file);
	parseStory (doc, cur,Output_file);

	xmlFreeDoc(doc);
	return 0;
}
int Parse_Application_response_xml(void)
{
	xmlDocPtr doc;
	xmlNodePtr cur;
	char *docname="/opt/Application_response.xml";
	char *Output_file="/etc/Application_release";

	doc = xmlParseFile(docname);

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
        cur = cur->xmlChildrenNode;
	remove(Output_file);
        while(cur!=NULL)
        {
                if (!xmlStrcmp(cur->name, (const xmlChar *) "ApplicationStatusResponse"))
			parseStory (doc, cur,Output_file);
                
		cur=cur->next;
        }

	xmlFreeDoc(doc);
	return 0;
}
/*int main()
{
	Parse_Application_response_xml();
	Parse_Firmware_response_xml(); 
	return 0;
}*/
