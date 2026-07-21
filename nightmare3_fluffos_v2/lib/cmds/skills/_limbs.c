/* /cmds/skills/_limbs.c
   A medical skill to sense the body condition of others.
   Rewritten 2026-07-19 for the whole_body damage model: limbs no
   longer carry damage, so this senses overall condition and any
   missing (severed) limbs instead of per-limb damage. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private string condition_word(int pct) {
    if(pct >= 95) return "in excellent condition";
    if(pct >= 75) return "lightly wounded";
    if(pct >= 50) return "wounded";
    if(pct >= 25) return "heavily wounded";
    if(pct > 0)   return "near death";
    return "at death's door";
}

int cmd_limbs(string str){
   string name, race;
   string *missing;
   object ob, tp;
   int cur, max, pct, i;

   if(!str){
     notify_fail("Correct syntax: limbs [whom]\n");
     return 0;
   }

   tp=this_player();

   if((int)tp->query_skill("medical doctor") < 1){
     notify_fail("You do not have the medical skill to do that.\n");
     return 0;
   }

   ob=find_living(str);
   if(!ob || !present(ob,environment(this_player()))){
     notify_fail(capitalize(str)+" is not here.\n");
     return 0;
   }

   if(ob->query_ghost()){
     notify_fail("That player has no limbs at all!\n");
     return 0;
   }

   say(tp->query_cap_name()+" examines the injuries carefully.");

   tp->add_skill_points("medical doctor",10);
   name=(string)ob->query_cap_name();
   race=(string)ob->query_race();
   if((int)RIFTS_D->is_mdc_race(race) ||
      (int)ob->query_property("mdc_creature")) {
     cur = (int)ob->query_stats("MDC");
     max = (int)ob->query_stats("max_MDC");
   }
   else if((int)ob->query_stats("max_rifts_hp") > 0) {
     cur = (int)ob->query_stats("rifts_hp");
     max = (int)ob->query_stats("max_rifts_hp");
   }
   else {
     cur = (int)ob->query_hp();
     max = (int)ob->query_max_hp();
   }
   if(max < 1) max = 1;
   if(cur < 0) cur = 0;
   pct = (100*cur)/max;
   write("You get the sense that "+name+" is "+condition_word(pct)+".");
   missing=(string *)ob->query_severed_limbs();
   if(missing && sizeof(missing)) {
     for(i=0; i<sizeof(missing); i++)
       write("You sense that "+name+" is missing "+
         ob->query_possessive()+" "+missing[i]+".");
   }
   return 1;
}

void help(){
    write("syntax: limbs [whom]\nThis allows a medical professional to "
          "examine another's body condition, including any limbs they "
          "are missing.");
}
