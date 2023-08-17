#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bnc.h"


bnc* create_bnc() {
     bnc* i;

     i = (bnc*) malloc(sizeof(bnc));
     if (i == NULL) {
          return NULL; //malloc failed
     }

     //alloc 3 chars for a null-terminated string "+0".
     i->s = (char *) malloc(3 * sizeof(char));
     if (i->s == NULL) {
          free(i);
          return NULL; //malloc failed
     }

     strcpy(i->s, "+0");
     i->n = 1;

     return i;
}


void delete_bnc(bnc** i) {

     if (i == NULL) {
          return;
     }

     if (*i == NULL) {
          return;
     }

     if ( (*i)->s != NULL ) {
          free( (*i)->s );
     }

     free(*i);
     *i = NULL;
     return;
}

bnc* remove_leading_zeros(bnc *num) {
     int i, j;
     int count = 0;
     for (i = 1; i < num->n; i++) {     //remove leading zeros
          if (num->s[1] != '0') {
               break;
          }
          if (num->s[1] == '0' && i != (num->n)) {
               for (j = 1; j <= num->n + 1; j++) {
                    num->s[j] = num->s[j + 1];
               }
               count++;
          }
     }

     num->s = (char *)realloc(num->s, sizeof(char) * (num->n + 2 - count));
     num->n -= (count);
     return num;
}


void read_bnc(bnc* i, char* str) {
     int n;

     if (i == NULL) {
          //printf("HELLO\n");
          return; //use create_bnc before calling read_bnc
     }
     if (str == NULL) {
          return; //invalid str
     }

     n = strlen(str);
     //printf("%d\n",n);

     if ( (str[0] == '+') || (str[0] == '-') ) {
          //it has a sign
          i->s = realloc(i->s, n + 1); //one extra char for null termination
          strcpy(i->s, str);
          i->n = n - 1;
     }
     else {
          //it doesn't have a sign and hence it's positive
          n++; //one extra for the + sign
          i->s = realloc(i->s, n + 1); //one extra char for null termination
          i->s[0] = '+';
          strcpy((i->s + 1), str);
          i->n = n - 1;
     }
     return;
}