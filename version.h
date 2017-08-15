#ifndef VERSION_INFO_H
#define VERSION_INFO_H

#ifdef HAVE_VERSION_INFO_H
  #include "version-info.h"
#endif

#ifndef VERSION_AMIGA
  #define VERSION_AMIGA "1.0"
#endif

#ifndef VERSION_AMIGA_DATE
  #define VERSION_AMIGA_DATE "01.01.1970"
#endif

#ifndef VERSION
  #define VERSION VERSION_AMIGA
#endif

#ifndef VERSION_FULL_STRING
  #define VERSION_FULL_STRING "v" VERSION "-unknown"
#endif

#ifndef CPU_NAME
  #if defined(GB)
    #define CPU_NAME "GB-Z80"
  #else
    #define CPU_NAME CPU
  #endif
#endif

#endif
