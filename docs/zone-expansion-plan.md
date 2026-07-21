# Zone expansion plan - AetherMUD toward original-game scale

Goal: grow toward the original medium-world feel (3,001-6,000 rooms on
Top Mud Sites) without blocking play. Expand in layers that players can
reach from existing hubs (Praxis, Splynn, Camelot, Chi-Town).

## Current footprint (measured room files, 2026-07-13)

Earlier versions of this table counted files (rooms + NPCs + items),
which overstated rooms. These are actual room-inheriting files, per
ASSESSMENT.md section 4. Total at original measurement: 583 room files
(included the 133 standardOld rooms, since moved to `attic/` 2026-07-19
and no longer part of the live tree). Current total ~450, of which ~432
are playable (excluding standardOld and admin/example rooms; the +2 over
the original ~430 is the pair of 2026-07-21 zone start rooms).

| Zone | Rooms | Role today |
|------|------:|------------|
| Praxis hub (top level) | 143 | Chargen arrival, bank, shops, Moxim |
| Praxis/areas (CS territory, market, nexus, catacombs) | ~35 | Complete |
| Praxis sub-areas (cemetery, mountains, orc valley, vehicles) | 37 | Complete, some legacy-flavored |
| Praxis/standardOld | 0 (moved) | Moved to `attic/` 2026-07-19, outside the lib root; no longer live, counted, or reachable |
| Chi-Town | 52 | CS military / burbs + fortified city (+1: `chitown_start.c`, 2026-07-21 zone start room) |
| Splynn / Alvurron / ocean / Preserves | ~34 | Atlantis market + gargoyle city; Preserves partial |
| New Camelot | 31 | Europe magic capital (+1: `newcamelot_start.c`, 2026-07-21 zone start room) |
| Tolkeen | 29 | Free city: mage quarter, siege edge, crypt |
| Horton + wilderness ring | 27 | Small town + wilderness ring |
| NGR Germany | 14 | Triax tech pocket |
| Demon plane | 9 | Off-world sliver |
| Lazlo | 8 | Magic city stub |
| Puerto Angel | 7 | Vampire coast |
| Lone Star | 7 | Mutant research |

Missing vs original splash / memories: Stormshire / Federation of Magic,
Africa, Japan, Australia, continuous wilderness between hubs. The
Preserves now has an 11-room hunting-grounds loop (Sprint 4, about 1/3
of target). Chi-Town fortified city + burbs largely done.

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
   - Target: +20–30 rooms

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
| 4 | Splynn Preserves | +30 (11 done as of 2026-07-13, ~19 to go) |
| 5 | Stormshire v1 core | +80 (not started) |
| 6 | Europe connectors + Lone Star | +40 (not started) |

**Horton wilderness ring - shipped (~17 new rooms).**

- Fixed station/main_street exit bug (station was mutually south)
- Rail spur, creek/marsh/orchard west loop
- Scrub ridge + bandit camp + cornfield
- Farmstead spokes: wrecked convoy, pine hollow, hunting blind
- Quarry rim/floor (lurker) + hermit cabin (safe talk NPC)
- Logging road, ley glade, ancient grove, wolf den
- New threats: feral boar, trail bandit, quarry lurker

After sprint 5 the lib should feel closer to “same game, smaller map”
than “hub with teleports.” Full original scale remains a multi-month
content project, not a blocker for playtesting.

## What not to do yet

- Modern FluffOS cutover - abandoned 2026-07-12 (the `archive/` porting
  copy was intentionally deleted, not paused; see `internal/report.md`
  section 10). Not a live parallel track.
- Africa/Japan/Australia before Stormshire and Tolkeen
- Mass-generating empty room grids without NPCs, shops, or threats
