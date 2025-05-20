%{
#include "eng.tab.h"
#include <stdlib.h>
extern YYSTYPE yylval; 
%}

%%
set             { return SET; }
to              { return TO; }
print           { return PRINT; }
[0-9]+          { yylval.num = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]*   { yylval.id = strdup(yytext); return IDENTIFIER; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MUL; }
"/"             { return DIV; }
"("             { return LPAREN; }
")"             { return RPAREN; }
[ \t\n]         ; 
.               ; 
%%