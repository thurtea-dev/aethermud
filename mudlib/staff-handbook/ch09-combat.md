# Chapter 09 - Combat Notes

Part of the AetherMUD Staff Handbook. See [index.md](index.md) for the full chapter list.

## Live Verification Checklist: Combat Round, Magic Net, Idle Strings, Corpse/Blood

Use this after touching daemon/rifts_combat.c, daemon/rifts_spells_d.c,
daemon/rifts.c, std/container.c, or std/monster.c. Run the reload commands
first, then work through the list in order.

### 1. Reload

```
update /daemon/rifts_combat
update /daemon/rifts_spells_d
update /daemon/rifts
```

std/container.c and std/monster.c do not reload for anyone or anything
already in the game (see ch01-server.md, "Warmboot vs Full Reboot"). If
either changed this session, a full reboot is required before steps 3, 4,
and 5 below can be trusted; `update /std/container` or `update /std/monster`
alone will not do it.

### 2. Idle string mapping

- Stand near a plains wolf and look at the room. It should read
  "prowls nearby." not "is standing around."
- Stand near a jungle stalker and look at the room. It should read
  "lurks nearby." not "is standing around."

### 3. Corpse and blood listing (needs full reboot to be trusted, see step 1)

- Kill an NPC and look at the room.
- The corpse should read "Plains wolf corpse." (victim name, not "the corpse
  of X"), on its own line.
- The blood pool should read "A pool of fresh blood." on its own line,
  listed after the corpse.
- There should be no "is here." or "are here." suffix on either line, and
  no combined "X and Y" line joining them.

### 4. Magic net (Wizard only, needs full reboot to be trusted for the
   room listing part)

- As a Wizard, cast magic net (or use the magicnet command) on a normal
  living target.
- The net should catch immediately. No "resists the magic net!" roll.
- Look in the room while the target is netted. There should be no
  standalone "A glowing magic net is here." line. The net reads as
  attached to the victim, not as a room object.
- As a non-Wizard caster, confirm the resist roll still happens as
  before. This change is Wizard-only.

### 5. Full block combat

- Attack a plains wolf (race "wolf", not a registered Rifts race) and
  confirm: 6 consecutive "You ..." player attack lines (Wizard, PP
  dependent, see ch13-ppe-isp.md or the Wizard APM formula in
  daemon/rifts_combat.c query_apm()), then exactly 1 wolf attack line, not
  a long uncontrolled stream. Plains wolf's attack count is 1 by code
  (std/living/combat.c execute_attack(), legacy branch, melee skill 0), not
  2. If you see more than 1 wolf attack per round, std/monster.c's
  npc_round_active() guard is not live, most likely because the full
  reboot from step 1 has not happened yet.
- For a target that survives longer, use Brodkil (100 MDC, race brodkil)
  or gargoyle (up to ~130 MDC, race gargoyle, both registered Rifts races).
  Confirm the same pattern: all of the player's attacks print as one
  consecutive block, then the foe's full APM block, then the round ends.
- Check log/combat_round after the fight for a line like:
  `player_apm=6 player_used=6 foe_apm=2 foe_used=2` (brodkil/gargoyle) or
  `player_apm=6 player_used=6 foe_apm=1 foe_used=1` (plains wolf).
  This confirms the full block ran without a premature stop or an
  over-attacking foe, even when the fight is too fast to read the message
  order by eye.

### Warmboot truth test

Of the five checks above, only 2 and part of 5 (the daemon-driven APM
math) are guaranteed live immediately after `update`. Anything touching
std/container.c (corpse/blood ordering and wording, magic net room
visibility) or std/monster.c (the foe over-attack guard in step 5)
requires a full reboot to be trusted for players, rooms, and NPCs that
were already in the game before the edit. See ch01-server.md for why.
