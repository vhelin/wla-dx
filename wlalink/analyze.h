
int parse_context_from_name(char *name, char *context);
int add_reference(struct reference *r);
int add_stack(struct stack *sta);
int free_stack(struct stack *s);
int add_label(struct label *l);
int add_section(struct section *s);
int free_section(struct section *s);
int find_label(char *name, struct section *context, struct label **l);
int parse_data_blocks(void);
int obtain_source_file_names(void);
int obtain_memorymap(void);
int obtain_rombankmap(void);
int obtain_rombanks(void);
int collect_dlr(void);
int append_sections(void);
int clean_up_dlr(void);
int add_pointer_to_a_pointer_array(void *ptr, int id, void ***array, int *max, int *array_max, int size_increase);
