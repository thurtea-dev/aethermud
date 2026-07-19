# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

This is **AetherMUD**, a TTRPG-inspired MUD built on the **Nightmare III MUD** base running on **FluffOS 2.9**. The goal is a faithful recreation of the classic early-2000s Palladium Books Rifts RPG MUD this project descends from (see the reference files below). The lib is in active Rifts conversion. Do NOT treat this as a standard NM3 lib.

- `nightmare3_fluffos_v2/fluffos-2.9-ds2.08/` - The C driver source
- `nightmare3_fluffos_v2/lib/` - The LPC mudlib (Rifts conversion)

Repo doc layout (since 2026-07-13, updated 2026-07-18): README.md,
CLAUDE.md, HOWTO.md, WHY.md, and LLM_BREADCRUMB.md live at the repo
root. Setup/ops/planning docs are under `docs/`, bulk reference
material (information.md, invocations.md, reference-chart.md) under
`docs/reference/`, and personal/machine-specific working notes under
`internal/`.

Per-directory docs (since 2026-07-15, landing in batches): every
directory in the workspace carries a README.md (what the directory is
for, plain English) and an LLM_BREADCRUMB.md (pointer telling an LLM to
check CLAUDE.md, then the local README.md, then the parent directory's
own LLM_BREADCRUMB.md, then relevant docs, plus directory-specific
gotchas) - except `nightmare3_fluffos_v2/fluffos-2.9-ds2.08/`'s internal
subdirectories (vendored FluffOS driver source; only its root gets the
pair). Maintain both files when adding a new directory or materially
changing what an existing one contains.

Reference files (READ THESE before making content decisions):
- `nightmare3_fluffos_v2/RiftsMUD Memories.txt` - Player memories of the original game (historical archive; keep its filename and content as-is)
- `/domains/Praxis/adm/master_gap_report.txt` - Current gap analysis (check header for latest batch notes)
- `www/index.html` (repo root) - Offline staff guides (QCS, coding, domain, roleplay, admin, chargen)
- GONE (do not cite): `RiftsMUD-AetherMUD Helpfiles.txt` and `RiftsMUD-AetherMUD RCC's OCC's List.txt` no longer exist in the repo; the OCC target list survives only via `docs/reference/reference-chart.md` and `daemon/occ.c` itself.

## CRITICAL: Verification Boundary - User Tests, Claude Does Not

Effective immediately and permanently, overriding anything to the
contrary anywhere else in this file or in prior session notes: Claude
makes changes, reports what changed and why, and stops. Claude never
runs a test, a verification, a reboot-and-check, a "quick single-shot
probe," or any other live check itself against this MUD - not even one
that seems small, narrowly scoped, or was pre-approved earlier in a
conversation. The user is the only one who tests, always, no exceptions.
If a future instruction from the user ever seems to ask Claude to
verify or test something itself, Claude must stop and ask the user to
confirm that is really what they mean, rather than just doing it.

## Session-end reporting (required)

At the end of every session, before stopping, produce a short
structured summary containing exactly these four things:

