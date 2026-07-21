// 	_replace.c
//	Medical skill for restoring severed limbs
//	created 02 November 1992 by Descartes of Borg

#include <std.h>

inherit DAEMON;

int do_add(object ob, string limb);

int cmd_replace(string str) {
    string who, limb;
    string *missing;
    string *limbs;
    int medskill;
    object tp, ob;

    if(this_player()->query_ghost()) {
        notify_fail("You cannot do that in your immaterial state.\n");
        return 0;
    }
    if(!str) {
        notify_fail("Replace what?\n");
	return 0;
    }
    tp = this_player();
    if((int)tp->query_skill("medical doctor") < 1) {
	notify_fail("You do not have the medical skill to do that.\n");
	return 0;
    }
    if(sscanf(str, "%s %s", who, limb) != 2) {
	notify_fail("Correct syntax: <replace [player] [limb]>\n");
	return 0;
    }
    ob = present(who, environment(tp));
    if(!ob) {
        notify_fail(capitalize(who) + " is not here.\n");
	return 0;
    }
    if(ob->query_ghost()) {
        notify_fail("You cannot help the dead in that way.\n");
	return 0;
    }
    if(!living(ob)) {
	notify_fail(ob->query_cap_name()+" is not a living thing!\n");
	return 0;
    }
    limbs = (string *)ob->query_limbs();
    missing = (string *)ob->query_severed_limbs();
    if(!missing) {
	if(ob==tp) notify_fail("You are not missing any limbs!\n");
	else notify_fail(ob->query_cap_name()+" is not missing any limbs.\n");
	return 0;
    }
    if(member_array(limb, missing) == -1) {
	if(tp==ob) notify_fail("You are not missing that limb!\n");
	else notify_fail(ob->query_cap_name()+" is not missing that limb!\n");
	return 0;
    }
    if(member_array(limb, limbs) != -1) {
	if(tp==ob) notify_fail("You are not missing that limb!\n");
	else notify_fail(ob->query_cap_name()+" is not missing that limb.\n");
	return 0;
    }
    medskill = (int)tp->query_skill("medical doctor");
    if(!do_add(ob, limb)) {
	notify_fail("A required limb is missing for that!\n");
	return 0;
    }
    if(tp==ob) {
	write("Your "+limb+" regenerates!\n");
	say(tp->query_cap_name()+"'s "+limb+" regenerates!\n", tp);
	tp->add_skill_points("medical doctor", random(medskill));
	tp->add_exp(random(medskill));
    }
    else {
	write("You regenerate "+ob->query_cap_name()+"'s "+limb+"!\n");
	tell_object(ob, tp->query_cap_name()+" regenerates your "+limb+"!\n");
	tell_room(environment(tp), tp->query_cap_name()+" works to "+
	    "regenerate "+ob->query_cap_name()+"'s "+limb+".\n", ({ tp, ob }) );
	tp->add_skill_points("medical doctor", medskill);
	tp->add_exp(medskill);
    }
    return 1;
}

int do_add(object ob, string limb) {
/*  Since 2026-07-19 limb restoration lives in body.c restore_limb(),
    which rebuilds the limb from the race body table.  */
    return (int)ob->restore_limb(limb);
}

void help() {
    write("Syntax: <replace [player] [limb]>\n\n"+
	"Uses medical skill to regenerate the missing limb of a player.\n");
}
