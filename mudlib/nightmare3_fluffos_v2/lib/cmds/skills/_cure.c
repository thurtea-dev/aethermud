/* 
 * prevented players from being able to cure immortals (thus
 * potentially gaining a lot of experience) Bohemund@Nightmare 940429
 */

#include <std.h>

inherit DAEMON;

void send_messages(object tp, object ob, string Class, string limb);
private void heal_rifts_pools(object ob, int amount);

int cmd_cure(string str) {
    string whom, limb, Class;
    int amount, cost, healing, i, wisdom;
    object tp, ob;
    string * limbs;

    if(!str) {
	notify_fail("Cure whom?\n");
	return 0;
    }
    tp = this_player();
    if(tp->query_casting()) {
	notify_fail("You are in the middle of another spell!\n");
	return 0;
    }
    if(!alignment_ok(this_player())) {
	notify_fail("You have betrayed the source of your powers.\n");
	return 0;
    }
    healing = (int)tp->query_skill("healing");
    Class = (string)tp->query_class();
    if(str == "me" || str == "myself") ob = tp;
    else ob = present(str, environment(tp));
    if(!ob) {
        if(sscanf(str, "%s %s", whom, limb) != 2) {
	    notify_fail("Correct syntax: <cure [who] [(limb)]\n");
    	    return 0;
  	}
	if(whom == "me" || whom == "myself") ob = tp;
	else ob = present(whom, environment(tp));
    }
    if(!ob) {
	notify_fail("Cure whom?\n");
	return 0;
    }
    if(creatorp(ob)) {
      notify_fail(capitalize(nominative(ob))+" does not require curing.\n");
      return 0;
    }
    if(!ob->is_player()) {
	notify_fail("That does not require curing.\n");
	return 0;
    }
    /* Limbs no longer take separate damage (2026-07-19); any limb
       argument is treated as a normal full-body cure. */
    cost = 90;
    if(ob != tp) cost = random(cost)+3;
    if((int)tp->query_mp() < cost) {
	notify_fail("Too low on magic power.\n");
	return 0;
    }
    tp->add_mp(-cost);
    if(healing < 15) {
	notify_fail("You are not skilled enough to do that!\n");
	return 0;
    }
    amount = (healing*((int)ob->query_max_hp()))/100;
    this_player()->set_magic_round();
    if(amount > (int)ob->query_max_hp() - (int)ob->query_hp())
      amount = (int)ob->query_max_hp() - (int)ob->query_hp();
    if(tp == ob) tp->add_skill_points("healing", amount);
    else {
	tp->add_skill_points("healing", amount * 2);
        tp->add_exp(random(amount));
        if((string)tp->query_class()=="monk" && (int)ob->query_alignment()<0)
        amount=amount*-1;
        tp->add_alignment(amount/10);
        if(amount<0) amount=amount*-1;
        if(amount > 200) {
    log_file("hmmm", this_player()->query_name()+" healing "+ob->query_short()+
              " "+amount+"\n");
        amount = 200;
        }
        else tp->add_exp(random(amount));
    }
    ob->add_hp(amount);
    heal_rifts_pools(ob, amount);
    send_messages(tp, ob, Class, 0);
    return 1;
}

/*  Rifts characters track wounds in rifts_hp, not the NM3 hp pool;
    heal both so cure works for everyone (2026-07-19).  */
private void heal_rifts_pools(object ob, int amount) {
    int cur, max;

    max = (int)ob->query_stats("max_rifts_hp");
    if(max < 1) return;
    cur = (int)ob->query_stats("rifts_hp");
    if(cur >= max) return;
    cur += amount;
    if(cur > max) cur = max;
    ob->set_stats("rifts_hp", cur);
}

void send_messages(object tp, object ob, string Class, string limb) {
    string what, thing;

    if(Class == "monk" || Class == "cleric") {
	thing = "prayer";
	what = "a prayer of healing";
    }
    else if(Class == "mage") {
	thing = "magic";
	what = "a spell of healing";
    }
    else {
	thing = "nonsense";
 	what = "nonsense";
    }
    if(tp == ob) {
	tell_object(tp, "You cure your own wounds through "+thing+".");
	tell_room(environment(tp), tp->query_cap_name() + " mutters "+what+".", ({tp}));
    }
    else {
	tell_object(tp, "You cure "+ob->query_cap_name()+"'s wounds through "+thing+".");
	tell_object(ob, tp->query_cap_name()+" cures your wounds through "+thing+".");
	tell_room(environment(tp), tp->query_cap_name()+" mutters "+what+".", ({tp, ob}));
    }
}

void help() {
    write("Syntax: <cure [player] [(limb)]>\n\n"+
	"Cures the player named to a degree depending on your skill level.  If no\n"+
	"limb is given, it heals hp.  Otherwise it heals the limb named.  The arg \"all\"\n"+
	"is a valid arg for the limb.\n");
}