1. **What was changed this session** - files touched, one line each.
2. **What was found but NOT fixed** - deferred items, with the reason.
3. **A rough completion percentage** for the specific subsystem being
   worked on (e.g. "chargen: ~85% functional" or "NPC dialogue system:
   90% consistent"), stated as an honest estimate based on what was
   actually inspected this session, not a guess pulled from nowhere.
4. **The single most impactful next thing to work on**, and why it
   ranks above other open items.

This applies regardless of whether the session was a fix, an
investigation, or a documentation-only change.

## Recent session work (2026-06-29)

Parallel sprint batches 1-4 plus stability fixes. Summary:

**Batch 1:** Racial cmds (breath, fly, regen), Gunfighter + Palmer OCC, CharGen guide NPC, 5 monsters, Alvurron zone stub, Chi-Town fixes, C-12 rifle, magic net, spells/psionics/skills batch, legacy reskins (hospital, pub).

**Batch 2:** +6 spells (impervious fire/cold, breath of life, exorcism, heavy darkness, words of truth), familiar spawns rifts_familiar.c, +5 psionics, +3 skills, CS Terror Trooper PA, boom gun, Praxis library/town_hall/sheriff reskins.

**Batch 3:** store/retrieve + dim_pocket_bag, racial telemechanics, impervious fire/cold in combat, +5 spells, +5 psionics, +3 skills, +4 cybernetics, CS quartermaster gear, cs_secure_wing, help files for store/retrieve/telemechanics.

**Batch 4:** remoteview command, +5 spells, +5 psionics, +3 skills, cyber-doc install at hospital, mailbox welcome letter, Splynn ocean monsters, staff HTML guides in `www/`.

**Stability fixes (same session):**
- `square.c`: monument spawn moved from `create()` to `reset()` (safe in-game update)
- `secure/cmds/creator/_update.c`: move players to void before destructing a room file on `update /path/to/room.c`
- `std/room.c`: single space when appending `affect_environment()` text
- `mailbox.c`: use `set_max_encumbrance()` not `set_max_weight()`; inherit `/std/storage`
- Daemon path: skills live at `/daemon/rifts_skills.c` (no `_d` suffix)

**Batch 5:** Palmer2 OCC, card/chat/assist commands, +5 spells, +5 psionics, +3 skills, Alvurron quarter, slave pens NPCs, hall.c reskin, help files for sbar/slave/card/chat/assist.

**Batch 6:** assassination/assassinate commands, Lone Star containment wing + mutant subjects, legacy reskin (hall2-5, wild1, jungle, forest1).

**Batch 7:** Puerto Angel dockmaster + help, guild retirement (monk/mage/rogue), +5 spells, +5 psionics, +3 skills.

**Approximate content counts:** ~116 spells, 50 psionics, ~158 skills, 62 races, 62 OCCs (races remeasured 2026-07-13 evening: 62 entries in rifts_races_list; see docs/ASSESSMENT.md).

## Recent session work (2026-07-13)

- **Casting resource-loss fix:** `_cast.c` / `_psi.c` validate the target
  BEFORE spending PPE/ISP/APM. Each file carries a `NEED_TARGET_EFFECTS`
  macro listing every effect whose `fx_` handler hard-fails without a
  target (audited against every `!target` guard in the daemons). If you
  add a spell/psionic whose effect requires a target, add its effect key
  to that list; if it self-defaults or auto-targets (like magic net in
  combat), leave it out. Verified live.
- **Help system is two-level:** `help` lists categories only; `help
  <category>` (races, classes, skills, combat, communication, systems,
  commands, alignments, staff) lists that category's topics; `help
  <topic>` opens a topic; `help index` shows the old flat list; `help
  <name> overview` opens a topic file shadowed by a category keyword.
  Topic lookup is separator-insensitive (`armorofithan` == `armor of
  ithan` == `armor_of_ithan`) via `squash_topic()` in `daemon/help.c`.
  Function prototypes for help.c live in `daemon/help.h` (rule 17).
- **`daemon/command.c` rehash fix** (distinct_array, no duplicate path
  entries) was committed in `affed3b` on 2026-07-13.
- `standardOld/` remains live and erroring on reset (see docs/ASSESSMENT.md
  section 7); untouched by design so far. UPDATE 2026-07-19: moved to
  `nightmare3_fluffos_v2/attic/domains/Praxis/standardOld/` (outside the
  driver's lib root, unreachable in-game) after confirming zero code,
  config, and save-file references; see docs/lib-cleanup-manifest.md.
- **Evening playtest fixes (same day):** windrush is now a true room
  spell: `cast windrush` with no target sweeps every living being in
  the room (per-victim PE save vs 14, blown out a random exit, 1d6
  landing damage, each loose non-worn/non-wielded item has a 1-in-3
  chance to scatter into the origin room); `cast windrush at <target>`
  still works and requires a living target. Removed windrush from
  NEED_TARGET_EFFECTS in `_cast.c`. A range audit retagged energy
  bolt, light target, telekinesis, and animate object from "room" to
  "single" (data-only fix; those handlers are single-target).
- `tongues` command no longer prints the legacy NM3 fantasy language
  table (Eltherian/Tlinghan/etc); it casts the tongues spell when the
  player knows it, else reports comprehension. New
  `LANGUAGE_D->understands_all()` (godling race, tongues_active
  property, or active_language "All tongues") gates `languages` and
  score, which now say "You understand all languages."
- Skill help files are a systemic gap: 0 of the 158 skills in
  `rifts_skills.c` have a help topic under `doc/help/user` (falconry
  was not a one-off).

## Recent session work (2026-07-14)

- **Chargen input model is plain-string only** (`setter.c`,
  `chargen_guide.c`, `login.c`): every step takes the option name typed
  bare, with no verb prefix and no numbered menus. Zone: `americas` /
  `europe` / `atlantis`. Attributes: `roll`, then `reroll`. Race, OCC,
  alignment, and skill picks: type the name (`human`, `vagabond`,
  `scrupulous`, `tracking`); `list` lists races or OCCs; `none` skips
  OCC. Gender at login: `male` / `female` (case-insensitive). Legacy
  verbs (`pick X`, `start X`, `region X`, `alignment X`, `no occ`,
  `pick skill X`) still parse but are no longer advertised anywhere.
- **Reroll bug fixed:** after the first roll `creation_step` becomes
  "race", and the old `pick()` only accepted roll/reroll during
  "stats", so `pick reroll` fell through to race matching and failed.
  Reroll now works until a race is chosen, and each reroll re-displays
  the new attribute block plus rerolls remaining.
- **add_action catch-all gotcha (FluffOS 2.9):** flag 1 to add_action
  is V_SHORT, not V_NOSPACE (see driver simulate.h). With
  `add_action(fn, "", 1)` the function gets only the text AFTER the
  first word; the first word must be read via `query_verb()` (this is
  how living.c's cmd_hook works). `chargen_catch()` in setter.c
  rebuilds the full line as verb + " " + arg. It returns 0 for
  unrecognized input so normal commands (say, look, quit) fall through
  to cmd_hook during chargen.
- `www/chargen.html` staff guide updated to the new flow.

## Recent session work (2026-07-15)

- **Admin bootstrap verified working on the local Fedora checkout.**
  Investigated a reported auth bug (thurtea login had no admin/wizard
  privileges); found no bug. `/secure/cfg/groups.cfg` already has
  `thurtea` in SECURE and ASSIST, and `secure/save/users/t/thurtea.o`
  is a level 15 godling with `position "head arch"` and
  `wiz_role":"admin"`. Live login test confirmed the staff listing
  shows `[Admin-Wiz] Thurtea` and `score` matches the save file. No
  fix needed; closed out.

## Recent session work (2026-07-16)

- **Fixed a real structural bug, not a testing artifact: the news pager
  was silently eating every new player's first chargen answer.**
  `std/user.c`'s `setup()` called `move(ROOM_SETTER)` (which
  synchronously shows setter.c's zone-selection prompt) and then, later
  in the same function, `NEWS_D->read_news()` (`daemon/news.c`), which
  registers an `input_to()` pager ("Press \<return\> to continue:") when
  there is unread news. `input_to()` callbacks always intercept the
  player's next raw line ahead of any `add_action`-based command,
  including every one of setter.c's chargen verbs (`chargen_catch`,
  `region_cmd`, `roll_cmd`, etc, all `add_action`, none `input_to`). Since
  both the zone prompt and the news pager's `input_to()` registration
  happened within the same synchronous `setup()` call, a fresh
  character's first typed answer (`americas`, or anything else) was
  always consumed by the news pager instead of reaching chargen,
  silently, with no error, leaving `creation_step` stuck on `"region"`
  forever. Everything downstream (bare `roll` at the stats step, `start
  <region>`, etc) then appeared broken too, but those were symptoms: once
  region selection can't advance, every later step's own precondition
  checks correctly decline, and the global `cmds/mortal/_roll.c` /
  `_start.c` commands catch the fallback instead. This affected real
  players exactly as much as scripted tests; a patient human who typed
  their zone name directly (rather than pressing Enter first in response
  to the pager) would have hit the exact same swallowed answer. An
  earlier note in this file (since removed) mischaracterized this as a
  "scripted-testing quirk" requiring automated walkthroughs to answer the
  pager before sending the zone choice; that was incomplete. The pager
  race is real for everyone, and requires the same care in any client.
