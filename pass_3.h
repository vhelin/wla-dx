
#ifndef _PASS_3_H
#define _PASS_3_H

int pass_3(void);
int is_label_anonymous(char *label);
int mangle_label(char *label, char *parent, int n, unsigned int label_size);
int add_namespace(char *label, char *name_space, unsigned int label_size);

#endif
