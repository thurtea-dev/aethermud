# AetherMUD Recreation - Progress & Remaining Work

Generated: 2026-07-07. **Superseded for current state by `ASSESSMENT.md`
(2026-07-13), which measured counts directly from the code; treat this
file as a historical snapshot.** Consolidates the prior `report.md` (full
section-by-section audit) and `report-new.md` (session snapshot) into one
document from when most sections were completed. `report-new.md` has been
retired; its content is folded in below.

Current live copy: `/home/thurtea/aethermud` (port 1122, FluffOS 2.9-ds2.08).
Lib path: `nightmare3_fluffos_v2/lib/`. Master tracker (daemon-level detail):
`lib/domains/Praxis/adm/master_gap_report.txt`.

**Note on naming:** this project was renamed from "RiftsMUD" to "AetherMUD" on
2026-07-07 (brand only - it still faithfully recreates the original RiftsMUD 2.1
ruleset and Palladium's Rifts RPG system; only the MUD's own name changed).

HOW TO USE THE PROMPTS: each open section below has a "PROMPT FOR CLAUDE" block.
Copy one block, paste it as your message, and Claude will finish that section.

---

## Executive summary

**Overall estimate: ~85-90% complete.** All core systems boot clean (zero
`log/catch` errors, all 12 Rifts daemons preload). Remaining work is verification
(live chargen/combat pass), a handful of nice-to-have zones/systems, and the
long-term modern-driver port (paused, not blocking).

| Area | Status |
|------|--------|
| 1. Legacy NM3 reskins + race retirement | Done |
| 2. Known bugs (password echo, dragon armor, radio, clothing) | Done (all were stale gap-report entries) |
| 3. Stubs (psi-sword hilt, dead file) | Done |
| 4. Content gaps (skills/bionics/tattoos/vehicles) | Done |
| 5. Zone depth (cemetery, inn, New Camelot, Alvurron, NGR) | Done; off-world zones partially covered by section 9 |
| 6. Missing commands + CharGen guide | Done |
| 7. Help file gaps (~352 user help files) | Done |
| 8. Live verification sweep | **Partial - open, see below** |
| 9. Parity extras (demon sliver, quests, bounty, Tolkeen) | Mostly done; shops/website parity open |
| 10. Driver upgrade (modern FluffOS) | Paused, archived, one real bug found+fixed - **open** |
| 11. Playtest bug list | Done and verified |
| Rebrand to AetherMUD | Done |
| Player account cleanup | Done |

---

## What's done

### 1. Legacy NM3 content (28 reskinned files, 12 races retired)
All player-facing "Nightmare" strings removed outside the intentional
`standardOld/` archive (37 files total, including 9 found during the sweep).
12 legacy fantasy races (artrell, faerie, giant, gnome, half-elf, halfling,
half-orc, hobbit, kender, klingon, nymph, satyr) retired from mortal chargen via
`RETIRED_LEGACY_RACES` in `setter.c`; creators can still pick them for testing.

### 2. Known bugs - all stale
Password echo, dragon-size armor restriction, radio transmit, and clothing wear
checks were all already correctly implemented; the gap report was out of date.
Corrected in place with dated notes.

### 3. Stubs
`psi_sword_hilt.c` fully implemented (ISP-gated activation/deactivation, `move()`
override kills the blade on drop). `execution.c` (0-byte dead file) deleted.

### 4. Content gaps
Skills 123 -> 158, bionics 9 -> 20 daemon entries, tattoos 17 -> 25, two new
vehicle interiors (NG-V7 Mountaineer, NG-150 Wastelander), `equipment/vehicles/`
and `equipment/magic_items/` now populated.

### 5. Zone depth
New Camelot 10 -> 30 rooms (full castle + academy + market). Alvurron 8 -> 15
rooms. NGR Germany 7 -> 14 rooms. Cemetery confirmed pre-existing. 10 new inn
guest rooms. Wizard lounge/channel cleanup as a follow-up.

CORRECTED 2026-07-07: the older "Chi-Town/Splynn are a good first pass, need
deeper interiors" note was stale. Actually walking the file structure: Chi-Town
(41 files) has a full CS military complex (barracks, armory, command plaza,
recruitment, secure wing), sewer/tunnel network, and black market, each with
their own NPCs and monsters. Splynn (40 files under
`domains/Praxis/areas/splynn/`) includes the entire 15-room Alvurron sub-city,
a 4-room catacomb complex, and a 4-room ocean rift zone on top of the main
market/docks/slave-pens area. Both read as fully-developed zones now, not a
first pass - later sprints closed this gap without the tracking table being
updated. Marking done.