- **Fix:** `NEWS_D->read_news()` is now skipped in `setup()` for any
  player about to be routed to `ROOM_SETTER` (a new `needs_chargen` local
  captures the existing chargen-routing condition and gates both the
  `move()` and the `read_news()` call). `setter.c`'s `finish_creation()`
  now calls `NEWS_D->read_news()` itself once `creation_step` reaches
  `"done"` and the player has already landed in their real starting room,
  when nothing else is waiting on their next input. Non-chargen players
  (returning characters, reconnects, creators) are unaffected: `needs_chargen`
  is false for them, so `read_news()` still runs at the same point in
  `setup()` it always did.
- Also fixed independently, defense in depth: `cmds/mortal/_roll.c` and
  `cmds/mortal/_start.c` now decline (`return 0`) if the player has an
  active `creation_step` (set and not `"done"`), letting the search fall
  through to setter.c's own `roll_cmd`/`start_cmd` instead of claiming
  the command themselves. This alone would not have fixed the actual
  block (the root cause was the news pager, not command priority), but
  it's a correct safety net regardless.
- Verified live: one fresh registration, one `americas` sent immediately
  after the zone prompt, no retries. Got setter.c's real success line
  ("The Americas. You will begin on the outskirts of Praxis.") and
  `creation_step` advanced to `"stats"` in the save file, both on the
  first attempt.

