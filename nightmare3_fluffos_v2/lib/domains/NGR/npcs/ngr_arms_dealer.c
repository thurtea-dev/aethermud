/* /domains/NGR/npcs/ngr_arms_dealer.c
   Triax sales officer at the NGR equipment depot.
   Sells energy weapons and field gear. Prices are fixed. Documentation required. */

#include <std.h>
#include <daemons.h>

#define EQ_PATH "/domains/Praxis/equipment/"

inherit "/std/rifts_vendor";

void create() {
    ::create();

    set_name("Triax sales officer");
    set_npc_name("Triax sales officer");
    set_id( ({ "triax sales officer", "sales officer", "officer", "dealer",
               "vendor", "arms dealer", "triax officer", "ngr vendor" }) );
    set_short("a Triax sales officer");
    set_long(
        "A Triax Industries sales officer in a clean grey uniform bearing\n"
        "the iron eagle logo. Middle-aged, precise, with the kind of bearing\n"
        "that comes from working inside NGR territory long enough to absorb\n"
        "its values. They process purchases without commentary and issue\n"
        "registration paperwork for every weapon sold. This is not optional.\n"
        "\nThey sell NGR and Triax-compatible equipment. Type 'list' or 'buy <item>'.");
    set_level(4);
    set_max_hp(60);
    set_hp(60);
    set_aggressive(0);
    set_moving(0);
    set_gender("neutral");
    set_race("human");
    set_alignment(150);
    set_body_type("human");

    add_response("hello",
        "Triax sales officer says: 'Welcome to the Triax depot. All purchases are "
        "documented and registered with the NGR Commerce Authority.'");
    add_response("hi",
        "Triax sales officer says: 'Good day. What equipment are you looking for?'");
    add_response("list",
        "Triax sales officer says: 'Type list to see current inventory. "
        "Prices are fixed. NGR Commerce Authority rates.'");
    add_response("buy",
        "Triax sales officer says: 'Type buy <item> to purchase. "
        "Payment in credits. Weapons require registration.'");
    add_response("triax",
        "Triax sales officer says: 'Triax Industries. Based here in the NGR. "
        "Best human-manufactured equipment on the continent. "
        "Everything we sell is documented and under warranty.'");
    add_response("ngr",
        "Triax sales officer says: 'The New German Republic maintains the highest "
        "manufacturing standards on Rifts Earth. You are purchasing "
        "the product of that discipline.'");
    add_response("price",
        "Triax sales officer says: 'All prices are posted. They are not negotiable. "
        "The NGR Commerce Authority sets rates.'");
    add_response("registration",
        "Triax sales officer says: 'All energy weapons require registration. "
        "A receipt is issued on purchase. Keep it. NGR patrols check documentation.'");
    add_response("bye",
        "Triax sales officer says: 'Thank you for your purchase. Safe travel.'");
    set_default_response(
        "The officer nods once. 'If you have a product question I can answer it. "
        "Otherwise: list, buy <item>.'");
    set_property("position_str", "stands at the counter reviewing inventory logs.");

    set_shop_items(([
        "cp-40"         : ({ EQ_PATH+"cp40_laser.c",  13500, "CP-40 pulse laser rifle" }),
        "cp40"          : ({ EQ_PATH+"cp40_laser.c",  13500, "CP-40 pulse laser rifle" }),
        "cp-30"         : ({ EQ_PATH+"cp30_laser.c",   6800, "CP-30 heavy laser pistol" }),
        "cp30"          : ({ EQ_PATH+"cp30_laser.c",   6800, "CP-30 heavy laser pistol" }),
        "c-18"          : ({ EQ_PATH+"c18_pistol.c",   5000, "C-18 laser pistol" }),
        "c18"           : ({ EQ_PATH+"c18_pistol.c",   5000, "C-18 laser pistol" }),
        "vibro-knife"   : ({ EQ_PATH+"vibro_knife.c",  2200, "vibro-knife" }),
        "vibro knife"   : ({ EQ_PATH+"vibro_knife.c",  2200, "vibro-knife" }),
        "vibro-sword"   : ({ EQ_PATH+"vibro_sword.c",  5500, "vibro-sword" }),
        "vibro sword"   : ({ EQ_PATH+"vibro_sword.c",  5500, "vibro-sword" }),
        "e-clip"        : ({ EQ_PATH+"e_clip.c",         320, "standard e-clip (30 shots)" }),
        "eclip"         : ({ EQ_PATH+"e_clip.c",         320, "standard e-clip (30 shots)" }),
        "clip"          : ({ EQ_PATH+"e_clip.c",         320, "standard e-clip (30 shots)" }),
        "long e-clip"   : ({ EQ_PATH+"e_clip_long.c", 520, "long e-clip (60 shots)" }),
        "long clip"     : ({ EQ_PATH+"e_clip_long.c", 520, "long e-clip (60 shots)" }),
        "first aid kit" : ({ EQ_PATH+"first_aid_kit.c",  220, "first aid kit" }),
        "medkit"        : ({ EQ_PATH+"first_aid_kit.c",  220, "first aid kit" })
    ]));
}

void catch_tell(string str) {
    string a, b;

    if(sscanf(str, "%s says: %s", a, b) != 2) return;
    b = lower_case(b);

    if(strsrch(b, "price") != -1 || strsrch(b, "negotiate") != -1 ||
       strsrch(b, "discount") != -1 || strsrch(b, "deal") != -1) {
        say_here("The officer says: 'Prices are fixed. NGR Commerce Authority rates. "
                 "There is no negotiation.'");
        return;
    }
    ::catch_tell(str);
}
