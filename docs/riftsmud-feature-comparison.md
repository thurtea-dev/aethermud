# AetherMUD mudlib vs RiftsMUD (public claims)

**Date:** 2026-08-07  
**Scope:** Research/reporting only. Mudlib path measured:
`mudlib/nightmare3_fluffos_v2/lib/`. RiftsMUD source is private; its
side of the comparison uses the long-standing public blurb (live site
unreliable at time of writing; same text preserved in the Wayback Machine
and in-repo `RiftsMUD Memories.txt`).

**RiftsMUD public claim (archived riftsmud.com / Memories “2.1” copy):**

> …select from over 50 races, 50 Occupational Character Classes, 150
> skills, and countless magic spells, psionics, and abilities. The game
> includes locations such as Chi-town, New Camelot, Splynn, and other
> worlds.

No public breakdown of exact race/OCC/spell lists or room counts was
available, so RiftsMUD numbers below are **marketing floors / slogans**,
not audited inventories.

Driver constraints for recommendations are taken from
`docs/STATUS.md` (custom C++ driver), not from FluffOS (which already
runs this mudlib).

---

## 1. Our mudlib — counts from live data tables

Measured by parsing the daemon sources (not from `ASSESSMENT.md`
estimates). Method notes under each row.

| System | Source | Count | How counted |
|---|---|---:|---|
| **Playable races** | `daemon/rifts.c` → `rifts_races_list` | **62** | String literals in the list initializer |
| Admin-only race | same file → `admin_races_list` | **1** (`wizard`) | Separate list; not in the playable 62 |
| **OCCs** | `daemon/occ.c` → `occ_list` | **66** | String literals in the list (includes comment-noted aliases such as `mercenary`, `coalition soldier`, and entries like `palmer` / `palmer2`) |
| **Skills (distinct data rows)** | `daemon/rifts_skills.c` → `query_rifts_skill()` | **157** | Contiguous `case` clusters that share one `return ([ "start": …])` |
| Skill case aliases | same | **206** | Unique `case "…":` labels (aliases fold into the 157 rows) |
| **Spells** | `daemon/rifts_spells_d.c` → `query_spell()` | **115** | Distinct `ppe_cost` / case-cluster definitions |
| **Psionics** | `daemon/rifts_psionics_d.c` → `query_psionic()` | **51** | Distinct `isp_cost` definitions |
| Legacy Nightmare skills daemon | `daemon/skills.c` | (NM3 class skill init; not the Rifts catalog) | Not used as the Rifts skill count |

### Skill category breakdown (157 rows)

| Category | Rows |
|---|---:|
| technical | 25 |
| espionage | 22 |
| weapons | 17 |
| pilot | 16 |
| physical | 13 |
| survival | 10 |
| military | 9 |
| lore | 8 |
| medical / rogue / creative | 6 each |
| science / trade / electrical | 5 each |
| domestic | 3 |
| psionic | 1 |

### Domains / locations (rooms ≈ files inheriting a room base)

| Domain / area | ~Rooms | Notes |
|---|---:|---|
| Praxis (hub + sub-areas, incl. Splynn tree) | 254 | Default domain; chargen, equipment, Splynn under `areas/splynn/` |
| **Splynn cluster** (subset of Praxis) | **65** `.c` files | Market, docks, preserves (28 preserve* files), Alvurron, ocean rift, catacombs |
| **ChiTown** | 53 | Coalition capital |
| **NewCamelot** | 31 | Named Camelot zone |
| Tolkeen | 29 | |
| Horton | 27 | |
| LoneStar | 23 | |
| NGR | 15 | Partial |
| **DemonPlane** | 9 | Explicit “other world” sliver |
| Lazlo | 8 | Stub |
| PuertoAngel | 7 | Stub |
| adm / wizards | 11 | Non-play / staff |
| **Playable domain rooms (excl. adm/wizards)** | **~456** | Sum of measured room inherits above |

Dimensional hooks that exist in-tree:

- `domains/Praxis/areas/nexus_dimensional_tear.c` exits **into**
  `DemonPlane/areas/demon_sliver_gate`.
- `/std/rift_portal.c` / `blue_rift_portal.c` (destination e.g. Splynn
  `ocean_rift`) — rift travel between Earth-side areas.
- Spell data includes dimensional-flavored entries (e.g. `dimensional
  pocket`); full book travel rituals are still a known content gap per
  prior audits, not re-counted here.

---

## 2. Side-by-side vs RiftsMUD’s claimed counts

