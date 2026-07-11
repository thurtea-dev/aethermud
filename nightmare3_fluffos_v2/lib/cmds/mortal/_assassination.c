// /cmds/mortal/_assassination.c
// Rifts assassination strike: prowl/sneak ambush with a blade.
// Syntax: assassination <target>
// Alias: assassinate <target> (via add_action in init if needed - also cmd_assassinate)

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private int has_assassin_training(object player) {
    string occ;
    string flags;

    occ = lower_case((string)player->getenv("rifts_occ"));
    if(occ == "master assassin" || occ == "sunaj assassin" ||
       occ == "professional thief" || occ == "freelance spy" ||
       occ == "forger" || occ == "headhunter" || occ == "bounty hunter")
        return 1;
    flags = lower_case((string)player->getenv("rifts_occ_flags"));
    if(flags && strsrch(flags, "assassin") != -1)
        return 1;
    if((int)player->query_skill("prowl") >= 40 &&
       (int)player->query_skill("wp knife") >= 30)
        return 1;
    return 0;
}

private object find_blade(object player) {
    object *wielded;
    string wtype;
    int i;

    wielded = (object *)player->query_wielded();
    if(!wielded || !sizeof(wielded)) return 0;
    for(i = 0; i < sizeof(wielded); i++) {
        if(!wielded[i]) continue;
        wtype = lower_case((string)wielded[i]->query_type());
        if(wtype == "knife" || wtype == "blade" ||
           (int)wielded[i]->query_property("assassin_blade"))
            return wielded[i];
    }
    return 0;
}

private int stealth_ok(object player) {
    int prowl;
    int roll;
    int level;

    if((int)player->query_property("is_sneaking")) return 1;
    if((int)player->query_property("shadow_meld")) return 1;
    if((int)player->query_property("invisible")) return 1;
    prowl = (int)player->query_skill("prowl");
    if(prowl <= 0) return 0;
    level = (int)player->query_level();
    if(level < 1) level = 1;
    prowl += (level - 1) * 5;
    if(prowl > 98) prowl = 98;
    roll = random(100) + 1;
    return (roll <= prowl);
}

private int occ_assassin_bonus(object player) {
    string occ;

    occ = lower_case((string)player->getenv("rifts_occ"));
    if(occ == "master assassin" || occ == "sunaj assassin")
        return 8;
    if(occ == "professional thief" || occ == "freelance spy")
        return 4;
    return 0;
}

int cmd_assassination(string str) {
    object victim;
    object blade;
    object here;
    object *obs;
    string vic_name;
    string att_name;
    int damage;
    int prowl;
    int knife;
    int bonus;
    int i;
    int ret;

    if(!str || str == "") {
        notify_fail("Assassinate whom?  Syntax: assassination <target>\n");
        return 0;
    }
    if(this_player()->query_ghost()) {
        notify_fail("You cannot do that while dead.\n");
        return 0;
    }
    if(environment(this_player())->query_property("no attack")) {
        notify_fail("You may not attack here.\n");
        return 0;
    }
    if(!has_assassin_training(this_player())) {
        notify_fail("You lack the training for an assassination strike.\n");
        return 0;
    }
    blade = find_blade(this_player());
    if(!blade) {
        write("You need a knife or blade wielded for an assassination.\n");
        return 1;
    }
    here = environment(this_player());
    victim = present(lower_case(str), here);
    if(!victim || !living(victim)) {
        notify_fail("You don't see that target here.\n");
        return 0;
    }
    if(victim == this_player()) {
        notify_fail("Assassinate yourself?\n");
        return 0;
    }
    if(victim->query_ghost()) {
        notify_fail("Your target is already dead.\n");
        return 0;
    }
    if((int)victim->query_property("no backstab")) {
        notify_fail("That target cannot be assassinated.\n");
        return 0;
    }
    if((object)this_player()->query_current_attacker()) {
        notify_fail("You are already in combat.\n");
        return 0;
    }
    if(member_array(victim, (object *)this_player()->query_attackers()) != -1) {
        notify_fail("They are already fighting you.\n");
        return 0;
    }
    if(!stealth_ok(this_player())) {
        write("You fail to get the drop on your target.\n");
        tell_object(victim,
            (string)this_player()->query_cap_name() +
            " lunges at you, but you see the attack coming!\n");
        if(!userp(victim) || interactive(victim))
            victim->kill_ob(this_player(), 0);
        return 1;
    }

    ret = (int)this_player()->kill_ob(victim, 0);
    if(!ret) return 1;

    vic_name = (string)victim->query_cap_name();
    att_name = (string)this_player()->query_cap_name();
    prowl = (int)this_player()->query_skill("prowl");
    knife = (int)this_player()->query_skill("wp knife");
    if(knife <= 0) knife = (int)this_player()->query_skill("wp vibroblade");
    bonus = occ_assassin_bonus(this_player());
    damage = random(6) + 1 + random(6) + 1;
    damage += prowl / 10;
    damage += knife / 10;
    damage += bonus;
    if(damage < 3) damage = 3;

    message("my_action",
        "%^BOLD%^%^RED%^You slip from the shadows and drive " +
        (string)blade->query_short() + " into " + vic_name + "!%^RESET%^",
        this_player());
    tell_object(victim,
        "%^BOLD%^%^RED%^" + att_name +
        " assassinates you from the shadows!%^RESET%^");
    tell_room(here,
        "%^BOLD%^%^RED%^" + att_name + " assassinates " + vic_name +
        "!%^RESET%^",
        ({ this_player(), victim }));
    this_player()->remove_property("is_sneaking");
    RIFTS_COMBAT_D->apply_rifts_damage(this_player(), victim, damage);
    this_player()->add_alignment(-3);
    return 1;
}

void help() {
    write(
        "Syntax: assassination <target>\n"
        "        assassinate <target>\n\n"
        "A stealth killing blow with a wielded knife or blade.\n"
        "Requires assassin training (OCC or high Prowl + W.P. Knife).\n"
        "You must be sneaking, hidden, or succeed a Prowl check.\n"
        "Breaks stealth and initiates combat on failure or success.\n\n"
        "See also: sneak, kill, backstab\n"
    );
}
