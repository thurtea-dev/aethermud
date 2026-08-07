/* /domains/adm/vehicle_test/vt_main.c
   Vehicle Test Zone: main staging area.
   Staff use only. Tests hover_cycle entry/exit/drive, EX-5 instructions.
   A 4-room loop (main, north, east, south) all have vehicle_accessible.

   HOW TO REACH:
     Staff must goto or move themselves here directly:
     > goto /domains/adm/vehicle_test/vt_main
     Or use a staff_of_creation "build goto" command.

   WHAT IS HERE:
     - Hover cycle (clones on reset, basic vehicle test)
     - Signs explaining all vehicle commands
     - Loop connecting north/east/south for drive testing
*/

#include <std.h>
#include <rooms.h>

#define HOVER_CYCLE "/domains/Praxis/equipment/hover_cycle.c"

inherit ROOM;

void create() {
    ::create();
    set_property("light", 3);
    set_property("indoors", 1);
    set_property("vehicle_accessible", 1);
    set_property("ex5_accessible", 1);
    set_short("Vehicle Test Zone: Staging Bay");
    set_long(
        "Vehicle Test Zone -- staff access only.\n"
        "A wide staging bay with permacrete floor and lane markings.\n"
        "Connects north, east, and south into a triangular test loop.\n"
        "All rooms: vehicle_accessible=1, ex5_accessible=1.\n\n"
        "VEHICLES PRESENT\n"
        "  hover cycle  -- Tier 1 simple vehicle (is_vehicle, no interior rooms)\n"
        "  EX-5 Behemoth -- Tier 2 multi-room (east pad; enter ex-5 to board)\n\n"
        "HOVER CYCLE COMMANDS\n"
        "  enter hover cycle     -- board\n"
        "  drive north           -- move (must be vehicle_accessible destination)\n"
        "  exit                  -- dismount\n"
        "  eject                 -- emergency dismount\n\n"
        "EX-5 QUEST PATH\n"
        "  Splynn Preserves -> east -> Forest Clearing\n"
        "  get shovel -> wield shovel -> dig\n"
        "  (falls into Splynn Underground)\n"
        "  enter ex-5 -> up (cockpit)\n"
        "  power on (needs Pilot Robot Combat Elite)\n"
        "  up (EX-5 moves to Splynn Market)\n"
        "  west (moves to Long Road)\n"
        "  fire <target> -- 5-second weapon cooldown\n"
        "  power off\n"
        "  eject -- lands outside, in EX-5 current zone\n"
        "  status -- shows fuel/power/crew from cockpit\n\n"
        "EX-5 INTERIOR LAYOUT\n"
        "  hull (entry) -- north: forward bay, south: crew quarters\n"
        "               -- west: portside, east: cargo, up: cockpit\n"
        "  out/eject -- drops to current exterior zone\n\n"
        "Exits: north, east (EX-5 pad), south (test loop).");
    set_exits( ([
        "north" : "/domains/adm/vehicle_test/vt_north",
        "east"  : "/domains/adm/vehicle_test/vt_ex5_pad",
        "south" : "/domains/adm/vehicle_test/vt_south"
    ]) );
    set_items( ([
        "signs"   : "Command reference plaques on the wall. Read 'look' for the full list.",
        "floor"   : "Permacrete with painted lane markings. Wide and flat.",
        "loop"    : "The test loop: north corridor, east corridor, south corridor.\n"
                    "All rooms are vehicle_accessible and ex5_accessible.",
        "cycle"   : "The hover cycle parked in the center. Enter it and drive\n"
                    "north/east/south/west to test Tier 1 vehicle movement.",
        "tier 1"  : "Simple vehicles. Player rides inside the vehicle object.\n"
                    "Movement: drive <direction>. Eject: eject.\n"
                    "Example: hover cycle.",
        "tier 2"  : "Multi-room vehicles. Interior is real rooms, daemon tracks state.\n"
                    "Movement: pilot from cockpit with directional commands.\n"
                    "Eject: from any interior room. Example: EX-5 Behemoth."
    ]) );
}

void reset() {
    ::reset();
    if(!present("hover cycle", this_object()))
        clone_object(HOVER_CYCLE)->move(this_object());
}
