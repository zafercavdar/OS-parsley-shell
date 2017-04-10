/**
 * myshell interface program
	COMP 304 Spring 2017 Project-1
	Implemented by:
	Zafer Cavdar - 49995
	Cisem Altan - 49824
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>
#include <limits.h>

#define MAX_LINE       80 /* 80 chars per line, per command, should be enough. */

int parseCommand(char inputBuffer[], char *args[],int *background);
void sighandler(int);
int createBookmark(char inputBuffer[], char bookmarks[][80], int *a);
void displayBookmarks(char bookmarks[][80], int a);
void execBookmark(char bookmarks[][80], int b, char *args[],int *background);

int main(void)
{
  char inputBuffer[MAX_LINE]; 	        /* buffer to hold the command entered */
  int background;             	        /* equals 1 if a command is followed by '&' */
  char *args[MAX_LINE/2 + 1];	        /* command line (of 80) has max of 40 arguments */
  pid_t child;            		/* process id of the child process */
  int status;           		/* result from execv system call*/
  int shouldrun = 1;
  char path[100];
  char path2[100];
  int histIndex;
  int execute = 1;
  char history[100][MAX_LINE][MAX_LINE];
  int i, upper,j,counter,k,a;
  int indicator = 0;
  int histSize[100];
  int hour, minute;
  char mp3[40];
  char file1addr[MAX_LINE];
  char file2addr[MAX_LINE];
  DIR* dirp;
  DIR* dirp2;
  struct dirent * dp;
  struct dirent * dp2;
  char full_path[MAX_LINE];
  char full_path2[MAX_LINE];
  char destpath[MAX_LINE];
  char *token, *back_token;
  char task[100];
  FILE *file;
  char bookmarks[10][MAX_LINE]; //array that will store bookmarks
  int track=0; //to keep the track of bookmarks

  while (shouldrun){            		/* Program terminates normally inside setup */
    background = 0;

    shouldrun = parseCommand(inputBuffer,args,&background);       /* get next command */

    // update history
    if (strcmp(args[0],"history") != 0 && strcmp(args[0],"!!") != 0 && args[0][0] != '!'){
      for (a=0; a < sizeof(args); a++){
        if (args[a] == NULL){
          histSize[indicator] = a;
          break;
        }
        strcpy(history[indicator][a],args[a]);
        strcat(history[indicator][a],"\0");
      }
      indicator = indicator + 1;
    }

    if (strncmp(inputBuffer, "exit", 4) == 0)
      shouldrun = 0;     /* Exiting from myshell*/

    if (shouldrun) {

      child = fork();

	     if (child == 0){ // child process
         pid_t helper;
         //printf("indicator: %d \n", indicator);
         execute = 1;
         counter = 10;

         if (strcmp(args[0],"history") == 0){
                execute = 0;
                if (indicator >= 10){
                  for (j = indicator-1 ; counter != 0; j--){
                      printf("%d ",counter);
                      counter -= 1;
                      for (k = 0; k < histSize[j]; k++){
                            printf("%s ", history[j][k]);
                      }
                      printf("\n");
                  }
                }
                else{
                  for (j = indicator-1; j >= 0; j--){
                      printf("%d ", (j+1));
                      for (k = 0; k < histSize[j]; k++){
                          printf("%s ", history[j][k]);
                      }
                      printf("\n");
                  }
                }
          }

          if (strcmp(args[0],"!!") == 0){
              if (indicator > 0){
                if (strcmp(history[indicator-1][0],"muzik") == 0 ||
                    strcmp(history[indicator-1][0],"bookmark") == 0 ||
                    strcmp(history[indicator-1][0],"codesearch") == 0 ||
                    strcmp(history[indicator-1][0],"fcdsame") == 0 ||
                    strcmp(history[indicator-1][0],"processInfo") == 0){
                      execute = 0;
                    }
                    else {
                      execute = 1;

                    }
                    for (i= 0; i <MAX_LINE/2 + 1; i++){
                        args[i] = NULL;
                    }
                    for (k = 0; k < histSize[indicator-1]; k++){
                        args[k] = history[indicator-1][k];
                    }

              } else {
                execute = 0;
                printf("error, history is empty!\n");
              }
          }

          if (args[0][0] == '!'){
              histIndex = args[0][1] - '0';
              //printf("histIndex %d\n",histIndex);
              if (histIndex > indicator || histIndex < 1){
                  execute = 0;

                  printf("error, history does not have this amount of command or your index could not be recognized!\n");
              } else {
                if (strcmp(history[histIndex-1][0],"muzik") == 0 ||
                    strcmp(history[histIndex-1][0],"bookmark") == 0 ||
                    strcmp(history[histIndex-1][0],"codesearch") == 0 ||
                    strcmp(history[histIndex-1][0],"fcdsame") == 0 ||
                    strcmp(history[indicator-1][0],"processInfo") == 0){
                      execute = 0;
                    }
                  else{
                    execute = 1;

                  }
                  for (i= 0; i <MAX_LINE/2 + 1; i++){
                      args[i] = NULL;
                  }
                  for (k = 0; k < histSize[histIndex-1]; k++){
                    args[k] = history[histIndex-1][k];
                  }
              }
          }

          if(strcmp(args[0],"bookmark") == 0){

            execute=0;
            if(strcmp(args[1],"-i")!=0 && strcmp(args[1],"-l")!=0 && strcmp(args[1],"-d")!=0){
              createBookmark(inputBuffer, bookmarks, &track);
            }

            if(strcmp(args[1],"-l")==0){
              displayBookmarks(bookmarks, track);
            }

            if(strcmp(args[1],"-d")==0){
              char sayi[1];
              strcpy(sayi, args[2]);
              int g=atoi(sayi);      //get the integer from the user
                if (g==(track-1)) {
                  bookmarks[g][0]='\0';
                } else {
                  while(bookmarks[g+1][0]!='\0'){
                    strcpy(bookmarks[g],bookmarks[g+1]);
                    g++;
                  }
                }
              track--;
          }

          if(strcmp(args[1],"-i")==0){
            char sayi[1];
            strcpy(sayi, args[2]);
            int b=atoi(sayi);     //get the integer from the user
            execBookmark (bookmarks, b, args, &background);
            execute=1;
          }
        }

        if(strcmp(args[0], "codesearch")==0) {
          execute=0;
          char name[256][256];
          DIR *d;
          struct dirent *dir;
          int count = 0;
          int index = 0;
          int recursive=0;
          int i;

          d = opendir(".");
          if (d)
          {
            while ((dir = readdir(d)) != NULL)
            {
              strcpy(name[count],dir->d_name);
              count++;
            }

            closedir(d);
          }

          if(strcmp(args[1], "-r") == 0){
            recursive=1;
          }

          while( count > 0 ){
             if((strstr(name[index], ".")==NULL) || (strstr(name[index], ".c")!=NULL) ||  (strstr(name[index], ".C")!=NULL) || (strstr(name[index], ".h")!=NULL) || (strstr(name[index], ".H")!=NULL) || (strstr(name[index], ".cpp")!=NULL) || (strstr(name[index], ".c++")!=NULL)){

              pid_t mychild; //since it calls execv more than one time(looping), it has its own child
              mychild=fork();

              if(mychild==0){
                char newstr5[30];
                char newstr4[30];
                char newstr3[30];
                char newstr2[30];

                for(int k=0; k<30; k++){
                  newstr3[k]='\0';
                }

                char path1[30];
                strcpy(path1, "/bin/grep");
                strcpy(newstr3, "grep\0");
                char hn[6];
                strcpy(newstr2, name[index]); //directory to be searched, last args
                strcpy(newstr4, "--binary-files=without-match");
                strcpy(args[0], newstr3);


                if(recursive==1){          //-r is entered
                  if(args[3]==NULL){       // it is a keyword
                    strcpy(hn,"-Hrn");
                    strcpy(newstr3, args[2]);
                    int w=0;
                    for(int k=0; k<30; k++){
                      if(newstr3[k]!='\0' && newstr3[k]!='\"'){ //get rid of quotes
                        newstr5[w]=newstr3[k];
                        w++;
                      }
                    }

                    args[1]=&hn[0];
                    args[2]=&newstr4[0];
                    args[3]=&newstr5[0];
                    args[4]=&newstr2[0];

                  } else  {                //it is a phrase
                    strcpy(hn,"-Hsrn");
                    int w=2;
                    char sentence[100];
                    char sentence2[100];
                    for(int k=0; k<100; k++){
                      sentence[k]='\0';
                    }
                    for(int k=0; k<100; k++){
                      sentence2[k]='\0';
                    }
                    while(args[w]!=NULL){  //merge the args to create string to be searched
                      char temp[80];
                      strcpy(temp, args[w]);
                      sprintf(sentence, "%s %s", sentence, temp);
                        w++;
                      }

                    int z=0;
                      for(int k=0; k<100; k++){
                        if(sentence[k]!='\0' && sentence[k]!='\"'){ //get rid of quotes
                          sentence2[z]=sentence[k];
                          z++;
                        }
                      }

                    args[1]=&hn[0];
                    args[2]=&newstr4[0];
                    args[3]=&sentence2[0];
                    args[4]=&newstr2[0];

                  }
                } else {                   // -r is not entered
                     if(args[2]==NULL){    // it is a keyword
                       strcpy(hn,"-Hsn\0");
                       strcpy(newstr3, args[1]);
                       int c=0;
                       for(int k=0; k<30; k++){
                         if(newstr3[k]!='\0' && newstr3[k]!='\"'){ //get rid of quotes
                           newstr5[c]=newstr3[k];
                           c++;
                         }
                       }
                       args[1]=&hn[0];
                       args[2]=&newstr4[0];
                       args[3]=&newstr5[0];
                       args[4]=&newstr2[0];
                     } else {              // it is a phrase
                       strcpy(hn,"-Hsn\0");
                       int c=1;
                       char sentence[100];
                       char sentence2[100];
                       for(int k=0; k<100; k++){ //initialize
                         sentence[k]='\0';
                       }
                       for(int k=0; k<100; k++){ //initialize
                         sentence2[k]='\0';
                       }

                         while(args[c]!=NULL){ //merge the args to create string to be searched
                           char temp[80];
                           strcpy(temp, args[c]);
                           sprintf(sentence, "%s %s", sentence, temp);
                           c++;
                         }

                       int z=0;
                         for(int k=0; k<100; k++){ //get rid of quotes
                           if(sentence[k]!='\0' && sentence[k]!='\"'){
                             sentence2[z]=sentence[k];
                             z++;
                           }
                         }
                       args[1]=&hn[0];
                       args[2]=&newstr4[0];
                       args[3]=&sentence2[0];
                       args[4]=&newstr2[0];
                     }
                }

            status=execv(path1, args);

          } else {
            wait(NULL);
           }
        }
        index++;
        count--;
      }
   }
          if (strcmp(args[0],"processInfo") == 0){
              long lnum;
              char *end;
              int pid, prio;
              int failed = 0;
              char temp[20], temp2[20],temp3[20],temp4[20];
              int z;
              int error = 0;
              execute = 0;


              helper = fork();

              if (helper == 0){

                pid_t helperhelper = fork();

                if(helperhelper == 0){
                    char *cargs[3];
                    cargs[0] = "sudo";
                    cargs[1] = "rmmod";
                    cargs[2] = "processInfo";
                    strcpy(path,"/usr/bin/");
                    strcat(path,cargs[0]);

                    status = execv(path,cargs);
                    if (status < 0){
                      error = 1;
                      printf("sudo rmmod error\n");
                    }
                }
                else{
                  wait(NULL);
                  if (args[1] != NULL && args[2] != NULL){
                      lnum = strtol(args[1],&end,10);
                      if (end == args[1]){
                          fprintf(stderr, "ERROR: can't convert pid string to number\n");
                          failed = 1;
                      } else {
                        pid = (int) lnum;
                      }
                      lnum = strtol(args[2],&end,10);
                      if (end == args[2]){
                          fprintf(stderr, "ERROR: can't convert prio string to number\n");
                          failed = 1;
                      } else {
                        prio = (int) lnum;
                      }

                      if (failed == 0){
                        for (i= 0; i <MAX_LINE/2 + 1; i++){
                            args[i] = NULL;
                        }

                        strcpy(temp,"processID=");
                        sprintf(temp2,"%d",pid);
                        strcat(temp,temp2);
                        args[3] = &temp[0];

                        strcpy(temp3,"processPrio=");
                        sprintf(temp4,"%d",prio);
                        strcat(temp3,temp4);
                        args[4] = &temp3[0];

                        args[0] = "sudo";
                        args[1] = "insmod";
                        args[2] = "processInfo.ko";
                        printf("%s %s %s %s %s\n",args[0],args[1],args[2],args[3],args[4]);

                        strcpy(path,"/usr/bin/");
                        strcat(path,args[0]);

                        status = execv(path,args);
                        if (status < 0){
                          error = 1;
                          printf("sudo insmod error\n");
                        }
                      }

                  } else{
                    error = 1;
                    printf("invalid number of arguments, expected: 2\n");
                  }
                }

              } else {
                wait(NULL);
                if (error == 0){
                    char *dargs[1];
                    dargs[0] = "dmesg";
                    strcpy(path,"/bin/");
                    strcat(path,dargs[0]);

                    status = execv(path,dargs);
                    if (status < 0){
                      error = 1;
                      printf("dmesg error\n");
                    }
                    //printf("now time to execute dmesg\n");
                }
              }

          }

          if (strcmp(args[0],"muzik") == 0){
              execute = 0;

              file = fopen("test","wt");

              if (strcmp(args[1], "-r") == 0){
                fprintf(file,"#START CRON LIST\n");
                fprintf(file,"SHELL=/bin/sh\n");
                fprintf(file,"PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin\n");
                fprintf(file,"#END CRON LIST\n");
                fclose(file);
                for (i= 0; i <MAX_LINE/2 + 1; i++){
                    args[i] = NULL;
                }

                args[0] = "crontab";
                args[1] = "test";

                strcpy(path,"/usr/bin/");
                strcat(path,args[0]);

                status = execv(path,args);
                if (status < 0){
                  printf("crontab run error\n");
                } else {
                  printf("successfully added to the crontab file\n");
                }
              }
              else{
                token = strtok(args[1],".");
                back_token = strtok(NULL, ".");

                strcpy(task,back_token);
                strcat(task," ");
                strcat(task,token);
                //hour = atoi(token);
                //minute = atoi(back_token);
                strcpy(mp3,args[2]);
                realpath(mp3,full_path);
                strcat(task, " * * * play ");
                strcat(task, full_path);
                strcat(task, "\n");
                //printf("%s",task);
                fprintf(file,"#START CRON LIST\n");
                fprintf(file,"SHELL=/bin/sh\n");
                fprintf(file,"PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin\n");
                fprintf(file,"%s",task);
                fprintf(file,"#END CRON LIST\n");
                fclose(file);
                for (i= 0; i <MAX_LINE/2 + 1; i++){
                    args[i] = NULL;
                }

                args[0] = "crontab";
                args[1] = "test";

                strcpy(path,"/usr/bin/");
                strcat(path,args[0]);

                status = execv(path,args);
                if (status < 0){
                  printf("crontab run error\n");
                } else {
                  printf("successfully added to the crontab file\n");
                }
              }
          }

          if (strcmp(args[0],"fcdsame") == 0){
              execute = 0;

              if (args[1] != NULL && args[2] != NULL){
                strcpy(file1addr,args[1]);
                strcpy(file2addr,args[2]);

                dirp = opendir(file1addr);
                dp = readdir(dirp);

                if (dirp != NULL){
                  while((dp = readdir(dirp)) != NULL){
                      //printf("%s ",dp->d_name);
                      dirp2 = opendir(file2addr);
                      dp2 = readdir(dirp2);

                      if (dirp2 != NULL){
                        while((dp2 = readdir(dirp2)) != NULL){
                            //printf("%s\n",dp2->d_name);
                            if (strlen(dp->d_name) == 1 && (dp->d_name)[0] == '.'){
                                continue;
                            }
                            if (strcmp(dp->d_name, dp2->d_name) == 0){
                                realpath(file1addr,full_path);
                                realpath(file2addr,full_path2);

                                strcat(full_path,"/");
                                strcat(full_path2,"/");
                                strcat(full_path,dp->d_name);
                                strcat(full_path2,dp->d_name);
                                strcpy(destpath,"/home/zafer/Desktop/commons/");
                                strcat(destpath,dp->d_name);
                                printf("%s has been deleted from folders and moved to Desktop.\n",dp->d_name);
                                printf("%s\n%s\n",full_path,destpath);

                                rename(full_path,destpath);
                                remove(full_path2);
                                break;
                            }
                        }
                      }
                  }
                } else {
                  printf("directory does not exist\n");
                }
              }
              else {
                printf("missing arguments for fcdsame, expected 2\n");
              }

          }


        if (execute == 1){
          strcpy(path,"/bin/");
          strcpy(path2,"/usr/bin/");
          strcat(path,args[0]);
          strcat(path2,args[0]);


          status = execv(path,args);
          if (status < 0){
            status = execv(path2,args);
          }

          if (status < 0){
            printf("execv error, status is smaller than 0\n");
          }
        }
    }
    else {
	      if (background == 0){
		         wait(NULL);
	      }
    }
      /*
	After reading user input, the steps are
	(1) Fork a child process using fork()
	(2) the child process will invoke execv()
	(3) if command included &, parent will invoke wait()
       */
    }
  }
  return 0;
}

