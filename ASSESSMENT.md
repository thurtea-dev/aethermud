# AetherMUD State-of-the-Project Assessment

Date: 2026-07-13. Scope: `/home/thurtea/aethermud/nightmare3_fluffos_v2` (lib +
driver), cross-referenced against `invocations.md`, `zone-expansion-plan.md`,
`playtest-checklist.md`, `staff-handbook/`, `report.md`, and
`lib/domains/Praxis/adm/master_gap_report.txt`. Read-only audit; no files were
modified. Counts below were measured directly from the code (grep on case
labels, data keys, and inherit lines), not taken from prior reports.

**Overall: roughly 80-85% of a playable faithful recreation.** Core systems
(chargen, OCC/race data, combat, magic, psionics, skills, economy, commands)
are implemented and verified by scripted tests. The main gaps are world scale
(Sprints 4-6 of the zone plan), depth of the spell list vs the book text,
a still-loaded legacy NM3 room tree throwing runtime errors, and stale
documentation drifting from the code.

---

## 1. OCC / RCC coverage - ~90%

Measured from `daemon/occ.c` (2,163 lines) and `daemon/rifts.c` (1,900 lines):

- **62 unique OCC entries** in `occ.c` (case labels, deduplicated). Every
  entry carries description, XP table, HP/level, base skills, OCC skill
  categories and pick counts, secondary skill count, attribute requirements,
  starting equipment text, flags, and PPE/ISP base + per-level.
- **61 playable races** in `rifts_races_list` in `rifts.c` (plus an
  admin-grantable list). Includes all four dragon subtypes, vampires,
  psi-stalkers, mutant animals, faerie folk, giants, Atlanteans, Kittani.
- **76 starting-package case labels** in `daemon/rifts_start_d.c`, so OCC
  starting gear/spell/psionic packages exceed the OCC list (aliases and
  RCC packages included). All 8 Rifts daemons are in `secure/cfg/preload.cfg`.

Against the Rifts core rulebook class list, coverage is complete: all core
book OCCs (Vagabond, Wilderness Scout, City Rat, Rogue Scholar/Scientist,
Operator, Body Fixer, Cyber-Doc, Headhunter, Merc Soldier, CS Grunt/Ranger/
Military Specialist/Technical Officer, Borg, Robot Pilot, Glitter Boy Pilot,
Juicer, Crazy, Cyber-Knight, Ley Line Walker, Shifter, Techno-Wizard, Mystic,
Mind Melter) plus core RCCs (Dog Boy, Psi-Stalker, Burster, Dragon Hatchling)
are present, and the lib goes well beyond the core book (Atlantis tattoo
classes, Sunaj, NGR/Triax, ISS, juicer variants, warlock, necromancer).

Caveats:

- Many attribute requirements are marked `/* approx */` in `occ.c` - fine for
  play, but not yet book-audited.
- The reference file `RiftsMUD-AetherMUD RCC's OCC's List.txt` cited in
  CLAUDE.md **no longer exists in the repo**, so the original target list can
  no longer be diffed against. `reference-chart.md` notes full tabletop detail
  exists only for Shifter, Techno-Wizard, and Mind Melter.
- CLAUDE.md's "51/51 races, 38/38 OCCs" is stale; actual counts are 61 races,
  62 OCCs.
- Gap report notes ~25 OCC help-file gaps; `doc/help/user/` has 331 topics
  including per-OCC/per-race entries, so the remaining gaps are narrow.

**Next:** restore or reconstruct the target RCC/OCC list file; book-audit the
"approx" attribute requirements; fill the remaining OCC help files.

## 2. Magic and psionics - ~75%

Measured from `daemon/rifts_spells_d.c` (3,907 lines) and
`daemon/rifts_psionics_d.c` (1,726 lines):

- **~116 spells** defined (`ppe_cost` entries) with **114 distinct effect
  handlers**, and every effect string in the data has a matching case in the
  `apply_spell_effect()` dispatch - no dangling effects.
- **~50 psionic powers** (`isp_cost` entries) with a complete
  `apply_psionic_effect()` dispatch and ~100 `fx_` references.
- **158 skills** in `daemon/rifts_skills.c` (start/per_level/max/category
  data for each), well past the "~102" noted in CLAUDE.md.

