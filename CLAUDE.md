# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

This is **AetherMUD**, a TTRPG-inspired MUD built on the **Nightmare III MUD** base running on **FluffOS 2.9**. The goal is a faithful recreation of the classic early-2000s Palladium Books Rifts RPG MUD this project descends from (see the reference files below). The lib is in active Rifts conversion. Do NOT treat this as a standard NM3 lib.

- `nightmare3_fluffos_v2/fluffos-2.9-ds2.08/` - The C driver source
- `nightmare3_fluffos_v2/lib/` - The LPC mudlib (Rifts conversion)

Reference files (READ THESE before making content decisions):
- `nightmare3_fluffos_v2/RiftsMUD Memories.txt` - Player memories of the original game (historical archive; keep its filename and content as-is)
- `/domains/Praxis/adm/master_gap_report.txt` - Current gap analysis (check header for latest batch notes)
- `www/index.html` (repo root) - Offline staff guides (QCS, coding, domain, roleplay, admin, chargen)
- GONE (do not cite): `RiftsMUD-AetherMUD Helpfiles.txt` and `RiftsMUD-AetherMUD RCC's OCC's List.txt` no longer exist in the repo; the OCC target list survives only via `reference-chart.md` and `daemon/occ.c` itself.

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

**Approximate content counts:** ~116 spells, 50 psionics, ~158 skills, 61 races, 62 OCCs (measured 2026-07-13, see ASSESSMENT.md).

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
- `standardOld/` remains live and erroring on reset (see ASSESSMENT.md
  section 7); untouched by design so far.
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
Log paths must use literal strings. Do NOT concatenate LOG_DIR with
a path that already starts with /log/. Use "/log/adm/setrole" not
LOG_DIR + "/log/setrole".

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
/cmds/adm/_setrole.c logs to "/log/adm/setrole" (literal string).

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
