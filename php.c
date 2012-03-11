#include "httpd.h"
void getPHP(char *path1, int num)
{
	static char buf2[256];
	sprintf(buf2, "/usr/bin/php -f .");
	strcat(buf2, path1);
	//log(LOG,"PHP",test,1);
	FILE *fp = popen(buf2, "r");

	char path[3126];
	static char buffer[sizeof(path)];
	while(fgets(path, sizeof(path)-1,fp) != NULL) {
		strcat(buffer,path);
	}
	//strcat(buffer,path1);
	pclose(fp);
	(void)write(num, buffer, strlen(buffer));
}

