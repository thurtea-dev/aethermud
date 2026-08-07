// /domains/adm/wiz_tools/tattoo_gun.c
// Tattoo Gun -- RP Wizard tool for Atlantean/Tattooed Man tattoo management.
// Uses the current location-based TATTOO_D API.
// Usage: use gun / use tattoo gun (numbered menu), or the direct verb:
//   tattoo <player> list                    -- tattoos they currently have
//   tattoo <player> available               -- tattoos they're eligible for
//   tattoo <player> <location> <tattoo name> -- apply directly
// The direct verb is an additive shortcut around the exact same TATTOO_D
// calls the menu makes (grant_tattoo/query_tattoos/query_tattoo_def) --
// it does not duplicate or bypass any validation, so a direct grant and
// a menu grant of the same tattoo/location/player produce an identical
// result. The numbered menu (tattoo_menu_d.c) is untouched and still
// works exactly as before.
// The actual menu logic lives in tattoo_menu_d.c (a shared, never-cloned
// daemon) so ring_of_dominion.c can drive the same menu without
// duplicating it. This file only owns identity, wear/carry lifecycle,
// and the "inscribe"/"tattoo" verbs.

#include <std.h>
#include <daemons.h>

inherit OBJECT;

private int tattoo_wizard_ok(object who);
private string match_location_prefix(string str, string *locs);
private void cmd_tattoo_list(object target);
private void cmd_tattoo_available(object target);

void create() {
    ::create();
    set_name("tattoo-gun");
    set_id( ({ "tattoo-gun", "tattoo gun", "gun", "tattoo" }) );
    set_short("a tattoo-gun");
    set_long(
        "A high-precision Atlantean tattoo device. It uses refined PPE to\n"
        "inscribe magical tattoos that grant real powers to True Atlanteans\n"
        "and Tattooed Men. For RP Wizard use only.\n"
        "Type: inscribe (menu), or tattoo <player> list|available|<location>\n"
        "<tattoo name> for the direct form.");
    set_mass(200);
    set_value(0);
    set_property("no_give", 1);
}

mixed *query_auto_load() {
    return ({ base_name(this_object()), ({}) });
}

void init() {
    ::init();
    if(environment(this_object()) != this_player()) return;
    add_action("cmd_use_gun", "inscribe");
    add_action("cmd_tattoo_direct", "tattoo");
}

int cmd_use_gun(string str) {
    return (int)"/domains/adm/wiz_tools/tattoo_menu_d"->begin_menu(this_player());
}

/* Same gate tattoo_menu_d.c's begin_menu() uses -- kept identical so the
   direct verb and the menu are equally restricted. */
private int tattoo_wizard_ok(object who) {
    if(!admin_wizp(who) && !rp_wizp(who) && !domain_wizp(who)) {
        write("The gun does not respond to you.\n");
        return 0;
    }
    if(!creatorp(who)) {
        write("The gun does not respond to you.\n");
        return 0;
    }
    return 1;
}

/* Locations are a small, fixed vocabulary where none is a prefix of
   another ("right hand" vs "right forearm" vs "right upper arm" etc all
   diverge immediately after "right "), so matching the longest valid
   location that prefixes the remaining string, followed by a space or
   end of string, is unambiguous -- no separator between location and
   tattoo name is needed. */
private string match_location_prefix(string str, string *locs) {
    int i, llen, slen;
    string loc;

    slen = strlen(str);
    for(i = 0; i < sizeof(locs); i++) {
        loc  = locs[i];
        llen = strlen(loc);
        if(slen < llen) continue;
        if(str[0..llen-1] != loc) continue;
        if(slen == llen || str[llen] == ' ') return loc;
    }
    return 0;
}

