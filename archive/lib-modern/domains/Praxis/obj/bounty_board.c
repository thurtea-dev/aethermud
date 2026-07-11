// /domains/Praxis/obj/bounty_board.c
// Praxis wanted board: lists highway bandit bounty and handles turn-in via wanted_d.

#include <std.h>
#include <daemons.h>

inherit OBJECT;

private int find_bandit_patch(object player) {
    object *inv;
    int i;

    if(!player) return 0;
    inv = all_inventory(player);
    for(i = 0; i < sizeof(inv); i++) {
        if(!inv[i]) continue;
        if((string)inv[i]->query_name() == "bandit patch") return 1;
    }
    return 0;
}

private object take_bandit_patch(object player) {
    object *inv;
    int i;

    if(!player) return 0;
    inv = all_inventory(player);
    for(i = 0; i < sizeof(inv); i++) {
        if(!inv[i]) continue;
        if((string)inv[i]->query_name() == "bandit patch") {
            inv[i]->move(this_object());
            destruct(inv[i]);
            return this_object();
        }
    }
    return 0;
}

void create() {
    ::create();
    set_name("bounty board");
    set_id(({ "bounty board", "board", "wanted board", "poster board",
              "wanted posters" }));
    set_short("a Praxis bounty board");
    set_long(
        "A cork board nailed to a post in Monument Square. Fresh wanted flyers\n"
        "cover older ones. The top listing offers credits for highway bandits\n"
        "preying on the old highway north of town.\n\n"
        "Commands: read board, accept bounty, claim bounty");
    set_mass(5000);
    set_value(0);
    set_prevent_get(1);
}

void init() {
    ::init();
    add_action("cmd_read", "read");
    add_action("cmd_accept", "accept");
    add_action("cmd_claim", "claim");
}

int cmd_read(string arg) {
    object tp;

    if(arg && arg != "board" && arg != "bounty board" && arg != "bounty" &&
       arg != "posters" && arg != "wanted")
        return notify_fail("Read what?\n");

    tp = this_player();
    write(
        "=== PRAXIS BOUNTY BOARD ===\n"
        "HIGHWAY BANDIT (repeatable)\n"
        "  Target: highway bandits on the old highway and supply routes.\n"
        "  Proof: scavenger raider patch from a defeated bandit.\n"
        "  Reward: 400 credits, hero standing with Praxis.\n"
        "  Start: accept bounty\n"
        "  Turn in: claim bounty (with patch in inventory)\n\n"
        "Note: Arbiter Kane at the hospital pays a life grant for bandit patches\n"
        "at level fifteen through a separate survival proof contract.\n");
    return 1;
}

int cmd_accept(string arg) {
    object tp;
    int remaining;

    if(arg && arg != "bounty" && arg != "bandit" && arg != "highway")
        return notify_fail("Accept what bounty? Try 'accept bounty'.\n");

    tp = this_player();
    if(!tp) return 0;

    if((int)QUEST_D->is_quest_active(tp, "praxis_bounty_bandit")) {
        write("You already have the highway bandit bounty active.\n");
        return 1;
    }
    remaining = (int)QUEST_D->query_quest_cooldown(tp, "praxis_bounty_bandit");
    if(remaining > 0) {
        write("That bounty is cooling down (" + ((remaining + 59) / 60) +
              " minutes).\n");
        return 1;
    }

    if((int)QUEST_D->start_quest(tp, "praxis_bounty_bandit")) {
        write("You pin a mental note to hunt highway bandits for the posted reward.\n");
        catch(WANTED_D->add_infamy((string)tp->query_name(), 0,
            "Accepted Praxis highway bandit bounty"));
    }
    return 1;
}

int cmd_claim(string arg) {
    object tp;
    int completed;

    if(arg && arg != "bounty" && arg != "bandit" && arg != "patch")
        return notify_fail("Claim what? Try 'claim bounty' with a bandit patch.\n");

    tp = this_player();
    if(!tp) return 0;

    if(!(int)QUEST_D->is_quest_active(tp, "praxis_bounty_bandit")) {
        write("Accept the highway bandit bounty first ('accept bounty').\n");
        return 1;
    }
    if(!find_bandit_patch(tp)) {
        write("You need a scavenger raider patch from a defeated highway bandit.\n");
        return 1;
    }

    take_bandit_patch(tp);
    catch(QUEST_D->advance_quest(tp, "praxis_bounty_bandit", "bandit_patch_turned"));
    completed = (int)QUEST_D->complete_quest(tp, "praxis_bounty_bandit");
    if(completed)
        write("The clerk's proxy notch on the board marks another bandit down.\n");
    return 1;
}
