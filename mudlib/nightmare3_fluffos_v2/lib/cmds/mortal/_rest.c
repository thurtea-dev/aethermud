/* /cmds/mortal/_rest.c
   Universal rest command.
   In rooms with rest_allowed, heals per Rifts tabletop rates each tick.
   PPE/ISP regen is handled separately by user.c heartbeat. */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private void do_rest_tick(object pl) {
    object env;
    int mdc_race;
    int cur;
    int max_val;
    int gain;
    int max_mdc;
    int max_sdc;
    int hp_full;
    int sdc_full;
    int interval;

    if(!pl || !objectp(pl)) return;
    if((string)pl->getenv("resting") != "1") return;
    if((int)pl->query_fighting()) {
        pl->setenv("resting", "0");
        tell_object(pl, "Combat rouses you from rest.\n");
        return;
    }

    env = environment(pl);
    if(!env || !(int)env->query_property("rest_allowed")) {
        pl->setenv("resting", "0");
        tell_object(pl, "This area is no longer safe to recover in.\n");
        return;
    }

    if(!(int)RIFTS_D->is_rifts_race((string)pl->query_race())) {
        pl->add_hp((int)pl->query_max_hp() - (int)pl->query_hp());
        pl->setenv("resting", "0");
        tell_object(pl, "You feel refreshed.\n");
        return;
    }

    mdc_race = (int)RIFTS_D->is_mdc_race((string)pl->query_race());
    if(mdc_race) {
        cur = (int)pl->query_stats("MDC");
        max_mdc = (int)pl->query_stats("max_MDC");
        max_sdc = (int)pl->query_stats("max_SDC");
        if(max_mdc == 0 && max_sdc > 0) {
            cur = (int)pl->query_stats("SDC");
            max_val = max_sdc;
        } else {
            max_val = max_mdc;
        }
        if(cur >= max_val) {
            tell_object(pl, "You are fully recovered.\n");
            pl->setenv("resting", "0");
            return;
        }
        gain = (int)RIFTS_D->query_rest_mdc_gain(pl);
        cur += gain;
        if(cur > max_val) cur = max_val;
        if(max_mdc == 0 && max_sdc > 0)
            pl->set_stats("SDC", cur);
        else
            pl->set_stats("MDC", cur);
        tell_object(pl, "You recover " + gain + " MDC while resting.\n");
        if(cur >= max_val) {
            tell_object(pl, "You are fully recovered.\n");
            pl->setenv("resting", "0");
            return;
        }
    } else {
        cur = (int)pl->query_stats("rifts_hp");
        max_val = (int)pl->query_stats("max_rifts_hp");
        if(cur < max_val) {
            gain = (int)RIFTS_D->query_rest_hp_gain(pl);
            cur += gain;
            if(cur > max_val) cur = max_val;
            pl->set_stats("rifts_hp", cur);
        }
        cur = (int)pl->query_stats("SDC");
        max_val = (int)pl->query_stats("max_SDC");
        if(max_val > 0 && cur < max_val) {
            gain = (int)RIFTS_D->query_rest_sdc_gain(pl);
            cur += gain;
            if(cur > max_val) cur = max_val;
            pl->set_stats("SDC", cur);
        }
        tell_object(pl, "You recover HP and SDC while resting.\n");
        hp_full = (int)pl->query_stats("rifts_hp") >= (int)pl->query_stats("max_rifts_hp");
        sdc_full = ((int)pl->query_stats("max_SDC") == 0 ||
            (int)pl->query_stats("SDC") >= (int)pl->query_stats("max_SDC"));
        if(hp_full && sdc_full) {
            tell_object(pl, "You are fully recovered.\n");
            pl->setenv("resting", "0");
            return;
        }
    }

    interval = (int)RIFTS_D->query_rest_interval(pl);
    if(interval < 60) interval = 60;
    call_out("do_rest_tick", interval, pl);
}

int cmd_rest(string str) {
    object env;
    int rest_allowed;

    if((int)this_player()->query_fighting()) {
        write("You cannot rest while in combat!\n");
        return 1;
    }

    if((string)this_player()->getenv("resting") == "1") {
        write("You are already resting.\n");
        return 1;
    }

    env = environment(this_player());
    rest_allowed = env ? (int)env->query_property("rest_allowed") : 0;

    this_player()->setenv("resting", "1");

    if(rest_allowed) {
        write("You settle in to rest. You recover slowly in safe areas.\n");
        this_player()->tell_room_living(env, this_player(), 0,
            " settles in to rest.\n");
        call_out("do_rest_tick", (int)RIFTS_D->query_rest_interval(this_player()),
            this_player());
    } else {
        write("You sit down to rest.\n"
              "(This area does not allow recuperation. Find a safe location to heal.)\n");
        this_player()->tell_room_living(env, this_player(), 0,
            " sits down to rest.\n");
    }
    return 1;
}

void help() {
    write(
        "Syntax: rest\n\n"
        "Sit down and rest. In designated safe areas you recover HP/SDC or MDC\n"
        "over time. Juicers recover fastest. Dragons recover slowest.\n"
        "Humans and most others recover at a moderate pace.\n"
        "Use 'wake' to get back up.\n\n"
        "See also: wake, score\n"
    );
}
