# WHY: lib/domains/

## Why the world is shaped like this

AetherMUD is a faithful recreation of a specific early-2000s Palladium
Rifts MUD, not a from-scratch design. Zone choices (Praxis as the
starting town, Chi-Town, Splynn, the slave markets, rift travel via
Moxim) follow player memories of the original game, archived in
`nightmare3_fluffos_v2/RiftsMUD Memories.txt` at the lib root's parent.
When deciding what to build next, that file and the gap report
(`Praxis/adm/master_gap_report.txt`) outrank invention.

## Why Praxis contains so much that is not Praxis

The lib started as Nightmare III's example world and was converted
in place. Praxis inherited the role of "default domain": chargen
(`setter.c`), the equipment tree, and even Splynn/Atlantis (under
`Praxis/areas/splynn/`) live there because splitting them out would
break dozens of hardcoded path references for little gain. The same
logic keeps `equipment/hover_cycle.c` and the legacy magic items at
their original paths: rooms and NPCs reference them, and the cost of a
repo-wide path migration exceeds the tidiness benefit.

## Why standardOld/ is still there

`Praxis/standardOld/` is legacy NM3 content that still errors on reset.
It is deliberately untouched: it is a reference for how the original
rooms worked, deleting it risks breaking things that still inherit or
reference it, and the errors are contained. See docs/ASSESSMENT.md
section 7 before deciding to clean it up.

## Why reskin instead of rewrite

Many rooms (hospital, pub, library, town hall, sheriff) are NM3 rooms
reskinned with Rifts descriptions rather than rebuilt. This preserved
working exit graphs and object wiring while the effort went into
systems (combat, magic, chargen). A reskin is "done" when no NM3
fantasy content remains player-visible, not when the file is pretty.
