/*    /std/room/exits.c
 *    from Nightmare IV
 *    handles players exiting from rooms
 *    created by Descartes of Borg 940711
 */

#include <std.h>
#include <rooms.h>
#include "exits.h"

#define MAX_TRACKS           10
#define MAX_SCENTS           10
#define TRACK_FADE_TIME      120

private nosave mapping __Exits, __Enters;
private nosave mapping __Tracks, __TrackFaded, __Scents, __ScentFaded;

protected void create() {
    __Exits = ([]);
    __Enters = ([]);
    __Tracks = ([]);
    __TrackFaded = ([]);
    __Scents = ([]);
    __ScentFaded = ([]);
}

void init() {
    string *cles;
    string *cardinals;
    int i;
    string dir;

    cardinals = ({
        "north", "south", "east", "west", "up", "down", "out",
        "northeast", "northwest", "southeast", "southwest",
        "n", "s", "e", "w", "u", "d", "ne", "nw", "se", "sw",
        "go", "enter"
    });
    add_action("cmd_go", "go");
    add_action("cmd_enter", "enter");
    /* full direction names */
    add_action("cmd_dir_north",     "north");
    add_action("cmd_dir_south",     "south");
    add_action("cmd_dir_east",      "east");
    add_action("cmd_dir_west",      "west");
    add_action("cmd_dir_up",        "up");
    add_action("cmd_dir_down",      "down");
    add_action("cmd_dir_out",       "out");
    add_action("cmd_dir_northeast", "northeast");
    add_action("cmd_dir_northwest", "northwest");
    add_action("cmd_dir_southeast", "southeast");
    add_action("cmd_dir_southwest", "southwest");
    /* short aliases */
    add_action("cmd_dir_north",     "n");
    add_action("cmd_dir_south",     "s");
    add_action("cmd_dir_east",      "e");
    add_action("cmd_dir_west",      "w");
    add_action("cmd_dir_up",        "u");
    add_action("cmd_dir_down",      "d");
    add_action("cmd_dir_northeast", "ne");
    add_action("cmd_dir_northwest", "nw");
    add_action("cmd_dir_southeast", "se");
    add_action("cmd_dir_southwest", "sw");
    /* non-cardinal named exits */
    cles = keys(__Exits);
    for(i = 0; i < sizeof(cles); i++) {
        dir = cles[i];
        if(member_array(dir, cardinals) == -1)
            add_action("cmd_named_exit", dir);
    }
}

int cmd_dir_north(string s)     { return cmd_go("north"); }
int cmd_dir_south(string s)     { return cmd_go("south"); }
int cmd_dir_east(string s)      { return cmd_go("east"); }
int cmd_dir_west(string s)      { return cmd_go("west"); }
int cmd_dir_up(string s)        { return cmd_go("up"); }
int cmd_dir_down(string s)      { return cmd_go("down"); }
int cmd_dir_out(string s)       { return cmd_go("out"); }
int cmd_dir_northeast(string s) { return cmd_go("northeast"); }
int cmd_dir_northwest(string s) { return cmd_go("northwest"); }
int cmd_dir_southeast(string s) { return cmd_go("southeast"); }
int cmd_dir_southwest(string s) { return cmd_go("southwest"); }

int cmd_go(string str) {
    if((int)this_player()->query_paralyzed())
      return notify_fail("You are unable to move.\n");
    if(!__Exits[str]) return notify_fail("You go nowhere at all.\n");
    if(__Exits[str]["pre"] && !((int)(*__Exits[str]["pre"])(str))) return 1;
    add_track(this_player(), str);
    add_scent(this_player(), str);
    this_player()->move_player(__Exits[str]["room"], str);
    if(__Exits[str]["post"]) (*__Exits[str]["post"])(str);
    return 1;
}

int cmd_enter(string str) {
    if((int)this_player()->query_paralyzed())
      return notify_fail("You are unable to move.\n");
    if(!__Enters[str]) return notify_fail("You cannot enter that.\n");
    if(__Enters[str]["pre"] && !((int)(*__Enters[str]["pre"])(str))) return 1;
    add_track(this_player(), str);
    add_scent(this_player(), str);
    this_player()->move_player(__Enters[str]["room"], str);
    if(__Enters[str]["post"]) (*__Enters[str]["post"])( str);
    return 1;
}

int cmd_named_exit(string str) {
    string dir;

    dir = query_verb();
    if(!dir || !__Exits[dir]) return 0;
    return cmd_go(dir);
}

void reinitiate() {
    object *obs;

    if(!sizeof(obs = all_inventory(this_object()))) return;
    obs->move(ROOM_VOID);
    obs->move(this_object());
}

