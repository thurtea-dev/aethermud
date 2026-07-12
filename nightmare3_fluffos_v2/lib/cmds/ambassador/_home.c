#include <std.h>
#include <move.h>
#include <objects.h>
#include <daemons.h>

inherit DAEMON;

/* Send a movement message to each living in the room, swapping the
   mover's cap name for the viewer-specific display name so unknown
   players show as their race, matching move_player() in std/user.c. */
static void announce_move(object room, string msg_class, string tmpl) {
    object *inv;
    string cname, vname;
    int i;

    if(!room || !tmpl || tmpl == "") return;
    cname = (string)this_player()->query_cap_name();
    inv = all_inventory(room);
    for(i = 0; i < sizeof(inv); i++) {
        if(!inv[i] || !living(inv[i]) || inv[i] == this_player()) continue;
        vname = (string)this_player()->query_display_name(inv[i]);
        if(!vname || vname == "") vname = cname;
        message(msg_class, replace_string(tmpl, cname, vname), inv[i]);
    }
}

int cmd_home(string str) {
    object ob, prev;
    string who;

    prev = environment(this_player());
    if(!str) who = (string)this_player()->query_name();
    else who = lower_case(str);
    str = user_path(who);
    if(ob = (object)this_player()->query("workroom")) {
        if(ob == prev) return notify_fail("You twitch.\n");
        if((int)this_player()->move(ob) == MOVE_OK) {
            this_player()->describe_current_room(1);
            if(hiddenp(this_player()) || this_player()->query_invis())
              return 1;
            announce_move(prev, "mmout", (string)this_player()->query_mhome());
            announce_move(ob, "mmin", (string)this_player()->query_mmin());
            return 1;
        }
    }
    if(!ob) {
      if(!(ob = load_object(str+"workroom")) &&
        str == user_path((string)this_player()->query_name())) {
        ob = new(OB_WORKROOM);
        this_player()->set("workroom", ob);
      }
      else if(!(ob = load_object(str+"workroom"))) {
        if(!(ob = find_player(who)) || !(ob=(object)ob->query("workroom")))
          return notify_fail(capitalize(who)+" has no active workroom.\n");
      }
      else {
        if(file_name(prev) == str+"workroom")
          return notify_fail("You twitch.\n");
        if((int)this_player()->move(ob) == MOVE_OK) {
            this_player()->describe_current_room(1);
            if(hiddenp(this_player()) || this_player()->query_invis())
              return 1;
            announce_move(prev, "mmout", (string)this_player()->query_mhome());
            announce_move(environment(this_player()), "mmin",
              (string)this_player()->query_mmin());
            return 1;
        }
        return notify_fail("You stay where you are.\n");
      }
    }
    if(ob == prev) return notify_fail("You twitch.\n");
    if((int)this_player()->move(ob) == MOVE_OK) {
        this_player()->describe_current_room(1);
        if(hiddenp(this_player()) || this_player()->query_invis()) return 1;
        announce_move(prev, "mmout", (string)this_player()->query_mhome());
        announce_move(ob, "mmin", (string)this_player()->query_mmin());
        return 1;
    }
    return notify_fail("You stay where you are.\n");
}
