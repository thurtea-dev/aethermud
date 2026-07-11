/* /cmds/mortal/_saving_throws.c
   Display Palladium-style saving throw values derived from player stats.
   Syntax: saving throws (also aliased as: saves) */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private int me_bonus(int me) {
    if(me >= 24) return 6;
    if(me >= 22) return 5;
    if(me >= 20) return 4;
    if(me >= 18) return 3;
    if(me >= 16) return 2;
    if(me >= 14) return 1;
    return 0;
}

private int ma_hf_bonus(int ma) {
    if(ma >= 20) return 4;
    if(ma >= 16) return 3;
    if(ma >= 12) return 2;
    if(ma >= 10) return 1;
    return 0;
}

int cmd_saving_throws(string str) {
    int me, ma, pe, level;
    int vs_magic, vs_psionics, vs_poison, vs_insanity, vs_hf;
    int bonus;

    if(this_player()->query_ghost()) {
        notify_fail("You cannot do that in an immaterial state.\n");
        return 0;
    }
    if(!(int)RIFTS_D->is_rifts_race(
            (string)this_player()->query_race())) {
        write("Saving throws only apply to Rifts characters.\n");
        return 1;
    }

    me    = (int)this_player()->query_stats("ME");
    ma    = (int)this_player()->query_stats("MA");
    pe    = (int)this_player()->query_stats("PE");
    level = (int)this_player()->query_level();
    bonus = me_bonus(me);

    vs_magic    = 12 - bonus;
    vs_psionics = 10 - bonus;
    vs_poison   = 14 - (pe / 5);
    vs_insanity = 12 - bonus;
    vs_hf       = 12 - ma_hf_bonus(ma);

    if(level >= 7)  { vs_magic -= 1; vs_psionics -= 1; vs_insanity -= 1; }
    if(level >= 13) { vs_magic -= 1; vs_psionics -= 1; }

    if(vs_magic < 4)    vs_magic    = 4;
    if(vs_psionics < 4) vs_psionics = 4;
    if(vs_poison < 4)   vs_poison   = 4;
    if(vs_insanity < 4) vs_insanity = 4;
    if(vs_hf < 4)       vs_hf       = 4;

    write(repeat_string("-", 48));
    write(sprintf("  %-26s %s", "SAVING THROWS", "Target (roll d20)"));
    write(repeat_string("-", 48));
    write(sprintf("  %-26s %d or higher", "vs. Magic Spells:",    vs_magic));
    write(sprintf("  %-26s %d or higher", "vs. Psionics:",        vs_psionics));
    write(sprintf("  %-26s %d or higher", "vs. Poison/Disease:",  vs_poison));
    write(sprintf("  %-26s %d or higher", "vs. Insanity:",        vs_insanity));
    write(sprintf("  %-26s %d or higher", "vs. Horror Factor:",   vs_hf));
    write(repeat_string("-", 48));
    write(sprintf("  ME: %d  MA: %d  PE: %d  Level: %d",
        me, ma, pe, level));
    return 1;
}

void help() {
    message("help",
        "Syntax: saving throws\n"
        "        saves\n\n"
        "Displays your saving throw target numbers.\n"
        "Roll d20 equal to or higher than the number shown to resist.\n"
        "ME improves saves vs. magic, psionics, and insanity.\n"
        "PE improves saves vs. poison and disease.\n"
        "MA improves saves vs. Horror Factor.\n\n"
        "See also: score, abilities, stats",
        this_player());
}
