#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tradutor.h"

int agentIndex = 0;
int planIndex = 0;
stringList *agents = NULL;
int numAgents = 0;
int flag = 0;

void generateAgent(char *parsedAgentName, stringList *parsedBeliefs, char *parsedGoal, planList *parsedPlans){
    agents = insertStringList(parsedAgentName, agents);
    numAgents++;
    FILE *output = fopen("output.py", "a");
    
    if(flag==0){
        fclose(output);
        output = fopen("output.py", "w");
        fprintf(output, "from maspy import *\n\n");
    }
    flag = 1;
    
    stringList *goalList = NULL;
    if(parsedGoal)
        goalList = insertStringList(parsedGoal, NULL);

    fprintf(output, "class Agente%d(Agent):\n", agentIndex);
    fprintf(output, "\tdef __init__(self, agt_name):\n");
    fprintf(output, "\t\tsuper().__init__(agt_name)\n");

    writeBeliefs(parsedBeliefs, output);
    if(parsedGoal)
        fprintf(output, "\t\tself.add(Goal(\"%s\"))\n\n", parsedGoal);
    else
        fprintf(output, "\n");

    writePlans(parsedPlans, &goalList, &parsedBeliefs, output);

    fclose(output);

    parsedBeliefs = freeStringList(parsedBeliefs);
    goalList = freeStringList(goalList);
    parsedPlans = freePlanList(parsedPlans);
}

void writeBeliefs(stringList *parsedBeliefs, FILE *output){
    while(parsedBeliefs){
        fprintf(output, "\t\tself.add(Belief(\"%s\"))\n", parsedBeliefs->string);
        parsedBeliefs = parsedBeliefs->next;
    }

    return;
}

void writePlans(planList *parsedPlans, stringList **parsedGoals, stringList **parsedBeliefs, FILE *output){
    int i = 0;
    while(parsedPlans){
        
        validateKnowledge(parsedPlans, parsedGoals, parsedBeliefs);

        i++;
        stringList *bodyIterator = parsedPlans->body;
        if(*parsedGoals){
            if(parsedPlans->ctx)
                fprintf(output, "\t@pl(gain, Goal(\"%s\"), Belief(\"%s\"))\n", (*parsedGoals)->string, parsedPlans->ctx);
            else
                fprintf(output, "\t@pl(gain, Goal(\"%s\"))\n", (*parsedGoals)->string);
            fprintf(output, "\tdef %s_(self, src):\n", (*parsedGoals)->string);
        }
        else{
            if(parsedPlans->ctx)
                fprintf(output, "\t@pl(gain, Belief(\"%s\"), Belief(\"%s\"))\n", parsedPlans->trigger, parsedPlans->ctx);
            else
                fprintf(output, "\t@pl(gain, Belief(\"%s\"))\n", parsedPlans->trigger);
            fprintf(output, "\tdef %s_(self, src):\n", parsedPlans->trigger);
        }

        while(bodyIterator){
            if(bodyIterator->string[0] == 'A')
                fprintf(output, "\t\tself.print(\"%s\")\n", bodyIterator->string+2);
            else if(bodyIterator->string[0] == 'B'){
                fprintf(output, "\t\tself.add(Belief(\"%s\"))\n", bodyIterator->string+2);
                *parsedBeliefs = insertStringList(bodyIterator->string+2, *parsedBeliefs);
            }
            else if(bodyIterator->string[0] == 'G'){
                 fprintf(output, "\t\tself.add(Goal(\"%s\"))\n", bodyIterator->string+2);
                *parsedGoals = insertStringList(bodyIterator->string+2, *parsedGoals);
            }
                bodyIterator = bodyIterator->next;
        }
        fprintf(output, "\n");
        parsedPlans = parsedPlans->next;
    }
}

