
/*     This file is part of SlimHTTPD.

    SlimHTTPD is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SlimHTTPD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SlimHTTPD.  If not, see <http://www.gnu.org/licenses/>.
-------------------
    httpd.h       |
   Alex Conrey    |
   2-18-11        |
-------------------
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BACKLOG 4
#define HTTPD_PORT 80
#define CONFIG_PATH "httpd.conf"
#define DIR_ROOT "/home/alex/ByteHTTPD"
#define HTTPD_ROOT "index.html"
#define HTTPD_PROTOCOL "HTTP/1.1"



#define BUFSIZE 8096
#define ERROR 42
#define SORRY 43
#define LOG   44



void log(int type, char *s1, char *s2, int num);
void web(int fd, int hit);
void status(int type, char *s1, int num);