void set_exits(mapping mp) {
    mixed it;
    string *cles;
    int i, j;

    __Exits = ([]);
    i = sizeof(cles = keys(mp));
    while(i--) {
	it = mp[cles[i]];
	if (stringp(it))
	    __Exits[cles[i]] = ([ "room" : it ]);
	else if ( pointerp(it) && (j = sizeof(it)) ) {
	    if (j && stringp(it[0]) ) {
		__Exits[cles[i]] = ([ "room" : it[0] ]);
		if (j > 1 && functionp(it[1])) {
		    __Exits[cles[i]]["pre"]=it[1];
		    if (j > 2 && functionp(it[2]))
			__Exits[cles[i]]["post"]=it[2];
		}
	    }
	}
    }
}

varargs void add_exit(string dir, string dest, function pre, function post) {
    if(!stringp(dir)) error("Bad argument 1 to add_exit().\n");
    if(!stringp(dest)) error("Bad argument 2 to add_exit().\n");
    __Exits[dir] = ([ "room" : dest ]);
    if(pre) __Exits[dir]["pre"] = pre;
    if(post) __Exits[dir]["post"] = post;
    reinitiate();
}

void remove_exit(string dir) {
    if(__Exits[dir]) map_delete(__Exits, dir);
    reinitiate();
}

string query_exit(string str) {
    if(!__Exits[str]) return 0;
    else return __Exits[str]["room"];
}

string *query_exits() { return keys(__Exits); }

void set_enters(mapping mp) {
    string *cles;
    int i;

    __Enters = ([]);
    i = sizeof(cles = keys(mp));
    while(i--) __Enters[cles[i]] = ([ "room" : mp[cles[i]] ]);
}

varargs void add_enter(string dir, string dest, function pre, function post) {
    if(!stringp(dir)) error("Bad argument 1 to add_enter().\n");
    if(!stringp(dest)) error("Bad argument 2 to add_enter().\n");
    __Enters[dir] = ([ "room" : dest ]);
    if(pre) __Enters[dir]["pre"] = pre;
    if(post) __Enters[dir]["post"] = post;
    reinitiate();
}

void remove_enter(string dir) { 
    if(__Enters[dir]) map_delete(__Enters, dir);
    reinitiate();
}

string query_enter(string dir) {
    if(!__Enters[dir]) return 0;
    else return __Enters[dir]["room"];
}

string *query_enters() { return keys(__Enters); }

string query_direction(string dest) {
    string *cles;
    int i;

    i = sizeof(cles = keys(__Exits));
    while(i--) if(__Exits[cles[i]]["room"] == dest) return "go "+cles[i];
    i = sizeof(cles = keys(__Enters));
    while(i--) if(__Enters[cles[i]]["room"] == dest) return "enter "+cles[i];
    return 0;
}

void add_track(object ob, string str) {
    string res;

    if(sizeof(__Tracks) > MAX_TRACKS) {
        __Tracks = ([]);
        __TrackFaded = ([]);
    }
    if(!(res = (string)ob->query_race())) return;
    if(!__TrackFaded[res]) __TrackFaded[res] = time();
    if(!__Tracks[res]) __Tracks[res] = 0;
    __Tracks[res]++;
}

int query_track(string res) {
    if(!__Tracks[res]) return 0;
    else __Tracks[res] -= (time() - __TrackFaded[res])/TRACK_FADE_TIME;
    __TrackFaded[res] = time();
    if(__Tracks[res] < 1) {
        map_delete(__Tracks, res);
        map_delete(__TrackFaded, res);
    }
    return __Tracks[res];
}

mapping query_tracks() {
    mapping tmp;
    string *cles;
    int i;

    tmp = allocate_mapping(i = sizeof(cles = keys(__Tracks)));
    while(i--) tmp[cles[i]] = query_track(cles[i]);
    return tmp;
}

void add_scent(object ob, string str) {
    string nom;

    if(sizeof(__Scents) > MAX_SCENTS) {
        __Scents = ([]);
        __ScentFaded = ([]);
    }
    if(!(nom = (string)ob->query_name())) return;
    if(!__ScentFaded[nom]) __ScentFaded[nom] = time();
    if(!__Scents[nom]) __Scents[nom] = 0;
    __Scents[nom] += (int)ob->query_scent();
}

int query_scent(string str) { 
    if(!__Scents[str]) return 0;
    else __Scents[str] -= (time() - __ScentFaded[str]);
    __ScentFaded[str] = time();
    if(__Scents[str] < 1) map_delete(__Scents, str);
    return __Scents[str]; 
}

mapping query_scents() {
    mapping tmp;
    string *cles;
    int i;

    tmp = allocate_mapping( i = sizeof( cles = keys(__Scents)));
    while(i--) tmp[cles[i]] = query_scent(cles[i]);
    return tmp;
}
