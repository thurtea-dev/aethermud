/* /d/standard/hotel.c
 * from Nightmare IV
 * a place to stay for rest and other things
 * created by Descartes of Borg (what a cutie!)
 * and Lassondra@Nightmare 940424
 */

#include <std.h>
#include <clock.h>
#include <daemons.h>

inherit ROOM;

int query_occupied(int chambre);
string query_key_id(int chambre);
object query_room(int chambre);
void check_out();
private nosave void random_key(int chambre);

private nosave mapping __Rooms;

void create() {
    int x;

    room::create();
    set_properties( ([ "indoors":1, "light":2, "no castle":1]) );
    set_short("the Praxis Refuge Inn");
    set_long(
      "You find yourself in the lobby of the Praxis Refuge Inn, the "
      "most prestigious lodging in Praxis. The floor is carpeted in "
      "plush red, patched but proudly kept, and the walls are trimmed "
      "with salvaged gold paint. Heavy oaken doors stand behind you, "
      "and the front desk faces the entrance. The guest hallway is "
      "north, past the desk."
" Type list to see the available rooms, and <rent 'type'> to rent a certain type of room."
	      );
    set_items( ([ "entrance" : "The lobby of the hotel is very "
		 "large and spacious. It looks like it is a rather "
		 "expensive hotel.",
		 "floor" : "The floor is covered with a very thick "
		 "red carpet, with golden fringes at the edges.",
		 "carpet" : "The carpet is in good shape, although "
		 "it is a bit worn in spots. There is a golden "
		 "symbol of the hotel standing in the center of it.",
		 ({ "wall", "walls" }) :
		 "The walls have been painted white with some gold "
		 "trim on all the edges.",
	      ({ "doors", "door" }) :
		 "The heavy oaken doors are ornately carved with "
		 "the hotel ensignia. The doors gleam in the light "
		 "from the large amount of polishing they receive.",
		 "desk" : "The front desk is also made from oak, and "
		 "stands at the back of the lobby.",
		 "hotel" : "You are in the Praxis Inn."
		 ])
	      );
set_exits( ([ "north" : "/domains/Praxis/hall" ]) );
    if(!event_pending(this_object())) {
        if((x=hour(time())) > 9) x= (9+(20-x))*HOUR;
        else x = HOUR*(9-x);
        event("check_out", x, 0, 1);
    }
    set_no_clean(1);
    __Rooms = allocate_mapping(10);
    this_object()->check_out();
}

void reset() {
    int x;

    room::reset();
    if(present("receptionist")) return;
    new("/domains/Praxis/obj/mon/receptionist")->move(this_object());
}

void set_occupied(int x) { __Rooms[x]["occupied"] = 1; }

int query_occupied(int chambre) {
    return __Rooms[chambre]["occupied"];
}

string query_key_id(int chambre) {
    return __Rooms[chambre]["key id"];
}

object query_room(int x) {
    return load_object(sprintf("/domains/Praxis/hotelroom%d", x));
  }

void check_out() {
    object *tmp;
    int i, j;

    if(previous_object() != this_object() &&
      base_name(previous_object()) != EVENTS_D) return;
    for(i=1; i<11; i++) {
        __Rooms[100+i] = ([ "occupied":0, "key id": random_key(100+i) ]);
        j = sizeof(tmp = livings() & all_inventory(query_room(100+i)));
        while(j--) {
            message("my_action", "Check out time!\nThe receptionist "
              "hurries you out into the lobby.", tmp[j]);
            tmp[j]->move_player("/domains/Praxis/hotel", "out");
	  }
      }
}

string random_key(int chambre) {
    return sprintf("praxishotel%d%d", chambre, random(100));
}






