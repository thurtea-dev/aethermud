// /obj/credcard.c
// Universal credcard used to track and display credit balances.

#include <std.h>
#include "move.h"

inherit OBJECT;

private int __credits;

private int current_balance() {
    object env;

    env = environment(this_object());
    if(objectp(env) && (int)env->is_player())
        return (int)env->query_money("credits");

    return __credits;
}

private void refresh_display() {
    int bal;

    bal = current_balance();
    set_short("a universal credcard");
    set_long(
        "A slim MDC polymer card with an embedded credit chip. "
        "The standard electronic currency of Rifts Earth. "
        "Most vendors accept it on sight.\n\n"
        "The display reads: " + bal + " credits.");
}

void create() {
    ::create();
    set_name("universal credcard");
    set_id(({ "credcard", "credit card", "card", "credits" }));
    set_property("credcard", 1);
    set_property("no drop", 0);
    __credits = 0;
    refresh_display();
    add_action("cmd_balance", "balance");
    add_action("cmd_balance", "credits");
}

int query_credits() {
    return current_balance();
}

int add_credits(int amount) {
    object env;

    if(amount < 0) return 0;
    env = environment(this_object());
    if(objectp(env) && (int)env->is_player())
        env->add_money("credits", amount);
    else
        __credits += amount;
    refresh_display();
    return query_credits();
}

int remove_credits(int amount) {
    object env;
    int bal;

    if(amount < 0) return 0;
    bal = query_credits();
    if(amount > bal) return 0;
    env = environment(this_object());
    if(objectp(env) && (int)env->is_player())
        env->add_money("credits", -amount);
    else
        __credits -= amount;
    refresh_display();
    return 1;
}

int cmd_balance(string str) {
    write("Your credcard shows " + query_credits() + " credits.\n");
    return 1;
}

string query_auto_load() {
    return file_name(this_object()) + "#" + __credits;
}

void init_arg(string arg) {
    __credits = to_int(arg);
    refresh_display();
}

int move(mixed dest) {
    int result;

    result = ::move(dest);
    if(result != MOVE_OK) return result;

    if(objectp(dest) && (int)dest->is_player()) {
        if((int)dest->query_money("credits") == 0 && __credits > 0) {
            dest->add_money("credits", __credits);
            __credits = 0;
        }
    }
    refresh_display();
    return result;
}