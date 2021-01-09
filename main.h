
#ifndef _MAIN_H
#define _MAIN_H

void procedures_at_exit(void);
int parse_flags(char **flags, int flagc);
int parse_and_add_definition(char *c, int contains_flag);
int parse_and_add_incdir(char* c, int contains_flag);
int generate_tmp_name(char **filename);
int generate_extra_definitions(void);

/* mersenne twister */
void init_genrand(unsigned long s);
unsigned long genrand_int32(void);

#endif
