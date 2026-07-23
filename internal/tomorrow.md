Read CLAUDE.md and confirm you are following all rules there
before doing anything else.

Retire the three legacy guild backdoor rooms:
cleric_join.c, fighter_join.c, and kataan_join.c. These were
missed by the monk/mage/rogue retirement pass and still grant
NM3 classes to any no-OCC character, reopening dead-pool
legacy skill commands (protect/shield/aura/absorb/dispell/
drain/bump).

Investigate first:
- Find all three files and confirm exactly what they do
  (what class they set, what skills/flags they grant, how
  a player reaches them)
- Check whether any live room, NPC, or zone file links to
  them as exits or clones them
- Check how monk/mage/rogue were retired so this pass uses
  the same pattern

Show me the full picture before proposing anything. No
changes until I say go.



SEND FIRST 



Read CLAUDE.md and confirm you are following all rules there
before doing anything else.

The VPS is confirmed up to date and the driver restarted
clean, but ooc still outputs the old format:
  (ooc) You say, "test"

The file on disk is correct. Something else is producing
this output. Investigate:

1. Is there a second ooc command defined anywhere — in
   secure/cmds/, domains/adm/, or anywhere else in the
   command search path that would take priority over
   cmds/mortal/_ooc.c?
2. Does the driver's command search path check any directory
   before cmds/mortal/ that could shadow this file?
3. Is there a master object, login object, or user.c hook
   that intercepts the ooc command before it reaches the
   normal command dispatch?

Read the driver config or master.c to confirm the full
command search order. Report exactly what you find before
touching anything.