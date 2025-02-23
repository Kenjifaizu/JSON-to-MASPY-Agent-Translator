#ifndef TRADUTOR_HEADER
#define TRADUTOR_HEADER


typedef struct stringList{
	char *string;
	struct stringList *next;
}stringList;

typedef struct planList{
	int planNumber;
	char *trigger;
	char *ctx;
	stringList *body;
	struct planList *next;
}planList;

void generateAgent(char *parsedAgentName, stringList *parsedBeliefs, char *parsedGoal, planList *parsedPlans);
stringList *insertStringList(char *parsedData , stringList *dataList);
planList *insertPlanList(char *parsedTrigger, char *parsedCtx, stringList *parsedBody, planList *dataList);
void writeBeliefs(stringList *parsedBeliefs, FILE *output);
void writePlans(planList *parsedPlans, stringList **parsedGoals, stringList **parsedBeliefs, FILE *output);
void validatePlanBody(stringList *parsedBody);
void validateAgentIndex(int index);
void validatePlanIndex(int index);
void validateKnowledge(planList *parsedPlans, stringList **parsedGoals, stringList **parsedBeliefs);
int searchStringList(char *target, stringList *arr);
void restartPlanIndex();
void yyerror(char *);
stringList *freeStringList(stringList* head);
planList *freePlanList(planList* head);
void writeConfigElements();
void writeAgents(FILE *output, int i, stringList *iterator);

extern FILE* yyin;
extern int yyparse();

#endif
