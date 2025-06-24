%{
#include "eng.tab.h"
#include <stdlib.h>
extern YYSTYPE yylval; 
%}

%%
set             { return SET; }
print           { return PRINT; }
loop            { return LOOP; }
from            { return FROM; }
to              { return TO; }
with            { return WITH; }
until           { return UNTIL; }
"++"            { return INC; }
"--"            { return DEC; }
"<="            { return LEQ; }
">="            { return GEQ; }
"<"             { return LT; }
">"             { return GT; }
"=="            { return EQ; }
"!="            { return NEQ; }
";"             { return SEMICOLON; }
"="             { return ASSIGN; }
"is greater than or equal to"    { return GEQ; }
"is less than or equal to"       { return LEQ; }
"is greater than or equal"       { return GEQ; }
"is less than or equal"          { return LEQ; }
"is greater than"                { return GT; }
"is less than"                   { return LT; }
"is equal to"                    { return EQ; }
"is not equal to"                { return NEQ; }
"is equal"                       { return EQ; }
"is not equal"                   { return NEQ; }
[0-9]+          { yylval.num = atoi(yytext); return NUMBER; }
if              { return IF; }
else            { return ELSE; }
then            { return THEN; }
[a-zA-Z_][a-zA-Z0-9_]*   { yylval.id = strdup(yytext); return IDENTIFIER; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MUL; }
"/"             { return DIV; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"{"             { return '{'; }
"}"             { return '}'; }
[ \t\n]         ; 
.               ; 
%%