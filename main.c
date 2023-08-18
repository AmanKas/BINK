#include <stdio.h>
#include "bnc.h"
#include <stdlib.h>
#include <string.h>

int main(void)
{
	char ch;
	char *str = (char *)calloc(sizeof(char), 1);
	int len = 0;
	int flag = 0;
	bnc *num1 = create_bnc();
	bnc *num2 = create_bnc();
	while((ch = getchar()) && ch != '\n' && (ch == '+' || ch == '-' || ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4'|| ch == '5'|| ch == '6'|| ch == '7'|| ch == '8'|| ch == '9' || '\0'))      //Input taken for num1
    {		
		str[len++] = ch;
		str = (char *)realloc(str,sizeof(char)*(len + 1));
	}
	str[len] = '\0';
	read_bnc(num1, str);

	str = (char *)realloc(str, sizeof(char));
	str[0] = 0;
	len = 0;
	while((ch = getchar()) && ch != '\n')                       //Input taken for num2
    {		
		str[len++] = ch;
		str = (char *)realloc(str,sizeof(char)*(len + 1));
	}
	str[len] = '\0';
	read_bnc(num2, str);

	num1 = remove_leading_zeros(num1);
	num2 = remove_leading_zeros(num2);
	
	bnc *resultadd = create_bnc();		                        //bncs to store the result
	bnc *resultsub = create_bnc();
	bnc *resultmul = create_bnc();
	bnc *resultdiv = create_bnc();
	bnc *resultpow = create_bnc();

	resultadd = add_bnc(num1, num2);		                    //addition
	printf("\n\nAddition : ");				                    //print the results
	print_bnc(resultadd);
	resultsub = subtract_bnc(num1, num2);	                    //subtraction
	printf("\n\nSubtraction : ");
	print_bnc(resultsub);
	resultmul = multiply_bnc(num1, num2);	                    //multiplication
	printf("\n\nMultiplication : ");
	print_bnc(resultmul);

	//printf("\n\nPlease wait while the division and Exponentiation functions are carried out.\n");
	
    resultdiv = divide_bnc(num1, num2);	                        //division
	if(!(resultdiv->s[1] == '?'))
    {
		printf("\n\nDivision : ");
		print_bnc(resultdiv);
	}
	printf("\n");

	if(num2->s[0] == '+')                                       //exponentiation is done if and only if the power is positive
    {					
		printf("\nPlease wait while the Exponentiation function is carried out.\n");
		flag = 1;
		resultpow = pow_bnc(num1, num2);	
	}
	if(flag)
    {
		printf("\nExponentiation : ");
		print_bnc(resultpow);
	}
	else
		printf("\n\nNegative Exponent. Please enter a positive exponent!");

	printf("\n\n");
	delete_bnc(&resultadd);
	delete_bnc(&resultsub);
	delete_bnc(&resultmul);
	delete_bnc(&resultdiv);
	delete_bnc(&resultpow);

	return 0;
}
