# Legacy RiftsMUD design memories - reference material

Status: **reference material only, not a task list.** Written 2026-07-17,
expanded 2026-07-18 with a second round of memories (races and
abilities, chargen flow, origin history, original website material).
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
Later detail recovered: the credits were handed over by the command
itself, not by saying anything to him, and you entered the rift
manually after it opened ("You had to manually enter the rifts, and
you handed him credits with a command instead of saying something").

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

## Races and abilities as remembered

- **Dragons were MDC-only:** Great Horned Dragons (and dragons
  generally) had MDC as their sole damage pool, not HP plus SDC.
- **Dragon metamorphosis:** dragons could `metamorph human` or
  `metamorph <race>` to change shape and appearance, and while
  metamorphed could fit into human-sized armor.
- **Psionics and spells:** psionics such as telepathy were available,
  and spells such as swimasafish and windrush; all of these names
  survive in this codebase's daemons.

## UI and display conventions from the original game

- Room exits displayed in **bold green**, with minimal other color in
  room descriptions.
- The `score` command was wrapped in an ASCII border. There used to be
  **multiple score commands**, not just one.
- A separate equipment/worn command existed, distinct from `score`,
  showing what you were wearing and had equipped.
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

## Chargen and ability granting (wizard-mediated)

How character setup worked in the original game: you picked your race
and/or class, and everything that automatically came with it (so long
as it was coded into the game) was immediately added. Beyond that
automatic grant, you had to wait for a wizard and go through lists
with them to see what else your character was entitled to take.

Captured alongside the memory (an observation, not a build task): a
clean skill picker that lists categories and displays only what your
character can actually have would replicate the useful part of that
flow without requiring a wizard to be online.

## Origin history

As remembered secondhand from a long-time wizard of the original game:
Chris (the administrator) did roughly 30-35% of the Rifts conversion,
mostly working at the level the coding wizards did and following
existing examples; a head coder modified most of the systems; and
Macgyver did the vast majority of the difficult work, including the
OCC and RCC conversions and MDC. Recorded because the original
website's claim that "the lib is custom coded by the administrator"
overstates one person's role; the same memory notes a later FluffOS
driver update took over a month when it should have been a small job,
consistent with the deeper systems knowledge having belonged to
others.

## Original website material (RiftsMUD 2.1)

Recovered text from the original website, condensed. Useful as tone
and scope reference for zone descriptions and the public site.

- **Welcome blurb:** over 50 races, 50 OCCs, 150 skills, and countless
  spells, psionics, and abilities; locations including Chi-town, New
  Camelot, Splynn, "and other worlds"; billed as the longest running
  Rifts MUD on the web. Lore framing: the Third World War's dead pour
  their energy back into the earth, erupting the ley lines; where the
  lines cross, nexus points open rifts to unknown lands. Ran on the
  server Tonster thanks to Arzach.
- **The Americas:** Federation of Magic vs the Coalition States in
  North America; vast dangerous wilderness outside their territories;
  vampires rule most of Mexico; South America is jungle, swamp, and
  mountains with scattered pockets of civilization and rare 20th
  century technology worth treasure hunting.
- **Chi-town:** CS capital; mutant dog boys, psi-stalkers, SAMAS
  pilots, and grunts patrol heavily; shanty towns with high crime
  surround the fortified city; dangerous for the unprepared D-bee.
- **Lone Star:** the Coalition's mutant creation and research
  headquarters; livable if human; D-bees captured for experiments.
- **Horton:** small friendly human town surrounded by forest;
  dangerous wild creatures prey on the weakly defended townsfolk; only
  practical access is by hovertrain.
- **Puerto Angel:** once human, now roamed by vampires; tolerant
  unless attacked; some travel as far north as the Chi-Town burbs;
  ocean beaches make a wonderful resting spot.
- **Announced but never seen:** expansions to the Burbs and the
  fortified city itself (secret entrances, quests, new shops), and
  Stormshire, a huge city in the magic zone.
- **Atlantis:** returned to Earth ruled by the Splugorth under Lord
  Splynncryth. Splynn is the capital, best known for The Market, an
  inter-dimensional shopping area selling everything from weapons and
  power armor to slaves. Alvurron is the gargoyle city. **The
  Preserves** are Splynncryth's great hunting grounds on the west side
  of the continent, filled with ravenous monsters and nomadic tribes
  of lesser creatures; "be prepared and pack plenty of e-clips."
  Highlights: slave trading (buying and owning slaves), The Market's
  countless services, and "dozens of secrets." Ratings: Services 4/5,
  Difficulty 5/5, Mob numbers 3/5.
- **Africa, Australia, Japan:** under construction.
- **Europe:** New Camelot with the King and his servant Mrrlyn, royal
  gardens ("some people say the gardens are alive"), and market street
  shops; NGR Germany with nearly unbeatable weapons technology and
  some soldiers who share the Coalition's hatred of d-bees.

The same recovered paste also contained the original site's full
helpfile index and the RCC/OCC availability lists (the repo's copies
of those files were deleted; see CLAUDE.md's reference-files note).
They are not reproduced here; if wanted, they belong under
`docs/reference/` as a separate archival file.

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
- Race/class-differentiated realtime natural healing rates: **struck
  from this list 2026-07-18** - see the addendum below; the system
  exists.

### Addendum: cross-reference for the 2026-07-18 additions

Checked against the codebase on 2026-07-18:

- **Dragon MDC-only pools:** implemented; `is_mdc_race()` in
  `daemon/rifts.c` lists the dragon races.
- **Dragon metamorphosis:** implemented in some form;
  `cmds/mortal/_metamorph.c` exists, with support in the spells daemon
  and `std/armour.c`. Fidelity to the remembered mechanics (fitting
  human-sized armor) not verified in play.
- **Telepathy, swimasafish, windrush:** all implemented (psionics and
  spells daemons).
- **Separate equipment command:** exists (`eq`, with the `rifts_slot`
  display convention).
- **The Preserves:** exist in some form; `preserve_*.c` room files sit
  in the Splynn area (`splynn_preserves.c`, `preserve_trail.c`,
  `preserve_waterhole.c`, `preserve_snag.c`).
- **Realtime healing rates:** implemented; `std/user.c`'s
  `rifts_hp_regen_tick()` differentiates by race and OCC (dragons on a
  600 second cadence, juicers 120, regenerating races 300, others
  slower), matching the remembered juicers-fast / dragons-medium /
  humans-slow ordering.
- **Wizard-mediated ability granting:** the modern equivalent is the
  admin command chain (`setrcc`, `grantrace`, `grantskills`) plus the
  player-facing `skillrequest` menu; no wizard-in-the-loop list
  walkthrough exists, and the captured skill-picker observation covers
  what would replace it.
- **Multiple score commands:** the current game has `score` and
  `sbar`; not audited further.
- **Origin history and website material:** historical record only;
  nothing to cross-reference.
