%{
    #include "tradutor.tab.h"
    void yyerror(char *);
%}

%%

"{" |
"}" |
"," |
"[" |
"]" |
\"			{ return yytext[0]; }

":"			{ return COLON; }

"agentCode"	{ return AGENTCODE; }
"beliefs"	{ return BELIEFS; }
"goal"		{ return GOAL; }
"plans"		{ return PLANS; }
"trigger"	{ return TRIGGER; }
"ctx"		{ return CTX; }
"body"		{ return BODY; }

"ag"[0-9]+  { 
                yylval.num = atoi(yytext + 2);
                return AGN;
            }

"p"[0-9]+  { 
                yylval.num = atoi(yytext + 1);
                return PN;
            }


[A-Za-z_][A-Za-z0-9_]*  { yylval.str = strdup(yytext); return STRING; }

[ \t]  /* Igora comentários */

\\\n   { printf("c> "); }  /* Ignora continuação de linha */

.       { yyerror("Caractere desconhecido\n"); }
%%
