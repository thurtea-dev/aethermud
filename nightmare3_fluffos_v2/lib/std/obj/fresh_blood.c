// /std/obj/fresh_blood.c
// Pool of blood left when a living being dies. Vampires may drink it.

#include <std.h>
#include <daemons.h>

inherit "/std/Object";

#define DRY_TIME 300

private string source_name;
private int charges;

void create() {
    ::create();
    source_name = "";
    charges = 3;
    set_name("fresh blood");
    set_id(({ "blood", "fresh blood", "pool", "pool of blood", "pool of fresh blood" }));
    set_short("a pool of fresh blood");
    set_long(
        "Blood has pooled on the ground from a recent kill. It is still warm\n"
        "and wet. Vampire races can feed on it with 'drink blood' or 'eat blood'.\n");
    set_mass(1);
    set_property("is_blood", 1);
    call_out("dry_up", DRY_TIME);
}

void set_source(string str) {
    if(!str) return;
    source_name = str;
    set_long(
        "Blood has pooled on the ground from " + source_name +
        "'s body. It is still warm and wet.\n"
        "Vampire races can feed on it with 'drink blood' or 'eat blood'.\n");
}

string query_source() { return source_name; }

void dry_up() {
    object room;

    room = environment(this_object());
    if(room)
        tell_room(room, "The pool of blood dries and soaks into the ground.\n");
    if(this_object()) destruct(this_object());
}

void init() {
    object env;

    ::init();
    env = environment(this_object());
    if(!env || !this_player()) return;
    if(env == this_player() || env == environment(this_player())) {
        add_action("cmd_drink", "drink");
        add_action("cmd_drink", "eat");
    }
}

int cmd_drink(string str) {
    object pl;
    object env;
    int mdc_race;
    int cur;
    int max_val;
    int gain;

    if(!str) return 0;
    str = lower_case(str);
    if(strsrch(str, "blood") == -1 && strsrch(str, "pool") == -1)
        return 0;

    pl = this_player();
    env = environment(this_object());
    if(!pl || env != environment(pl)) return 0;

    if(!(int)RIFTS_D->is_vampire_race(pl)) {
        write("You have no use for that.\n");
        return 1;
    }
    if((object)pl->query_current_attacker()) {
        write("You cannot drink during combat.\n");
        return 1;
    }

    mdc_race = (int)RIFTS_D->is_mdc_race((string)pl->query_race());
    if(mdc_race) {
        cur = (int)pl->query_stats("MDC");
        max_val = (int)pl->query_stats("max_MDC");
        gain = random(6) + 1;
        if(cur + gain > max_val) gain = max_val - cur;
        if(gain < 1) {
            write("You drink the blood but you are already at full strength.\n");
            return 1;
        }
        pl->set_stats("MDC", cur + gain);
        write("You drink the fresh blood. +" + gain + " MDC.\n");
        pl->tell_room_living(env, pl, 0, " drinks from a pool of fresh blood.\n");
    } else {
        cur = (int)pl->query_stats("rifts_hp");
        max_val = (int)pl->query_stats("max_rifts_hp");
        gain = random(4) + 1;
        if(cur + gain > max_val) gain = max_val - cur;
        if(gain < 1) {
            write("You drink the blood but you are already at full strength.\n");
            return 1;
        }
        pl->set_stats("rifts_hp", cur + gain);
        write("You drink the fresh blood. +" + gain + " HP.\n");
        pl->tell_room_living(env, pl, 0, " drinks from a pool of fresh blood.\n");
    }

    charges--;
    if(charges <= 0) {
        write("The last of the blood is gone. The pool is drained dry.\n");
        destruct(this_object());
    }
    return 1;
}
