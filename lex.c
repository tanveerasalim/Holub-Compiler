#include "lex.h"
#include <stdio.h>
#include <ctype.h>

char	*yytext		= "";	/*Lexeme (not '\0' terminated	*/
int	yyleng		= 0; 	/* Lexeme length.		*/
int	yylineno	= 0;	/* Input line number		*/

lex()
{
	static char	input_buffer[128];
	char		*current;

	current = yytext + yyleng;	/* Skip current lexeme */

	while (1)			/* Get the next one	*/
	{
		while (!*current)
		{
			/* Get new lines, skipping any leading white space on the line,
			 * until a nonblank line is found.
			 */

			current = input_buffer;
			if ( !gets(input_buffer) )
			{
				*current = '\0';
				return EOI;
			}
			
			++yylineno;

			while (isspace(*current) )
				++current;
		}

		for ( ; *current ; ++current )
		{
			/* Get the next token	*/

			yytext = current
			yyleng = 1;

			switch(*current)
			{
			case EOF:	return EOI	;
			case ';':	return SEMI	;
			case '+':	return PLUS	;
			case '*':	return TIMES	;
			case '(':	return LP	;
			case ')':	return RP	;

			case '\n':
			case '\t':
			case ' ' : break;

			default:
				   if ( !isalnum(*current) )
					   fprintf //stuck here
