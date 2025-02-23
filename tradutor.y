%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "tradutor.h"

    int yylex(void);
%}

%union {
    int num;
    char *str;
    struct stringList *strArray;
    struct planList *planArray;
}

%token <num> AGN PN
%token <str> STRING

%token COLON
%token AGENTCODE
%token BELIEFS
%token GOAL
%token PLANS
%token TRIGGER CTX BODY

%type <strArray> beliefs belieflist body bodylist
%type <planArray> plans planlist
%type <str> agentname goal trigger ctx

%start object

%%


object:         '{' agentcode '}'                               { writeConfigElements(); } 
        ;

agentcode:      '"' AGENTCODE '"' COLON '[' agentlist ']'
        |       '"' AGENTCODE '"' COLON '{' agent '}'
        ;

agentlist:      '{' agent '}'
        |       '{' agent '}' ',' agentlist;
        ;

agent:          agentname ',' beliefs ',' goal ',' plans        { generateAgent($1, $3, $5, $7); restartPlanIndex();  }
        ;       

agentname:      '"' agentnumber '"' COLON '"' STRING '"'        { $$ = $6; }
        ;

agentnumber:    AGN                                             { validateAgentIndex($1); }


beliefs:        '"' BELIEFS '"' COLON '[' belieflist ']'        { $$ = $6; }
        ;

belieflist:     '"' STRING '"'                                  { $$ = insertStringList($2, NULL); }
        |       '"' STRING '"' ',' belieflist                   { $$ = insertStringList($2, $5); }
        ;


goal:           '"' GOAL '"' COLON '"' STRING '"'               { $$ = $6; }
        |       '"' GOAL '"' COLON '"' '"'                      { $$ = NULL; }
        ;


plans:          '"' PLANS '"' COLON '{' planlist '}'            { $$ = $6; }
        ;

planlist:       '"' plannumber '"' COLON '{' trigger ',' ctx ',' body '}'               { $$ = insertPlanList($6, $8, $10, NULL); }
        |       '"' plannumber '"' COLON '{' trigger ',' ctx ',' body '}' ',' planlist  { $$ = insertPlanList($6, $8, $10, $13); }
        |       '"' plannumber '"' COLON '{' trigger ',' body '}'                       { $$ = insertPlanList($6, NULL, $8, NULL); }
        |       '"' plannumber '"' COLON '{' trigger ',' body '}' ',' planlist          { $$ = insertPlanList($6, NULL, $8, $11); }
        ;

plannumber:     PN                                              { validatePlanIndex($1); }
        ;

trigger:        '"' TRIGGER '"' COLON '"' STRING '"'            { $$ = $6; }
        ;

ctx:            '"' CTX '"' COLON '"' STRING '"'                { $$ = $6; }
        ;

body:           '"' BODY '"' COLON '[' bodylist ']'             { $$ = $6; validatePlanBody($6); }
        ;

bodylist:       '"' STRING '"'                                  { $$ = insertStringList($2, NULL); }
        |       '"' STRING '"' ',' bodylist                     { $$ = insertStringList($2, $5); }
        ;

%%