/**
 * The parseCommand function below will not return any value, but it will just: read
 * in the next command line; separate it into distinct arguments (using blanks as
 * delimiters), and set the args array entries to point to the beginning of what
 * will become null-terminated, C-style strings.
 */

int parseCommand(char inputBuffer[], char *args[],int *background)
{
    int length,		/* # of characters in the command line */
      i,		/* loop index for accessing inputBuffer array */
      start,		/* index where beginning of next command parameter is */
      ct,	        /* index of where to place the next parameter into args[] */
      command_number;	/* index of requested command number */
      char ch1, ch2;
    ct = 0;

    /* read what the user enters on the command line */
    do {
  	  printf("parsley:~$  ");
  	  fflush(stdout);
      signal(SIGINT, sighandler);
  	  length = read(STDIN_FILENO,inputBuffer,MAX_LINE);
    }
    while (inputBuffer[0] == '\n');   /* swallow newline characters */


    /**
     *  0 is the system predefined file descriptor for stdin (standard input),
     *  which is the user's screen in this case. inputBuffer by itself is the
     *  same as &inputBuffer[0], i.e. the starting address of where to store
     *  the command that is read, and length holds the number of characters
     *  read in. inputBuffer is not a null terminated C-string.
     */
    start = -1;
    if (length == 0)
      exit(0);            /* ^d was entered, end of user command stream */

    /**
     * the <control><d> signal interrupted the read system call
     * if the process is in the read() system call, read returns -1
     * However, if this occurs, errno is set to EINTR. We can check this  value
     * and disregard the -1 value
     */

    if ( (length < 0) && (errno != EINTR) ) {
      perror("error reading the command");
      exit(-1);           /* terminate with error code of -1 */
    }

    /**
     * Parse the contents of inputBuffer
     */

    for (i=0;i<length;i++) {
      /* examine every character in the inputBuffer */

      switch (inputBuffer[i]){
      case ' ':
      case '\t' :               /* argument separators */
		if(start != -1){
		  args[ct] = &inputBuffer[start];    /* set up pointer */
		  ct++;
		}
		inputBuffer[i] = '\0'; /* add a null char; make a C string */
		start = -1;
		break;
      case '\n':                 /* should be the final char examined */
		if (start != -1){
		  args[ct] = &inputBuffer[start];
		  ct++;
		}
		inputBuffer[i] = '\0';
		args[ct] = NULL; /* no more arguments to this command */
		break;
      default :             /* some other character */
		if (start == -1)
		  start = i;
		if (inputBuffer[i] == '&') {
		  *background  = 1;
		  inputBuffer[i-1] = '\0';
		}
      } /* end of switch */
    }    /* end of for */

    /**
     * If we get &, don't enter it in the args array
     */

    if (*background)
      args[--ct] = NULL;

    args[ct] = NULL; /* just in case the input line was > 80 */

	/*for (i = 0; i < ct; i++) {
      printf("Value of var[%d] = %s\n", i, (args[i]));
   	}*/

    return 1;

} /* end of parseCommand routine */

