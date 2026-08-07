/*	_peace.c
 *	Monk and cleric ability
 *	created by Descartes 14 November 1992
 *      modified by Bohemund 9 March 1994
 *        - You automatically stop yourself from attacking
 *        - Removed unnecessary parameter
 *        - Added ansi; neatened output strings
 *        - Chance per monster of making it stop attacking. The
 *          monster's level and aggressiveness are taken into
 *          account. It costs as much as each living thing's level.
 */

#include <std.h>

inherit DAEMON;

int cmd_peace() {
    object *inv, who;
    string flags;
    int isp, i, agg, level;

    who = this_player();
    if(!creatorp(who)) {
        flags = (string)who->getenv("rifts_occ_flags");
        if(!stringp(flags)) flags = "";
        if(strsrch(lower_case(flags), "psychic") == -1)
            return notify_fail("Your training doesn't include psychic pacification.\n");
    }
    isp = (int)who->query_stats("ISP");
    if (!creatorp(who) && isp < 10) {
	notify_fail("Not enough ISP to project a calming field.\n");
	return 0;
    }
  if(!creatorp(who) && !alignment_ok(who)) {
    notify_fail("You have betrayed the source of your power.\n");
    return 0;
  }
  if (!creatorp(who)) who->set_stats("ISP", isp - 10);
  if (!creatorp(who)) who->add_alignment(5);
  inv = all_inventory(environment(who));
  write("You call for peace.");
  say((string)who->query_cap_name()+" calls for peace.");
  who->cease_all_attacks();

  for(i=0; i<sizeof(inv); i++) {
    if(!living(inv[i])) continue;
    if(inv[i] == who) continue;
    agg = 10;
    level = (int)inv[i]->query_level();
    if(!creatorp(who) && (random(agg) + level) > random(isp)) continue;
    inv[i]->cease_all_attacks();
  }
  return 1;
}

void help() {
  write("Syntax: <peace>\n"
        "Projects a psychic calming field over the area, urging nearby "
	"combatants to break off their attacks. Costs 10 ISP. Each "
	"opponent's chance to respond depends on your ISP reserve "
	"against their level.\n");
}
