Read CLAUDE.md first and confirm you're following the rules there before doing anything else.

Context: last session built the ocean access system (breathing gate on
the three underwater rooms, rebreather item on the neck cosmetic slot,
blue rift spawner in chitown_boulevard.c, unique Sword of Atlantis via
daemon/unique_items.c). It was committed (a18e26d) and deployed, but I
have not yet run a live test pass myself.

Design confirmation: the rebreather using the neck slot is correct and
final - no change needed there. Players simply need to remove any
necklace/amulet they're wearing first to equip it, which is intended and
fine as-is. Do not add a separate slot or rework this.

Before I run my live pass, re-read back through equipment/rebreather.c,
the three ocean/ room files, ocean_rift.c, sword_of_atlantis.c, moxim.c,
and chitown_boulevard.c exactly as they exist in the committed code right
now (not from memory of writing them), and confirm out loud, file by file,
that each does what the session report claimed. Specifically:

1. Confirm wearing the rebreather correctly requires removing any current
   neck-slot item first (normal slot-swap behavior), and that the item's
   description/messaging makes it clear to a player why "wear rebreather"
   fails if their neck slot is occupied - if the error message is generic
   or unclear, improve it so it explicitly says to remove the neck item
   first.
2. Confirm the bounce-on-fail logic in the three ocean rooms actually
   fires for a plain vagabond-type mortal with zero gear (trace it, don't
   assume).
3. Confirm the blue rift's !random(4) chance in chitown_boulevard.c reset()
   doesn't collide with or get suppressed by the room's existing Moxim/
   guardian spawn chances already in that same reset() function.
4. Confirm daemon/unique_items.c was actually added to preload.cfg in the
   correct load order relative to any daemon it depends on, and that the
   full reboot instruction is still accurate.

Report any discrepancy between what was claimed and what the code actually
does. Do not make changes unless you find an actual bug or the unclear-
error-message issue in point 1 - if you find either, fix it and clearly
flag what changed. If everything else checks out as claimed, say so
plainly and confirm the file list is otherwise unchanged from last
session's report.

End with the standard session-end report.