void sighandler(int signum)
{
   printf("Caught signal %d, coming out...\n", signum);
   exit(1);
}

//adds bookmarks to the array
int createBookmark(char inputBuffer[80], char bookmarks[][80], int *track){

  char str[80];
  int count=0;
  int j=0;
  int flag=0;
  memset(&str[0], 0, 80);

  for(int i=0; i<80; i++){
    if('"' !=inputBuffer[i] && flag !=2){
      count++;
    } else if(flag ==0){
      flag++;
      count++;
    } else {
      count++;
      flag++;
      break;
    }
  }

  for(j=0; j<count; j++){
    if('\0'==inputBuffer[j]){
      str[j]=' ';
    } else {
      str[j]=inputBuffer[j];
    }
  }


  str[j+1]='\0';
  strncpy(bookmarks[*track], str+9, count);
  *track=*track+1;
  return count-9;


} //end of createBookmark

//lists all bookmarks
void displayBookmarks(char bookmarks[][80], int track){
  for(int i=0; i<track; i++){
    printf("%d %s\n", i, bookmarks[i]);
  }
}

//modifies args to execute normally
void execBookmark (char bookmarks[][80], int b, char *args[],int *background)
{
  char str[80];
  memset(&str[0], '\0', 80);
  memset(args[0], '\0', 41*80);

  strcpy(str, bookmarks[b]+1);



  int length,		/* # of characters in the command line */
  i,		/* loop index for accessing inputBuffer array */
  start,		/* index where beginning of next command parameter is */
  ct,	        /* index of where to place the next parameter into args[] */
  command_number;	/* index of requested command number */

  ct = 0;
  length=0;

  for(int i=0; i<80; i++){
    if(str[i]=='\"'){
      str[i]='\n';
    }
    if(str[i]!='\0'){
      length++;
    }
  }

  start = -1;
  /**
  * Parse the contents of bookmarks[b]
  */
  for (i=0;i<length;i++) {
    /* examine every character in the str */

    switch (str[i]){
      case ' ':
      case '\t' :               http://stackoverflow.com/questions/42990061/what-is-the-type-of-elements-of-thread-group-list-in-sched-h/42990445#42990445/* argument separators */
      if(start != -1){
        args[ct] = &str[start];    /* set up pointer */
        ct++;
      }
      str[i] = '\0'; /* add a null char; make a C string */
      start = -1;
      break;

      case '\n':                 /* should be the final char examined */
      if (start != -1){
        args[ct] = &str[start];
        ct++;
      }
      str[i] = '\0';
      args[ct] = NULL; /* no more arguments to this command */
      break;

      default :             /* some other character */
      if (start == -1)
      start = i;
      if (str[i] == '&') {
        *background  = 1;
        str[i-1] = '\0';
      }
    } /* end of switch */
  }    /* end of for */

  /**
  * If we get &, don't enter it in the args array
  */

  if (*background)
  args[--ct] = NULL;

  //args[ct] = NULL; /* just in case the input line was > 80 */


} // end of execBookmark
