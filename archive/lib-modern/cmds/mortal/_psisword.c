// /cmds/mortal/_psisword.c
// Syntax: psisword  (toggle on/off)
// Cyber-Knight only. Manifests a blade of psychic energy. Costs 10 ISP.

#include <std.h>
#include <daemons.h>

inherit DAEMON;

int cmd_psisword(string str) {
    string occ;
    int isp;
    int level;
    int dur;

    occ = lower_case((string)this_player()->getenv("rifts_occ"));
    if(!occ) occ = "";
    if(strsrch(occ, "cyber-knight") == -1 &&
       strsrch(occ, "cyber knight") == -1) {
        write("Only Cyber-Knights can manifest a psi-sword.\n");
        return 1;
    }

    if((int)this_player()->query_property("psi_sword_active")) {
        this_player()->set_property("psi_sword_active", 0);
        write("The psi-sword fades.\n");
        say(this_player()->query_cap_name() +
            "'s shimmering energy blade dissipates.");
        return 1;
    }

    isp = (int)this_player()->query_stats("ISP");
    if(isp < 10) {
        write("Not enough ISP to manifest the psi-sword. "
              "(Need 10, have " + isp + ".)\n");
        return 1;
    }

    this_player()->set_stats("ISP", isp - 10);
    this_player()->set_property("psi_sword_active", 1);
    this_player()->set_property("psi_sword_damage", "2d6");

    level = (int)this_player()->query_level();
    if(level < 1) level = 1;
    dur = level * 300;

    write("A blade of psychic energy crackles to life in your hand, "
          "visible only as a shimmer in the air.\n"
          "Damage: 2d6 MDC. Duration: " + (dur / 60) + " minutes.\n");
    say(this_player()->query_cap_name() +
        " extends a hand as a faint energy blade materializes from their will.");

    call_out("end_psi_sword", dur, this_player());
    return 1;
}

void end_psi_sword(object who) {
    if(!objectp(who)) return;
    if(!(int)who->query_property("psi_sword_active")) return;
    who->set_property("psi_sword_active", 0);
    tell_object(who, "The psi-sword fades.\n");
}

void help() {
    write(
        "Syntax: psisword\n\n"
        "Cyber-Knight only. Toggle a blade of psychic energy in your dominant hand.\n"
        "  Cost: 10 ISP\n"
        "  Damage: 2d6 MDC (penetrates MDC targets)\n"
        "  Duration: 5 minutes per level\n"
        "  Use 'psisword' again to deactivate early.\n\n"
        "The psi-sword works only while unarmed (no weapon wielded).\n"
    );
}
