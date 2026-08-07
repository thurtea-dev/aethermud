/* /domains/LoneStar/areas/lone_star_support_row.c
   Lone Star support town: the civilian strip south of the outer gate.
   The "livable city, if human" side of Lone Star (RiftsMUD Memories, 11).
   Connects north to the outer gate, east to the canteen.

   Registration sweep (2026-07-26): a Coalition patrol challenges loitering
   D-Bees for papers. Trigger and challenge logic live entirely in this
   file; the patrol reuses the existing cs_dog_boy_guard NPC unchanged.
   See the block comment above sweep_arrive() for the full sequence. */

#define NPC_PATH     "/domains/LoneStar/npcs/"
#define PATROL_NPC   "/domains/LoneStar/npcs/cs_dog_boy_guard.c"
#define DETENTION    "/domains/LoneStar/areas/lone_star_containment"

/* Sweep timing, in seconds. A D-Bee walking straight through is never
   challenged; only one who is still on the row when the patrol comes
   around is. */
#define SWEEP_MIN_DELAY  20
#define SWEEP_VAR_DELAY  40
#define SWEEP_WINDOW     30
#define SWEEP_COOLDOWN  300

#include <std.h>
#include <daemons.h>

inherit ROOM;

/* Rule 17: these are called above their definitions. */
void sweep_arrive();
void sweep_resolve();
int  cmd_show(string str);
private int  sweep_passes(object who);
private void sweep_end(int remove_patrol);
private void sweep_detain(object who);

private object __sweep_target;
private object __sweep_patrol;
private int    __sweep_ready_at;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 0);
    set_property("no attack", 0);
    set_property("vehicle_accessible", 1);
    set_short("Lone Star: Support Row");
    set_long(
        "A single paved street outside the wire, built for the people who\n"
        "keep the complex running and are not allowed to sleep inside it.\n"
        "Contractor housing on one side, a laundry, a clinic storefront, a\n"
        "credit office with its shutter half down. Everything is Coalition\n"
        "gray with the numbers repainted every few years.\n\n"
        "For a human with papers this is an ordinary, dull, reasonably safe\n"
        "place to live. For anyone who is not, the posted notice board makes\n"
        "the terms plain: registration is mandatory, and sweeps are weekly.\n\n"
        "The outer gate is north. The canteen is east.");
    set_exits( ([
        "north" : "/domains/LoneStar/areas/lone_star_gate",
        "east"  : "/domains/LoneStar/areas/lone_star_canteen"
    ]) );
    set_listen("default",
        "A laundry extractor running out of balance, someone arguing with\n"
        "the credit office through the shutter, and traffic on the gate road.");
    set_smell("default",
        "Hot laundry vent, frying oil from the canteen, and road dust.");
    set_items( ([
        "housing" : "Contractor housing in Coalition gray, unit numbers\n"
                    "repainted often enough to stay legible.",
        "laundry" : "An industrial laundry serving the complex. It runs on\n"
                    "the same shift clock as the labs.",
        "clinic"  : "A storefront clinic. Human patients only, per the card\n"
                    "taped inside the glass.",
        "office"  : "A credit office, shutter half down, hours posted and\n"
                    "not kept.",
        "notice"  : "REGISTRATION MANDATORY FOR ALL NON-HUMAN RESIDENTS.\n"
                    "WEEKLY VERIFICATION SWEEPS. FAILURE TO REGISTER IS\n"
                    "GROUNDS FOR DETENTION.",
        "board"   : "A notice board of standing orders and sweep schedules.\n"
                    "Older notices have been papered over, never removed.",
        "street"  : "One paved street, kept in better repair than the road\n"
                    "outside it."
    ]) );
    __sweep_target  = 0;
    __sweep_patrol  = 0;
    __sweep_ready_at = 0;
}

/* A character passes the sweep if the Coalition does not consider them an
   enemy at all (human, Dog Boy, CS psi-stalker, staff, CS member, or
   friendly coalition standing -- WANTED_D->is_cs_enemy() decides all of
   that), or if worn armor is presenting them as somebody else. The second
   test is the "proper gear" answer the memories describe: dead_boy_armor.c
   already sets an appearance name, so a D-Bee wearing one reads as a
   Coalition Dead Boy to anyone who does not know them. */
private int sweep_passes(object who) {
    string ap;

    if(!who || !objectp(who)) return 1;
    if(!(int)WANTED_D->is_cs_enemy(who)) return 1;
    ap = (string)who->query_appearance_override();
    if(ap && strlen(ap)) return 1;
    return 0;
}

void init() {
    ::init();
    /* Gated rather than unconditional (cf. horton_station.c): sweep_arrive()
       moves the patrol in from a call_out, where this_player() is 0. */
    if(!this_player() || !(int)this_player()->is_player()) return;
    add_action("cmd_show", "show");

    if((int)this_player()->query_ghost()) return;
    if(time() < __sweep_ready_at) return;
    if(__sweep_target && objectp(__sweep_target)) return;
    /* find_call_out guard matches chitown_gate.c so repeated entries in the
       same tick do not stack sweeps. */
    if(find_call_out("sweep_arrive") != -1) return;
    if(sweep_passes(this_player())) return;

    __sweep_target = this_player();
    call_out("sweep_arrive", SWEEP_MIN_DELAY + random(SWEEP_VAR_DELAY));
}

/* Sweep sequence:
     1. init() arms this call_out when an unregistered D-Bee stays on the row.
     2. sweep_arrive() brings the patrol in and demands papers, then opens a
        SWEEP_WINDOW-second window.
     3. The player may comply ('show papers'), walk out, or do nothing.
        Complying is detention. Leaving is a clean escape. Doing nothing is
        read as refusal and the patrol opens fire.
   Every message below is room-local: nothing here reaches a player who is
   not standing in this room (rule 20). */
