#include <stddef.h>

typedef struct bnc {
     char *s; //null terminated string of decimal digits preceded by a +/- sign
     int n; //length of the integer in terms of decimal digits
} bnc;


bnc* create_bnc();


void delete_bnc(bnc** i);

bnc* remove_leading_zeros(bnc *num);


void read_bnc(bnc* i, char* str);