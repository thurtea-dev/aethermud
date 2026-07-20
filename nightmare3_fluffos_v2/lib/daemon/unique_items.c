/* /daemon/unique_items.c
   One-time unique item tracker. A room's reset() asks query_taken()
   before spawning a unique item; the item calls mark_taken() the
   first time a player picks it up. State persists across reboots in
   /daemon/save/unique_items.o. First use: the Sword of Atlantis. */

#include <std.h>

inherit DAEMON;

#define SAVE_FILE "/daemon/save/unique_items"

private mapping __taken;

void create() {
    daemon::create();
    set_no_clean(1);
    __taken = ([]);
    if(file_size(SAVE_FILE + __SAVE_EXTENSION__) > 0)
        restore_object(SAVE_FILE);
    if(!mapp(__taken)) __taken = ([]);
}

int query_taken(string key) {
    if(!key || !stringp(key) || !strlen(key)) return 0;
    return __taken[key] ? 1 : 0;
}

void mark_taken(string key) {
    if(!key || !stringp(key) || !strlen(key)) return;
    if(__taken[key]) return;
    __taken[key] = time();
    save_object(SAVE_FILE);
}

/* Admin/eval helper: clear a flag so the item can spawn again. */
void clear_taken(string key) {
    if(!key || !stringp(key) || !strlen(key)) return;
    if(!archp(previous_object()) && !archp(this_player())) return;
    map_delete(__taken, key);
    save_object(SAVE_FILE);
}

mapping query_all_taken() { return copy(__taken); }
