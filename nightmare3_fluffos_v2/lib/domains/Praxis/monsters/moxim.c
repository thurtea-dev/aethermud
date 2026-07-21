/* /d/Praxis/monsters/moxim.c
   Moxim, dimensional mage and rift opener.
   Player types: rift <destination>
   Moxim checks credits, deducts, speaks a flavor line, clones a rift portal.
   Portal lasts 30 seconds. Any player in the room can enter before it closes. */

#include <std.h>
#include <daemons.h>

inherit "/std/rifts_npc";

private mapping __dests;

void create() {
    ::create();

    set_name("Moxim");
    set_npc_name("Moxim");
    set_id( ({ "moxim", "mage", "dimensional mage", "rift opener", "man", "traveler" }) );
    set_short("Moxim, dimensional mage");
    set_long(
        "A compact, middle-aged man in travel-worn robes covered in\n"
        "dimensional notation stitched in silver thread. His eyes have\n"
        "the slightly unfocused quality of someone who sees things at an\n"
        "angle most people cannot perceive. A crystal staff leans against\n"
        "the wall behind him, humming faintly.\n\n"
        "He opens rifts for a fee. Type 'rift <destination>' to travel.\n"
        "Ask him about 'destinations' to see where he can send you.");
    set_level(15);
    set_max_hp(300);
    set_hp(300);
    set_aggressive(0);
    set_moving(0);
    set_gender("male");
    set_race("human");
    set_alignment(400);
    set_body_type("human");

    add_response("hello", "Moxim says, 'The ley lines are strong here. Where do you need to go?'");
    add_response("hi", "Moxim says, 'The ley lines are strong here. Where do you need to go?'");
    add_response("cost", "Moxim says, 'Most destinations are 500 credits. Praxis is only 200.'");
    add_response("price", "Moxim says, 'Most destinations are 500 credits. Praxis is only 200.'");
    add_response("bye", "Moxim says, 'Travel safely. The rifts are wild today.'");
    set_default_response("Moxim glances at you. 'Ask about destinations if you want to travel.'");
    set_property("position_str", "stands here, watching the rifts.");

    /* Destinations are the rooms that spawn a Moxim instance, so travelers
       arrive beside that zone's Moxim - not the zone gate/welcome room.
       ({ room_path, display_name, cost, faction_key, flavor }) */
    __dests = ([
        "americas" : ({ "/domains/ChiTown/areas/chitown_boulevard",
                         "Chi-Town", 500, "",
                         "Moxim mutters, 'Iron and propaganda. Safe travels, if you can manage it.'" }),
        "chitown" : ({ "/domains/ChiTown/areas/chitown_boulevard",
                         "Chi-Town", 500, "",
                         "Moxim mutters, 'Iron and propaganda. Safe travels, if you can manage it.'" }),
        "atlantis" : ({ "/domains/Praxis/areas/splynn/splynn_market",
                         "Splynn", 500, "splugorth",
                         "Moxim mutters, 'The shores of Atlantis. Mind the Splugorth.'" }),
        "splynn" : ({ "/domains/Praxis/areas/splynn/splynn_market",
                         "Splynn", 500, "splugorth",
                         "Moxim mutters, 'The shores of Atlantis. Mind the Splugorth.'" }),
        "europe" : ({ "/domains/NewCamelot/areas/camelot_square",
                         "New Camelot", 500, "new_camelot",
                         "Moxim mutters, 'Old magic and older stones. Europe awaits.'" }),
        "camelot" : ({ "/domains/NewCamelot/areas/camelot_square",
                         "New Camelot", 500, "new_camelot",
                         "Moxim mutters, 'Old magic and older stones. Europe awaits.'" }),
        "lazlo" : ({ "/domains/Lazlo/areas/lazlo_market",
                         "City of Lazlo", 500, "lazlo",
                         "Moxim mutters, 'A city of peace. Enjoy the quiet while it lasts.'" }),
        "tolkeen" : ({ "/domains/tolkeen/areas/tolkeen_square",
                         "Free City of Tolkeen", 500, "tolkeen",
                         "Moxim mutters, 'Magic and stubbornness. Mind the Coalition guns.'" }),
        "praxis" : ({ "/domains/Praxis/areas/monument_square",
                         "Monument Square", 200, "",
                         "Moxim mutters, 'Home again. A short trip.'" })
    ]);
}

