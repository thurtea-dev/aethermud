//      /bin/system/_update.c
//      from the Nightmare Mudlib
//      destructs an old version of a file and loads a new one
//      created by Sulam 911219
//      security changes by Descartes of Borg 930810

#include <std.h>
#include <rooms.h>
#include <security.h>
#include <move.h>

inherit DAEMON;

int cmd_update(string str) {
    object *ob;
    object *room_players;
    object obb, ob2;
    string file, res, load_path;
    int n, master_flag;
    int is_room;

    if(ambassadorp(previous_object())) return 0;
    if(!str) {
        if(!environment(this_player())) {
            write("No environment!");
            return 1;
        }
        obb = environment(this_player());
        file = file_name(obb);
        message("Nsystem", "Update environment ("+file+"): ", this_player());
        ob = all_inventory(obb);
        for (n = 0; n < sizeof(ob); n++)
        {
            if (ob[n]->is_player()) ob[n]->move(ROOM_VOID);
        }
        destruct(obb);
        if(this_player()->move(file) != MOVE_OK) 
          message("system", "Error in loading file.", this_player());
        for (n = 0; n < sizeof(ob); n++)
        {
	    if( ob[n] ) /* something may have happened during the update */
		ob[n]->move(environment(this_player()));
        }
        message("system", "Ok.", this_player());
        return 1;
    }
    str = absolute_path((string)this_player()->get_path(), str);
    load_path = str;
    if(strlen(load_path) > 2 && load_path[strlen(load_path)-2..] == ".c")
        load_path = load_path[0..strlen(load_path)-3];
    is_room = 0;
    room_players = ({});
    if((ob2 = find_object(load_path))) {
        if(function_exists("query_exits", ob2)) {
            is_room = 1;
            ob = all_inventory(ob2);
            for(n = 0; n < sizeof(ob); n++) {
                if(ob[n] && ob[n]->is_player()) {
                    ob[n]->move(ROOM_VOID);
                    room_players += ({ ob[n] });
                }
            }
        }
        if(ob2 == master()) master_flag = 1;
        catch(destruct(ob2));
    }
    res = catch(call_other(str, "???"));
    if(is_room && !res) {
        ob2 = find_object(load_path);
        if(ob2) {
            for(n = 0; n < sizeof(room_players); n++) {
                if(room_players[n])
                    room_players[n]->move(ob2);
            }
        }
    }
    if(res)
        message("system", str + ": " + res, this_player());
    else
        message("system", str + ": updated and loaded.", this_player());
    return 1;
}

void help() {
    message("help",
      "Syntax: <update [file]>\n\n"
      "Destructs the master object of the file named and then attempts "
      "to reload a new version of it.\n\n"
      "See also: call, clone, cref, dest, dref, load, renew", this_player()
    );
}
