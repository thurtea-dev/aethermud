# Legacy RiftsMUD design memories - reference material

Status: **reference material only, not a task list.** Written 2026-07-17.
This document captures design memories from the original RiftsMUD (the
early-2000s Palladium Books Rifts MUD this project descends from) so
they can inform future work. Nothing here is a commitment to build
anything; treat it as historical source material alongside
`nightmare3_fluffos_v2/RiftsMUD Memories.txt`. Where an item already
exists in this codebase, the cross-reference status section at the end
says so; the memory is still recorded in full so the modern
implementation can be checked against the original intent.

## Items and locations

- **Obsidian blade (hydra lair):** an obsidian blade hidden behind a
  pushable rock. The room holding it was reachable by casting windrush
  on the hydra guarding the area, relocating the hydra out of the room
  so the player could search in peace. The windrush-the-guardian trick
  was the intended solution, not an exploit.
- **Flame hilt:** a metal hilt that, when charged with PPE, produced a
  temporary flame blade effect. A melee weapon fueled by the caster's
  own PPE reserve.
- **Ghostly katana of soul slaying:** loaded in the catacombs. It
  ignored armor entirely, dealing damage directly to the target's body:
  MD against MDC targets, SDC against SDC targets, depending on target
  type.
- **Sword of Atlantis:** said to be found in an ocean rift.
- **Armor talisman (Rocky's bar, Splynn):** Rocky's bar in Splynn sold
  a rechargeable PPE-charged armor talisman providing 100 MD. It worked
  on every race except Hawrk-duhk types.
- **Hidden black market (Splynn):** reachable by going south toward
  Rocky's, then all west, one north, and knocking on a pillar. It sold
  invisibility-granting predator-type armors and net guns.

## NPC travel mechanic: Moxim

An NPC named Moxim opened rifts between three areas via a
`rift <destination>` command. He took universal credits as payment.
The rift stayed open for a short duration before collapsing, and any
player in the room could use it while it was open, not just the payer.

Cross-reference: `domains/Praxis/monsters/moxim.c` already exists and
the mechanic is **fully implemented**, and goes beyond the memory: the
`rift <destination>` add_action checks and deducts credits, plays a
per-destination flavor line, and clones a `/std/rift_portal` that lasts
30 seconds and is usable by anyone in the room. The current version
serves seven destination keys across six areas (Chi-Town, Splynn, New
Camelot, Lazlo, Tolkeen, Praxis) rather than the remembered three, with
faction-standing side effects on travel.

## Pet system: falconry hawks

A falconry skill let players trade an item to a shopkeeper in exchange
for a hawk pet. The hawk:

- followed the player from room to room;
- ate the corpses of enemies the player killed;
- obeyed instructions given through the `say` command, with a success
  chance tied to the owner's falconry skill level: roughly 98%
  obedient at high skill, around 65% at lower skill. On a failed
  check it would just squawk instead of obeying.

## UI and display conventions from the original game

- Room exits displayed in **bold green**, with minimal other color in
  room descriptions.
- The `score` command was wrapped in an ASCII border.
- A separate equipment/worn command existed, distinct from `score`.
- Specific status-effect coloring: a thirst-quenched status shown in
  blue, and a drug-intoxication "stoned" status also in blue.
- HUD elements similar in spirit to Dead Souls but minimal, for
  example describing power armor standing in a room.
- Color was used sparingly overall, limited to radio (green), OOC
  (red/white), and tells (blue).

Most of this is already codified as the color conventions section in
the root `CLAUDE.md` and should stay that way.

## Realtime healing rates by race/class

As remembered, natural healing ran in real time and varied hugely by
what you were playing:

- **Juicers:** healed in minutes.
- **Dragons:** healed in an hour or two.
- **Ordinary humans:** much slower than either.

## Future feature brainstorm (raw ideas, not designed)

These are captured as raw ideas for later evaluation. None of them
have been designed, scoped, or approved.

- **Smuggler credit exchange:** a smuggler ability to convert between
  regular and black market credits at an exchange rate the player can
  improve, potentially offered to other players as a paid service via
  a menu system.
- **Unlockable secret classes:** classes similar to Sunaj Assassin,
  obtained through account-level achievements earned in-game (as
  opposed to the existing per-character lore gate).
- **Automated mercenary base creation:** adapt the room-creation tool
  into a mob-driven process so mercenary group bases can be built
  without staff hand-crafting each one. Cross-reference:
  `docs/mercenary-groups-plan.md` already covers secret faction bases
  (three hand-placed hidden entrances in existing zones); this idea
  would extend that plan, not replace it.
- **Creation tables:** a "creation table" system for building cyborgs
  and androids, and for implementing the Juicer conversion process as
  an in-game mechanic rather than only a starting OCC.
- **OCC respec:** eventually allow a player to restart their OCC at
  level 1, with a pfile flag preventing it from being done more than
  once. Noted at capture time: this would need heavy testing to avoid
  power creep.

## Cross-reference status

Checked against the codebase on 2026-07-17. More of the above already
exists than expected; the "already exists" list is longer than the
"new" list.

Already exists in some form:

- **Obsidian blade / hydra / pushable rock:**
  `domains/Praxis/areas/hydra_lair.c` (push the flat rock),
  `hydra_treasure.c`, and `equipment/obsidian_blade.c`. A Chi-Town
  hydra lair/treasure pair also exists. Whether the windrush-the-hydra
  relocation trick works as originally remembered has not been
  verified in play.
- **Flame hilt:** `equipment/flame_hilt.c`.
- **Ghostly katana:** `equipment/ghostly_katana.c`.
- **Sword of Atlantis:** `equipment/sword_of_atlantis.c`.
- **Armor talisman and Rocky's bar:** `equipment/armor_talisman.c`,
  `areas/rockys_bar.c`, and `areas/splynn/rockys_bar_splynn.c`.
- **Splynn black market:** `areas/splynn/splynn_back_alley.c` (knock
  pillar) leading to `splynn_black_market.c`, stocking
  `equipment/splynn/predator_armor.c` and `equipment/splynn/net_gun.c`.
- **Moxim rift travel:** fully implemented, see the Moxim section
  above.
- **Falconry pets:** already implemented, contrary to earlier
  assumptions. The `falconry` skill is in `daemon/rifts_skills.c`, and
  `equipment/trained_hawk.c` / `equipment/new_trained_hawk.c` implement
  a hawk obtained from a falconer NPC for a leather falconry glove,
  with owner-following, eating from the room, and say-command
  obedience rolled against the owner's falconry skill percentage
  (a d100 roll under skill, rather than the remembered fixed 98%/65%
  tiers). Note: `new_trained_hawk.c` appears to contain leftover
  scripting text after its final function and needs inspection before
  being trusted.
- **Secret unlockable classes (partial):** `cmds/mortal/_clan.c`
  already gates the Sunaj Assassin OCC behind joining the Aerihman
  clan. The brainstormed idea differs in being account-level and
  achievement-driven.
- **Mercenary base planning:** `docs/mercenary-groups-plan.md`
  (planning only, not implemented).
- **Currency exchange (partial):** `cmds/mortal/_exchange.c` and bank
  exchange logic exist for ordinary currencies; there is no black
  market credit currency or smuggler rate mechanic.

Entirely new (no trace found in the codebase):

- OCC respec (restart OCC at level 1 with a once-only pfile flag).
- Smuggler regular/black market credit conversion as a player ability
  or paid service.
- Account-level achievement unlocks for secret classes.
- Creation tables for cyborgs/androids and an in-game Juicer
  conversion process.
- Automated, mob-driven mercenary base construction.
- Race/class-differentiated realtime natural healing rates (current
  healing behavior was not audited for this doc; treat the remembered
  juicer/dragon/human rates as a target to compare against, not a
  confirmed gap).
