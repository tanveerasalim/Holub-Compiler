#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>
#include <ctype.h>
#include "lex_exp.h"
#include "uint.h"

bool is_valid_expression = 0;

uint16_t dec_specs = 0;

size_t param_dec_i = 0;

bool in_parameter_type_list = 0;
#if 0

	int * (*(*a[4])(int,int,int))(int,int);
	
	int *** volatile const **** p;

	int (*arr[10]) (long,bool,int);
	
	int d = 0, e = 0, g = 0; float h = 1.0;

	int (*var)(int,int,int); 

	long (((*array)))(long,bool,bool);			

	unsigned one = 0;

	int (arr_2[4]);

	int (*arz[10]) (long,bool,int);

	long long arroz(long,bool,int);
	
	int (*czar(long))[10]; // valid: a function that accepts a long argument and returns an array of 10 int elements

	int (*czar_second[10])(long); // valid array of 10 pointers to functions each accepting a long as an argument and each returning an int argument

	int (*czar_third)(long)[10];

#if 0
declare czar_third as an array of functions(long) that each returns a pointer to an int
#endif

#if 0	
invalid: viewed as an array of functions each returning an int. You cannot

declare an array of functions.
#endif
	int (*czar_fourth(long))(long);

#if 0	
   declare czar_fourth as function (long) 
   returning pointer to function 
   (long) returning int
#endif
	int (*czar_fifth(long));

   	int (* arr_pointers(long)) [100];
#if 0
declare arr_pointers as function (long) returning pointer to array 100 of int
#endif

	int (*func_pointers(long))[100];

#if 0
Declare a function that accepts argument long that returns a pointer to an array of 100 ints
#endif

	int (*func_p[45][34][99])(long,long,int);

	int (*func_2p)(long,long)(long); //invalid, you cannot make a function returning a function

	int (*func_nested(int *func_nested_two(long)))[45]; //actually invalid but gcc does not catch error

	int (*func_3p(long))[45];

	int (*func_4p(long,long))[56][65][78];

	int poisson()(); //invalid: cannot declare a function that returns a function

	int (**poison(long,long));

	int spec(int[],int ***)[45]; //invalid: function cannot return an array
	
	int (*buf[45])(unsigned long long, unsigned long);
#endif		

void error_msg(char * error_msg,uint64_t lineno,uint64_t charno)
{
	printf("\e[38;5;208m"); //Orange	
	
	fprintf(stderr,"%llu:%llu:Error:%s\n",lineno,charno,error_msg);

	printf("\e[0m");

	printf("\e[38;5;201m"); //Purple					
	
	printf("%s",input); //no need for newline char, already at end of line!
	
	printf("\e[0m");

	uint8_t * char_p = input;

	printf("\e[38;5;208m"); //Orange	

	while ( char_p <  yytext )
	{
		putchar(0x20);

		char_p++;
	}

	putchar('^');

	char_p++;

	while ( char_p < yycurrent )
	{
		putchar('~');

		char_p++;
	}

	printf("\e[0m");

	putchar(0xa);

	putchar(0xa);
	

}

void storage_class_specifier(void)
{
	
	if ( 
		match(AUTO) 	   
	   )
		
	{

		if ( ( dec_specs & 0b1 ) != 1 )
		{	

			dec_specs |= 0b1;

		}

		else
		{
			error_msg("Duplicate storage-class"
					" specifier:auto\n",
				yylineno,yytext-&input[0]
			       );	

		}

		advance();

	}	
	
	else if ( 
			match(STATIC) 	   	
		)
	{

		if ( ( ( dec_specs >> 1 ) & 0b1 ) != 1 )
		{	

			dec_specs |= 0b10;

		}

		else
		{
			error_msg("Duplicate storage-class"
					" specifier:static\n",
				yylineno,yytext-&input[0]
			       );	

		}

		advance();

	}
	
	else if ( 
			match(EXTERN) 	   	
		)
	{

		if ( ( ( dec_specs >> 2 ) & 0b1 ) != 1 )
		{	

			dec_specs |= 0b100;

		}

		else
		{
			error_msg("Duplicate storage-class"
					" specifier:extern\n",
				yylineno,yytext-&input[0]
			       );	

		}

		advance();

	}
	
	else if ( 
			match(TYPEDEF) 	   	
		)
	{

		if ( ( ( dec_specs >> 3 ) & 0b1 ) != 1 )
		{	

			dec_specs |= 0b1000;

		}

		else
		{
			error_msg("Duplicate storage-class"
					" specifier:typedef\n",
				yylineno,yytext-&input[0]
			       );	

		}
		
		advance();
	}
	
	else if ( 
			match(REGISTER) 	   	
		)
	{

		if ( ( ( dec_specs >> 4 ) & 0b1 ) != 1 )
		{	

			dec_specs |= 0b10000;

		}

		else
		{
			error_msg("Duplicate storage-class"
					" specifier:register\n",
				yylineno,yytext-&input[0]
			       );	

		}
		
		advance();
	}

}

