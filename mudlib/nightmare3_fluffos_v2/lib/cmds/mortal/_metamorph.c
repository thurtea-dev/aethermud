/* /cmds/mortal/_metamorph.c
   Metamorph command for shapeshifting RCC players.

   Races with access:
     great horned dragon  - full metamorph (humanoid or any race)
     changeling           - full metamorph
     secondary vampire    - human form only
     wild vampire         - human form only

   Syntax:
     metamorph             - show current form
     metamorph human       - take humanoid form
     metamorph <race>      - take on appearance of another race
     metamorph dragon      - revert to true dragon form (GHD only)
     metamorph true        - revert to true form (all races) */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private int can_metamorph() {
    string race;
    race = lower_case((string)this_player()->query_race());
    if((int)RIFTS_D->is_admin_race(race)) return 1;
    return (strsrch(race, "dragon") != -1 ||
            strsrch(race, "changeling") != -1 ||
            strsrch(race, "vampire") != -1);
}

private int vampire_only() {
    string race;
    race = lower_case((string)this_player()->query_race());
    return (strsrch(race, "vampire") != -1 &&
            strsrch(race, "dragon") == -1 &&
            strsrch(race, "changeling") == -1);
}

private int is_ghd() {
    string race;
    race = lower_case((string)this_player()->query_race());
    return (strsrch(race, "great horned dragon") != -1 ||
            strsrch(race, "great_horned_dragon") != -1);
}

/* "a" / "an" for a race word. Same vowel test std/user.c
   query_display_name() uses, so "an elf" and "an ogre" read correctly
   instead of "a elf". */
private string article(string word) {
    if(!word || word == "") return "a";
    switch(word[0]) {
    case 'a': case 'e': case 'i': case 'o': case 'u':
        return "an";
    default:
        return "a";
    }
}

/* What a stranger who does not know this character would currently see.
   Deliberately mirrors the precedence chain in std/user.c
   query_apparent_race() (visible_race, then metamorphed ->
   apparent_race/current_form, then the true race, with the secondary
   vampire override last). Keep the two in sync: this command must be able
   to tell whether a change actually took effect before it reports one.

   Note the chain this mirrors is only ever reached for a viewer who is not
   the character, not staff, and does not know them. Those three exemptions
   live in user.c and are intentional; see cmd_metamorph()'s creator
   warning below. */
private string stranger_race(object who) {
    string r;

    if(!who || !objectp(who)) return "";
    r = (string)who->query_property("visible_race");
    if((!r || r == "") && (int)who->query_property("metamorphed")) {
        r = (string)who->query_property("apparent_race");
        if(!r || r == "")
            r = (string)who->query_property("current_form");
    }
    if(!r || r == "") r = (string)who->query_race();
    if(!r) return "";
    r = lower_case(r);
    /* secondary vampires read as human to everyone but an arch */
    if(r == "secondary vampire") r = "human";
    return r;
}

private void revert_to_true_form() {
    object *worn;
    object *to_remove;
    string true_race;
    int i;

    true_race = (string)this_player()->query_race();
    this_player()->set_property("metamorphed",    0);
    this_player()->set_property("current_form",   true_race);
    this_player()->set_property("apparent_race",  true_race);

    if(is_ghd()) {
        this_player()->set_property("dragon_sized", 1);
        /* strip non-dragon-sized armor */
        worn = (object *)this_player()->query_worn();
        to_remove = ({});
        if(worn && sizeof(worn)) {
            for(i = 0; i < sizeof(worn); i++) {
                if(worn[i] && !(int)worn[i]->query_property("dragon_sized"))
                    to_remove += ({ worn[i] });
            }
            for(i = 0; i < sizeof(to_remove); i++) {
                write("Your returning dragon form tears free from " +
                    (string)to_remove[i]->query_short() + ".\n");
                to_remove[i]->unwear();
            }
        }
    }

    write("You release your held form. Your true nature reasserts itself.\n");
    this_player()->tell_room_living(environment(this_player()),
        this_player(), 0, " shimmers and returns to their true form.\n");
}

