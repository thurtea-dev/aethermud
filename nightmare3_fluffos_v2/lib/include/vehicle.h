/* /include/vehicle.h
   Vehicle property name constants for AetherMUD.

   TWO-TIER VEHICLE SYSTEM
   -----------------------
   Tier 1: Simple vehicles (rifts_vehicle.c inheritors).
     - Object-based containers. Players move inside the vehicle object.
     - Movement via _drive.c: vehicle->vehicle_move(dest).
     - Eject via _eject.c: move player to environment(vehicle).
     - Required property on object: PROP_IS_VEHICLE.
     - Required on source AND destination rooms: PROP_VEHICLE_ACCESSIBLE.
     Example: hover cycle.

   Tier 2: Multi-room vehicles (daemon-controlled, room-based interiors).
     - Each room in the vehicle's interior is a real ROOM file.
     - A controlling daemon tracks position, fuel, passengers, weapon state.
     - Movement via daemon: daemon->pilot_move(who, dir).
     - Eject via _eject.c: call daemon->query_current_room() for position.
     - Required property on each interior room: PROP_VEHICLE_INTERIOR.
     - Required on each interior room: PROP_VEHICLE_DAEMON (daemon path string).
     - Required on destination rooms: PROP_VEHICLE_ACCESSIBLE or PROP_EX5_ACCESSIBLE.
     Example: EX-5 Behemoth. */

/* Marks a Tier 1 vehicle container object (set in rifts_vehicle.c). */
#define PROP_IS_VEHICLE         "is_vehicle"

/* Marks an interior room of a Tier 2 multi-room vehicle. */
/* Must also set PROP_VEHICLE_DAEMON on the same room. */
#define PROP_VEHICLE_INTERIOR   "vehicle_interior"

/* Path string to the controlling daemon for a Tier 2 vehicle interior room. */
/* _eject.c reads this to find the daemon without hardcoding a vehicle name. */
/* Example: set_property(PROP_VEHICLE_DAEMON, "/daemon/ex5_d"); */
#define PROP_VEHICLE_DAEMON     "vehicle_daemon"

/* Marks a room that standard vehicles (Tier 1 and Tier 2) can enter. */
/* _drive.c requires this on both source and destination rooms. */
/* ex5_d.c pilot_move accepts this OR PROP_EX5_ACCESSIBLE on the destination. */
#define PROP_VEHICLE_ACCESSIBLE "vehicle_accessible"

/* Marks a room that the EX-5 Behemoth specifically can traverse.             */
/* The EX-5 has an oversize footprint; not all vehicle-accessible rooms can   */
/* accommodate it. Some rooms have this without PROP_VEHICLE_ACCESSIBLE.      */
/* ex5_d.c pilot_move accepts PROP_VEHICLE_ACCESSIBLE OR PROP_EX5_ACCESSIBLE. */
#define PROP_EX5_ACCESSIBLE     "ex5_accessible"
