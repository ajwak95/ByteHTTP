#include "httpd.h"

/* this is a child web server process, so we can exit on errors */

struct {

        char *ext;

        char *filetype;

}

extensions1 [] = {

        {"gif", "image/gif" },

        {"jpg", "image/jpeg"},

        {"jpeg","image/jpeg"},

        {"png", "image/png" },

        {"zip", "image/zip" },

        {"gz",  "image/gz"  },

        {"tar", "image/tar" },

        {"htm", "text/html" },

        {"html","text/html" },

        {0,0} };
void web(int fd, int hit)

{

        int j, file_fd, buflen, len;

        long i, ret;

        char * fstr;

        static char buffer[BUFSIZE+1]; /* static so zero filled */



        ret =read(fd,buffer,BUFSIZE);   /* read Web request in one go */

        if(ret == 0 || ret == -1) {     /* read failure stop now */

             log(SORRY,"failed to read browser request","",fd);

        }

        if(ret > 0 && ret < BUFSIZE)    /* return code is valid chars */

                buffer[ret]=0;          /* terminate the buffer */

        else buffer[0]=0;



        for(i=0;i<ret;i++)      /* remove CF and LF characters */

                if(buffer[i] == '\r' || buffer[i] == '\n')

                        buffer[i]='*';

        log(LOG,"request",buffer,hit);



        if( strncmp(buffer,"GET ",4) && strncmp(buffer,"get ",4) )

                log(SORRY,"Only simple GET operation supported",buffer,fd);



        for(i=4;i<BUFSIZE;i++) { /* null terminate after the second space

           to ignore extra stuff */

                if(buffer[i] == ' ') { /* string is "GET URL " +lots of

                   other stuff */

                        buffer[i] = 0;

                        break;

                }

        }



        for(j=0;j<i-1;j++)      /* check for illegal parent directory use  .. */

                if(buffer[j] == '.' && buffer[j+1] == '.')

                        log(SORRY,"Parent directory (..) path names not supported",buffer,fd);



        if( !strncmp(&buffer[0],"GET /\0",6) || !strncmp(&buffer[0],

          "get /\0",6) ) /* convert no filename to index file */

                (void)strcpy(buffer,"GET /index.html");



        /* work out the file type and check we support it */

        buflen=strlen(buffer);

        fstr = (char *)0;

        for(i=0;extensions1[i].ext != 0;i++) {

              len = strlen(extensions1[i].ext);

              if( !strncmp(&buffer[buflen-len], extensions1[i].ext, len)) {

                        fstr =extensions1[i].filetype;

                        break;

                }

        }

        if(fstr == 0) log(SORRY,"file extension type not supported",

           buffer,fd);



        if(( file_fd = open(&buffer[5],O_RDONLY)) == -1) /* open the file for reading */
        {
        	 //log(SORRY, "failed to open file",&buffer[5],fd);
        	 status(404, &buffer[5], fd);

        }




        log(LOG,"SEND",&buffer[5],hit);



        if(!(file_fd = open(&buffer[5],O_RDONLY)) == -1)
        {
        	(void)sprintf(buffer,"HTTP/1.0 200 OK\r\nContent-Type: %s\r\n\r\n", fstr);

        (void)write(fd,buffer,strlen(buffer));
        }


        /* send file in 8KB block - last block may be smaller */

        while ( (ret = read(file_fd, buffer, BUFSIZE)) > 0 ) {

                (void)write(fd,buffer,ret);

        }

#ifdef LINUX

        sleep(1);       /* to allow socket to drain */

#endif

        exit(1);

}
