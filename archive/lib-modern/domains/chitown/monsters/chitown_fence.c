/* /domains/chitown/monsters/chitown_fence.c
   Black market operator in the Chi-Town black market. Level 6, SDC 45.
   Non-aggressive, will fight if attacked. Buys CS dog tags for credits. */

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/chitown/equipment/"

inherit "/std/rifts_npc";

void create() {
    ::create();

    set_name("Chi-Town fence");
    set_npc_name("Fence");
    set_id( ({ "chi-town fence", "fence", "operator", "black market operator",
               "human" }) );
    set_short("a black market operator");
    set_long(
        "Calm, unhurried, and impossible to place an age on. They have\n"
        "moved enough contraband through this junction to know exactly\n"
        "what something is worth and exactly when to stop asking questions\n"
        "about where it came from.\n\n"
        "Say 'goods' to see what is for sale. Say 'sell <item>' to offer\n"
        "something up.");
    set_level(6);
    set_max_hp(35);
    set_hp(35);
    set_stats("SDC", 45);
    set_stats("max_SDC", 45);
    set_stats("is_MDC", 0);
    set_aggressive(0);
    set_moving(0);
    set_gender("neutral");
    set_race("human");
    set_alignment(-150);
    set_body_type("human");

    add_response("hello", "The fence says, 'Quiet voice, quick business. What do you need?'");
    add_response("hi", "The fence says, 'Quiet voice, quick business. What do you need?'");
    add_response("goods", "");
    add_response("buy", "The fence says, 'Crates change hands fast down here. Look around, "
                        "take what you can carry, leave the rest.'");
    add_response("sell", "The fence says, 'Dog tags, mostly. Say sell dog tag if you have one.'");
    add_response("contraband", "The fence says, 'That crate isn't mine to sell. Find your own buyer.'");
    add_response("bye", "The fence says, 'Word of this conversation goes nowhere. See that yours does too.'");
    set_default_response("The fence watches you without comment, waiting.");
}

void show_goods() {
    write("The fence gestures at the stacked crates without much interest:\n");
    write(" 'Whatever's in the crates is already spoken for. I deal in\n");
    write(" information and salvage. Say sell dog tag if you have CS tags\n");
    write(" to unload.'\n");
}

void catch_tell(string str) {
    string a, b;

    if(sscanf(str, "%s says: %s", a, b) != 2) {
        ::catch_tell(str);
        return;
    }
    b = lower_case(b);
    if(strsrch(b, "goods") != -1 || strsrch(b, "list") != -1) {
        call_out("show_goods", 1);
        return;
    }
    ::catch_tell(str);
}

void init() {
    ::init();
    add_action("cmd_sell", "sell");
}

int cmd_sell(string str) {
    object tag;

    if(!str || (strsrch(lower_case(str), "dog tag") == -1 &&
                strsrch(lower_case(str), "tag") == -1)) {
        say_here("Sell what? I'm buying CS dog tags. Say sell dog tag.");
        return 1;
    }
    tag = present("dog tag", this_player());
    if(!tag) {
        say_here("You don't have one of those on you.");
        return 1;
    }
    this_player()->add_money("credits", 150);
    tell_object(this_player(),
        "The fence takes the dog tag without a word and slides you 150 credits.\n");
    tell_room(environment(this_object()),
        "The fence completes a quiet exchange with " +
        (string)this_player()->query_cap_name() + ".\n",
        ({ this_object(), this_player() }));
    tag->remove();
    return 1;
}