## Recent session work (2026-07-17)

- **Correction: the VPS was never deleted.** A 2026-07-15 entry in this
  file (now removed) incorrectly stated the VPS instance
  (`aethermud-server` on the `aethermud.com` box) had been fully
  deleted and was out of scope. That was wrong, confirmed by live
  testing on 2026-07-17 (Mudlet connection to `aethermud.com:1122`,
  live chargen, live NPC interaction). **The VPS at `aethermud.com`
  (SSH host `zeus.cybercitizen.net`, user `thurtea`, working directory
  `/home/thurtea/aethermud`) is the live production server.** The
  local Fedora checkout under `/home/thurtea/aethermud` is where all
  development and testing happens. Changes are committed and pushed
  from local, then pulled on the VPS via git only after local testing
  is complete. The VPS should never be edited directly except to pull
  already-tested commits.

## What is still open (high level)

See `master_gap_report.txt` for detail. Major remaining work:

- **Legacy Praxis rooms:** cemetery, inn guest rooms (not yet built, no file exists)
- **Zones:** deeper Horton/NGR Germany/Chi-Town/Splynn interiors
- **Commands:** customize (suicide and pemote already exist, contrary to earlier notes)
- **Content depth:** vehicle interiors, more Palladium encyclopedia
- **Help files:** ~25 OCC help gaps
- **CharGen:** Full gamemaster walkthrough NPC

Already done: Puerto Angel, assassination, Lone Star, guild joins retired, card/chat/assist/sbar/slave, Palmer/Palmer2, NGR/Kittani OCCs, monastery.c and pit.c reskin (already Rifts-appropriate, no NM3 content remaining).

## Running the MUD

```bash
cd /path/to/nm3   # project root
./mud.sh stop
./mud.sh start
```

Never use `kill` directly. Always use mud.sh. Full reboot required after editing any file under /std/ (not just user.c, living.c, room.c, armour.c, combat.c) or secure/cmds/. Warmboot does NOT reload these for objects already in memory. See "UPDATE VS FULL REBOOT" below for why.

