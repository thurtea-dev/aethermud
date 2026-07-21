/* /cmds/mortal/_rift.c
   Global rift travel command. Works from anywhere; no Moxim required.
   Mechanic identical to Moxim: check credits, clone rift_portal, player enters.
   Usage: rift <destination>   (no arg lists destinations) */

#include <std.h>
#include <daemons.h>

inherit DAEMON;

private mapping __rifts_dests;

int cmd_rift(string str) {
    mixed *ddata;
    string dest_key;
    string dest_room;
    string dest_name;
    string faction;
    int cost;
    object tp;
    object portal;

    tp = this_player();
    if(!tp) return 0;

    /* Same Moxim-room destinations as domains/Praxis/monsters/moxim.c */
    if(!mapp(__rifts_dests)) {
        __rifts_dests = ([
            "americas" : ({ "/domains/ChiTown/areas/chitown_boulevard",
                            "Chi-Town", 500, "" }),
            "chitown"   : ({ "/domains/ChiTown/areas/chitown_boulevard",
                             "Chi-Town", 500, "" }),
            "chi-town"  : ({ "/domains/ChiTown/areas/chitown_boulevard",
                             "Chi-Town", 500, "" }),
            "atlantis" : ({ "/domains/Praxis/areas/splynn/splynn_market",
                            "Splynn", 500, "splugorth" }),
            "splynn"   : ({ "/domains/Praxis/areas/splynn/splynn_market",
                            "Splynn", 500, "splugorth" }),
            "europe"      : ({ "/domains/NewCamelot/areas/camelot_square",
                              "New Camelot", 500, "new_camelot" }),
            "camelot"     : ({ "/domains/NewCamelot/areas/camelot_square",
                              "New Camelot", 500, "new_camelot" }),
            "newcamelot"  : ({ "/domains/NewCamelot/areas/camelot_square",
                              "New Camelot", 500, "new_camelot" }),
            "new-camelot" : ({ "/domains/NewCamelot/areas/camelot_square",
                              "New Camelot", 500, "new_camelot" }),
            "lazlo"    : ({ "/domains/Lazlo/areas/lazlo_market",
                            "City of Lazlo", 500, "lazlo" }),
            "tolkeen"  : ({ "/domains/tolkeen/areas/tolkeen_square",
                            "Free City of Tolkeen", 500, "tolkeen" }),
            "praxis"   : ({ "/domains/Praxis/areas/monument_square",
                            "Monument Square", 200, "" })
        ]);
    }

    if(!str || !strlen(str)) {
        write(
            "The ley line network reaches:\n"
            "  americas / chitown    Chi-Town          500 credits\n"
            "  atlantis / splynn     Splynn             500 credits\n"
            "  europe / camelot      New Camelot        500 credits\n"
            "  lazlo                 City of Lazlo      500 credits\n"
            "  tolkeen               Free City Tolkeen  500 credits\n"
            "  praxis                Monument Square    200 credits\n"
            "\nType: rift <destination>\n");
        return 1;
    }

    dest_key = lower_case(str);
    ddata = __rifts_dests[dest_key];
    if(!ddata || !pointerp(ddata)) {
        write("Moxim's rifts do not reach that place.\n");
        return 1;
    }

    dest_room = ddata[0];
    dest_name = ddata[1];
    cost      = ddata[2];
    faction   = ddata[3];

    if(!creatorp(tp) && (int)tp->query_money("credits") < cost) {
        write("You cannot afford passage. The cost is " + cost + " credits.\n");
        return 1;
    }
    if(!creatorp(tp))
        tp->add_money("credits", -cost);

    write("The ley lines respond. A shimmering rift tears open before you.\n");
    tell_room(environment(tp),
        "The ley lines flare. A shimmering rift tears open in the air.\n",
        ({ tp }));

    portal = clone_object("/std/rift_portal");
    portal->set_destination(dest_room);
    portal->set_label(dest_name);
    portal->move(environment(tp));

    if(faction && strlen(faction))
        catch(FACTION_D->modify_standing(
            (string)tp->query_name(), faction, 5, "traveled via rift"));
    return 1;
}

void help() {
    write(
        "Syntax: rift <destination>\n\n"
        "Open a rift to another city. Costs universal credits.\n"
        "Admins travel free.\n\n"
        "Destinations:\n"
        "  americas / chitown    Chi-Town          500 credits\n"
        "  atlantis / splynn     Splynn             500 credits\n"
        "  europe / camelot      New Camelot        500 credits\n"
        "  lazlo                 City of Lazlo      500 credits\n"
        "  tolkeen               Free City Tolkeen  500 credits\n"
        "  praxis                Monument Square    200 credits\n\n"
        "Once the rift opens, enter it with: enter rift\n"
        "Also accepted: go rift, go through, step through\n"
        "The rift collapses after 30 seconds.\n");
}
