#ifndef _DBGEXP_H
#define _DBGEXP_H

#define DEBUG_EXPORT_VICE       0x0001
#define DEBUG_EXPORT_RGBDS      0x0002
#define DEBUG_EXPORT_MESEN      0x0004
#define DEBUG_EXPORT_EMULICIOUS 0x0008
#define DEBUG_EXPORT_CSPECT     0x0010
#define DEBUG_EXPORT_NOCASH     0x0020
#define DEBUG_EXPORT_JSON       0x0040
#define DEBUG_EXPORT_MAME       0x0080

int debug_export_parse_flags(const char *formats, int *flags);
int debug_export_all(struct debug_model *model, int flags, char *outname);

#endif
