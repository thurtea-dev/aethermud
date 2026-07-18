# WHY: lib/std/

## Why edits here are expensive

LPC resolves `inherit` at compile time: every room, player, and NPC has
the std code it inherited baked in from the moment it compiled.
Reloading a blueprint later does not retrofit existing objects. That is
why CLAUDE.md draws a hard line: std edits mean a full reboot, and the
project batches std work accordingly instead of hotfixing.

## Why the NM3 layering is preserved

user.c / living.c / monster.c / room.c and the `living/` split are pure
Nightmare III architecture. The Rifts conversion deliberately layered
on top (rifts_npc.c, rifts_corpse.c, rifts_vehicle.c, the daemons)
instead of rewriting the bases, because the bases encode years of
debugged edge cases (movement, containers, limbs, saving). New Rifts
mechanics prefer daemons (update-safe) over std edits (reboot) whenever
the mechanic does not need to live inside the object.

## Why armour.c keeps its British name

Upstream NM3 API. Rooms, items, and headers across the lib call
`set_armour_type` and friends; renaming would be a repo-wide breaking
change with zero player-visible benefit. The compromise, codified in
CLAUDE.md: internal identifiers stay British, player-facing strings are
American.

## Why user.c owns so much

The player object is the only place that reliably sees every player
action (movement, message receipt, login/logout), so cross-cutting
player features accumulate there: room display order, message-class
colors, the prowl movement roll, promotion plumbing (set_position,
workroom setup). When adding to it, put pure logic in a daemon and keep
only the hook in user.c if you can; hooks are cheap to keep, logic in
user.c costs a reboot per iteration.
