#include "httpd.h"
void getPHP(char *path1, int num)
{

	FILE *fp = popen("/usr/bin/php -f test.php", "r");
	char path[3126];
	static char buffer[sizeof(path)];
	while(fgets(path, sizeof(path)-1,fp) != NULL) {
		strcat(buffer,path);
	}
	strcat(buffer,path1);
	pclose(fp);
	(void)write(num, buffer, strlen(buffer));
}

