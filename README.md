# JSON-to-MASPY-Agent-Translator
Translates agents in a JSON format to the MASPY Python Framework format

MASPY: https://github.com/laca-is/MASPY.git

The JSON input file format is as follow:

[Uploading inpu{ 
  "agentCode" : {
    "ag1" : "agent1",
    "beliefs" : [ "belief1" ],
    "goal" : "goal1",
    "plans" : { 
      "p1" : { 
        "trigger" : "goal1", 
        "ctx" : "belief",
        "body" : [ "A_new_action", "B_new_belief", "G_new_goal" ]
      }
    }
  }
}
t1.json…]()


Where multiple agents and plans can be defined.
