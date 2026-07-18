# HOWTO: lib/domains/

## Build a room by hand

```c
/* /domains/Praxis/my_room.c */
#include <std.h>
#include <rooms.h>

inherit ROOM;

void create() {
    ::create();
    set_property("light", 2);
    set_property("indoors", 1);
    set_short("A dusty outpost");
    set_long("Prose description. Do not mention NPCs or items that "
        "spawn here; they display themselves below the exits line.");
    set_exits( ([ "south" : "/domains/Praxis/square" ]) );
}

void reset() {
    object npc;

    ::reset();
    if(!present("guard", this_object())) {
        npc = new("/domains/Praxis/obj/mon/my_guard");
        if(npc) npc->move(this_object());
    }
}
```

Non-negotiables (CLAUDE.md): spawns in `reset()` with a `present()`
guard, never in `create()`; no spawned things in `set_long()`; no
ambient messages; smell/listen only via `set_smell()`/`set_listen()`.

Reload with `update /domains/Praxis/my_room.c` in-game, then check
`/log/catch` and `log/errors/<dir>` for compile errors.

## Build an NPC

Put it under the zone's monster directory (Praxis:
`/domains/Praxis/obj/mon/`). Inherit the monster base, set stats with
`set_stats()`. For Mega-Damage creatures use `set_stats("MDC", n)` and
`set_stats("max_MDC", n)` plus `set_property("mdc_creature", 1)`; there
is no `set_mdc()` on monsters. Give named NPCs
`set_property("position_str", "...")` for their room line.

## Build equipment

Weapons, armor, cybernetics, power armor, grenades, and magic items
each have a subdirectory under `/domains/Praxis/equipment/`. Copy the
closest existing file in the right subdirectory and match its API
exactly (`set_armour_type`, `equip_armour_to_limb`, and the ARMOUR
macro keep their British spelling; user-facing strings use American
spelling). Protective armor gets
`set_property("rifts_slot", "armor")` for the eq display.

## Or skip all of the above with QCS

Coding wizards holding the Staff of Creation can generate rooms, NPCs,
weapons, and armor into `/realms/<name>/area/` with the `build`
command (`help build` in-game). QCS output is plain LPC that can be
hand-polished afterward and later moved into a domain by an admin.

## Checklist before calling a room done

1. `update` it in-game; check `/log/catch`.
2. Walk every exit both ways.
3. `look` at everything mentioned in the long description.
4. Confirm NPCs appear below the exits line, not inside the prose.