Versus `invocations.md` (146 named invocations extracted from the book text):
after normalizing punctuation differences, roughly **55-65 book invocations
have no implementation**, concentrated in ritual/utility magic: summoning
(Summon and Control Animals/Rain/Storm, Summon Lesser Being, Shadow Beast),
creation rituals (Golem, Zombie, Mummy, Magic Scroll, Talisman, Amulet,
Wards/Sanctum/Protection Circle: Superior), curses and removals (Minor Curse,
Curse: Phobia, Remove Curse, Sickness, Spoil, Mute), travel (Dimensional
Portal, Teleport: Superior, Ley Line Transmission, Wind Rush is present but
e.g. Fly as the Eagle exists only partially), and misc (Anti-Magic Cloud,
Invulnerability, Oracle, Second Sight, Stone to Flesh, Transferal, Wards).
Conversely the code contains spells beyond that file (it only covers part of
the book list). So: **combat/buff/utility magic that matters in a MUD loop is
in; ritual-flavored magic is the open tail.**

Targeting consistency:

- Spell data carries a `range` field (`self` 38, `room` 26, `single` 22,
  `touch` 21, plus `dimensional`/`unlimited`), **but ~7 entries lack the
  field**, and `_cast.c` **never enforces range**: it parses an optional
  "at target" and passes whatever it found (or 0) to the effect function.
  Each `fx_` function individually guards against a missing target, so
  behavior is correct in practice but duplicated ~114 times, and `touch` vs
  `single` vs `room` are not mechanically distinct at the command layer.