## UPDATE VS FULL REBOOT

`update <path>` and `warmboot` reload one file by destructing its blueprint
object and recompiling it at that exact path. In LPC, `inherit` is resolved
at compile time: an object that inherits another file gets that file's
compiled code baked in at the moment the inheriting object was compiled.
Reloading the parent's blueprint later does NOT change any object that
already inherited it.

- Daemon files (`/daemon/`): update-safe. Nothing inherits a daemon; every
  caller reaches it through `->` (call_other), resolved fresh on every call.
- Command files (`/cmds/`, `/secure/cmds/creator/`): update-safe. The
  command parser looks up the cmd file fresh on every command.
- A single room file: update-safe for that one room. `_update.c` moves
  players to the void and back around the reload.
- Any file under `/std/` that is inherited by rooms, players, or monsters:
  update only affects instances compiled after the reload. This is not
  limited to user.c, living.c, room.c, armour.c, and combat.c. It includes
  container.c, body.c, storage.c, and everything else reached through
  `secure/include/living.h`'s inherit chain. Every already-loaded room,
  every connected player, and every already-spawned NPC keeps running the
  old code until a full reboot. In-game `warmboot std` says this directly:
  "Connected players keep old inherited code until relog."
- Full reboot (`./mud.sh stop && ./mud.sh start`) is the only way to make
  a `/std/` change take effect for everything already in the game.

## CRITICAL: Known Mistakes - Do Not Repeat

### 1. Player save path
Player saves are at: `/secure/save/users/<first_letter>/<name>.o`
NOT at `/save/players/`. Never use the old NM3 path.

### 2. Room display order
The correct display order is:
  room description → exits line → NPCs → objects
This is already fixed in std/user.c describe_current_room().
Do NOT revert this order.

### 3. Smell/listen must NOT auto-display
set_smell() and set_listen() text must only appear when player
explicitly types `smell` or `listen`. Never put them in set_long()
and never call them during automatic room display.

### 4. C89 compliance required
All LPC files must use C89-style variable declarations.
ALL variables must be declared at the TOP of their function block,
before any code. Never declare variables mid-function.
This causes compile errors on FluffOS 2.9.

### 5. Warmboot vs full reboot
Warmboot (via the in-game `warmboot` command; there is no in-game
`reboot` command) does NOT
reload core std files: user.c, living.c, room.c, armour.c, combat.c
After editing any of these: run `./mud.sh stop && ./mud.sh start`
from the shell. Never tell the user warmboot is sufficient for these.

### 6. Log path double-slash bug
Two different conventions; mixing them causes /log//log/ paths:
- log_file() (the simulefun) prepends /log/ itself. Pass a path
  RELATIVE to /log: log_file("adm/setrole", ...). Never pass a path
  starting with /log/ (all such call sites were fixed 2026-07-17).
- write_file() takes the literal absolute path: "/log/adm/setrole".
Never concatenate LOG_DIR/DIR_LOGS with a path that already starts
with /log/.

### 7. Ambient messages
Do NOT add set_ambient_messages() or start_ambient() to rooms.
The rifts_ambient mixin was removed from all rooms intentionally.
Ambient messages interrupted players and were unwanted.

### 8. NPC/object in set_long
NEVER describe spawned NPCs or objects in set_long(). They spawn
as real objects via reset() and display themselves below the exits
line. Hardcoding NPC names in set_long() causes them to appear
twice and in the wrong position (above exits).

### 9. Setrole log path
/cmds/adm/_setrole.c logs via log_file("adm/setrole", ...), which
writes to /log/adm/setrole (see rule 6 for the two conventions).

### 10. Wiz tools location
Admin/wizard tools live at /domains/adm/wiz_tools/ NOT /domains/Praxis/wiz_tools/

### 11. Room spawns belong in reset(), not create()
NEVER clone NPCs or objects with `new()->move(this_object())` inside a room's
`create()`. It breaks `update /path/to/room.c` and can duplicate or crash.
Use `reset()` with `present("id", this_object())` checks instead.
See `/domains/Praxis/square.c` and `/domains/Praxis/hospital.c` for the pattern.

