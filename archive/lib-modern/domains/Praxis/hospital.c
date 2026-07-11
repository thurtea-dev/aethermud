// /domains/Praxis/hospital.c
// Praxis Field Hospital. Rifts medical services for credits.

#include <std.h>
#include <daemons.h>

inherit ROOM;

void init() {
    ::init();
    add_action("do_heal", "heal");
    add_action("do_treat", "treat");
    add_action("do_read", "read");
}

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("no attack", 1);
    set_property("rest_allowed", 1);
    set_short("Praxis Field Hospital");
    set_long(
        "A converted pre-Rifts clinic now run by a Body Fixer team. Clean\n"
        "white tiles, surgical lamps, and MDC-rated trauma beds line the ward.\n"
        "A laminated sign lists services. The main road is east.");
    set_exits( ([ "east" : "/domains/Praxis/n_centre2" ]) );
    set_items( ([
        "sign" : "Medical services. Type 'read sign'.",
        "beds" : "Trauma beds with auto-injector ports and MDC stabilizers.",
        "tiles" : "Scrubbed white tile. Someone takes cleanliness seriously."
    ]) );
}

void reset() {
    object medic;
    object doc;

    ::reset();
    if(!present("medic", this_object())) {
        medic = new(MONSTER);
        medic->set_name("field medic");
        medic->set_id(({ "medic", "body fixer", "doctor", "nurse" }));
        medic->set_short("a Praxis field medic");
        medic->set_long(
            "A Body Fixer in clean scrubs with a medical kit and tired eyes.\n"
            "Type 'heal' or 'treat' for services.");
        medic->set_race("human");
        medic->set_gender("female");
        medic->set_level(5);
        medic->set_aggressive(0);
        medic->set_moving(0);
        medic->move(this_object());
    }
    if(!present("cyber-doc", this_object()) &&
       !present("cyber doc", this_object())) {
        doc = clone_object("/domains/Praxis/npcs/cyber_doc_medic");
        if(doc) doc->move(this_object());
    }
}

int do_read(string str) {
    if(!str || str != "sign") return notify_fail("Read what?\n");
    write(
        "Praxis Field Hospital - Services\n"
        "--------------------------------\n"
        "  heal           Full heal for 200 credits (out of combat)\n"
        "  treat          Stabilize wounds for 50 credits\n"
        "  install        Cyber-doc installs cybernetics (carry implant)\n"
        "  rest           Rest here to recover (see help rest)\n");
    return 1;
}

int do_heal(string str) {
    int cost;
    int cur_hp;
    int max_hp;
    int cur_mdc;
    int max_mdc;

    if(sizeof((object *)this_player()->query_attackers()) > 0) {
        write("The medic says: Not while you're fighting!\n");
        return 1;
    }
    cost = 200;
    if((int)this_player()->query_money("credits") < cost) {
        write("The medic says: That costs " + cost + " credits.\n");
        return 1;
    }
    this_player()->add_money("credits", -cost);
    cur_hp  = (int)this_player()->query_stats("rifts_hp");
    max_hp  = (int)this_player()->query_stats("max_rifts_hp");
    cur_mdc = (int)this_player()->query_stats("MDC");
    max_mdc = (int)this_player()->query_stats("max_MDC");
    if(max_hp > 0) this_player()->set_stats("rifts_hp", max_hp);
    if(max_mdc > 0) this_player()->set_stats("MDC", max_mdc);
    this_player()->set_hp((int)this_player()->query_max_hp());
    write("The medic patches you up. You feel fully restored.\n");
    say((string)this_player()->query_cap_name() + " receives full medical treatment.");
    return 1;
}

int do_treat(string str) {
    int cost;
    int cur_hp;
    int max_hp;

    cost = 50;
    if((int)this_player()->query_money("credits") < cost) {
        write("The medic says: That costs " + cost + " credits.\n");
        return 1;
    }
    this_player()->add_money("credits", -cost);
    cur_hp = (int)this_player()->query_stats("rifts_hp");
    max_hp = (int)this_player()->query_stats("max_rifts_hp");
    if(max_hp > 0 && cur_hp < max_hp) {
        cur_hp += max_hp / 4;
        if(cur_hp > max_hp) cur_hp = max_hp;
        this_player()->set_stats("rifts_hp", cur_hp);
    }
    write("The medic stabilizes your wounds.\n");
    return 1;
}
