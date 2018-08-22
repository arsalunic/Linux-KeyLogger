#ifndef CONFIGURATION_H
#define CONFIGURATION_H

typedef struct Configuration{

   char *myLogFile;
   char *specialFile;

} Configuration;

void cleanup(Configuration *configuration);


#endif