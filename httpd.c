//httpd.c derived from the IBM nweb tutorial.
#include "httpd.h"


struct {

        char *ext;

        char *filetype;

}

extensions [] = {

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


int main(int argc, char **argv)

{

        int i, port, pid, listenfd, socketfd, hit;
        size_t length;
        char *str;
        static struct sockaddr_in cli_addr; /* static = initialised to zeros */
        static struct sockaddr_in serv_addr; /* static = initialised to zeros */



        if( argc < 3  || argc > 3 || !strcmp(argv[1], "-?") ) {

                (void)printf("hint: ByteHTTPD Port-Number Top-Directory\n\n" "\tByteHTTPD is a small and very safe mini web server\n" "\tByteHTTPD only servers out file/web pages with extensions namedbelow\n"

        "\t and only from the named directory or its sub-directories.\n"

        "\tThere is no fancy features = safe and secure.\n\n"

        "\tExample: bytehttpd 80 /var/www &\n\n"

        "\tOnly Supports:");

                for(i=0;extensions[i].ext != 0;i++)

                        (void)printf(" %s",extensions[i].ext);



                (void)printf("\n\tNot Supported: URLs including \"..\", Java, Javascript, CGI\n" "\tNot Supported: directories / /etc /bin /lib /tmp /usr /dev     /sbin \n""\tThe end-user of this software assumes all liability. (C) 2012 Alex Conrey. Modified from the original IBM nweb script.\n");

                exit(0);

        }

        if( !strncmp(argv[2],"/"   ,2 ) || !strncmp(argv[2],"/etc", 5 ) ||

            !strncmp(argv[2],"/bin",5 ) || !strncmp(argv[2],"/lib", 5 ) ||

            !strncmp(argv[2],"/tmp",5 ) || !strncmp(argv[2],"/usr", 5 ) ||

            !strncmp(argv[2],"/dev",5 ) || !strncmp(argv[2],"/sbin",6) ){

               (void)printf("ERROR: Bad top directory %s, see nweb -?\n",

                  argv[2]);

               exit(3);

        }

        if(chdir(argv[2]) == -1){

             (void)printf("ERROR: Can't Change to directory %s\n",argv[2]);

             exit(4);

        }



        /* Become deamon + unstopable and no zombies children

           (= no wait()) */

        if(fork() != 0)

                return 0; /* parent returns OK to shell */

        (void)signal(SIGCLD, SIG_IGN); /* ignore child death */

        (void)signal(SIGHUP, SIG_IGN); /* ignore terminal hangups */

        for(i=0;i<32;i++)

                (void)close(i);      /* close open files */

        (void)setpgrp();             /* break away from process group */



        log(LOG,"nweb starting",argv[1],getpid());

        /* setup the network socket */

        if((listenfd = socket(AF_INET, SOCK_STREAM,0)) <0)

                log(ERROR, "system call","socket",0);

        port = atoi(argv[1]);

        if(port < 0 || port >60000)

                log(ERROR,"Invalid port number (try 1->60000)",argv[1],0);

        serv_addr.sin_family = AF_INET;

        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        serv_addr.sin_port = htons(port);

        if(bind(listenfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr))

           <0)

                log(ERROR,"system call","bind",0);

        if( listen(listenfd,64) <0)

                log(ERROR,"system call","listen",0);



        for(hit=1; ;hit++) {

                length = sizeof(cli_addr);

                if((socketfd = accept(listenfd, (struct sockaddr *)

                   &cli_addr, &length)) < 0)

                        log(ERROR,"system call","accept",0);



                if((pid = fork()) < 0) {

                        log(ERROR,"system call","fork",0);

                }

                else {

                        if(pid == 0) {  /* child */

                                (void)close(listenfd);

                                web(socketfd,hit); /* never returns */

                        } else {        /* parent */

                                (void)close(socketfd);

                        }

                }

        }

}