void catch_tell(string str) {
    string a, b;
    object tp;

    /* _say.c format: Player says, "text" -- match that, not "says: " */
    if(sscanf(str, "%s says, \"%s\"", a, b) != 2) {
        ::catch_tell(str);
        return;
    }
    /* Skip our own say output so show_destinations does not re-trigger. */
    if(lower_case(a) == lower_case((string)this_object()->query_name())) {
        ::catch_tell(str);
        return;
    }
    b  = lower_case(b);
    tp = this_player();
    if(!tp) { ::catch_tell(str); return; }

    /* Checked before the destination keywords: "blue rift" contains
       "rift" and would otherwise route to the destination table. */
    if(strsrch(b, "blue") != -1 || strsrch(b, "ocean") != -1) {
        call_out("blue_hint", 1, tp);
        return;
    }
    if(strsrch(b, "destination") != -1 || strsrch(b, "travel") != -1 ||
       strsrch(b, "where") != -1 || strsrch(b, "rift") != -1) {
        call_out("show_destinations", 1, tp);
        return;
    }
    ::catch_tell(str);
}

/* tp is passed explicitly because this_player() is 0 inside a call_out. */
void blue_hint(object tp) {
    if(!tp || !objectp(tp)) return;
    tell_object(tp,
        "Moxim says: Once in a while a rift comes through tinted deep\n"
        "blue. Not my work. It opens on the Chi-Town boulevard when it\n"
        "pleases and it leads to open ocean. Do not step through it\n"
        "without a way to breathe water.\n");
}

/* tp is passed explicitly because this_player() is 0 inside a call_out. */
void show_destinations(object tp) {
    if(!tp || !objectp(tp)) return;
    tell_object(tp,
        "Moxim says: I can open rifts to these places:\n"
        " americas   Chi-Town       500 credits\n"
        " atlantis   Splynn         500 credits\n"
        " europe     New Camelot    500 credits\n"
        " lazlo      City of Lazlo  500 credits\n"
        " tolkeen    Free Tolkeen   500 credits\n"
        " praxis     Monument Sq.   200 credits\n"
        "\nType: rift <destination>\n");
}

void init() {
    ::init();
    add_action("cmd_rift", "rift");
}

int cmd_rift(string str) {
    mixed *ddata;
    string dest_key;
    string dest_room;
    string dest_name;
    string faction;
    string flavor;
    int cost;
    object tp;
    object portal;

    tp = this_player();
    if(!tp) return 0;

    if(!str || !strlen(str)) {
        write("Rift where? Ask Moxim about 'destinations' to see available locations.\n");
        return 1;
    }
    dest_key = lower_case(str);
    ddata = __dests[dest_key];
    if(!ddata || !pointerp(ddata)) {
        say_here("I do not know that destination.");
        return 1;
    }
    dest_room = ddata[0];
    dest_name = ddata[1];
    cost = ddata[2];
    faction = ddata[3];
    flavor = ddata[4];

    if(!creatorp(tp) && (int)tp->query_money("credits") < cost) {
        say_here("You cannot afford passage. The cost is " + cost + " credits.");
        return 1;
    }
    if(!creatorp(tp))
        tp->add_money("credits", -cost);

    tell_object(tp, flavor + "\n");
    tell_room(environment(this_object()),
        "Moxim gestures and a shimmering rift tears open in the air.\n",
        ({ this_object() }));

    portal = clone_object("/std/rift_portal");
    portal->set_destination(dest_room);
    portal->set_label(dest_name);
    portal->move(environment(this_object()));

    if(faction && strlen(faction))
        catch(FACTION_D->modify_standing(
            (string)tp->query_name(), faction, 5, "traveled via Moxim"));
    return 1;
}