int cmd_tattoo_direct(string str) {
    string pname, rest, sub, location, tattoo_name;
    object target;
    string *locs;

    /* Bare `tattoo` opens the menu rather than falling through, so the
       menu stays reachable from either verb (2026-07-26). `inscribe`
       still opens it too; nothing about the menu changed. */
    if(!str || !sizeof(str)) return cmd_use_gun(0);
    if(sscanf(str, "%s %s", pname, rest) != 2) {
        write("Syntax: tattoo <player> list | available | <location> <tattoo name>\n"
              "        tattoo             (no argument) opens the menu\n");
        return 1;
    }
    if(!tattoo_wizard_ok(this_player())) return 1;

    target = find_player(lower_case(pname));
    if(!target) {
        write("Player '" + capitalize(pname) + "' not found online.\n");
        return 1;
    }

    sub = lower_case(rest);
    if(sub == "list") {
        cmd_tattoo_list(target);
        return 1;
    }
    if(sub == "available") {
        cmd_tattoo_available(target);
        return 1;
    }

    rest = lower_case(rest);
    locs = (string *)TATTOO_D->query_all_locations();
    location = match_location_prefix(rest, locs);
    if(!location) {
        write("Unknown body location. Valid: " + implode(locs, ", ") + "\n");
        return 1;
    }
    if(strlen(rest) <= strlen(location) + 1) {
        write("Syntax: tattoo <player> <location> <tattoo name>\n");
        return 1;
    }
    tattoo_name = rest[strlen(location) + 1..];
    if(!(mapping)TATTOO_D->query_tattoo_def(tattoo_name)) {
        write("Unknown tattoo: " + tattoo_name + "\n");
        return 1;
    }

    /* grant_tattoo() writes its own failure reason (bad slot, already
       occupied, no free slots, etc) via this_player() -- which is this
       calling wizard -- so failures need nothing more here. Success is
       the one path it does NOT message the caster for (it only tells
       the target), matching tattoo_menu_d.c's pick_grant_location(),
       whose confirmation text this mirrors exactly. */
    if((int)TATTOO_D->grant_tattoo(target, tattoo_name, location)) {
        write("Inscribed " + tattoo_name + " on " + location + " for " +
              (string)target->query_cap_name() + ".\n");
        tell_object(target,
            "A " + tattoo_name + " tattoo is inscribed on your " +
            location + ".\n");
    }
    return 1;
}

private void cmd_tattoo_list(object target) {
    mapping tattoos, def;
    string *locs;
    string name;
    int i;

    tattoos = (mapping)TATTOO_D->query_tattoos(target);
    locs    = keys(tattoos);
    write("Tattoos for " + (string)target->query_cap_name() + ":");
    if(!sizeof(locs)) {
        write("  (none)");
        return;
    }
    for(i = 0; i < sizeof(locs); i++) {
        name = tattoos[locs[i]];
        def  = (mapping)TATTOO_D->query_tattoo_def(name);
        write(sprintf("  %-20s : %-22s  [%s]",
            locs[i], name, def ? (string)def["type"] : "?"));
    }
}

/* "Eligible" = True Atlantean race (tattoo magic is innate to the race
   in Palladium canon), or an OCC whose occ.c definition carries the
   "magic_tattoos" flag (Tattooed Man, Tattoo Warrior, Sunaj Assassin --
   setter.c copies occ.c's "flags" array into rifts_occ_flags verbatim
   at chargen, so this reads the same flag the OCC was actually built
   with, not a hardcoded OCC name list). Sunaj-only tattoos (currently
   just a warning in grant_tattoo(), not enforced) are filtered out of
   this list for anyone whose OCC isn't literally Sunaj Assassin, since
   this is a "what can I actually get" view, not a "what will the gun
   let me force through" one. */
private void cmd_tattoo_available(object target) {
    mapping all, def;
    string *names;
    string race, occflags, occ;
    int eligible, i;

    race     = lower_case((string)target->query_race());
    occflags = (string)target->getenv("rifts_occ_flags");
    occ      = lower_case((string)target->getenv("rifts_occ"));
    eligible = (race == "atlantean") ||
        (occflags && sizeof(occflags) &&
         member_array("magic_tattoos", explode(occflags, ",")) != -1);

    write("Tattoo eligibility for " + (string)target->query_cap_name() +
          " (race: " + capitalize(race) + ", occ: " +
          ((occ && sizeof(occ)) ? capitalize(occ) : "none") + "):");
    if(!eligible) {
        write("  Not eligible for tattoo magic (needs Atlantean race or a\n"
              "  magic-tattoo OCC: Tattooed Man, Tattoo Warrior, Sunaj Assassin).");
        return;
    }

    all   = (mapping)TATTOO_D->query_all_tattoos();
    names = sort_array(keys(all), 1);
    for(i = 0; i < sizeof(names); i++) {
        def = all[names[i]];
        if((int)def["sunaj_only"] && occ != "sunaj assassin") continue;
        write(sprintf("  %-22s %s [%s, PPE:%d] locations: %s",
            names[i],
            def ? (string)def["desc"] : "?",
            def ? (string)def["type"] : "?",
            def ? (int)def["ppe"]     : 0,
            implode((string *)def["locations"], ", ")));
    }
}