void validateKnowledge(planList *parsedPlans, stringList **parsedGoals, stringList **parsedBeliefs){
    if(!searchStringList(parsedPlans->trigger, *parsedGoals) && !searchStringList(parsedPlans->trigger, *parsedBeliefs)){
        printf("\nO Trigger %s nao existe em Goals ou Beliefs\n", parsedPlans->trigger);
        exit(1);
    }
    if(parsedPlans->ctx && !searchStringList(parsedPlans->ctx, *parsedGoals) && !searchStringList(parsedPlans->ctx, *parsedBeliefs)){
        printf("\nO Ctx %s nao existe em Goals ou Beliefs\n", parsedPlans->ctx);
        exit(1);
    }
    return;
}

int searchStringList(char *target, stringList *arr){
    while(arr){
        if(!strcmp(target, arr->string))    
            return 1;
        arr = arr->next;
    }
    return 0;
}

void writeConfigElements(){
    if(!agents){
        exit(1);
    }
    FILE *output = fopen("output.py", "a");
    writeAgents(output, numAgents, agents);

    fprintf(output, "Admin().start_system()");

    fclose(output);
    free(agents);
}

void writeAgents(FILE *output, int i, stringList *iterator){
    if(iterator){
        writeAgents(output, i-1, iterator->next);
        fprintf(output, "Agente%d(\"%s\")\n", i, iterator->string);
    }
    return;
}

stringList *insertStringList(char *parsedData , stringList *dataList){
    
    stringList *node = (stringList *)malloc(sizeof(stringList));
    if(!node){
        printf("Erro ao guardar String");
        exit(1);
    }

    node->string = strdup(parsedData);
    node->next = NULL;

    if(!dataList)
        return node;
    node->next = dataList;
    return node;
}

planList *insertPlanList(char *parsedTrigger, char *parsedCtx, stringList *parsedBody, planList *dataList){
    
    planList *node = (planList *)malloc(sizeof(planList));
    if(!node){
        printf("Erro ao criar plano");
        exit(1);
    }

    //planIndex++; could maybe replace validation...?
    node->planNumber = planIndex;
    node->trigger = parsedTrigger;
    node->ctx = parsedCtx;
    node->body = parsedBody;
    node->next = NULL;

    if(!dataList){
        return node;
    }
    node->next = dataList;
    return node;
}

void validatePlanBody(stringList *parsedBody){
    while(parsedBody){
        if(strncmp(parsedBody->string, "A_", 2) && strncmp(parsedBody->string, "G_", 2) && strncmp(parsedBody->string, "B_", 2)){
            printf("Body invalido");
            exit(1);
        }
        parsedBody = parsedBody->next;
    }
}

void validateAgentIndex(int index){
    if(index != agentIndex+1){
        char err[100];
        snprintf(err, sizeof(err), "Expected ag%d but received ag%d", agentIndex+1, index);
        yyerror(err);
        exit(1);
    } 
    agentIndex++;
}

void validatePlanIndex(int index){
    if(index != planIndex+1){
        char err[100];
        snprintf(err, sizeof(err), "Expected p%d but received p%d", planIndex+1, index);
        yyerror(err);
        exit(1);
    } 
    planIndex++;
}

void restartPlanIndex(){
    planIndex = 0;
}

void yyerror(char *s){
      fprintf(stderr, "error: %s\n", s);
}

stringList *freeStringList(stringList* head){
   stringList* iterator;

   while (head)
    {
       iterator = head;
       head = head->next;
       free(iterator);
    }
    return NULL;
}

planList *freePlanList(planList* head){
   planList* iterator;

   while (head)
    {
       iterator = head;
       head = head->next;
       iterator->body = freeStringList(iterator->body);
       free(iterator);
    }
    return NULL;
}

int main(int argc, char** argv) {
    extern FILE* yyin;
    if (argc > 1) {
        FILE *opened = fopen(argv[1], "r");
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror(argv[1]);
            return 1;
        }
    } else {
        printf(">");
    }

    int r = yyparse();
    if (yyin && yyin != stdin) {
        printf("\n");
        fclose(yyin);
    }
    return r;
}