int cmd_metamorph(string str) {
    string target_form;
    string current;
    string before;
    string after;
    string prev_form;
    string prev_app;
    int prev_meta;
    int prev_dsize;

    if(!can_metamorph()) {
        write("You do not have the ability to change form.\n");
        return 1;
    }

    if(!str || str == "") {
        current = (string)this_player()->query_property("current_form");
        if(!current || current == "")
            current = (string)this_player()->query_race();
        write("Current form: " + capitalize(current) + "\n");
        return 1;
    }

    target_form = lower_case(str);

    if(target_form == "true" ||
       target_form == "dragon" ||
       target_form == "true form" ||
       target_form == "natural") {
        if(!(int)this_player()->query_property("metamorphed")) {
            write("You are already in your natural form.\n");
            return 1;
        }
        revert_to_true_form();
        return 1;
    }

    if(vampire_only() && target_form != "human") {
        write("Vampires can only take human form.\n");
        return 1;
    }

    if(!(int)RIFTS_D->is_rifts_race(target_form)) {
        write("There is no such race. Type 'metamorph' with no argument to see your current form.\n");
        return 1;
    }

    /* Nothing to do if the form a stranger already sees is the one being
       asked for. Previously this fell through and reported success anyway. */
    before = stranger_race(this_player());
    if(before == target_form) {
        write("You already appear to be " + article(target_form) + " " +
              target_form + ".\n");
        return 1;
    }

    prev_meta  = (int)this_player()->query_property("metamorphed");
    prev_form  = (string)this_player()->query_property("current_form");
    prev_app   = (string)this_player()->query_property("apparent_race");
    prev_dsize = (int)this_player()->query_property("dragon_sized");

    /* apply transformation */
    this_player()->set_property("metamorphed",   1);
    this_player()->set_property("current_form",  target_form);
    this_player()->set_property("apparent_race", target_form);

    /* Verify before reporting. The old code printed its success text
       unconditionally, which is what made a no-op look like a success. */
    after = stranger_race(this_player());
    if(after != target_form) {
        this_player()->set_property("metamorphed",   prev_meta);
        this_player()->set_property("current_form",  prev_form);
        this_player()->set_property("apparent_race", prev_app);
        this_player()->set_property("dragon_sized",  prev_dsize);
        write("Your form will not hold that shape. Nothing changes.\n");
        return 1;
    }

    if(is_ghd() && target_form == "human") {
        this_player()->set_property("dragon_sized", 0);
        write("Your massive form flows inward. You take on the appearance of a human.\n"
              "In this form you can wear human-sized armor.\n");
        say("A massive dragon shimmers and compresses into human form.");
    } else if(is_ghd()) {
        this_player()->set_property("dragon_sized", 0);
        write("Your body reshapes. You now appear to be " +
              article(target_form) + " " + target_form + ".\n");
        this_player()->tell_room_living(environment(this_player()),
            this_player(), 0,
            " shimmers and shifts, taking on a new form.\n");
    } else {
        write("Your body reshapes. You now appear to be " +
              article(target_form) + " " + target_form + ".\n");
        this_player()->tell_room_living(environment(this_player()),
            this_player(), 0,
            " shimmers and shifts, taking on a new form.\n");
    }

    write("Anyone who does not already know you now sees " +
          article(after) + " " + after + ".\n");

    /* std/user.c query_apparent_race() and query_display_name() both return
       the true race when the character being looked at is a creator, so a
       staff character's disguise is inert for every viewer, including
       mortals. Say so rather than let the command read as broken. */
    if(creatorp(this_player()))
        write("Your staff status overrides this: your true race is still "
              "shown to everyone. Test this on a mortal character.\n");

    return 1;
}

void help() {
    write(
        "Syntax: metamorph\n"
        "        metamorph human\n"
        "        metamorph <race name>\n"
        "        metamorph dragon\n"
        "        metamorph true\n\n"
        "Shapeshifting ability for dragon, changeling, and vampire races.\n"
        "In human form dragons can wear human-sized armor. Reverting to dragon\n"
        "form will remove any non-dragon-sized items you are wearing.\n"
    );
}