### 6. Commands
All 10 deferred commands (customize, echo, face, forward, greet, lineecho,
picture, sentinel, trigger, view) implemented or documented as historical.
CharGen guide NPC upgraded to a full conversational walkthrough.

### 7. Help files
74 new files (policy docs, command/topic aliases, 9 new OCC files). Full 66/66
OCC audit passed. ~352 total user help files.

### 8. Verification - partial (see Open below)

### 9. Parity extras
Demon-plane sliver (10 rooms), three `quest_d` quests, Praxis bounty board,
Tolkeen 6-room stub. Remaining: player-run shops, website parity (open below).

### 10. Driver upgrade - paused (see Open below)

### 11. Playtest bugs
`tanstaafl_base.c` MOVE_OK inversion and relative-path clone resolution fixed;
6 wiz_tools files had capital-article shorts lowercased. Verified live: driver
restarted clean after the fix, no `catch` errors.

### Rebrand: RiftsMUD -> AetherMUD (2026-07-07)
Brand-only rename. Self-reference ("this MUD" - banners, docs, help, source
comments/strings) renamed; historical references (the real original MUD this
project recreates) left as "RiftsMUD" by design, same for `RiftsMUD Memories.txt`
and all `rifts_*` code identifiers (game-system name, not brand). Touched: root
docs, staff handbook, website, ~44 mudlib source files, ~50 help files,
`mudos.cfg` server name, login banner. `doc/help/user/riftsmud` is now a pointer
to the new `doc/help/user/aethermud` topic. Verified via full reboot, clean
`log/catch`, boot banner reads AetherMUD.

### Player account cleanup (2026-07-07)
Removed thurtest, psypher, dacon, mcguyver, sola (all level 1, no `wiz_role`, not
in `groups.cfg`) via the same file operations `cmds/adm/_playerwipe.c` performs
(save + `.o.bak` + postal dir + realm dir, logged to `log/adm/playerwipes`).
thurtea and splynncryth (both `wiz_role admin`) kept.

Note for next time: there is no `mud.sh` flag for player wipes - it only has
`start/stop/restart/status/log/boot/doctor/init-config`. Wipes are the in-game
`playerwipe <name>` / `playerwipe confirm all` admin command (or the equivalent
file operations, as done this session).

### Colorize wording (2026-07-07)
Removed redundant "colorize on" advice from the login banner, help files, and
hint text - color is off by default so bare `colorize` already turns it on;
only `colorize off` needs an argument.

### Project root cleanup (2026-07-07)
Moved `bin-modern/`, `fluffos-modern/`, `lib-modern/`, `mud-modern.sh`, and
`driver-upgrade-notes.md` into `archive/` - none of it is needed for AetherMUD to
run on the live 2.9 driver. `RUNNING.md` and `staff-handbook/ch01-server.md`
updated to point at the new paths. Deleted 7 stale `driver-boot-*.log` scratch
files after folding anything useful into `archive/driver-upgrade-notes.md`.

---

## What's still open

### 8. Live verification sweep

- [x] Full chargen walkthrough on a fresh alt through elective/secondary picks.
      CONFIRMED WORKING 2026-07-07: scripted socket session as a Rogue Scholar
      completed race -> alignment -> OCC -> 6 elective picks (technical/
      medical/lore categories) -> 5 secondary picks -> reached the live world,
      with score/eq/mail/help/autododge/autoparry all responding correctly
      afterward. The `setter.c` OCC-category/skill-category mapping fix
      (`map_occ_pick_category`/`normalize_skill_category`) genuinely works.
- [x] Found (via tracing, confirmed against real skill data) and fixed a
      **related but distinct** bug the category-mapping fix didn't cover:
      `daemon/rifts_skills.c` `query_skills_by_category()` (~line 657) did an
      exact string match on category, so OCCs whose elective categories get
      aliased at the chargen layer ("science" -> "technical", "rogue" ->
      "espionage" in `setter.c`) could never *browse* the skills that were
      still tagged with the original raw category - e.g. a Rogue Scholar
      (Science-category picks) could type `pick skill archaeology` if they
      already knew the name, but `skills technical` would never list
      archaeology/astronomy/botany/anthropology/geology, and an
      Espionage-eligible OCC would never see seduction/escape
      artist/cardsharp/trap detection/palming/beg under `skills espionage`.
      Fixed by aliasing science<->technical and rogue<->espionage inside
      `query_skills_by_category()` itself. **Not yet live**: this is a daemon
      file (update-safe per CLAUDE.md, no full reboot needed) but applying it
      requires an in-game `update /daemon/rifts_skills` by an admin, or the
      next full reboot - I don't have admin login credentials to push it
      myself. UPDATE 2026-07-08: went live via full reboot and CONFIRMED
      in-game: skills technical lists the science set, skills espionage
      lists the rogue set, during real chargen elective picks. Verified via test data (archaeology/astronomy/botany/
      anthropology/geology are literally category "science"; seduction/beg/
      trap detection/escape artist/palming/cardsharp are literally category
      "rogue" - confirmed by reading every case block in rifts_skills.c).
