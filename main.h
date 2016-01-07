
#ifndef _MAIN_H
#define _MAIN_H

void procedures_at_exit(void);
int parse_flags(char *flags);
int parse_defines_and_get_final_name(char **c, int n);
int parse_and_add_definition(char *c);
int parse_and_set_incdir(char *c);
int generate_tmp_names(void);
int generate_extra_definitions(void);

/* mersenne twister */
void init_genrand(unsigned long s);
unsigned long genrand_int32(void);

#endif