### 12. Container weight API
Use `set_max_encumbrance(n)` on containers. There is no `set_max_weight()`.

### 13. Updating room files in-game
`update /domains/foo/room.c` while standing in that room is safe after the
_update.c fix (players moved to void, room reloaded, players restored).
Bare `update` with no path also works when inside the room.

### 15. NPC MDC stats
Use `set_stats("MDC", n)` and `set_stats("max_MDC", n)` on NPCs.
There is no `set_mdc()` on monsters (only on rifts_vehicle.c).

### 16. No em dashes in player-facing or repo text
Do NOT use em dashes (U+2014, the long dash character) anywhere in:
- in-game strings (set_long, set_short, message, tell_object, write)
- help files and player-facing documents
- code comments and CLAUDE.md
Use a hyphen (-), comma, colon, or rephrase instead.
Example: write "Rifts 2.1, a Palladium MUD" not "Rifts 2.1 [long dash] a Palladium MUD".

### 17. Functions must be declared before use (FluffOS 2.9)
Calling a function defined later in the same file is a COMPILE ERROR
("Undefined function X"), unlike modern LPC. When adding a helper that
is called above its definition, add a prototype near the top of the
file (see the existing prototype blocks in daemon/rifts.c, setter.c,
and std/monster.c). This broke /std/monster for a whole boot on
2026-07-08 (no NPCs loaded anywhere): the error only shows in
log/errors/<dir>, not in log/runtime, so check there when an object
mysteriously fails to load.

### 18. sscanf trailing literals do not gate the match
FluffOS sscanf counts %d as matched even when a LATER literal in the
format fails, so sscanf("50", "%d black", n) == 1. Never dispatch on
formats like "%d black" vs "%d credits" - capture the tail with
"%d %s" and compare the word explicitly (see bank.c
parse_amount_currency and cmds/mortal/_exchange.c, fixed 2026-07-08).
Literals BEFORE the first specifier are safe.

Related trap, same root cause: a literal that has NOTHING left to match
(not a mismatch, an absence) stops the match early too. secure/daemon/
master.c's load_access() parses groups.cfg lines with
sscanf(line, "(%s) %s", fl, ac); a member-less group line like
"(AMBASSADOR)" has no trailing space/name for the literal " " to
consume, so the match returns 1 instead of 2. load_access() now
tries a bare sscanf(line, "(%s)", fl) as a second pass and treats that
as a valid empty group by design - do not "fix" that fallback by
requiring a trailing space in groups.cfg lines again (fixed
2026-07-15).

## Rifts-Specific Architecture

### Race/OCC system
- Races: /daemon/rifts.c
- OCCs: /daemon/occ.c
- Starting packages: /daemon/rifts_start_d.c
- Skills: /daemon/rifts_skills.c
- Spells: /daemon/rifts_spells_d.c
- Psionics: /daemon/rifts_psionics_d.c
- Combat: /daemon/rifts_combat.c
- Bionics: /daemon/bionic_d.c

### Equipment paths
- General equipment: /domains/Praxis/equipment/
- Power armor: /domains/Praxis/equipment/power_armor/
- Cybernetics: /domains/Praxis/equipment/cybernetics/
- Vehicles (rideable items): /domains/Praxis/equipment/vehicles/
  (legacy hover_cycle.c remains at the equipment root; do not move it,
  rooms reference that path)
- Vehicle interiors (enterable rooms): /domains/Praxis/vehicles/
  (ex5/ and mountaineer/)
- Grenades: /domains/Praxis/equipment/grenades/
- Magic items: /domains/Praxis/equipment/magic_items/
  (legacy magic items such as armor_talisman.c, sword_of_atlantis.c,
  ghostly_katana.c, flame_hilt.c remain at the equipment root; do not
  move them, rooms and NPCs reference those paths)
- Admin tools: /domains/adm/wiz_tools/

