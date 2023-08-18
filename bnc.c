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


bnc* subtract_bnc(bnc* a, bnc* b) {
     int len, loop, ans, flag = 0, i, j, prev;
     bnc temp;
     if (a->n > b->n) {
          len = a->n;
          loop = b->n;
     }
     else {
          len = b->n;
          loop = a->n;
     }

     bnc a1; //copies to work with
     bnc b1;

     a1.n = 0;
     a1.s = NULL;

     b1.n = 0;
     b1.s = NULL;

     a1 = deep_copy(a1, a);        //create deep copies
     b1 = deep_copy(b1, b);

     bnc *result = (bnc *)malloc(sizeof(bnc));        //contains the result of the subtraction

     if ((a1.s[0] == '+' && b1.s[0] == '+') || (a1.s[0] == '-' && b1.s[0] == '-')) {
          result->s = (char *)malloc(sizeof(char) * (len + 2));
          for (i = 0; i < (len + 2); i++) {
               result->s[i] = '0';
          }
          result->n = len;
          int index = len + 1;
          result->s[index--] = '\0';
          if ((a1.s[0] == '+' && b1.s[0] == '+')) {
               if (a->n < b->n) {
                    flag = 1;
                    temp = a1;
                    a1 = b1;
                    b1 = temp;
               }
               else if (a->n == b->n) {
                    for (i = 1; i <= a->n; i++) {
                         if (a->s[i] != '0' || b->s[i] != '0') {
                              if ((a->s[i] - '0') < (b->s[i] - '0')) {
                                   flag = 1;
                                   temp = a1;
                                   a1 = b1;
                                   b1 = temp;
                                   break;
                              }
                              else {
                                   break;
                              }
                         }
                    }
               }
          }
          else if ((a1.s[0] == '-' && b1.s[0] == '-')) {
               if (a->n < b->n) {
                    temp = a1;
                    a1 = b1;
                    b1 = temp;
               }
               else if (a->n == b->n) {
                    for (i = 1; i <= a->n; i++) {
                         if (a->s[i] != '0' || b->s[i] != 0) {
                              if ((a->s[i] - '0') < (b->s[i] - '0')) {
                                   temp = a1;
                                   a1 = b1;
                                   b1 = temp;
                                   break;
                              }
                              else {
                                   flag = 1;
                                   break;
                              }
                         }
                    }
               }
               else {
                    flag = 1;
               }
          }
          while (loop > 0) {
               ans = (a1.s[len] - '0') - (b1.s[loop] - '0');
               if (ans < 0) {
                    a1.s[len] = (((a1.s[len] - '0') + 10) + '0');
                    j = len - 1;
                    prev = ((a1.s[j] - '0') - 1);
                    while (prev < 0) {
                         a1.s[j] = (((a1.s[j] - '0') + 9) + '0');
                         prev = ((a1.s[--j] - '0') - 1);
                    }
                    a1.s[j] = (((a1.s[j] - '0') - 1) + '0');
                    result->s[index--] =  (((a1.s[len] - '0') - (b1.s[loop] - '0')) + '0');
               }
               else {
                    result->s[index--] = (ans + '0');
               }
               len--;
               loop--;
          }
          while (len > 0) {
               result->s[index--] = a1.s[len--];
          }
          int count = 0;
          for (i = 0; i < result->n; i++) {       //remove leading zeros
               if (result->s[0] != '0') {
                    break;
               }
               if (result->s[0] == '0' && result->s[1] == '0' && i != (result->n - 1)) {
                    for (j = 0; j <= result->n; j++) {
                         result->s[j] = result->s[j + 1];
                    }
                    count++;
               }
          }
          result->s = (char *)realloc(result->s, sizeof(char) * (result->n + 2 - count));
          result->n -= (count);
          if (flag == 1) {
               result->s[0] = '-';
          }
          else {
               result->s[0] = '+';
          }
     }
     else if (a1.s[0] == '+' && b1.s[0] == '-') { // (+A) - (-B) = A + B. Forward call to add_bnc for addition.
          b1.s[0] = '+';
          result = add_bnc(&a1, &b1);
          b1.s[0] = '-';
     }
     else if (a1.s[0] == '-' && b1.s[0] == '+') { // (-A) - (B) = -(A + B). Forward call to add_bnc for addition.
          a1.s[0] = '+';
          b1.s[0] = '+';
          result = add_bnc(&a1, &b1);
          result->s[0] = '-';
          a1.s[0] = '-';
     }
     return result;
}



bnc* multiply_byten(bnc *a, int exp) 
{
    int i, j;
    bnc *result = (bnc *)malloc(sizeof(bnc));
    result->s = (char *)malloc(sizeof(char) * (a->n + exp) + 2);
    result->n = a->n + exp;
    for (i = 0; i <= a->n; i++)
        result->s[i] = a->s[i];
    for (j = a->n + 1 ; j < result->n + 2; j++)
        result->s[j] = '0';
    result->s[result->n + 1] = '\0';
    return result;
}



