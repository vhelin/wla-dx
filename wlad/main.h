int main(int argc, char *argv[]);
int output_bank_opcodes(int *b, unsigned char *in, int bank_size, int *i);
int output_bank_data(int *b, unsigned char *in, int bank_size, int *i);
int output_bank_data_detect_strings(int *b, unsigned char *in, int bank_size, int *i);
int parse_flags(int argc, char *argv[]);
int letter_check(unsigned char c);