### Key daemon preload order
All Rifts daemons must be in secure/cfg/preload.cfg.
If a daemon is missing from preload: it will not be available at boot.
Check preload.cfg when adding new daemon files.

### MDC vs SDC
- MDC creatures: set_property("mdc_creature", 1). Use query_mdc()/set_mdc().
- SDC creatures: standard HP + SDC system.
- MDC weapons required to damage MDC creatures.
- psi_sword_active property allows unarmed MDC penetration for Cyber-Knights.

### Color conventions (match the original game)
- Exits line: bold green
- Radio transmissions: green [Radio] prefix
- OOC channel: red/white
- Tells: blue
- Status effects in score (thirst, drugs, psi-sword): blue/cyan
- Room descriptions: no color
- NPC/object names: no color
Keep color minimal; the original had very little color.

## Mudlib Structure

```
lib/
  secure/save/users/     <- player saves (NOT /save/players/)
  secure/cfg/preload.cfg <- daemon preload list
  std/user.c             <- core player display (exits before NPCs - DO NOT REVERT)
  std/armour.c           <- wear/remove logic including GHD size checks
  cmds/mortal/           <- player commands (_radio, _rest, _remoteview, _store, etc.)
  cmds/adm/              <- admin commands (_setrole, _playerwipe, etc.)
  secure/cmds/creator/   <- update, clone (room-safe update logic here)
  domains/Praxis/        <- main Rifts game world
  domains/adm/           <- admin domain (wiz tools, etc.)
  domains/chitown/       <- Chi-Town zone
  daemon/                <- game daemons (rifts, occ, skills, spells, etc.)
```

Staff HTML guides: `www/` at the repo root (not under lib/)

## Building / Testing

After editing any file:
1. `update /path/to/file` from in-game to recompile (use `/daemon/rifts_skills.c` not `_d` for skills)
2. Check log/runtime for errors immediately (`/log/catch`)
3. For std/ core files or secure/cmds/: full reboot required (mud.sh stop/start)
4. Warmboot only for domain/daemon/cmd files under lib/, never for std/ or secure/

## LPC Language Notes

C89 rules apply strictly:
- All variable declarations at top of function, before any statements
- No `//` comments; use `/* */` only
- No mixed declarations and code

Common patterns:
- `this_player()->query_property("x")` - read player property
- `this_player()->set_property("x", val)` - set player property
- `environment(this_player())` - get current room
- `tell_object(ob, "msg\n")` - send message to specific object
- `message("type", "text\n", room)` - send typed message to room

## Security Notes

Never allow player-accessible commands to:
- Read or write to /secure/ without privilege check
- Delete save files (use _playerwipe.c admin command instead)
- Access other players' save data

## Spelling Rules

- Use American English in string literals and comments: armor, color, defense, offense, center, favor.
- Never rename or alter `/std/armour.c` or any `armour.h` header.
- Never rename these functions: `equip_armour_to_limb`, `remove_armour_from_limb`, `set_armour_type`, `query_armour_type`.
- Never rename the `ARMOUR` macro in `std.h`.
- Apply spelling fixes only in user-facing strings (`set_long`, `set_short`, `message`, `tell_object`, `write`) and comments.

## Help Files

- Help files have no extension: use `/doc/help/user/score`, not `score.hlp`.
- Before creating a new help entry, check existing files in the same directory and match that format.

## Position System

- `position <string>` sets an RP position string shown in room descriptions.
- `position` with no argument clears it and resets display to `is standing around.`.
- Position resets on room change via movement hook in `std/living.c`.
- Named NPCs should set `set_property("position_str", "...")` in `create()`.
- Combat stance is separate: the `stance` command uses the `combat_stance` property.

## Armor Slot

- Use `set_property("rifts_slot", "armor")` on protective armor for `_eq` display.
- This is display-only and does not change wear/remove internals.
- `std/living/body.c` remains the source of truth for wear/remove behavior.
- Cosmetic items should continue using their own slot labels (head, neck, shirt, back, belt, legs, hands, feet, etc).