- [x] Starting gear/skills check: `eq` and `score` both returned real,
      populated output for a fully-created character (equipment list,
      attribute block, skill count). Save-file field-format assertions in my
      test script gave false negatives (looking for an exact
      `"rifts_occ":"rogue scholar"` substring) - inconclusive on exact save
      format, not treated as a real failure since the in-session `pick occ`
      response and skill-category gating both proved the OCC was correctly
      applied.
- [x] Psi-sword combat test, MDC vs SDC combat - 2026-07-08: psi-sword
      activation confirmed as a Titan Cyber-Knight (10 ISP, 2d6 MDC
      message, say-emote fired). Combat vs the SDC coalition grunt in the
      test zone exercised via tools/section8_verify4.py; doing so exposed
      and fixed two crash bugs that had silently blocked combat: monsters
      without set_body_type() had empty limb arrays (std/monster.c now
      defaults human), and augment_body divided by zero on the whole_body
      pseudo-limb during OCC-step rerolls (std/living/body.c guarded).
- [x] Bank ATM card deposit/withdraw - CLOSED 2026-07-08: full in-room
      walkthrough passed (show id -> ATM card, balance 0 -> deposit 50 ->
      50 -> withdraw 10 -> 40, close account). Two fixes were needed to
      get there: bounty_board.c set_prevent_get(1) blocked entering Krasna
      square, and bank.c parse_amount_currency treated "deposit 50" as
      black credits (FluffOS sscanf counts %d matched even when a trailing
      literal fails; same trap fixed in cmds/mortal/_exchange.c).
      Original note follows: confirmed NOT a stub: `bank` is a
      physical room at `domains/Praxis/bank.c` with `add_action`s for `show`/
      `read`/`deposit`/`withdraw`/`balance`/`close`, gated on holding an ATM
      card (`atm_card` property) issued by the teller via `BANK_D`. My test
      tried a global `bank <verb>` command (wrong syntax) and got "What?" -
      that's a test-script error, not a game bug. Still needs a real
      in-room walkthrough to confirm the flow end-to-end - open.
      UPDATE 2026-07-08: walked in-room via scripted session: read sign,
      show id -> ATM card issued by the teller. The route itself was
      blocked until this session's bounty_board.c fix (set_prevent_get(1)
      threw during Krasna square load, making the square unenterable).
      deposit/withdraw/balance assertions are in section8_verify4.py.
- [x] Radio transmit between two characters - CONFIRMED 2026-07-08 with
      two simultaneous scripted connections: both radios tuned to channel
      5, receiver saw "[Radio 5] A human: hello ..." (introduce system
      hides the name as designed). tools/section8_verify4.py test_radio.
- [x] autododge/autoparry APM spend: confirmed working with real mechanic
      detail in the response ("Autododge ON. You will spend 1 APM to dodge
      each incoming attack (1d20+0)." / "Autoparry is currently ON. Parry
      bonus: 1d20+0.") for a fully-chargen'd character. (An *incomplete*
      chargen correctly refuses this with "That is a Rifts combat option." -
      that's correct gating, not a bug, confirmed by testing an
      intentionally-incomplete character mid-run.)