bnc* padarray(bnc* a, int num) 
{
    int padval = num - a->n;
    bnc *new = (bnc *)malloc(sizeof(bnc));
    new->s = (char *)malloc(sizeof(char) * (num + 2));
    new->s[0] = a->s[0];
    new->n = num;
    int i = 1, j = 1;
    while (i <= new->n) 
    {
        while (padval != 0) 
        {
            new->s[i++] = '0';
            padval--;
        }
        new->s[i++] = a->s[j++];
    }
    new->s[new->n + 1] = '\0';
    return new;
}




bnc* multiply_bnc(bnc* a, bnc* b) 
{
    if (a->n == 1 && b->n == 1)                            //base case if number has only 1 digit
    {      
        bnc *result = (bnc *)malloc(sizeof(bnc));
        int res = (a->s[1] - '0') * (b->s[1] - '0');      //multiply the numbers
        int rescpy = res;
        int len = 0, i;
        char temp;
        if (res == 0)
            len++;
        while (rescpy != 0 ) 
        {
            len++;
            rescpy = rescpy / 10;
        }
        result->s = (char *)malloc(sizeof(char) * (len + 2));
        result->n = len;
        for (i = 1; i <= len; i++) 
        {
            result->s[i] = ((res % 10) + '0');
            res = res / 10;
        }
        if (len == 2) 
        {
            temp = result->s[1];
            result->s[1] = result->s[2];
            result->s[2] = temp;
        }

        if ((a->s[0] == '-' && b->s[0] == '+') || (a->s[0] == '+' && b->s[0] == '-'))  //choose appropriate sign
            result->s[0] = '-';
        else
            result->s[0] = '+';
        return result;
    }

    int i;
    bnc *result = (bnc *)malloc(sizeof(bnc));       //result to be returned
    bnc a1;                                         //copies to work with
    bnc b1;

    a1.n = 0;
    a1.s = NULL;

    b1.n = 0;
    b1.s = NULL;

    a1 = deep_copy(a1, a);                          //create deep copies
    b1 = deep_copy(b1, b);

    if (a1.n > b1.n)
        b1 = *padarray(&b1, a1.n);                  //equalize the array sizes
    else if (b1.n > a1.n)
        a1 = *padarray(&a1, b1.n);
    if (a1.n % 2 == 1)                             //if length is odd, padd 1 zero to make the array size even
    {               
        a1 = *padarray(&a1, a1.n + 1);
        b1 = *padarray(&b1, b1.n + 1);
    }

    int m = a1.n / 2, j, tlen;                     //calculate the mid value
    if (m == 1)
        tlen = m + 1;
    else
        tlen = m + 2;
    bnc *c1 = (bnc *)malloc(sizeof(bnc));
    bnc *c2 = (bnc *)malloc(sizeof(bnc));
    bnc *d1 = (bnc *)malloc(sizeof(bnc));
    bnc *d2 = (bnc *)malloc(sizeof(bnc));
    bnc *c3 = (bnc *)malloc(sizeof(bnc));
    bnc *d3 = (bnc *)malloc(sizeof(bnc));

    c1->s = (char *)malloc(sizeof(char) * (m + 2));   //create the sub-arrays
    c2->s = (char *)malloc(sizeof(char) * (m + 2));
    d1->s = (char *)malloc(sizeof(char) * (m + 2));
    d2->s = (char *)malloc(sizeof(char) * (m + 2));
    c3->s = (char *)malloc(sizeof(char) * (tlen + 2));
    d3->s = (char *)malloc(sizeof(char) * (tlen + 2));

    for (i = 0; i <= m; i++) 
    {
        c1->s[i] = a1.s[i];
        d1->s[i] = b1.s[i];
    }
    c1->s[i] = '\0';
    d1->s[i] = '\0';
    c1->n = m;
    d1->n = m;

    c2->s[0] = a1.s[0];
    d2->s[0] = b1.s[0];
    c2->n = m;
    d2->n = m;

    j = 1;

    for (i = m + 1; i < (a1.n + 2); i++) 
    {
        c2->s[j] = a1.s[i];
        d2->s[j++] = b1.s[i];
    }

    c3 = add_bnc(c1, c2);                           //c3 = c1 + c2
    d3 = add_bnc(d1, d2);                           //d3 = d1 + d2

    if (c3->n != d3->n)                             //equalize array size
    {         
        if (c3->n > d3->n)
            d3 = padarray(d3, c3->n);
        else
            c3 = padarray(c3, d3->n);
    }

    if (c3->n % 2 == 1 && c3->n != 1)               //if array size if odd, pad one zero to make array size even.
    { 
        c3 = padarray(c3, c3->n + 1);
        d3 = padarray(d3, d3->n + 1);
    }

    c3->s[0] = c1->s[0];
    d3->s[0] = d1->s[0];

    bnc* p1 = (bnc *)malloc(sizeof(bnc));
    bnc* p2 = (bnc *)malloc(sizeof(bnc));
    bnc* p3 = (bnc *)malloc(sizeof(bnc));

    p1 = multiply_bnc(c1, d1);                  //p1 = c1[0...m] * b1[0...m]
    p2 = multiply_bnc(c2, d2);                  //p2 = c2[m+1...n] * d2[m+1...n]
    p3 = multiply_bnc(c3, d3);                  //p3 = c3[0...m+1] * d3[0...m+1]

    bnc* p1c = (bnc *)malloc(sizeof(bnc));
    bnc* p3c = (bnc *)malloc(sizeof(bnc));

    p1c = multiply_byten(p1, (2 * m));          //p1c = p1*(10^n)

    p3c = subtract_bnc(p3, p2);                 //p3c = (p3 - p2 - p1)*(10^m)
    p3c = subtract_bnc(p3c, p1);
    p3c = multiply_byten(p3c, m);
    result = add_bnc(p1c, p2);
    result = add_bnc(result, p3c);              //result = p1c + p3c + p2
    int count = 0;
    for (i = 0; i < result->n; i++)             //remove leading zeros
    {       
        if (result->s[1] != '0')
            break;
        if (result->s[1] == '0' && i != (result->n - 1)) 
        {
            for (j = 1; j <= result->n; j++)
                result->s[j] = result->s[j + 1];
            count++;
        }
    }
    result->s = (char *)realloc(result->s, sizeof(char) * (result->n + 2 - count)); //reallocate after removing leading zeros.
    result->n -= (count);
    return result;
}



