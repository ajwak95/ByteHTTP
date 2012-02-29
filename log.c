#include "httpd.h"
void log(int type, char *s1, char *s2, int num)

{

        int fd ;

        char logbuffer[BUFSIZE*2];
        char pidbuffer[10];
        (void)sprintf(pidbuffer,"%d", getpid());


        switch (type) {

        case ERROR: (void)sprintf(logbuffer,"ERROR: %s:%s Errno=%d exiting pid=%d",s1, s2, errno,getpid()); break;

        case SORRY:

                (void)sprintf(logbuffer, "<HTML><BODY><H1>ByteHTTPD Web Server Sorry: %s %s</H1></BODY></HTML>\r\n", s1, s2);

                (void)write(num,logbuffer,strlen(logbuffer));

                (void)sprintf(logbuffer,"SORRY: %s:%s",s1, s2);

                break;

        case LOG: (void)sprintf(logbuffer," INFO: %s:%s:%d",s1, s2,num);

           break;

        }

       /* no checks here, nothing can be done a failure anyway */

       if((fd = open("bytehttpd.log", O_CREAT| O_WRONLY | O_APPEND,0644)) >= 0)

           {

                (void)write(fd,logbuffer,strlen(logbuffer));

                (void)write(fd,"\n",1);

                (void)close(fd);

        }
       if((fd = open("bytehttpd.pid", O_CREAT| O_WRONLY | O_APPEND,0644)) >= 0)
       {
    	   (void)write(fd,pidbuffer,strlen(pidbuffer));
    	   (void)write(fd,"\n",1);
    	   (void)close(fd);
       }
        if(type == ERROR || type == SORRY) exit(3);

}
