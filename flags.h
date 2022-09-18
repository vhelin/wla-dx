
#ifndef _FLAGS_H
#define _FLAGS_H

/* use this file to specify global defines for every .c file */

/* define PROFILE_FUNCTIONS if you want to collect some timings for executed functions. these are
   appended to two files called "_profile_function_names.wla" and "_profile_function_times.wla".
   if you repeat the run more data is collected to these files which should then give better results.
   after all the data has been collected run "display_statistics.exe" (this tool must be manually
   compiled and is located in its folder in the root of wla's project dir) to see the statistics */
/*
#define PROFILE_FUNCTIONS 1
*/

/* define WLA_DEBUG to see outputted instructions and created stack calculations */
/*
#define WLA_DEBUG 1
*/

#endif
