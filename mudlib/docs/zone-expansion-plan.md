# Zone expansion plan - AetherMUD toward original-game scale

Goal: grow toward the original medium-world feel (3,001-6,000 rooms on
Top Mud Sites) without blocking play. Expand in layers that players can
reach from existing hubs (Praxis, Splynn, Camelot, Chi-Town).

## Current footprint (measured room files, remeasured 2026-07-27)

Earlier versions of this table counted files (rooms + NPCs + items),
which overstated rooms. These are actual room-inheriting files, per
ASSESSMENT.md section 4. Total at original measurement (2026-07-13):
583 room files (included the 133 standardOld rooms, since moved to
`attic/` 2026-07-19 and no longer part of the live tree).

Current measured counts (2026-07-27), three figures because three
different questions get asked and mixing them is what caused the
earlier drift:

- **466** room files on disk under `domains/` (2026-07-27 measurement;
  +4 since - 2 in the Preserves, see note below, +1 in NGR
  (`ngr_gargoyle_ridge.c`), +1 in Chi-Town
  (`chitown_collapsed_nest.c`), both 2026-07-28 - now 470).
- **459** live rooms, excluding the 7 inherited Nightmare III example
  rooms (`adv_main`, `adv_inner`, `lpc_inner`, `lpmud_room`, `mudlib`,
  `void`, `areas/test_zone`).
- **449** playable rooms, additionally excluding `domains/adm/` (1) and
  the `domains/wizards/` staff wing (9, including `thurtea_test/`).

449 is the number the website quotes. A prior revision of this section
said "~470 total, ~452 playable"; both were wrong and are corrected
above.

Measurement command (files inheriting a room base directly):

```bash
cd nightmare3_fluffos_v2/lib
grep -rlE '^[[:space:]]*inherit[[:space:]]+(ROOM|VAULT|"?/?std/room"?|"?/std/vote_room"?)[[:space:]]*;' \
  --include=*.c domains | sort -u | wc -l
```

The +20 since 2026-07-13 breaks down as the Splynn Preserves
loop-closer + depth batch and the first Lone Star depth batch (+8,
both 2026-07-24), and the Lone Star kennel / incubation /
escape-route / support-town batch (+12, 2026-07-26).

Per-zone counts below are measured the same way and sum to 466. The
Praxis rows are split by directory because `domains/Praxis/` is not
only Praxis (see the note under the table).

| Zone | Rooms | Role today |
|------|------:|------------|
| Praxis hub (`domains/Praxis/*.c`, top level) | 138 | Chargen arrival, bank, shops, Moxim. Includes 6 example/void rooms that are not playable |
| Praxis/areas (CS territory, market, nexus, catacombs) | 20 | Complete. Includes `areas/test_zone.c` (not playable) |
| Praxis/areas/splynn (Splynn, Alvurron, ocean, Preserves) | 64 | The entire Atlantis zone, nested under Praxis on disk. See note below. +2 2026-07-28 (`preserve_blackwater.c`, `preserve_oldhold.c`) |
| Praxis sub-areas (cemetery 8, mountains 9, orc valley 8, vehicles 10, death 1) | 36 | Complete, some legacy-flavored |
| Praxis/standardOld | 0 (moved) | Moved to `attic/` 2026-07-19, outside the lib root; no longer live, counted, or reachable |
| Chi-Town | 53 | CS military / burbs + fortified city (+1: `chitown_start.c`, 2026-07-21 zone start room; +1 2026-07-28: `chitown_collapsed_nest.c`) |
| New Camelot | 31 | Europe magic capital (+1: `newcamelot_start.c`, 2026-07-21 zone start room) |
| Tolkeen | 29 | Free city: mage quarter, siege edge, crypt |
| Horton + wilderness ring | 27 | Small town + wilderness ring |
| Lone Star | 23 | Mutant research, expanded 2026-07-26: kennel wing, incubation core, escape route out through the drainage, civilian support town |
| NGR Germany | 15 | Triax tech pocket. +1 2026-07-28 (`ngr_gargoyle_ridge.c`) |
| Demon plane | 9 | Off-world sliver |
| Lazlo | 8 | Magic city stub |
| Puerto Angel | 7 | Vampire coast |
| wizards (staff wing) | 9 | Staff-only, not playable |
| adm | 1 | Admin, not playable |

