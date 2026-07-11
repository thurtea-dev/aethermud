/* /cmds/mortal/_improve.c
   Spend a pending skill slot to improve a known skill.
   Syntax: improve <skill> */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private string match_player_skill(object player, string arg) {
    string *skills;
    string key;
    string *matches;
    int i;

    key = (string)RIFTS_SKILLS_D->normalize_skill(arg);
    if(!key || !sizeof(key)) key = lower_case(arg);
    skills = (string *)RIFTS_SKILLS_D->query_player_skills(player);
    if(!skills) return 0;
    matches = ({});
    for(i = 0; i < sizeof(skills); i++) {
        if((int)player->query_skill(skills[i]) <= 0) continue;
        if(skills[i] == key) return skills[i];
        if(strsrch(skills[i], key) != -1) matches += ({ skills[i] });
    }
    if(sizeof(matches) == 1) return matches[0];
    return 0;
}

int cmd_improve(string str) {
    object player;
    string matched;
    int pending;
    int new_pct;

    player = this_player();
    if(!str || !sizeof(str)) {
        write("Improve which skill?  Syntax: improve <skill>\n");
        return 1;
    }
    if(!(int)RIFTS_D->is_rifts_race((string)player->query_race())) {
        notify_fail("That command is for Rifts characters only.\n");
        return 0;
    }
    pending = (int)player->query_property("pending_skill_slots");
    if(pending <= 0) {
        write("You have no pending skill slots. Advance a level to earn more.\n");
        return 1;
    }
    matched = match_player_skill(player, str);
    if(!matched) {
        write("You do not have that skill, or the name matches more than one skill.\n");
        return 1;
    }
    new_pct = (int)RIFTS_SKILLS_D->improve_skill(player, matched);
    if(new_pct <= 0) {
        write("That skill cannot be improved further with a slot.\n");
        return 1;
    }
    player->set_property("pending_skill_slots", pending - 1);
    write(sprintf("You improve %s to %d%%. Pending skill slots: %d\n",
        capitalize(matched), new_pct, pending - 1));
    return 1;
}

void help() {
    write(
        "Syntax: improve <skill>\n\n"
        "Spend one pending skill slot to raise a known skill by its\n"
        "normal per-level amount. Earn slots when you advance a level.\n\n"
        "See also: skills, advance, score\n"
    );
}
