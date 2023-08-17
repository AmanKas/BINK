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


void print_bnc(bnc* i) 
{
    if(i == NULL)
        return;                                                    // no bnc to print

    if(i->s != NULL) 
    {
        if(i->s[0] == '+')
            printf("%s", i->s + 1);
        else
            printf("%s", i->s);
    }
    return;
}



bnc deep_copy(bnc a1, bnc *a) 
{
    int i;
    a1.n = a->n;
    a1.s = (char *)malloc(sizeof(char) * (a1.n + 2));
    for(i = 0; i < (a1.n + 2); i++)
        a1.s[i] = a->s[i];
    return a1;
}


bnc* add_bnc(bnc* a, bnc* b) 
{
    int len, loop, carry = 0, ans, i, flag = 0;
    bnc temp;
    bnc *result = (bnc *)malloc(sizeof(bnc));

    bnc a1;                                                         //copies to work with
    bnc b1;

    a1.n = 0;
    a1.s = NULL;

    b1.n = 0;
    b1.s = NULL;

    a1 = deep_copy(a1, a);                                         //create deep copy
    b1 = deep_copy(b1, b);

    if((a1.s[0] == '+' && b1.s[0] == '+') || (a1.s[0] == '-' && b1.s[0] == '-'))    //addition happens between numbers having same sign
    {                           
        if(a1.s[0] == '-')                                        // (-) + (-) = (-). Result is negative
            flag = 1;
        
        if(a1.n > b1.n) 
        {
            len = a1.n;
            loop = b1.n;
        }
        else 
        {
            len = b1.n;
            loop = a1.n;
        }
        result->s = (char *)malloc(sizeof(char) * (len + 3));   //contains the result of the addition
        for(i = 0; i < (len + 3); i++) 
            result->s[i] = '0';
        result->n = len + 1;
        int index = len + 2;
        result->s[index--] = '\0';
        while(loop > 0) 
        {
            if(a1.n > b1.n)
                ans = (a1.s[len] - '0') + (b1.s[loop] - '0') + carry;
            else
                ans = (a1.s[loop] - '0') + (b1.s[len] - '0') + carry;

            if(carry != 0)                                   //set carry = 0 after using it
                carry = 0;

            if(ans >= 10) 
            {
                result->s[index--] = ((ans % 10) + '0');
                carry = ans / 10;
            }
            else
                result->s[index--] = ans + '0';
            loop--;
            len--;
        }
        while(len > 0) 
        {
            if(a1.n > b1.n)
                ans = (a1.s[len] - '0') + carry;
            else
                ans = (b1.s[len] - '0') + carry;

            if(carry != 0)
                carry = 0;

            if(ans >= 10) 
            {
                result->s[index--] = ((ans % 10) + '0');
                carry = ans / 10;
            }
            else
                result->s[index--] = ans + '0';
            len--;
        }
        if(carry != 0)
            result->s[index] = carry + '0';

        if(result->s[1] == '0') 
        {
            for(i = 0; i <= (result->n) ; i++)             //remove leading zeros in the result
                result->s[i] = result->s[i + 1];
            if(flag == 1)
                result->s[0] = '-';
            else 
                result->s[0] = '+';
            result->s = (char *)realloc(result->s, sizeof(char) * ((result->n) + 1));
            result->n = result->n - 1;
        }
        else 
        {
            if(flag == 1)
                result->s[0] = '-';
            else
                result->s[0] = '+';
        }
    }
    else if((a1.s[0] == '+' && b1.s[0] == '-'))             // A + (-B) = A - B. Forward the call to subtract_bnc for subtraction.
    {                                               
        b1.s[0] = '+';
        result = subtract_bnc(&a1, &b1);
    }
    else if((a1.s[0] == '-' && b1.s[0] == '+'))             // (-A) + (B) = B - A. Forward the call to subtract_bnc for subtraction.
    {                                               
        if(a1.n > b1.n)
            flag = 1;
        else if(a1.n == b1.n) 
        {
            for(i = 1; i <= a1.n; i++) 
            {
                if(a1.s[i] != '0' || b1.s[i] != '0') 
                {
                    if((a1.s[i] - '0') < (b1.s[i] - '0')) 
                    {
                        temp = a1;
                        a1 = b1;
                        b1 = temp;
                        break;
                    }
                    else if((a1.s[i] - '0') > (b1.s[i] - '0')) 
                    {
                        flag = 1;
                        break;
                    }
                }

            }
        }
        else
            flag = 0;
        a1.s[0] = '+';
        b1.s[0] = '+';
        result = subtract_bnc(&a1, &b1);
        if(flag == 1)
            result->s[0] = '-';
        else
            result->s[0] = '+';
    }
    return result;
}