# JSON-to-MASPY-Agent-Translator
Translates agents in a JSON format to the MASPY Python Framework format

MASPY: https://github.com/laca-is/MASPY.git

The JSON input file format is as follow:

{ 
  "agentCode" : {
    "ag1" : "agent1",
    "beliefs" : [ "belief1" ],
    "goal" : "goal1",
    "plans" : { 
      "p1" : { 
        "trigger" : "goal1", 
        "ctx" : "belief",
        "body" : [ "A_new_action", "B_new_belief", "G_new_goal" ]
      },
      "p2" : { 
        "trigger" : "novo_objetivo", 
        "ctx" : "nova_crenca",
        "body" : [ "A_obtive_um_novo_objetivo", "A_obtive_uma_nova_crenca", "G_finalizar" ]
      }
    }
  }
}

Where multiple agents and plans can be defined.