> **PROMPT FOR CLAUDE (Section 8 remainder):**
> Work in /home/thurtea/aethermud (AetherMUD, FluffOS 2.9, lib at
> nightmare3_fluffos_v2/lib, port 1122 - check ./mud.sh status first, note
> current connection count before restarting anything). Read CLAUDE.md.
> Three things left: (1) if nobody has run `update /daemon/rifts_skills`
> yet, do that (or it'll happen on the next reboot) to apply the
> science/rogue category-alias fix in query_skills_by_category(), then
> re-verify with `skills technical` as a Science-category OCC and confirm
> archaeology/astronomy/botany/anthropology/geology now appear; (2) use
> tools/section8_verify3.py as the working pattern (it's the one that
> succeeded - handles variable-length news-board pagination via an
> expect()-style poll loop, unlike the original section8_verify.py) to walk
> a fresh character to a bank room and to a weak monster near Praxis, and
> actually exercise deposit/withdraw/balance and MDC/SDC combat +
> autododge/autoparry APM spend + psi-sword activation in a real fight; (3)
> radio transmit needs two simultaneous socket connections - script both in
> one process. Clean up test character saves when done. Tick checkboxes and
> update master_gap_report.txt.

### 9. Parity extras remainder

- [ ] Player-run shops / economy depth (economy daemon exists, unused for this)
- [ ] Website parity: online help browser, who list on www/

> **PROMPT FOR CLAUDE (Section 9 remainder):**
> Work in /home/thurtea/aethermud (AetherMUD, FluffOS 2.9, lib at
> nightmare3_fluffos_v2/lib). Read CLAUDE.md. Two remaining parity items: (1)
> player-run shops - find the existing economy daemon (grep for economy_d or
> similar), design a minimal player shop/vendor-stall system consistent with
> its API, hook a shop-setup location in Praxis; (2) website parity - add a
> simple online help browser and a live "who's online" page to www/ (check
> daemon/http.c for what data is already exposed). Restart, check log/runtime
> and log/catch, tick report.md checkboxes, note in master_gap_report.txt.

### 10. Driver upgrade track (ABANDONED 2026-07-21 note: `archive/` deleted)

**The `archive/` directory described below no longer exists.** It was
removed entirely in commit `099ce07` ("restructuring of project",
2026-07-12) and is now gitignored - this was an intentional removal,
not an accidental loss, so there is nothing to restore and the modern-
driver port track below is abandoned, not paused. The checkboxes and
"PROMPT FOR CLAUDE" block that follow are historical record of what was
attempted before the removal; do not act on the prompt as written, it
references a directory that is gone. `RUNNING.md`'s "Modern driver
trial" section (which pointed at this same `archive/` layout) was
removed 2026-07-21 for the same reason.

Status (as of 2026-07-07, before the abandonment above): paused and
moved to `archive/` (bin-modern/, fluffos-modern/,
lib-modern/, mud-modern.sh, driver-upgrade-notes.md) to keep the project root
clean. Not needed for the live game. One real bug was found and fixed while
archiving: `archive/lib-modern/std/room/senses.c` used `status` as a function
return type (legacy MudOS/2.9 syntax) in two functions, which modern FluffOS's
parser rejects - changed to `int`. This means the modern driver's boot has
**not** actually completed cleanly yet, contrary to an earlier (incorrect)
progress note - see `archive/driver-upgrade-notes.md` for the full error
catalog from all 6 boot attempts plus the working 1124 boot's actual failure
point.

- [ ] Get `archive/lib-modern` booting clean on the modern driver (port 1124)
- [ ] Regression pass: chargen, combat, spells/psionics, mail, boards,
      shops, save/restore all verified on the new driver
- [ ] Cutover plan: swap mud.sh to the new driver, retire addr_server, enable
      websocket port, relax C89 rules in CLAUDE.md for new code

> **PROMPT FOR CLAUDE (Section 10 - resume driver port):**
> Work in /home/thurtea/aethermud/archive (AetherMUD modern-driver port track,
> paused; live game on port 1122 is untouched by anything here - do not
> touch nightmare3_fluffos_v2/ in this task). Read
> /home/thurtea/aethermud/CLAUDE.md and archive/driver-upgrade-notes.md first,
> the latter has the full error catalog from prior boot attempts including
> the senses.c `status` return-type bug already fixed in
> archive/lib-modern/std/room/senses.c. Attempt a boot: `cd archive &&
> ./mud-modern.sh start`, tail archive/lib-modern/log/runtime and
> log/catch, and iterate on any new errors the same way prior attempts did
> (catalog by cause in driver-upgrade-notes.md, fix in lib-modern only).
> When it boots clean, scripted-telnet a test login and a few rooms, then
> update driver-upgrade-notes.md with current status and next blocker.
> This is a long-running side track - do not attempt full cutover in this
> pass, just get further than the current failure point.

---

## Suggested execution order

1. Section 8 verification sweep (confirms recent fixes actually work)
2. Section 9 remainder (shops, website parity - lower player impact)
3. Section 10 driver port (parallel track, no rush, does not block content)
