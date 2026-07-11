/* /std/rift_portal.c
   A shimmering dimensional rift portal cloned by Moxim.
   Lasts 30 seconds. Any player in the room can enter before it closes.
   call_this_player_through() is defined first to avoid forward-reference
   issues with private functions in FluffOS 2.9. */

#include <std.h>

inherit OBJECT;

private string __destination;
private string __label;

void create() {
    ::create();
    set_name("rift");
    set_id( ({ "rift", "portal", "rift portal", "shimmering rift", "dimensional rift" }) );
    set_short("a shimmering rift");
    set_long(
        "A swirling oval of crackling dimensional energy fills the air before\n"
        "you. Through the distortion you can almost make out the other side.\n"
        "Step through to travel.");
    set_mass(0);
    set_property("no_get", 1);
    call_out("self_destruct", 30);
}

void set_destination(string dest) { __destination = dest; }

void set_label(string lbl) {
    __label = lbl;
    set_short("a shimmering rift to " + lbl);
}

string query_destination() { return __destination; }
string query_label()       { return __label; }

/* Move this_player() through the rift to __destination. */
private void call_this_player_through() {
    object tp;
    object src;
    object dst_room;
    string dest;
    string lbl;

    tp   = this_player();
    src  = environment(this_object());
    dest = __destination;
    lbl  = __label ? __label : "your destination";

    if(!tp || !objectp(tp)) return;
    if(!dest || !strlen(dest)) {
        write("The rift leads nowhere. It collapses with a crack.\n");
        destruct(this_object());
        return;
    }
    write(
        "You step through the rift. Reality folds around you. For a moment\n"
        "there is only light and the roar of dimensions colliding. Then you arrive.\n");
    if(src) {
        tell_room(src,
            (string)tp->query_cap_name() + " steps into the rift and vanishes.\n",
            ({ tp, this_object() }));
    }
    tp->move_player(dest);
    dst_room = environment(tp);
    if(dst_room) {
        tell_room(dst_room,
            "A shimmering rift tears open and " + (string)tp->query_cap_name() +
            " steps through from the other side.\n",
            ({ tp }));
    }
    call_out("self_destruct", 1);
}

void init() {
    ::init();
    add_action("do_enter", "enter");
    add_action("do_go",    "go");
    add_action("do_step",  "step");
}

/* Only intercept 'enter rift' or 'enter portal' -- never bare 'enter'. */
int do_enter(string str) {
    string s;

    if(!str || !strlen(str)) return 0;
    s = lower_case(str);
    if(strsrch(s, "rift") == -1 && strsrch(s, "portal") == -1) return 0;
    call_this_player_through();
    return 1;
}

/* Accept: go rift, go portal, go through */
int do_go(string str) {
    string s;

    if(!str || !strlen(str)) return 0;
    s = lower_case(str);
    if(strsrch(s, "rift") == -1 && strsrch(s, "portal") == -1 &&
       strsrch(s, "through") == -1) return 0;
    call_this_player_through();
    return 1;
}

/* Accept: step rift, step portal, step through */
int do_step(string str) {
    string s;

    if(!str || !strlen(str)) return 0;
    s = lower_case(str);
    if(strsrch(s, "rift") == -1 && strsrch(s, "portal") == -1 &&
       strsrch(s, "through") == -1) return 0;
    call_this_player_through();
    return 1;
}

void self_destruct() {
    object env;

    env = environment(this_object());
    if(env) {
        tell_room(env,
            "The shimmering rift collapses with a sharp crack of displaced air.\n",
            ({}) );
    }
    if(objectp(this_object())) destruct(this_object());
}
