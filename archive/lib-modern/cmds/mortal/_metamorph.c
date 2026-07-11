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

    /* apply transformation */
    this_player()->set_property("metamorphed",   1);
    this_player()->set_property("current_form",  target_form);
    this_player()->set_property("apparent_race", target_form);

    if(is_ghd() && target_form == "human") {
        this_player()->set_property("dragon_sized", 0);
        write("Your massive form flows inward. You take on the appearance of a human.\n"
              "In this form you can wear human-sized armor.\n");
        say("A massive dragon shimmers and compresses into human form.");
    } else if(is_ghd()) {
        this_player()->set_property("dragon_sized", 0);
        write("Your body reshapes. You now appear to be a " + target_form + ".\n");
        this_player()->tell_room_living(environment(this_player()),
            this_player(), 0,
            " shimmers and shifts, taking on a new form.\n");
    } else {
        write("Your body reshapes. You now appear to be a " + target_form + ".\n");
        this_player()->tell_room_living(environment(this_player()),
            this_player(), 0,
            " shimmers and shifts, taking on a new form.\n");
    }
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