int compare_bnc(bnc a1, bnc b1) 
{
    int i;
    if (a1.n < b1.n)                        //if b has more digits b > a
        return -1;
    if (a1.n > b1.n)                        //if a has more digits a > b
        return 1;
    for (i = 1; i <= a1.n; i++) 
    {
        if (a1.s[i] != 0 || b1.s[i] != 0) 
        {
            if (a1.s[i] < b1.s[i])          //if the first non-zero digit of a1 is less then b1 => a1 < b1 => return -1
                return -1;
            else if (a1.s[i] > b1.s[i])     //if the first non-zero digit of b1 is less then a1 => b1 < a1 => return +1
                return 1;
        }
    }
    return 0;                               //if both are equal return 0
}


bnc* divide_bnc(bnc* a, bnc* b) {
     bnc *result = create_bnc();         //stores the result. Initially 0.
     bnc a1;           //copy to work with
     bnc b1;           //copy to work with

     a1.n = 0;
     a1.s = NULL;

     b1.n = 0;
     b1.s = NULL;

     a1 = deep_copy(a1, a);
     b1 = deep_copy(b1, b);

     bnc *zero = create_bnc();

     bnc res;          //copy of b1
     int count;          //used to keep track of the number of subtrations in each iteration
     int len = 1;   //regulate the length of the result array

     res.n = 0;
     res.s = NULL;

     if ((a1.s[0] == '-' || b1.s[0] == '-') && !(a1.s[0] == '-' && b1.s[0] == '-')) {
          result->s[0] = '-';
     }
     a1.s[0] = '+';
     b1.s[0] = '+';

     if (compare_bnc(b1, *zero) == 0) {
          result->s[1] = '?';
          printf("\n\nDivision by zero is not possible!");
          return result;
     }

     res = deep_copy(res, &b1);

     while (compare_bnc(a1, res) >= 0) {        //mutiply res by 10 as many times as possible such that res < a1
          res = *multiply_byten(&res, 1);
     }
     res.s[res.n] = res.s[res.n + 1];
     res.s = (char *)realloc(res.s, sizeof(char) * (res.n + 1));
     res.n = res.n - 1;

     while (compare_bnc(res, b1) >= 0) {
          count = 0;
          while (compare_bnc(a1, res) >= 0) {        //loop runs as long as a1 >= res
               a1 = *subtract_bnc(&a1, &res);   //subtract a1 by res
               count++;       //number of subtractions
          }
          result->s = (char *)realloc(result->s, sizeof(char) * (len + 2));
          result->n = len;
          len++;
          result->s[result->n] = count + '0';               //append count to result
          res.s[res.n] = res.s[res.n + 1];
          res.s = (char *)realloc(res.s, sizeof(char) * (res.n + 1)); //strip one zero off of res and repeat.
          res.n = res.n - 1;
     }
     return result;
}


bnc* pow_bnc(bnc* a, bnc* b) {
     bnc bcpy;
     bnc acpy;

     acpy.n = 0;
     acpy.s = NULL;

     bcpy.n = 0;
     bcpy.s = NULL;

     bcpy = deep_copy(bcpy, b);
     acpy = deep_copy(acpy, a);

     bnc *strone = create_bnc();         //stores 1 to decrement bcpy by 1 in every iteration
     strone->s[1] = '1';
     bnc *result = create_bnc();         //result is initially 1
     result->s[1] = '1';

     bnc base;
     base.s = (char *)malloc(sizeof(char) * 3);   //used to compare with bcpy. If bcpy = 0, loop terminates
     base.s[0] = '+';
     base.s[1] = '0';
     base.s[2] = '\0';
     base.n = 1;
     while (compare_bnc(bcpy, base) != 0) {
          result = multiply_bnc(result, &acpy); //multiply the existing result by acpy
          bcpy = *subtract_bnc(&bcpy, strone);  //decrement bcpy
     }
     return result;
}