**Directory-layout note (flagged 2026-07-27, no rename approved):** the
62-room Atlantis zone lives at `domains/Praxis/areas/splynn/`, so
`domains/Praxis/` is the on-disk parent of a chargen start zone on a
different continent. Earlier revisions of this table listed "Splynn /
Alvurron / ocean / Preserves" as a peer zone, which read as if it were
a top-level domain and hid the nesting. Moving it to a top-level
`domains/Atlantis/` (or `domains/Splynn/`) would touch 220 path
references across 74 files; 0 player save files reference the path, so
saves are not a blocker. Per CLAUDE.md rule 19 this needs explicit
approval before anyone runs it, and it is not scheduled.

Missing vs original splash / memories: Stormshire / Federation of Magic,
Africa, Japan, Australia, continuous wilderness between hubs. The
Preserves now has 26 rooms with one closed loop and depth on 5 of its
former dead ends (2026-07-24 batch gave 3 dead ends depth; 2026-07-28
gave two more a room each - Sunken Shallows got Blackwater, closing
the "something moves beneath the surface" foreshadowing that had been
sitting unpaid since 2026-07-24, and Lower Cage Pit got Old Hold,
closing the "stock they are not yet ready to bring up, or have simply
forgotten about" line the same way). Two dead ends were deliberately
left alone rather than extended: Hive Heart (`preserve_hivecore.c`) is
a boss chamber - the innermost/deepest point of that branch, extending
past a named-boss climax room would undercut it. Rope Nook
(`preserve_nook.c`) is written as a small hidden vignette (an old
hunter's stash), not a through-path. One structurally-framed dead end
remains untouched: Thornbrake Den (`preserve_den.c`, a preserve
beast's lair) - a plausible next pick, not yet extended. Chi-Town
fortified city + burbs largely done.

**Quest gap: CLOSED for all 7 zones (found 2026-07-28, closed
2026-07-28, same session).** Room-count completion does not mean a
zone has content to do once there. `daemon/quest_d.c` had 11 quests
total (6 Praxis, 2 Chi-Town, 1 New Camelot, 1 demon plane, 1
secret/faction) and **zero in Tolkeen, Horton, Lone Star,
Splynn/Atlantis, NGR, Puerto Angel, or Lazlo** - seven zones with
built, playable rooms and nothing to do in them beyond wander and
fight. Each now has exactly one quest (18 total registered): Lone Star
`cold_storage_escapee`, Splynn/Atlantis `preserve_beast_hide`, Tolkeen
`silence_the_scout`, Horton `quarry_lurker_cull`, NGR
`border_gargoyle_raider`, Lazlo `lazlo_shore_dog`, Puerto Angel
`harbor_watch`. See `master_gap_report.txt`'s 2026-07-28 (part 3) entry
for the giver/trigger detail on each. One quest per zone is a floor,
not a ceiling - each zone can still take more, and none of the seven
have a repeatable/bounty-style quest yet (compare Praxis's
`survival_proof`/`praxis_bounty_bandit`).

**Combat-threat density: addressed for NGR and Puerto Angel
(2026-07-28). Lazlo is intentionally lower-danger by design, not an
open gap - see below.** A first pass counted files by which
subfolder they lived in (`monsters/` vs `npcs/`), which is unreliable
in this lib - several monsters are filed under `npcs/` (e.g.
`NGR/npcs/gargoyle_raider.c`) and vice versa. Re-measured by
`set_alignment()` value (hostile vs. allied/neutral) instead: NGR had
10 living-being files but only 1 hostile; Puerto Angel had 4, only 1
hostile (and that one, `pa_secondary_vampire`, is a truce-protected
town elder per its own file comment, not actually a threat a player
would fight); Lazlo has 6, 1 hostile file.

Fixed by adding second (NGR) and second (Puerto Angel) spawn points to
an *existing* monster class in a room whose own description already
foreshadowed the threat, rather than writing new monsters: NGR's
`gargoyle_raider.c` now also spawns in `ngr_frontline_trench.c` and
`ngr_patrol_road.c` (both already said "gargoyle" in their room text);
Puerto Angel's `wild_vampire.c` now also spawns in `pa_outskirts.c`
(already said "no movement in daylight," matching the harbor's
day/night gate). Deliberately not added to rooms whose own text
establishes them as safe (Puerto Angel's beach is
`rest_allowed`, its main street/residential district are explicitly
truce-protected peaceful vampire territory; NGR's underground bunker is
an explicitly hardened, heavily-guarded interior). See
`master_gap_report.txt`'s 2026-07-28 (part 4) entry for full detail.

**NGR extended further (2026-07-28, part 9): `ngr_gargoyle_ridge.c`
added east of `ngr_no_mans_land.c`.** No Man's Land's own text named
"the gargoyle ridge" as a place beyond it without a room ever having
been built there - an unpaid geographic reference, not a "dead end"
room needing the foreshadowing-vs-terminal test the Preserves rooms
needed (there was no terminal declaration to weigh against extending
it in the first place). Spawns `gargoyle_raider.c` again (same
template already used at No Man's Land and the two Preserves-style
spawn points added in part 4) - the ridge is gargoyle territory
proper, so "more of the same, in greater numbers, on their ground"
fits better than a mechanically escalated threat.

**Chi-Town extended (2026-07-28, part 10): `chitown_collapsed_nest.c`
added up from `chitown_dead_end_junction.c`.** The strongest hook found
this pass, stronger than a named-but-unbuilt landmark: the *parent*
room, `chitown_maintenance_tunnel.c`, explicitly states "Something has
nested in the east junction" in its own long description, and the
junction room it points to carried a dev comment reading "Atmosphere
only, no mob spawn" - a direct textual promise that was never
delivered, not an inference. The junction's own text also independently
promised the payoff twice more ("Scratch marks... too large to be
human," "Something wet drips from the junction overhead"). Checked
existing Chi-Town monsters for reuse first: `burb_dog_boy.c` is a
uniformed CS checkpoint patrol, wrong fit for something feral and
nested; `chitown_tunnel_rat.c` is explicitly "dog-sized," too small to
match "too large to be human." No clean match existed, so
`chitown_junction_stalker.c` was written new, closely mirroring
`quarry_lurker.c`'s stat block and archetype (SDC beast, level 7,
ambush-from-above) rather than inventing new mechanics. The junction
room's ceiling gap became a climbable "up" exit into the new nest room,
where the stalker actually lives - matches "dropping down through the
cracked ceiling to feed" better than spawning it in the junction
itself would have.

**Lazlo: intentional lower-danger zone by design (decided 2026-07-28),
not a combat-density gap.** Investigated twice (2026-07-28): first
pass found `lazlo_shoreline.c`'s `reset()` already tops up to 2 live
feral dogs per visit (a count-based loop, not visible in a file-count
audit - the earlier "1 hostile" figure undercounted actual encounter
density there). Second pass read the full room text of every other
Lazlo room (`lazlo_magic_quarter.c`, `lazlo_library.c`,
`lazlo_market.c`, `lazlo_square.c`, `lazlo_gate.c`) rather than
skimming, and found a consistent, deliberate tone across all five:
"clean, no cracks," "unhurried purpose," Cyber-Knights posted at the
gate "without hostility," scholars and mages going about business,
"nobody stares." This is not a zone with foreshadowing NGR/Puerto
Angel had that just never got paid off - Lazlo reads as Rifts Earth's
most orderly, civilized magic city on purpose, consistent with its
canon reputation. User decision 2026-07-28: leave it that way. No new
hostiles, no rewritten room lore, no further scaling of the shoreline
dogs. Revisit only if explicitly requested later.
Lone Star's population was also undercounted in the first pass (6
files, missed monsters filed under `/std/rifts_npc` instead of the
`MONSTER` macro) - actual count is 9, a reasonable mix for the room
count. Splynn/Atlantis looked empty (1 file) when measured against
its own `areas/splynn/` subtree, but its ~13-14 real NPCs/monsters are
filed at the `domains/Praxis/` level instead (a filing quirk tied to
the same directory nesting flagged above, not a content gap).

---

## Phase A - Deepen what players already reach (highest ROI)

Do these first. Each extends an existing exit graph.

1. **Chi-Town burbs + fortified city**
   - Secret entrances, more shops, CS patrol density, D-bee danger loop
   - Original site listed this as Coming Soon; memories expect it
   - Target: +40–60 rooms off existing Chi-Town

2. **Splynn Market depth + The Preserves**
   - More Market services, slave-trade loop polish, hunting grounds west
   - Target: +30–50 rooms; Preserves as wilderness combat sink

3. **Tolkeen from stub to playable city**
   - 6 rooms → ~25–35: walls, mage quarter, CS siege edge, one dungeon
   - Gives Americas a second magic pole vs Lazlo

4. **Horton wilderness ring**
   - Forest approaches, hovertrain stop, wildlife that actually threatens
   - Target: +15–25 rooms; teaches new players outdoor danger

Exit criteria: a mortal can spend 2–3 hours exploring without looping the
same 20 rooms, starting from Praxis via Moxim / roads.

---

## Phase B - Signature remembered destinations

5. **Stormshire / Federation of Magic**
   - Biggest feel gap for players who remember the magic zone
   - Build as a new domain hooked via Moxim + Americas wilderness
   - Target: 80–120 rooms (city core + wilderness fringe), not the full
     original mega-zone on day one

6. **New Camelot / NGR connective tissue**
   - Road / ley-line / rift links so Europe feels like a region, not two
     teleports
   - Target: +20 rooms of travel content + 1–2 side villages

7. **Lone Star expansion**
   - Mutant labs, containment, Dog Boy kennels, escape routes
   - Target: +20–30 rooms (11 → 23 as of 2026-07-26; kennels, incubation
     core, and the escape route are done, so what is left of this item is
     depth, not the named sub-areas)
   - "Depth" specifically means quest content (currently zero), not more
     rooms - population itself (9 living-being files) is reasonable for
     the room count, see the notes above

Exit criteria: every name on the 2009 splash (Chi-Town, New Camelot,
Splynn) plus Stormshire and Tolkeen are “worth a visit” for a mid-level
character.

---

## Phase C - Other worlds (optional / later)

Original site marked these under construction; do not block soft launch.

- Africa (Phoenix Empire pocket)
- Japan (tech / mystic contrast)
- Australia
- Deeper demon / dimensional planes beyond the current sliver

Each can start as a 15–25 room “postcard” zone with one vendor, one
threat, one unique loot, then thicken later.

---

## Build rules (keep quality high)

- Spawns in `reset()` with `present()` guards; never NPCs in `set_long`
- One purpose per area: combat sink, shopping, RP hub, or quest spine
- Hook every new zone to Moxim and/or a walkable path from an existing hub
- Preload-compile new rooms; clean `log/catch` before calling a phase done
- Prefer Palladium place-names from memories / reference-chart over inventing
  parallel geography

## Sprint 1 status (2026-07-10)

**Chi-Town burbs pass - first chunk shipped (+14 rooms).**

New rooms under `domains/chitown/areas/burb_*.c`:
tenements, soup kitchen, flophouse, preacher square, water pump, gang
corner, chop shop, night market, fence stall, rooftops, wall shadow,
smuggler crawl, dog boy checkpoint, refugee camp.

NPCs: soup cook, preacher, mechanic, refugee.
Monsters: burb thug, Dog Boy patrol.
Hooks: shanty/alley/burb row/burbs/illinois road/maintenance tunnel.

**Fortified city depth - shipped (+12 rooms, memories-guided).**

Inside the wall (Coming Soon from original splash: shops, secret
entrances, new mobs):

- Shopping arcade + ration depot, civilian arms, outfitter
- Residential block, apartment lobby, **service hatch** secret into
  maintenance tunnels (pairs with burb smuggler crawl)
- ISS checkpoint, Psi-Stalker post, SAMAS hangar, Dog Boy kennels
- Propaganda hall off Officer Row
- Boulevard patrols now include CS grunt + Dog Boy (memories: dog boys,
  psi-stalkers, SAMAS, grunts heavily patrol)
- Hydra treasure also loads **flame hilt** (PPE flame blade from memories)
- Removed mistaken market->burbs shortcut; market now links to arcade

Chi-Town area rooms total: **51**. Sprint 1 +40 target effectively met
between burbs + fortified city.

**Tolkeen playable - shipped (~29 rooms).**

- City core: gate, Liberation Square (Moxim), market, barracks, walls E/W/N,
  towers NE/NW, mage quarter, TW workshop, library, clinic, inn + rooms,
  armory, component alley, refugee yard
- Siege edge: approach, scorched field, ruined suburb, CS trench,
  no-man's-land, siege edge (CS scout)
- Crypt dungeon: entrance → hall → ward (golem) → depths (shade) → vault
  (ward charm loot)
- Travel: Moxim `rift tolkeen` / destinations list updated

Next Phase A: Splynn Preserves (Horton wilderness ring shipped).

| Sprint | Item | Target rooms |
|--------|------|-------------:|
| 2 | Tolkeen playable | +25 (done ~29) |
| 3 | Horton wilderness | +20 (done ~17 new; ~27 total) |
| 4 | Splynn Preserves | +30 (26 done as of 2026-07-28: loop closed, depth on 5 dead ends; ~4 more to reach the original +30 target. 2 dead ends - Hive Heart, Rope Nook - are deliberately terminal, not pending growth. 1 dead end - Thornbrake Den - remains available for a future pass) |
| 5 | Stormshire v1 core | +80 (not started) |
| 6 | Europe connectors + Lone Star | +40 (Lone Star: +4 done 2026-07-24 barracks/armory + motor pool + escape tunnel, +12 done 2026-07-26 kennel wing + incubation core + escape route + support town, 23 rooms total; Europe connectors not started) |

**Horton wilderness ring - shipped (~17 new rooms).**

- Fixed station/main_street exit bug (station was mutually south)
- Rail spur, creek/marsh/orchard west loop
- Scrub ridge + bandit camp + cornfield
- Farmstead spokes: wrecked convoy, pine hollow, hunting blind
- Quarry rim/floor (lurker) + hermit cabin (safe talk NPC)
- Logging road, ley glade, ancient grove, wolf den
- New threats: feral boar, trail bandit, quarry lurker

**Lone Star expansion - shipped (+12 rooms, 2026-07-26).**

Takes Lone Star from 11 rooms to 23. Four wings, each hooked into the
existing exit graph, plus one loop closer.

- Kennel wing off the barracks: kennel yard (hub), kennel rows (kennel
  master NPC, juvenile Dog Boys), psi-scent training ring. Covers the
  "Dog Boy kennels" line of Phase B item 7.
- Incubation core beyond the lab and containment: gene vault, incubation
  gallery (the vat floor where Dog Boy lines are actually grown),
  observation booth, cold storage (failed specimen threat). Closes a loop
  plaza → lab → containment → incubation → observation → gene vault → lab.
- Escape route past the previously dead-ended breached tunnel: tunnel
  junction, flooded sump (loot), culvert mouth. Surfaces outside the wire
  and rejoins the outer gate, closing the loop gate → perimeter → tunnel
  → junction → culvert → gate.
- Civilian support town south of the gate: Support Row and the Dry Kennel
  canteen. Covers the memories' "a decent livable city...if human" note,
  with the registration/sweep notice board as the counterweight.
- Loop closer: the motor pool roll-up door (already described in its own
  long text) now actually opens onto the perimeter road.

New NPCs: kennel master, civilian contractor, canteen keeper (vendor).
New monsters: juvenile Dog Boy, failed specimen.

After sprint 5 the lib should feel closer to “same game, smaller map”
than “hub with teleports.” Full original scale remains a multi-month
content project, not a blocker for playtesting.

## What not to do yet

- Modern FluffOS cutover - abandoned 2026-07-12 (the `archive/` porting
  copy was intentionally deleted, not paused; see `internal/report.md`
  section 10). Not a live parallel track.
- Africa/Japan/Australia before Stormshire and Tolkeen
- Mass-generating empty room grids without NPCs, shops, or threats