void sweep_arrive() {
    object patrol;
    object *inv;
    int i;

    if(!__sweep_target || !objectp(__sweep_target)) { sweep_end(0); return; }
    if(environment(__sweep_target) != this_object()) { sweep_end(0); return; }
    if((int)__sweep_target->query_ghost()) { sweep_end(0); return; }
    /* Rechecked here: the player may have put armor on during the delay. */
    if(sweep_passes(__sweep_target)) { sweep_end(0); return; }

    /* A tagged patrol from an earlier sweep may still be standing here after
       a fight the player broke off. Do not stack a second one on top of it
       (same guard chitown_gate.c uses for its alarm). */
    inv = all_inventory(this_object());
    for(i = 0; i < sizeof(inv); i++) {
        if(inv[i] && (int)inv[i]->query_property("ls_sweep_patrol")) {
            sweep_end(0);
            return;
        }
    }

    patrol = clone_object(PATROL_NPC);
    if(!patrol) { sweep_end(0); return; }
    patrol->set_property("ls_sweep_patrol", 1);
    patrol->move(this_object());
    __sweep_patrol = patrol;

    tell_room(this_object(),
        "Boots on pavement. A Coalition patrol turns onto the row and works\n"
        "its way down the housing block, a Dog Boy at heel.\n", ({}) );
    tell_object(__sweep_target,
        "The Dog Boy stops in front of you and puts out a hand.\n"
        "\"Registration papers. Now.\"\n"
        "Type 'show papers' to comply.\n");
    tell_room(this_object(),
        "The patrol stops " + (string)__sweep_target->query_cap_name() +
        " and demands papers.\n", ({ __sweep_target }) );

    call_out("sweep_resolve", SWEEP_WINDOW);
}

/* Window expired with the player still standing there: read as refusal. */
void sweep_resolve() {
    object who;

    who = __sweep_target;
    if(!who || !objectp(who)) { sweep_end(1); return; }
    if((int)who->query_ghost()) { sweep_end(1); return; }
    /* Walked out during the window: the patrol moves on. No penalty -- the
       delayed trigger already gave them the chance, and taking it is the
       intended counterplay. */
    if(environment(who) != this_object()) { sweep_end(1); return; }
    if(sweep_passes(who)) {
        tell_object(who, "The Dog Boy looks you over once more and waves "
                         "you past.\n");
        sweep_end(1);
        return;
    }

    tell_room(this_object(),
        "\"Non-compliance. Take it down.\"\n", ({}) );
    if(__sweep_patrol && objectp(__sweep_patrol))
        __sweep_patrol->kill_ob(who, 0);
    /* Patrol stays in the room to fight; only the tracking state clears. */
    sweep_end(0);
}

/* Room-scoped 'show' handler. Returns 0 for anything it does not own so
   other handlers and the normal command search still get their turn. */
int cmd_show(string str) {
    object who;

    who = this_player();
    if(!who || who != __sweep_target) return 0;
    if(!__sweep_patrol || !objectp(__sweep_patrol)) return 0;
    if(!str || !strlen(str)) return 0;
    str = lower_case(str);
    if(str != "papers" && str != "paper" && str != "id" &&
       str != "id card" && str != "card" && str != "registration") return 0;

    if(find_call_out("sweep_resolve") != -1) remove_call_out("sweep_resolve");
    sweep_detain(who);
    return 1;
}

/* Compliance outcome. The ID card every character carries prints its
   holder's race (see equipment/id_card.c), so producing papers on this row
   is what confirms the problem rather than solving it. Non-lethal: the
   player keeps everything they are carrying and comes to in containment,
   where the breached maintenance tunnel is the way back out. */
private void sweep_detain(object who) {
    object dest_room;
    string wname;

    wname = (string)who->query_cap_name();
    tell_object(who,
        "You hold out your card. The Dog Boy reads it once, then reads the\n"
        "race line again, and does not hand it back.\n"
        "\"Unregistered. Take it in.\"\n");
    tell_room(this_object(),
        wname + " is marched off the row at gunpoint.\n", ({ who }) );

    catch(WANTED_D->add_infamy((string)who->query_name(), 50,
        "detained in a Lone Star registration sweep"));
    catch(FACTION_D->modify_standing((string)who->query_name(),
        "coalition", -25, "detained without registration"));

    who->cease_all_attacks();
    who->move(DETENTION);
    dest_room = environment(who);
    if(dest_room && dest_room != this_object()) {
        tell_object(who,
            "A cell door cycles shut behind you.\n");
        tell_room(dest_room,
            wname + " is pushed into the wing under guard.\n", ({ who }) );
    }
    sweep_end(1);
}

/* Clears sweep state. Pass 0 for remove_patrol when the patrol is mid-fight
   and has to stay in the room.
   The cooldown starts only if a patrol actually turned up. Starting it on
   the early-return paths instead would let a player step in and straight
   back out to buy themselves SWEEP_COOLDOWN seconds of quiet. */
private void sweep_end(int remove_patrol) {
    if(__sweep_patrol && objectp(__sweep_patrol)) {
        if(remove_patrol) destruct(__sweep_patrol);
        __sweep_ready_at = time() + SWEEP_COOLDOWN;
    }
    __sweep_patrol   = 0;
    __sweep_target   = 0;
    if(find_call_out("sweep_resolve") != -1) remove_call_out("sweep_resolve");
}

void reset() {
    ::reset();
    if(!present("contractor", this_object()))
        clone_object(NPC_PATH + "ls_civilian_contractor.c")->move(this_object());
}
