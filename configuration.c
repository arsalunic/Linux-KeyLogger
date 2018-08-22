#include <stdlib.h>

#include "configuration.h"

void cleanup(Configuration *configuration){
	
	free(configuration->myLogFile);
	free(configuration->specialFile);
	 	 
	 	 configuration->myLogFile 	= NULL;
		 configuration->specialFile = NULL;
}