- **Resource-loss bug:** `_cast.c` deducts PPE (and an APM when in combat)
  *before* calling the effect, and target-validation happens inside the
  effect. `cast energy bolt` with no target prints "Cast at whom?" but the
  PPE and the attack action are already spent. `_psi.c` has the same
  order (ISP deducted before the effect's own target check), though it does
  use the range field for `remote` powers and has an introduce-gate for
  targeting other players.
- Classic no-verb casting (`windrush grunt`) is supported via
  `try_spell_shortcut()` with longest-prefix matching - nice fidelity touch.

**Next:** move the target/range check into `_cast.c`/`_psi.c` before the
resource deduction; add the missing `range` fields; then work down the
ritual-magic tail (summoning and circles first, they are the most remembered).

## 3. Combat system - ~85%

`daemon/rifts_combat.c` (1,510 lines) is a substantive Palladium adaptation:

- **APM system:** OCC-based attacks per melee (`occ_base_apm`), +PP bonus,
  +level bonus at 3/6/9/12/15 via `rifts_apm_bonus`; per-round attack
  budgeting (`can_do_attack`/`use_rifts_attack`), spells/psionics consume APM.
- **MDC/SDC:** `apply_rifts_damage()` implements the full gate - SDC attackers
  cannot hurt MDC defenders (damage is soaked by the SDC buffer with
  "scratches the surface" messaging), MDC weapon detection, psi-sword unarmed
  penetration, `built_in_mdc`, armor bypass properties (`armor_bypass`,
  `penetrate_ar`, `ar_bypass_half`), and separate vehicle MDC handling with
  armor HP tracking and destruction (`init_armor_hp`/`destroy_armor`).
- **Damage typing:** elemental tags via `last_attack_element` with
  impervious-to-fire/cold/energy checks. There is no broader kinetic vs
  energy vs explosive matrix; that matches the original MUD's simplicity.
- **Defense:** strike/parry/dodge bonuses from PP, position and stance
  modifiers, autododge/autoparry commands, saving throws command.
- **Initiative:** `query_initiative_bonus()` exists but rounds are ordered
  "player acts first by default" rather than a per-round opposed roll - a
  deliberate simplification, worth knowing about rather than fixing.
- **NPC side:** NPC combat ticks, ranged auto-equip, NPC psionic use,
  regen (`do_rifts_regen`), death handling.

Caveats: `daemon/bionic_d.c` is thin (141 lines) relative to the cybernetics
content under `equipment/cybernetics/` (26 files); gap report flags a few
item-level PARTIALs (flame hilt damage unverified, hawrk-duhk talisman
exclusion broken). Scripted verification (section8) covered chargen, combat
autododge/autoparry, psi-sword, bank, radio.

**Next:** decide whether per-round initiative rolls matter for fidelity;
deepen `bionic_d.c` if bionics are meant to be mechanical rather than
cosmetic; fix the two flagged item bugs.

## 4. Zones and rooms - ~60% of the zone plan's near-term vision

Measured by counting files that inherit a room base (ROOM/vault/pier/vote
room). Note: `zone-expansion-plan.md`'s "current footprint" numbers appear to
be **file counts** (rooms + NPCs + items), which overstate rooms - e.g. New
Camelot is listed as ~54 but has 30 actual rooms; NGR listed ~25, actual 14.

| Zone | Rooms (measured) | Status |
|------|-----------------:|--------|
| Praxis hub (top level) | 143 | Complete, reskinned, live hub |
| Praxis/areas (CS territory, market, nexus, catacombs) | ~35 | Complete |
| Splynn + Alvurron + ocean + Preserves | ~34 | Partial (see Sprint 4) |
| Praxis sub-areas (cemetery 8, mountains 9, orc valley 8, vehicles 10, misc 2) | 37 | Complete but some still legacy-flavored |
| Praxis/standardOld | 133 | **Legacy NM3 rooms, not Rifts content, throwing runtime errors** |
| Chi-Town (burbs + fortified city) | 51 | Complete per Sprint 1 |
| New Camelot | 30 | Complete stub-plus |
| Tolkeen | 29 | Complete per Sprint 2 |
| Horton + wilderness ring | 27 | Complete per Sprint 3 |
| NGR Germany | 14 | Partial (plan calls for connectors) |
| Demon plane | 9 | Placeholder sliver (by design) |
| Lazlo | 8 | Stub |
| Lone Star | 7 | Stub (Sprint 6 target: +20-30) |
| Puerto Angel | 7 | Stub |
| adm / Examples / wizards | ~20 | Non-play infrastructure |

Total room files: **583**, of which ~430 are playable Rifts content (excluding
standardOld and admin/example rooms).

Against the sprint table in `zone-expansion-plan.md`:

- Sprints 1-3 (Chi-Town, Tolkeen, Horton): **done**, targets met or exceeded.
- **Sprint 4 (Splynn Preserves, +30): ~1/3 done.** 11 Preserves rooms exist
  (`preserve_*.c` + `splynn_preserves.c`, the committed "10 room loop").
  About 20 more rooms needed to hit target.
- **Sprint 5 (Stormshire v1, +80): not started.** No domain or files exist.
  This is the plan's own "biggest feel gap" item.
- **Sprint 6 (Europe connectors + Lone Star, +40): not started.**

**Next:** finish the Preserves loop to ~30 rooms, then Stormshire core. Decide
the fate of `standardOld/` (see section 7) - it is 133 rooms of the total that
players should probably never see.

## 5. Commands - ~90%

- **173 mortal commands** in `cmds/mortal/` - all classic original-game verbs
  the gap report tracked (cast, psi, breath, fly, metamorph, psisword, radio,
  store/retrieve, remoteview, assassinate, card/chat/assist, sbar, slave,
  stance, position, customize, suicide, pemote, etc.). The gap report's
  2026-07-07 pass concluded "GENUINELY MISSING, deferred: (none)" with the
  sole exception of `email` (covered by `mail`).
- **29 admin commands** (`cmds/adm/`): makewiz, setrole, setocc, setrcc,
  grantrace, grantskills, setskill, playerwipe, force, trans, warmboot, etc.
- **74 creator commands** across `secure/cmds/creator/` (28, including the
  room-safe `_update.c`) and `cmds/creator/` (46: qcs suite, roomcheck,
  clean, dest, etc.), plus hm/ambassador/guild/soul trees.
- **Staff-handbook verbs check out** with one documentation quirk: the
  apprentice/staff workflow verbs (kit, build, inscribe, approve, deny,
  promote, demote, purge, review, rptool, tool, domain) are `add_action`
  verbs on the wiz-tool objects in `domains/adm/wiz_tools/`, not command
  files - they all exist. But `playtest-checklist.md` says "Domain review:
  `dominate` option 10" while the actual verb on `staff_of_dominion.c` is
  **`domain`**. Small checklist error that will trip a playtester.
- `cmds/wiz/` is an empty directory (roles use adm/creator/hm trees instead).

**Next:** fix the `dominate` -> `domain` reference in playtest-checklist.md;
optionally delete the empty `cmds/wiz/` dir to avoid confusion.

## 6. Player systems - ~85%

- **Chargen:** full flow (name, gender, region -> start delegation, race,
  attribute rolls + rerolls, alignment picker, OCC with attribute gating,
  skill picks) in `domains/Praxis/setter.c` + `rifts_start_d.c`, with a
  chargen guide NPC. The 2026-07-10 fix wired racial attribute dice
  (`apply_rifts_race_attributes`), previously a real fidelity bug. Scripted
  verification covered 3 OCC flows end to end.
- **Saves:** correct path (`/secure/save/users/<letter>/<name>.o`), frequent
  `save_player()` calls throughout `user.c`.
- **Prompt/score/display:** custom prompt (`query_prompt` override), room
  display order fixed (desc -> exits -> NPCs -> objects). Gap report flags
  the remaining cosmetic gap: **score has no blue status effects** (thirst
  quenched, stoned) - alcohol intoxication is the only status shown.
- **Skills:** 158-skill daemon, category picks at chargen, `improve`,
  `skillrequest` flow (though `skill_request_d.c` is only 40 lines - verify
  it does what the command promises).
- **Inventory/equipment:** eq display with `rifts_slot`, GHD armor size
  enforcement verified, encumbrance API correct.
- **Known incomplete:** pet corpse-eating exists but is never triggered
  (PARTIAL in gap report); **insanities are not implemented at all** despite
  `staff-handbook/ch15-insanities.md` documenting the system (no code
  references outside `_saving_throws.c`); alignment is the numeric NM3 scale
  in `user.c` while Palladium alignments exist as chargen choice + help
  files - the two systems coexist without full integration.

**Next:** blue status effects in score (small, high-nostalgia); decide
whether insanities are in scope (handbook says yes, code says no); audit
skill_request end to end.

## 7. Known technical debt

Ranked by risk:

1. **`domains/Praxis/standardOld/` (133 legacy NM3 rooms) is live and
   erroring.** `log/runtime` shows recurring `Bad argument 1 to call_other()`
   in `reset()` for `standardOld/pit`, `spider_pit`, `app_room`, and `crypt`.
   These are pre-Rifts fantasy rooms; players reaching them breaks theme, and
   the errors spam the log every reset cycle. Fix the resets or unlink and
   archive the tree.
2. **`/secure/daemon/events` destructed function-pointer errors** referencing
   `domains/Praxis/supply2` (`change_sky` at line 190): an object registered
   an event callback and was destructed. Needs a liveness guard in the events
   daemon; currently a steady error source.
3. **`log/crashes` shows 15 "Process terminated" entries July 10-12.** Some
   are likely manual `mud.sh stop` cycles during the playtest push, but the
   volume is worth confirming - if any were real crashes, nothing else in the
   logs attributes them.
4. **Casting resource-loss bug** (section 2): PPE/ISP and APM spent before
   target validation in `_cast.c`/`_psi.c`.
5. **Uncommitted working-tree change:** `daemon/command.c` has a real fix
   (rehash no longer appends duplicate path entries; uses
   `distinct_array`). Commit it or lose it on the next reset - the memory
   note says a fresh git repo/remote move is pending, which raises the risk.
6. **Editor droppings in the repo:** `#s_centre2.c#`, `n_centre1.c~`,
   `adv_main.c~`, `wild2.backup`, `daemon/services.old`, several `.c~` under
   `standardOld/` and `obj/mon/`. Harmless at runtime, noise in review.
7. **Documentation drift:** CLAUDE.md content counts stale (spells 113 -> 116,
   skills 102 -> 158, races 51 -> 61, OCCs 38 -> 62); the RCC/OCC target list
   file it cites is gone; zone plan footprint numbers are file counts, not
   room counts; playtest checklist references the wrong verb (`dominate`);
   `master_gap_report.txt` body still carries stale MISSING entries (Lone
   Star, Puerto Angel) that its own header sections supersede.
8. **CLAUDE.md's "no `//` comments" rule vs reality:** 146 files under
   daemon/cmds/std use `//` comments (including `occ.c` line 1) and compile
   fine on this driver. Either the rule is stale or a latent portability trap
   for the planned modern-FluffOS cutover - worth resolving one way.
9. **Zero TODO/FIXME markers in the lib** - genuinely clean on that axis; the
   debt above lives in logs and structure, not in marked stubs.

---

## Bottom line

The systems layer is in strong shape and matches the "faithful recreation"
brief; scripted verification and the July playtest push clearly paid off.
The honest gaps are: world scale (Sprints 4-6 unstarted or 1/3 done, no
Stormshire), the ritual-magic tail of the spell list, the erroring legacy
standardOld tree, the casting resource-loss ordering, and documentation that
has fallen behind a codebase moving faster than its own trackers.

---

## Addendum: fixed after this assessment (2026-07-13, same day)

Do not re-fix these; the sections above describe the pre-fix state:

- **Casting resource-loss ordering (sections 2, 7.4): FIXED.** `_cast.c`
  and `_psi.c` now validate the target before PPE/ISP/APM deduction,
  gated per effect via `NEED_TARGET_EFFECTS` lists audited against every
  `!target` guard in both daemons. Verified live.
- **Uncommitted `daemon/command.c` rehash fix (section 7.5): COMMITTED**
  in `affed3b` (2026-07-13 00:28), together with this assessment file.
- **CLAUDE.md stale counts (section 7.7): UPDATED** to the measured
  figures; the `dominate` -> `domain` checklist error is also fixed.
- **Help system:** rewritten to a two-level index (categories, then
  topics per category) with separator-insensitive topic lookup; the flat
  list survives as `help index`. Not a finding above, noted for currency.
