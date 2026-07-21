// /domains/ChiTown/npcs/cs_rift_liaison.c
// Coalition intelligence liaison at Chi-Town gate. Quest giver for chi_town_rift_report.

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("CS rift liaison");
    set_npc_name("the CS rift liaison");
    set_id(({ "cs rift liaison", "rift liaison", "liaison", "intelligence officer",
              "officer" }));
    set_short("a CS rift liaison");
    set_long(
        "A Coalition intelligence officer in gray dress uniform, CA-4 sidearm\n"
        "holstered but accessible. A data-slate shows ley line maps centered on\n"
        "Praxis. They watch the gate traffic with professional boredom.");
    set_level(5);
    set_max_hp(70);
    set_hp(70);
    set_aggressive(0);
    set_moving(0);
    set_gender(random(2) ? "male" : "female");
    set_race("human");
    set_alignment(-200);
    set_body_type("human");

    add_response("hello",
        "the CS rift liaison says: Coalition business only. State your purpose.");
    add_response("praxis",
        "the CS rift liaison says: Our sensors flagged a dimensional anomaly on the Praxis ley nexus.");
    add_response("rift",
        "the CS rift liaison says: I need eyes on that tear. Locate it, step through, and report back.");
    add_response("work", "");
    add_response("job", "");
    add_response("quest", "");
    add_response("report", "");
    set_default_response(
        "The liaison taps notes into a data-slate without looking up.");
}

void catch_tell(string str) {
    string a, b;
    object tp;

    if(sscanf(str, "%s says: %s", a, b) != 2) return;
    b = lower_case(b);

    if(strsrch(b, "work") != -1 || strsrch(b, "job") != -1 ||
       strsrch(b, "quest") != -1 || strsrch(b, "rift") != -1) {
        tp = this_player();
        if(tp) call_out("offer_rift_report", 1, tp);
        return;
    }

    if(strsrch(b, "report") != -1 || strsrch(b, "confirm") != -1 ||
       strsrch(b, "done") != -1) {
        tp = this_player();
        if(tp) call_out("try_complete_report", 1, tp);
    }
}

void offer_rift_report(object tp) {
    if(!tp || !objectp(tp)) return;

    if(WANTED_D->is_cs_enemy(tp)) {
        tell_object(tp,
            "The liaison says: Coalition clearance denied. Leave the gate.\n");
        return;
    }
    if((int)QUEST_D->is_quest_complete(tp, "chi_town_rift_report")) {
        tell_object(tp,
            "The liaison says: Command has the Praxis tear on file. Stand down.\n");
        return;
    }
    if((int)QUEST_D->is_quest_active(tp, "chi_town_rift_report")) {
        tell_object(tp,
            "The liaison says: Locate the tear below the Praxis nexus, cross into the sliver, "
            "then report here.\n");
        return;
    }
    tell_object(tp,
        "The liaison says: Praxis ley sensors show a live demon-plane sliver. "
        "Find the dimensional tear below the nexus, confirm entry, and report back. "
        "Accepting assignment now.\n");
    catch(QUEST_D->start_quest(tp, "chi_town_rift_report"));
}

void try_complete_report(object tp) {
    int completed;

    if(!tp || !objectp(tp)) return;
    if(!(int)QUEST_D->is_quest_active(tp, "chi_town_rift_report")) {
        tell_object(tp,
            "The liaison says: I have no open assignment for you.\n");
        return;
    }
    if(!(int)QUEST_D->is_objective_complete(tp, "chi_town_rift_report", "tear_located")) {
        tell_object(tp,
            "The liaison says: You have not confirmed the tear location yet.\n");
        return;
    }
    if(!(int)QUEST_D->is_objective_complete(tp, "chi_town_rift_report",
            "demon_sliver_entered")) {
        tell_object(tp,
            "The liaison says: I need confirmation you crossed into the sliver.\n");
        return;
    }

    completed = (int)QUEST_D->complete_quest(tp, "chi_town_rift_report");
    if(completed) {
        tell_object(tp,
            "The liaison files your report and nods once. Coalition standing improved.\n");
        catch(FACTION_D->modify_standing((string)tp->query_name(),
            "coalition", 10, "Praxis demon-plane tear confirmed"));
    }
}
