/* /cmds/skills/_limbs.c
   A cleric, monk, kataan prayer to sense the body condition of others.
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
   int x, cur, max, pct, i;

   if(!str){
     notify_fail("Correct syntax: limbs [whom]\n");
     return 0;
   }

   tp=this_player();

   if((string)tp->query_class()!="kataan" && (string)tp->query_class()!="monk"
      && (string)tp->query_class()!="cleric"){
        notify_fail("Only people of high faith may perform this action.\n");
        return 0;
   }

   x=(int)this_player()->query_skill("faith");
   if(x<20){
     notify_fail("You are not yet faithful enough to do that.\n");
     return 0;}

   if((int)tp->query_mp()<20){
     notify_fail("You do not have the magic strength to do that right now.\n");
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

   tp->add_mp(-20);
   if((string)tp->query_class()=="kataan")
     say(tp->query_cap_name()+" begins to pray to the demons of the "
         "underworld for guidance.");
   else say(tp->query_cap_name()+" utters a request for divine guidance.");

   if(x-random(51)<0){
     write("Your prayers for guidance are not heard.");
     return 1;
   }
   tp->add_skill_points("faith",10);
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
    write("syntax: limbs [whom]\nThis allows the faithful to offer a prayer "
          "for guidance in determining the condition of another's body, "
          "including any limbs they are missing.");
}