void type_qualifier(void)
{
	while ( match(CONST) || match(VOLATILE) )
	{
		advance();
	}

}

#if 0

A uint16_t global variable named dec_specs stores information on what

type_specifiers have been written for the variable type-specification

so far.

The bitmap for uint16_t is as follows:

x  x  x  x  |   x   |	x   x |  x  x | x  x | 	x   x  x  x   x
15 14 13 12     11	10  9  	 8  7 	6  5   	4   3  2  1   0

The following type-specifiers collide with unsigned/signed type-specifiers:

15 - bool

14 - void

13 - double

12 - float

--------------

The following type-specifier is legal with short,char,long,unsigned, and signed:

11 - int

--------------

10 - char
9  - short

--------------

The "extra long" is for the SECOND legal type-specifier in a legal declaration:

8  - extra long

7  - long

--------------

The following type-specifiers are legal with long (including with the extra long),

short, char, and int

6  - signed

5  - unsigned

--------------

The following are storage-class specifiers and only ONE of these are permitted

in a declaration of any kind.

4  - register

3  - typedef

2  - extern

1  - static

0  - auto

#endif

void declaration_specifiers(void)
{
	dec_specs = 0;


	while ( 
		( Lookahead > ( 0b1 << 8 ) )

		&&

		( Lookahead < ( 0b1 << 24 ) )		

	      )
	{
	
		if ( 
			match(AUTO) 	   
		   )
		
		{

			if ( ( dec_specs & 0b11111 ) == 0 )
			{	

				dec_specs |= 0b1;

			}

			else
			{
				error_msg("Duplicate storage-class"
					" specifier: auto\n",
				yylineno,yytext-&input[0]
			       		);	

			}


		}	
	
		else if ( 
				match(STATIC) 	   	
			)
		{

			if ( ( dec_specs & 0b11111 )  == 0 )
			{	

				dec_specs |= 0b10;

			}

			else
			{
				error_msg("Duplicate storage-class"
					" specifier: static\n",
				yylineno,yytext-&input[0]
			       		);	
			}


		}
	
		else if ( 
				match(EXTERN) 	   	
			)
		{

			if ( ( dec_specs & 0b11111 ) == 0 )
			{		

				dec_specs |= 0b100;

			}

			else
			{
				error_msg("Duplicate storage-class"
					" specifier: extern\n",
				yylineno,yytext-&input[0]
			       		);	

			}


		}
	
		else if ( 
				match(TYPEDEF) 	   	
			)
		{

			if ( dec_specs & 0b11111 == 0 )
			{	

				dec_specs |= 0b1000;

			}

			else
			{
				error_msg("Duplicate storage-class"
					" specifier: typedef\n",
				yylineno,yytext-&input[0]
			       		);	

			}
		
		}
	
		else if ( 
				match(REGISTER) 	   	
			)
		{

			if ( ( dec_specs & 0b11111 ) == 0 )
			{	

				dec_specs |= 0b10000;

			}

			else
			{
				error_msg("Duplicate storage-class"
					" specifier: register\n",
				yylineno,yytext-&input[0]
			       		);	

			}
		
		}
	
		else if ( match(CONST)||match(VOLATILE) )
		{
			advance();
		}	

		else if ( match(UNSIGNED) )
		{
			if ( 
					( ( dec_specs & 0b100000 ) >> 5 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of unsigned type-specifier with another unsigned type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}
			
			if ( 
					( ( dec_specs & 0b1000000 ) >> 6 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of unsigned type-specifier with signed type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b1000000000000 ) >> 12 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of unsigned type-specifier with float type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}
			
			if ( 
					( ( dec_specs & 0b10000000000000 ) >> 13 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of unsigned type-specifier with double type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b100000000000000 ) >> 14 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of unsigned type-specifier with void type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs ) >> 15 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of unsigned type-specifier with bool type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			dec_specs |= 0b100000; 
		}

		else if ( match(SIGNED) )
		{
			if ( 
					( ( dec_specs & 0b100000 ) >> 5 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of signed type-specifier with another unsigned type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}
			
			if ( 
					( ( dec_specs & 0b1000000 ) >> 6 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of signed type-specifier with another signed type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b1000000000000 ) >> 12 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of signed type-specifier with float type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}
			
			if ( 
					( ( dec_specs & 0b10000000000000 ) >> 13 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of signed type-specifier with double type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b100000000000000 ) >> 14 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of signed type-specifier with void type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b1000000000000000 ) >> 15 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of signed type-specifier with bool type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			dec_specs |= 0b1000000; 
		}

		else if ( match(INT) )
		{

			if ( 
				( ( dec_specs >> 11 ) & 0b1 )
			   )
			{
				error_msg("Extra int\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b1000000000000 ) >> 12 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of int type-specifier with float type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}
			
			if ( 
					( ( dec_specs & 0b10000000000000 ) >> 13 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of int type-specifier with double type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b100000000000000 ) >> 14 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of int type-specifier with void type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b1000000000000000 ) >> 15 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of int type-specifier with bool type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			dec_specs |= 0b100000000000;

		}

		else if ( match(FLOAT) ) 
		{


			if ( ( dec_specs >> 5 ) & 0b1 )
			{
				error_msg("Collision of float type-specifier with unsigned type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if ( ( dec_specs >> 6 ) & 0b1 )
			{
				error_msg("Collision of float type-specifier with signed type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if ( ( dec_specs >> 7 ) & 0b1 )
			{
				error_msg("Collision of float type-specifier with long type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if ( ( dec_specs >> 9 ) & 0b1 )
			{
				error_msg("Collision of float type-specifier with  short type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if ( ( dec_specs >> 10 ) & 0b1 )
			{
				error_msg("Collision of float type-specifier with  char type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}	


			if ( 
				( ( dec_specs >> 11 ) & 0b1 )
			   )
			{
				error_msg("Collision of float type-specifier with  int type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b1000000000000 ) >> 12 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of float type-specifier with float type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}
			
			if ( 
					( ( dec_specs & 0b10000000000000 ) >> 13 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of float type-specifier with double type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b100000000000000 ) >> 14 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of float type-specifier with void type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b1000000000000000 ) >> 15 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of float type-specifier with bool type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}


			dec_specs |= 0b1000000000000; 		
		}

		else if ( match(DOUBLE) )
		{


			if ( ( dec_specs >> 5 ) & 0b1 )
			{
				error_msg("Collision of double type-specifier with unsigned type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if ( ( dec_specs >> 6 ) & 0b1 )
			{
				error_msg("Collision of double type-specifier with signed type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if ( ( dec_specs >> 8 ) & 0b1 )
			{
				error_msg("Collision of double type-specifier with long long type-specifiers\n",
					yylineno,yytext-&input[0]
				       );


			}

			if ( ( dec_specs >> 9 ) & 0b1 )
			{
				error_msg("Collision of double type-specifier with  short type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if ( ( dec_specs >> 10 ) & 0b1 )
			{
				error_msg("Collision of double type-specifier with  char type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}	


			if ( 
				( ( dec_specs >> 11 ) & 0b1 )
			   )
			{
				error_msg("Collision of double type-specifier with  int type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b1000000000000 ) >> 12 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of double type-specifier with float type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}
			
			if ( 
					( ( dec_specs & 0b10000000000000 ) >> 13 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of double type-specifier with double type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b100000000000000 ) >> 14 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of double type-specifier with void type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b1000000000000000 ) >> 15 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of double type-specifier with bool type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}


			dec_specs |= 0b10000000000000; 		
		}

		else if ( match(VOID) )
		{


			if ( ( dec_specs >> 5 ) & 0b1 )
			{
				error_msg("Collision of void type-specifier with unsigned type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if ( ( dec_specs >> 6 ) & 0b1 )
			{
				error_msg("Collision of void type-specifier with signed type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if ( ( dec_specs >> 7 ) & 0b1 )
			{
				error_msg("Collision of void type-specifier with long type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if ( ( dec_specs >> 9 ) & 0b1 )
			{
				error_msg("Collision of void type-specifier with  short type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if ( ( dec_specs >> 10 ) & 0b1 )
			{
				error_msg("Collision of void type-specifier with  char type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}	


			if ( 
				( ( dec_specs >> 11 ) & 0b1 )
			   )
			{
				error_msg("Collision of void type-specifier with  int type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b1000000000000 ) >> 12 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of void type-specifier with float type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}
			
			if ( 
					( ( dec_specs & 0b10000000000000 ) >> 13 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of void type-specifier with double type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b100000000000000 ) >> 14 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of void type-specifier with void type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b1000000000000000 ) >> 15 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of void type-specifier with bool type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}


			dec_specs |= 0b100000000000000; 		
		}

		else if ( match(BOOL) )
		{

			if ( ( dec_specs >> 5 ) & 0b1 )
			{
				error_msg("Collision of bool type-specifier with unsigned type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if ( ( dec_specs >> 6 ) & 0b1 )
			{
				error_msg("Collision of bool type-specifier with signed type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if ( ( dec_specs >> 7 ) & 0b1 )
			{
				error_msg("Collision of bool type-specifier with long type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if ( ( dec_specs >> 9 ) & 0b1 )
			{
				error_msg("Collision of bool type-specifier with  short type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if ( ( dec_specs >> 10 ) & 0b1 )
			{
				error_msg("Collision of bool type-specifier with  char type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}	


			if ( 
				( ( dec_specs >> 11 ) & 0b1 )
			   )
			{
				error_msg("Collision of bool type-specifier with  int type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b1000000000000 ) >> 12 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of bool type-specifier with float type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}
			
			if ( 
					( ( dec_specs & 0b10000000000000 ) >> 13 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of bool type-specifier with double type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b100000000000000 ) >> 14 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of bool type-specifier with void type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b1000000000000000 ) >> 15 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of bool type-specifier with bool type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}


			dec_specs |= 0b1000000000000000; 		
		}

		else if ( match(LONG) )
		{

			if ( ( dec_specs >> 9 ) & 0b1 )
			{
				error_msg("Collision of long type-specifier with  short type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if ( ( dec_specs >> 10 ) & 0b1 )
			{
				error_msg("Collision of long type-specifier with  char type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}	


			if ( 
					( ( dec_specs & 0b1000000000000 ) >> 12 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of long type-specifier with float type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}
			
			if ( 
					( ( dec_specs & 0b100000000000000 ) >> 14 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of long type-specifier with void type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}


			if ( 
					( ( dec_specs & 0b1000000000000000 ) >> 15 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of long type-specifier with bool type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if (
				( 
				 ( ( dec_specs >> 7 ) & 0b1 )
				)	
			   )
			{
				if (
					( ( dec_specs >> 8 ) & 0b1 )
				   )
				{
					error_msg("Too many"
							" \'long\' type-specifiers\n",
						yylineno,yytext-&input[0]
					       );

				}

				if (
					( ( dec_specs >> 13 ) & 0b1 )
				   )
				{
					error_msg("Second long type-specifier"
						" with double type-specifier\n",
						yylineno,yytext-&input[0]
					       );
				}

				dec_specs |= 0b100000000;
			}

			else
			{
				dec_specs |= 0b10000000;
			}

		}

		else if ( match(SHORT)  )
		{
			
			if ( ( dec_specs >> 7 ) & 0b1 )
			{
				error_msg("Collision of short type-specifier with  long type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if ( ( dec_specs >> 9 ) & 0b1 )
			{
				error_msg("Collision of short type-specifier with  short type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if ( ( dec_specs >> 10 ) & 0b1 )
			{
				error_msg("Collision of short type-specifier with  char type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}	


			if ( 
					( ( dec_specs & 0b1000000000000 ) >> 12 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of short type-specifier with float type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}
			
			if ( 
					( ( dec_specs & 0b10000000000000 ) >> 13 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of short type-specifier with double type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b100000000000000 ) >> 14 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of short type-specifier with void type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}


			if ( 
					( ( dec_specs & 0b1000000000000000 ) >> 15 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of short type-specifier with bool type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

				dec_specs |= 0b1000000000;
		}

		else if ( match(CHAR) )
		{
			
			if ( ( dec_specs >> 7 ) & 0b1 )
			{
				error_msg("Collision of char type-specifier with  long type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if ( ( dec_specs >> 9 ) & 0b1 )
			{
				error_msg("Collision of char type-specifier with  short type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}

			if ( ( dec_specs >> 10 ) & 0b1 )
			{
				error_msg("Collision of char type-specifier with  char type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}	

			if ( ( dec_specs >> 11 ) & 0b1 )
			{
				error_msg("Collision of char type-specifier with  int type-specifier\n",
					yylineno,yytext-&input[0]
				       );
			}	

			if ( 
					( ( dec_specs & 0b1000000000000 ) >> 12 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of char type-specifier with float type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}
			
			if ( 
					( ( dec_specs & 0b10000000000000 ) >> 13 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of char type-specifier with double type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

			if ( 
					( ( dec_specs & 0b100000000000000 ) >> 14 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of char type-specifier with void type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}


			if ( 
					( ( dec_specs & 0b1000000000000000 ) >> 15 ) 
					
					& 
					
					0b1 
					
			   )
			{
				error_msg("Collision of char type-specifier with bool type-specifier\n",
					yylineno,yytext-&input[0]
				       );

			}

				dec_specs |= 0b1000000000;
		}
		
		advance();		

	} //end while loop for declaration_specifiers

}

void init_declarator_list();

void pointer(void)
{

if ( match(ASTK) )
{	
	while ( match(ASTK) )
	{

		while ( match(ASTK) )
		{
			advance();
		}

		if ( match(CONST)||match(VOLATILE) )
		{
			while (match(CONST)||match(VOLATILE))
			{
				advance();
			}
		}

	}
}

}

void declaration(void)
{
	while (!match(EOI))
	{

		declaration_specifiers();

		
		if( ( ( dec_specs >> 5 ) & (~0b0) ) == 0 )
		{

			
			error_msg("Missing at least one"
					" type-specifier before identifier\n",
				yylineno,yytext-&input[0]
			       );

		}

		init_declarator_list();

		if (!match(SEMI))
		{
			error_msg("Missing semicolon\n",
				yylineno,yytext-&input[0]
			       );

		}

		advance();
	}
}

void init_declarator(void);

void init_declarator_list(void)
{
	while (!match(SEMI))
	{
		init_declarator();


		if (!match(COMMA) && !match(SEMI))
		{
			error_msg("Missing comma or semicolon\n",
				yylineno,yytext-&input[0]
			       );

			if ( match(LP) )
			{
			
				error_msg("Declaration of array of functions\n",
				yylineno,yytext-&input[0]
			       		);

			}

			else if ( match(LB) )
			{
				error_msg("Declaration of function returning array\n",
				yylineno,yytext-&input[0]
			       		);

			}
		}
		
		if ( match(COMMA) )
		{	
			advance();
		}
	
	}
}

void declarator(void);

void init_declarator(void)
{
	declarator();
}

void pointer(void);

void direct_declarator(void);

void declarator(void)
{

	pointer(); direct_declarator();
}

void parameter_type_list(void);

void direct_declarator(void)
{

	if ( match(ID) )
	{	advance();	}

	else if ( match(LP) )
	{
		advance();

		declarator();

		if ( !match(RP) )
		{
			error_msg("Missing right-parenthesis\n",
				yylineno,yytext-&input[0]
			       );

		}

		advance();
	}

	else
	{
		error_msg("Missing identifier or declarator starting with a \'(\' and ending with a \')\'\n",
			yylineno,yytext-&input[0]
		       );

		
		advance();
	}

	if ( match(LB) )
	{
		while ( match(LB) )
		{
			advance();

			if ( !match(NUM) )
			{
				error_msg("Missing integer-"
						"constant in array\n",
					yylineno,yytext-&input[0]
				       );
			}

			advance();

			if ( !match(RB) )
			{
				error_msg("Missing right-bracket\n",
					yylineno,yytext-&input[0]
				       );

			}

			advance();
		}
	}

	else if ( match(LP) ) //parameter_type_list
	{

		advance();

		parameter_type_list();

		if ( !match(RP) )
		{
			error_msg("Missing right-parenthesis\n",
				yylineno,yytext-&input[0]
			       );
			
		}

		advance();
	}

		
}

void parameter_declaration(void);

void parameter_type_list(void)
{
	in_parameter_type_list=1;

	if (match(RP)){return;}


	if ( 
		!( Lookahead > ( 0b1 << 8 ) )
			
		|| 
		
		!(Lookahead < ( 0b1 << 24 ) ) 
		
	   )


	{
		error_msg("Missing declaration-specifier\n",
			yylineno,yytext-&input[0]
		       );

	}


		while (
			( Lookahead > ( 0b1 << 8 ) )

			&&

			( Lookahead < ( 0b1 << 24 ) )

		      )

		{
#if 0
			if (match(VOID))
			{

				advance();

				if(
						!match(RP) 
						
						&& 
						
						!match(ASTK) 
						
						&& 
						
						!match(ID)

						&&

						!match(LP) 
						
						&& 
						
						( param_dec_i == 0 ) 
						
				)
				
				{	
				
					error_msg("Only void declaration-specifier can be in parameter_type_list by itself or a void pointer must be the parameter argument\n",
					yylineno,yytext-&input[0]
						);

				}

				else if (match(RP)&&(param_dec_i == 0) ){return;}
				
				else if (match(RP)&&(param_dec_i > 0) )
				{
					error_msg("After the first parameter argument, only void pointers or void pointers to functions allowed\n",
					yylineno,yytext-&input[0]
						);
						
					return;
				
				}

				else if (match(LP))
				{
					advance();

					declarator();

					if (!match(RP))
					{
						error_msg("Missing Right Parenthesis",
					yylineno,yytext-&input[0]
						);
					}
											
				}
				
				else if (match(ID))
				{

					direct_declarator();	
					
					if(!match(RP)&&!match(COMMA))
					{
						error_msg("Missing right parenthesis or comma\n",
					yylineno,yytext-&input[0]
						);
					}

					if( match(RP)){return;}
					

					else{advance();} //COMMA
				}
				
				else if (match(ASTK))	
				{
					pointer();

					if(!match(COMMA)&&!match(RP))
					{
						direct_declarator();	
					}
					

					if( match(RP)){return;}
					

					else{advance();} //COMMA
				
				}
				
				param_dec_i++;

				continue;
			}
#endif
			
			parameter_declaration();

			if ( match(COMMA) )
			{
				advance();

				if (
					!( Lookahead > ( 0b1 << 8 ) )

					||

					!(Lookahead < ( 0b1 << 24 ) )

				   )

				{
					error_msg("Missing"
							" declaration-specifier\n",
						yylineno,yytext-&input[0]
					       );

				}
			}
			
			param_dec_i++;
		}

		param_dec_i = 0;

}

void parameter_declaration(void)
{
	declaration_specifiers();

	if ( match(COMMA) || match(RP) )
	{
		if (match(RP) && ( ( dec_specs >> 14 ) & 0b1 ) && param_dec_i)
		{		
			error_msg("After first paramter argument, a void abstract parameter declaration is not allowed. Only a void pointer, or a pointer to function\n",
			yylineno,yytext-&input[0]
		       );
		}
		return;
	}

	if (match(ASTK) )
	{
		pointer();
	}	

	if ( match(COMMA) || match(RP) )
	{
		return;
	}

	declarator();
}

void test_specifiers_and_qualifiers(void)
{
	if (	
			( INT > ( 0b1 << 8 ) )

			&&

			( INT < ( 0b1 << 24 ) )		

	   )
	{
		printf("INT worked and its value is:\n%llu\n\n",INT);			
	}

	if (	
			( LONG > ( 0b1 << 8 ) )

			&&

			( LONG < ( 0b1 << 24 ) )		

	   )
	{
		printf("LONG worked and its value is:\n%llu\n\n",LONG);			
	}
		
	if (	
			( UNSIGNED > ( 0b1 << 8 ) )

			&&

			( UNSIGNED < ( 0b1 << 24 ) )		

	   )
	{
		printf("UNSIGNED worked and its value is:\n%llu\n\n",UNSIGNED);			
	}
	
	if (	
			( SIGNED > ( 0b1 << 8 ) )

			&&

			( SIGNED < ( 0b1 << 24 ) )		

	   )
	{
		printf("SIGNED worked and its value is:\n%llu\n\n",SIGNED);			
	}

	if (	
			( SHORT > ( 0b1 << 8 ) )

			&&

			( SHORT < ( 0b1 << 24 ) )		

	   )
	{
		printf("SHORT worked and its value is:\n%llu\n\n",SHORT);			
	}
	
	if (	
			( CHAR > ( 0b1 << 8 ) )

			&&

			( CHAR < ( 0b1 << 24 ) )		

	   )
	{
		printf("CHAR worked and its value is:\n%llu\n\n",CHAR);	
	}

	if (	
			( AUTO > ( 0b1 << 8 ) )

			&&

			( AUTO < ( 0b1 << 24 ) )		

	   )
	{
		printf("AUTO worked and its value is:\n%llu\n\n",AUTO);	
	}

	if (	
			( STATIC > ( 0b1 << 8 ) )

			&&

			( STATIC < ( 0b1 << 24 ) )		

	   )
	{
		printf("STATIC worked and its value is:\n%llu\n\n",STATIC);	
	}

	if (	
			( EXTERN > ( 0b1 << 8 ) )

			&&

			( EXTERN < ( 0b1 << 24 ) )		

	   )
	{
		printf("EXTERN worked and its value is:\n%llu\n\n",EXTERN);	
	}

	if (	
			( TYPEDEF > ( 0b1 << 8 ) )

			&&

			( TYPEDEF < ( 0b1 << 24 ) )		

	   )
	{
		printf("TYPEDEF worked and its value is:\n%llu\n\n",TYPEDEF);	
	}

	if (	
			( VOID > ( 0b1 << 8 ) )

			&&

			( VOID < ( 0b1 << 24 ) )		

	   )
	{
		printf("VOID worked and its value is:\n%llu\n\n",VOID);	
	}

	if (	
			( BOOL > ( 0b1 << 8 ) )

			&&

			( BOOL < ( 0b1 << 24 ) )		

	   )
	{
		printf("BOOL worked and its value is:\n%llu\n\n",BOOL);	
	}

	if (	
			( FLOAT > ( 0b1 << 8 ) )

			&&

			( FLOAT < ( 0b1 << 24 ) )		

	   )
	{
		printf("FLOAT worked and its value is:\n%llu\n\n",FLOAT);	
	}

	if (	
			( DOUBLE > ( 0b1 << 8 ) )

			&&

			( DOUBLE < ( 0b1 << 24 ) )		

	   )
	{
		printf("DOUBLE worked and its value is:\n%llu\n\n",DOUBLE);	
	}

	if (	
			( CONST > ( 0b1 << 8 ) )

			&&

			( CONST < ( 0b1 << 24 ) )		

	   )
	{
		printf("CONST worked and its value is:\n%llu\n\n",CONST);	
	}

	if (	
			( VOLATILE > ( 0b1 << 8 ) )

			&&

			( VOLATILE < ( 0b1 << 24 ) )		

	   )
	{
		printf("VOLATILE worked and its value is:\n%llu\n\n",VOLATILE);	
	}

	if (	
			( REGISTER > ( 0b1 << 8 ) )

			&&

			( REGISTER < ( 0b1 << 24 ) )		

	   )
	{
		printf("REGISTER worked and its value is:\n%llu\n\n",REGISTER);	
	}


}

int main(int argc, char ** argv)
{
	if ( argv[1] != NULL )
	{
		if ( ( input_file = fopen(argv[1],"r+") ) == NULL )
		{
			fprintf(stderr,"Error: Failed to open %s\n",argv[1]);

			exit(1);	
		}

	}

	declaration(); 

	return 0;
}
