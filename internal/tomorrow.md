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