| Feature | RiftsMUD (public) | AetherMUD (measured) | Verdict |
|---|---|---|---|
| Races | “over 50” | **62** playable (+1 admin) | **Ahead** of the floor |
| OCCs | “50” | **66** list entries | **Ahead** (some aliases inflate uniqueness slightly) |
| Skills | “150” | **157** distinct rows (206 aliases) | **Slightly ahead** |
| Spells / psi / abilities | “countless” | **115** spells + **51** psionics = **166** named powers | Cannot compare numerically; we have a large concrete catalog |
| Chi-town | Named | **ChiTown** domain, ~53 rooms | **Present** |
| New Camelot | Named | **NewCamelot** domain, ~31 rooms | **Present** |
| Splynn | Named | **Praxis/areas/splynn/**, 65 `.c` files | **Present** (substantial) |
| Other worlds / dimensions | Named | **DemonPlane** + nexus tear + rift portals | **Present but thin** (9-room demon sliver + portal toys) |

**Headline:** Against the only numbers RiftsMUD published, this mudlib is
already at or above the advertised character-system scale, and it already
ships the three named megacity/world locations. The open comparison is
not “do we have Chi-Town?” but “how deep is each zone, and how complete
are ritual/dimension systems?” — which their public page does not quantify.

---

## 3. Location / domain gaps

| Claimed landmark | In our mudlib? | Gap assessment |
|---|---|---|
| Chi-town | Yes — `domains/ChiTown/` | Covered at city/burbs scale (~53 rooms). Depth vs a long-running commercial MUD is unknown (no public room map). |
| New Camelot | Yes — `domains/NewCamelot/` | Covered (~31 rooms). Docs historically call it “stub-plus”; still a real named zone. |
| Splynn | Yes — under Praxis, not its own top-level domain | Strong presence: market, slave/black-market flavor, Preserves loop, Alvurron, ocean rift. Layout mirrors Memories notes more than a greenfield design. |
| Other worlds / dimensions | Partial | **DemonPlane** is a real second plane linked from Praxis nexus, but only ~9 rooms. Rift portals move players between Earth-side destinations (e.g. ocean/Splynn). Not a multi-world atlas. |

**Additional Earth zones we have that the blurb never named:** Tolkeen,
Horton, Lone Star, NGR, Lazlo, Puerto Angel, Praxis hub — so geographic
breadth is already wider than the marketing sentence.

**Meaningful location gap (vs the spirit of “other worlds”):** more
planes / dimensional destinations with play loops, not another rename of
Chi-Town. Lazlo / Puerto Angel / NGR depth is the nearer Earth-side gap
if the goal is “feels as big as a long-run Rifts MUD,” but that is
qualitative.

---

## 4. Skills: close, ahead, or behind?

**Ahead — narrowly.**

- RiftsMUD claim: **150**
- Ours: **157** distinct skill data rows in `rifts_skills.c`

So we are not chasing a skill-count deficit. Further skill work should be
about missing Palladium categories / OCC wiring / help text, not hitting
150.

---

## 5. Recommendations (scoped to `docs/STATUS.md` driver reality)

The mudlib already clears RiftsMUD’s public *quantity* bar. The custom
C++ driver does **not** yet run this mudlib end-to-end. STATUS’s live
path stops short of full `std/user` (closure / lambda forms still missing
for `std/user/editor.c`), and critical runtime services are stubs:

- `call_out` validates but **does not schedule**
- `heart_beat` is **never invoked**
- `throw()` missing; many efuns absent; destruct/interactive semantics incomplete

Recommendations, ordered:

### A. Do not close the RiftsMUD gap with more races / OCCs / skills

We are already ahead of the advertised 50 / 50 / 150. Expanding
`rifts_races_list` / `occ_list` / `rifts_skills.c` for marketing parity
is wasted effort.

### B. Prioritize driver bring-up before large new content

Content that depends on timers, combat ticks, daemons’ `call_out`,
channel/heart_beat patterns, or full player object compile will not be
exercisable on the C++ driver until STATUS’s blockers clear. Highest
leverage for “play the Rifts mudlib we already have”:

1. Remaining closure / `(*fp)()` surface so `std/user` (+ editor inherit) compiles  
2. Real `call_out` + `heart_beat` scheduling  
3. Efuns/`throw`/destruct fidelity enough for combat, skills, and cast commands  

Until then, treat FluffOS as the runtime that validates content; treat
new zone sprawl as optional.

### C. If adding mudlib content anyway (on FluffOS), prefer depth over new named cities

| Investment | Why | Driver note |
|---|---|---|
| Deepen **DemonPlane** (or a second plane) into a real loop | Matches “other worlds” better than another Earth town | Rooms/NPCs are mostly data + simple LPC; still need working objects/combat on whatever driver you play on |
| Flesh **Lazlo / Puerto Angel / NGR** | Earth-map credibility | Same |
| Ritual / travel spells from the known book gap (portal, superior teleport, summoning circles) | Closes “countless … abilities” *feel* where we are thinner | Spell rows in `rifts_spells_d.c` are table + `fx_` handlers — fine as content, but casting path needs a working player/combat stack |
| Avoid counting contests with RiftsMUD | Their numbers are unverifiable floors | — |

### D. What not to recommend yet

- Massive new OCC/race packs “to beat 50/50”
- Content that assumes rich closure consumers (`map`/`filter`/`sort_array`
  with `$1` lambdas) on the C++ driver — STATUS explicitly does not
  implement those forms yet
- Anything that only works if `call_out`/`heart_beat` fire (idle timers,
  delayed NPC scripts, regen ticks) **on the C++ driver** until those
  stubs are filled

---

## 6. Sources

**Ours (measured):**

- `lib/daemon/rifts.c` (`rifts_races_list`, `admin_races_list`)
- `lib/daemon/occ.c` (`occ_list`)
- `lib/daemon/rifts_skills.c` (`query_rifts_skill`)
- `lib/daemon/rifts_spells_d.c` (`query_spell`)
- `lib/daemon/rifts_psionics_d.c` (`query_psionic`)
- `lib/domains/*` room inherit scan; `Praxis/areas/splynn/` file list
- `lib/domains/Praxis/areas/nexus_dimensional_tear.c`
- `lib/std/rift_portal.c`, `blue_rift_portal.c`
- `docs/STATUS.md` (driver capability gate)

**RiftsMUD (public only):**

- https://web.archive.org/web/20051216052004/http://riftsmud.com/
- `nightmare3_fluffos_v2/RiftsMUD Memories.txt` (same feature sentence under “2.1”)

**Context (not used as counts):** `mudlib/docs/ASSESSMENT.md` (earlier
audit; figures here were remeasured from source).
