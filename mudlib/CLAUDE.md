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
- `docs/STYLE.md` - Player-facing and comment tone/voice (no casual slang; exceptions for deliberate NPC dialect). Apply whenever writing or editing strings.
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

**Scoped exception: custom driver development and regression testing.**
The boundary above governs this MUD as a live game: `mud.sh`,
telnet/client connections, and anything that touches real player data
on the production instance or the shared local development instance.
It does not apply to work under `aethermud-repo/driver` (the custom
C++ lpcdriver project) when Claude builds and exercises that driver
against a private scratch instance for regression testing: a
non-default port (never the production port or the user's own
development port), a throwaway scratch config, and save data created
and cleaned up within the same session. In that narrow context Claude
may rebuild, start/stop the scratch instance, and run live socket
tests itself without asking each time, consistent with how the driver
project has been run throughout. Any test reaching outside that
scratch instance (the production MUD, the user's own running
instance, or persistent save data outside the scratch scope) falls
back under the full boundary above, including the stop-and-confirm
requirement.

## CRITICAL: Git Push Boundary - User Pushes, Claude Does Not

Effective immediately and permanently, overriding anything to the
contrary anywhere else in this file or in prior session notes:
Claude never runs `git push` in any form - not `git push`, not
`git push --force`, not `git push -u`, to any remote, to any
branch - under any circumstances, no matter how urgent, how small,
how "obviously correct," or how explicitly a task seems to call
for it. This is the same category of boundary as the verification
rule above and must be treated with the same seriousness: the user
runs every git push themselves, from their own terminal, always,
no exceptions. Local, reversible git operations (status, log,
diff, add, commit, a history rewrite tool like git-filter-repo
against a repo that has been backed up and verified) remain fine
to run locally when a task genuinely calls for them, but the
moment a step is "send this to a remote," Claude's job stops at
getting everything ready and verified locally, then telling the
user the exact command to run - never running it. If a future
instruction from the user ever seems to ask Claude to push
directly, even with detailed steps and clear reasoning, Claude
must stop and hand back the exact command instead of running it.

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

## Command naming rule (binding, permanent)

Any spell, skill, psionic power, or ability with a multi-word
name must NEVER use underscores or spaces as the player-facing
help/command key. Multi-word names are always collapsed into a
single lowercase word with no separator.

Correct: help armorofithan, help remoteviewing, help telepathy,
help languages, help primaryskills, help secondaryskills

Incorrect: help armor_of_ithan, help armor of ithan (as two
tokens requiring exact spacing), help remote_viewing

The help system must still accept a spaced form as input
(e.g. "help armor of ithan") and normalize it internally to the
collapsed key, but the canonical stored key, file name, and
displayed reference in any help listing must always be the
single collapsed word. Never display or store underscore_case
as the topic name anywhere player-facing.

This applies retroactively. Any existing help file or menu
using underscores or multi-word spaced keys must be renamed to
the collapsed form as encountered, not left as-is.

## Help system rule (binding, permanent)

There is exactly ONE help system in this codebase. The old
paged "Category: *player general ... Selection:" style menu
system is deprecated and must not be extended, referenced, or
left reachable. Do not maintain two parallel help systems.

The only acceptable help system format is the category-list
format already in production, e.g.:

===================== AetherMUD Help =====================
races            Playable races and RCCs
classes          Occupational Character Classes (OCCs)
skills           Skills, spells, psionics, abilities
...
Type help <category> to list the topics in a category.
Type help <topic> for details on a specific topic.
Type help index to see every topic in one list.
=====================================================

Any code path, command, or menu still using the old paged
"Selection:" / "q to quit" style must be flagged immediately
when found and scheduled for removal - do not leave it running
alongside the new system.

## Website writing rule (binding, permanent)

Website copy (www/*.html) must be factual, concise, and free of
narrative/marketing language. Before writing or editing any
www/ page, self-check against this list and reject any of the
following:

- Sentences that address the reader as part of a story
  ("This is where you come in.")
- Restating the product name/tagline more than once per page
- Descriptive scene-setting or flavor prose outside of
  in-game room/NPC text (that belongs in the mudlib, not the
  website)
- Vague hype adjectives or unnecessary superlatives
- Any sentence whose only function is tone/atmosphere rather
  than conveying a fact (a mechanic, a number, a command, a
  status)

Preferred style: short factual statements, lists, and tables.
State what a system is and how to use it. Do not explain why a
feature might matter to the reader, do not narrate travel or
setting, do not use rule-of-cool language.

Do not add scope, lore, or claims beyond what already exists in
the code/docs when doing a tone/style pass. A rewrite for tone
must never expand content.

## Recent session work (2026-06-29)

- 2026-06-29: Completed sprint batches 1-7 and stability fixes across commands, content, and help, with tracked totals at 114 spells, 51 psionics, 157 skills, 62 races, and 66 OCCs.

## Recent session work (2026-07-13)

- 2026-07-13: Landed the cast/psi target-before-cost guard, two-level separator-insensitive help behavior, command rehash cleanup, windrush and targeting corrections, tongues/language handling updates, and documented the systemic skill-help gap.

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
  is a level 15 wizard with `position "head arch"` and
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
  `/home/thurtea/am`) is the live production server.** The
  local Fedora checkout under `/home/thurtea/am` is where all
  development and testing happens. Changes are committed and pushed
  from local, then pulled on the VPS via git only after local testing
  is complete. The VPS should never be edited directly except to pull
  already-tested commits.

## Recent session work (2026-07-19)

- **Whole-body damage model, cosmetic wear slots, appearance override,
  narrative severing** all landed this session; the full rules live in
  the "Armor, Slots, and the Whole-Body Damage Model" section below.
  Touched: std/living/body.c, std/living/combat.c, std/armour.c,
  std/user.c, std/monster.c, std/living.c, daemon/rifts_combat.c,
  daemon/rifts_spells_d.c, cmds/mortal/_body.c, _eq.c,
  cmds/skills/_limbs.c, _cure.c, _replace.c, _backstab.c, _balefire.c,
  _demonfire.c, _drain.c, _lockpick.c, cmds/adm/_sever.c (new),
  domains/Praxis/crypt.c, sheriff.c, obj/misc/stone.c,
  equipment/dead_boy_armor.c, equipment/splynn/predator_armor.c.
  FULL REBOOT required (std/ files changed).
- Legacy heal skills (_mheal, _mend, _heal, _rot) still call
  heal_limb(); harmless no-ops on gameplay under the pooled model,
  left for a later cleanup pass alongside their class gating.

## Recent session work (2026-07-25)

- **Gamemaster NPC removed from chargen entirely**, per user decision
  (not just silenced). Deleted `domains/Praxis/npcs/chargen_guide.c`
  and its New Camelot reflavor `domains/NewCamelot/npcs/camelot_guide.c`.
  Removed all four spawn sites: `rifts_welcome.c`, `setter.c`,
  `chitown_start.c` (spawned chargen_guide directly), and
  `newcamelot_start.c` (spawned camelot_guide, which persisted in the
  New Camelot gatehouse room well after chargen, not just during it).
  See `master_gap_report.txt`'s 2026-07-25 entry for detail.
- **Duplicate gold ring fix:** `ring_of_dominion.c` (the personal, hand-
  placed admin ring, see its file header) had no dedup protection --
  unlike the physical wiz staves, it never goes through
  `WIZTOOLS_D->give_tools()`/`ensure_wiz_tools()`'s `has_wiz_tools` env
  flag or wiz_tools-substring inventory scan, so nothing stopped a
  second one from ending up in inventory alongside the worn one. Added
  `dedup_ring()`, called from `init()`, which destructs whichever copy
  isn't worn when two turn up in the same inventory. No automated code
  path was found that clones this ring at all (confirmed via full-repo
  grep) - it appears to be genuinely one-off and manually placed, so
  the duplicate likely came from a manual double-clone, not a live
  promotion-path bug. No other player save file references
  `ring_of_dominion`; no broader cleanup pass appeared necessary.
- **Item-drop-on-quit is real and confirmed, not a bug:** traced the
  full mechanism (`std/user.c` quit() -> remove() -> `std/living.c`
  remove() -> `std/Object.c` remove() -> `std/clean_up.c` remove(),
  which explicitly moves ALL remaining inventory into the player's
  current room before destructing the player object; the driver's own
  `destruct_object()` in simulate.c does the same at the C level as a
  second layer). Only objects implementing `query_auto_load()` (wiz
  tools, credcard.c, id_card.c, handcuffs.c, dim_pocket_bag.c,
  wed_ring.c) survive a disconnect/relog at all; ordinary equipment
  (weapons, armor, most loot) has none and is dropped in the room on
  every quit. This predates Rifts conversion (inherited from the
  Nightmare IV base) and affects everyone, not just reported test
  items like `obsidian_blade.c`. Directly informs the pending player-
  storage/housing feature proposal (not yet built).
- **Follow-up same day: three proposals approved, one parked.**
  - Default who-list titles: `_makewiz.c`/`_setrole.c` now auto-set a
    `whotitle` env per rank ("A NEW Apprentice/Admin/Domain/Coding/
    Roleplay Wizard!"). Guarded by `is_auto_whotitle()` (same list
    duplicated in both files): replaces an unset whotitle OR a
    leftover auto-default from an earlier rank (so the normal
    makewiz -> setrole pipeline correctly progresses through titles),
    but never a genuine custom title staff set some other way. The
    bootstrap "head arch" founder's hardcoded "The First Admin"
    fallback in `_who.c` is untouched (setrole's "admin" role always
    sets position "arch", never "head arch").
  - Pet storage: `std/rifts_pet.c` gained a generic `cmd_stable`
    (add_action "stable", matches any subclass's own `id()` list) that
    destructs the pet and records `stabled_pet`/`stabled_pet_name`/
    `stabled_pet_ids` as plain env strings on the owner (survives
    save/reload natively, no autoload trickery needed). New
    `cmds/mortal/_call.c` is the recall fallback -- it only ever runs
    when no living pet's own add_action-bound "call" (trained_hawk.c)
    answers first, since add_action always wins over a directory-
    searched command. Uses `function_exists("bind_to", pet)` to prefer
    a subclass's own binding hook (rifts_familiar.c) over the base
    `set_owner()` (sufficient for trained_hawk.c, which has no
    override), so both pet types recreate correctly with no orphaned
    duplicate.
  - **Universal item-drop-on-quit fixed** (the higher priority than
    Task 5's storage/housing feature, which stays parked): `std/
    weapon.c` and `std/armour.c` each gained a `query_auto_load()`
    gated on `query_wielded()`/`query_worn()` (dynamic, self-updating,
    no flag to maintain) plus an `init_arg()` that re-wields/re-wears
    via `cmd_wield(query_name())` / `wear(query_name())` (same pattern
    `ring_of_dominion.c` already used). This alone is sufficient with
    NO clean_up.c/living.c changes needed: `pre_save()` runs at the
    START of quit(), before anything unequips, so equipped gear is
    captured into `__AutoLoad` while still equipped; `std/user.c`'s
    `remove()` runs `autosave::remove()` (which destructs anything
    autoload-tagged directly, cleanly, via each file's own existing
    `remove()` -> unequip() override, no floor-drop) BEFORE
    `living::remove()`'s drop-gated loop and clean_up.c's
    move-everything-to-the-room dump ever see it. Loose/unequipped
    inventory is unaffected and still drops on quit exactly as before.
    Storage/housing (Task 5) stays parked past the playtest window per
    user decision; design notes above remain the reference when it's
    picked back up.

## Recent session work (2026-07-26)

- **Magic net dissolve message** simplified in `std/obj/magic_net.c`
  (dropped the PPE reference). That file is cloned via
  `new("/std/obj/magic_net")` and never inherited, so `update` reaches
  new nets; nets already in play keep the old string for at most their
  120-second lifetime.
- **Lone Star expanded 11 -> 23 rooms** (roadmap Phase B item 7). Four
  wings, all hooked into the existing exit graph: kennel wing (yard,
  rows, psi-scent training ring), incubation core (gene vault,
  incubation gallery, observation booth, cold storage), escape route
  past the formerly dead-ended breached tunnel (junction, flooded sump,
  culvert mouth), and a civilian support town south of the gate
  (Support Row, the Dry Kennel canteen). Two closed loops now exist:
  plaza -> lab -> containment -> incubation -> observation -> gene
  vault -> lab, and gate -> perimeter -> tunnel -> junction -> culvert
  -> gate. The motor pool roll-up door, already described in its own
  long text, now actually opens onto the perimeter road. New NPCs:
  kennel master, civilian contractor, canteen keeper (vendor). New
  monsters: juvenile Dog Boy, failed specimen. `docs/zone-expansion-plan.md`
  updated with real counts.
- **Support Row registration sweep** (`lone_star_support_row.c` only, no
  new files, `cs_dog_boy_guard.c` untouched). Room `init()` arms a
  20-60s `call_out` when `WANTED_D->is_cs_enemy()` flags a loitering
  D-Bee; the patrol then demands papers on a 30-second window.
  Complying (`show papers`) means detention: +50 infamy, -25 coalition
  standing, moved to `lone_star_containment` with gear intact, escape
  via the new drainage route. Leaving during the window is a clean
  escape with no penalty. Refusing (timeout) means the patrol attacks.
  Bypass is `query_appearance_override()`, so Dead Boy armor gets a
  D-Bee waved through - the "proper gear" line from the memories, using
  a feature that already existed with no mechanic attached. Cooldown
  only starts if a patrol actually arrived (otherwise stepping in and
  out bought free quiet time), `add_action` is gated on `is_player()`
  because the patrol arrives from a `call_out` where `this_player()` is
  0, and a tagged-guard scan prevents patrol stacking.
- **Metamorph false-success fixed.** `cmds/mortal/_metamorph.c` printed
  its success text unconditionally, without checking that anything
  changed. It now computes what a stranger would see before and after
  (`stranger_race()`, which mirrors `std/user.c`
  `query_apparent_race()`'s precedence chain), declines with "You
  already appear to be a X" when there is nothing to change, and rolls
  the properties back if verification fails. It also reports what
  strangers now see, and warns a creator that staff status makes their
  own disguise inert.
- **Important, and the reason metamorph looked broken:**
  `query_apparent_race()` and `query_display_name()` in `std/user.c`
  return the TRUE race when the viewer is the character themselves, is
  a creator, or already knows the character. Those exemptions are
  deliberate. A staff character testing metamorph on themselves, or
  observed by another staff character, will therefore never see the
  disguise take effect through `look`. **Metamorph must be tested on a
  mortal character, observed by a different mortal character.** Note
  also that `creatorp(this_object())` means a staff character's
  disguise is inert for every viewer, mortals included.
- Supporting changes for the above: `cmds/mortal/_score.c` now shows
  `Race: <true> (as <held form>)` while metamorphed; `std/living.c`
  `query_long()` appends "Currently holding the form of a X" for self
  and staff viewers only (never strangers, which would leak the
  disguise); `std/user.c` `id()` additionally matches the held form
  word for self and staff, so `look at human` works for the people who
  need to verify it, but not for strangers (a stranger matching both
  the held form and the true race could confirm a disguise by trying
  the true race word). FULL REBOOT required for the `std/living.c` and
  `std/user.c` parts.
- Rule 8 / rule 3 text fixes in `lone_star_lab.c` (removed the two
  sentences describing the spawned scientist and guard, dropped the
  dead `guard` and `scientists` item keys) and `lone_star_plaza.c`
  (removed the sentence describing spawned staff and patrols, which
  also duplicated `set_smell()` text, and dropped the dead `staff`
  item key).

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

### 19. Directory and file naming under domains/ (updated 2026-07-28)

Top-level directories directly under `lib/domains/` name a narrative
game zone. They may keep their established canonical name, whatever
style that is:

- CamelCase proper-name domains: `Horton`, `Lazlo`, `LoneStar`,
  `Praxis`, `PuertoAngel`, `ChiTown`, `NewCamelot`, `DemonPlane`,
  `Tolkeen`.
- Lowercase/snake_case legacy domains: `adm` (admin tooling, not a
  narrative zone), `wizards`.
- `NGR` stays all-caps (an intentional acronym, not a CamelCase word).

Do not invent new mixed naming styles arbitrarily (no
`Demon_Plane`, no `newCamelot`, no `LONEstar`).

For a NEW top-level domain directory, prefer matching the canonical
style of the setting/proper noun if it names a place: CamelCase is
appropriate for a multi-word proper name (`NewCamelot` is the model
to follow). A single lowercase word or an existing acronym follows
its own natural casing instead of being forced into CamelCase.

File names beneath `domains/` (and elsewhere in the project - `std/`,
`cmds/`, `docs/`, etc.) use lowercase_with_underscores:
`bandit_camp.c`, `burb_chop_shop.c`. Avoid introducing CamelCase file
names.

When touching existing content for an unrelated reason, do not rename
legacy paths just to make them style-consistent unless the user has
explicitly asked for that rename. If a naming inconsistency turns up
while working on something else, flag it in the session report
instead of renaming it - renames need explicit approval before they
happen (see the reference audit before running one; `wizards`, for
example, touches `/std/user.c` and needs a full reboot).

Status: `chitown` -> `ChiTown` and `newcamelot` -> `NewCamelot` were
renamed 2026-07-21. `demon_plane` -> `DemonPlane` and `tolkeen` ->
`Tolkeen` were renamed 2026-07-28 (explicit user approval to override
the grandfather exception for these two specifically; both were
zero-risk, no `std/` or `secure/` references, no dynamic path
construction, no full reboot needed). `wizards` remains grandfathered
under its existing lowercase name - it is hardcoded into `std/user.c`
with dynamic path construction (`"/domains/wizards/" + query_name() +
"/workroom"`), so a rename there needs a full reboot and its own
explicit approval; it was evaluated 2026-07-28 and deliberately left
alone. `adm` also remains lowercase by design (admin tooling, not a
narrative zone - the proper-noun capitalization question doesn't apply
to it).

### 20. No player chat channels of any kind
All player-to-player communication is room-local (same reach as say),
except the radio system, which requires a carried radio item tuned to
a matching frequency (std/rifts_radio.c). There are no global,
domain-wide, or otherwise bare cross-room channels for players, and no
channel on/off toggles, because there is no channel to toggle.
Before adding or editing any player-facing command: if it loops
users(), or uses find_player()/tell_object() to reach a player who is
not present() in the room, that is a bug, not a feature. Convert it to
room-local scope (see cmds/mortal/_say.c, _ooc.c) or route it through
a carried radio item instead. This applies even to system
notifications (invites, requests), not just open chat -- if a command
reaches a player outside the room without a radio, it needs a decision
before it ships, not an assumption that it's fine.

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
  domains/ChiTown/       <- Chi-Town zone
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
- Multi-word topic filenames use lowercase_with_underscores (`armor_of_ithan`,
  `holistic_medicine`), matching the project's general file-naming convention
  and the order `find_user_help_file()` in `daemon/help.c` already tries
  (literal name, then the underscored slug, before falling back to full
  squash matching). This is a consistency convention only, not a functional
  requirement: `squash_topic()` strips every separator before matching, so
  `help holistic medicine`, `help holistic_medicine`, and `help
  holisticmedicine` all resolve identically regardless of which separator
  the on-disk filename uses.
- Two small legacy exceptions, not the pattern to follow: a few files
  hyphenate because that's the correct English spelling of the term itself
  (`half-elf`, `half-orc`, `rahu-man`, `getting-started`, `multi-playing`) -
  keep hyphenating a naturally-hyphenated word if you touch one of these;
  and eight older psionic/skill-system files (`psisword`, `objectread`,
  `readaura`, `senseevil`, `mindblock`, `pskills`, `sskills`,
  `skillrequest`) squash two words together with no separator at all.
  Both groups are grandfathered, not renamed without explicit approval -
  same policy as the domain-directory naming rule.

## Position System

- `position <string>` sets an RP position string shown in room descriptions.
- `position` with no argument clears it and resets display to `is standing around.`.
- Position resets on room change via movement hook in `std/living.c`.
- Named NPCs should set `set_property("position_str", "...")` in `create()`.
- Combat stance is separate: the `stance` command uses the `combat_stance` property.

## Armor, Slots, and the Whole-Body Damage Model (since 2026-07-19)

Damage model:
- All combat and hazard damage lands on pooled stats only: barrier
  fields (properties ithan_armor, invincible_armor, psi_shield_mdc),
  then worn armor pools (armor_mdc/armor_sdc properties on the item),
  then the character's MDC (MDC beings) or SDC then rifts_hp. Pure
  legacy victims with no Rifts pools fall back to the NM3 hp pool via
  do_damage("whole_body", n).
- The single damage-bearing pseudo-limb is named `whole_body` (players
  in std/user.c new_body(), NPCs in std/monster.c set_body_type()).
  Do NOT rename it: the body/limbs mappings are serialized into player
  saves.
- Per-limb damage, damage-triggered severing, and limb targeting are
  retired. body.c do_damage() pools everything; check_on_limb() and
  severed_limb() are no-op stubs. return_limb()/return_target_limb()
  exist for message flavor only and never return whole_body.
- Non-combat damage sources must call
  RIFTS_COMBAT_D->apply_direct_damage(victim, damage), never
  do_damage() with a named limb.
- Protective spells (Armor of Ithan, Invincible Armor, psychic body
  field) are depleting barrier pools stored in properties and drained
  by the damage chain. They never touch the character's MDC stat.

Limb severing is narrative-only:
- body.c sever_limb(limb) / restore_limb(limb); admin command `sever`
  (cmds/adm/_sever.c: sever/restore/list); cleric `replace` skill uses
  restore_limb. Severed limbs show in look, body, and limbs output.
  whole_body, torso, and FATAL limbs (head) cannot be severed.

Wear slots:
- Protective armor (mdc_armor/sdc_armor property, type "body armour",
  or rifts_slot "armor") is worn on whole_body via the legacy equip
  path; std/armour.c is_protective_armour() decides.
- Cosmetic pieces occupy slot-based wear (std/armour.c, NOT the limb
  system): head, neck, shirt, back, belt, legs, hands, feet, ring1,
  ring2. One item per slot; rings fill ring1 then ring2. The slot
  comes from the rifts_slot property (aliases hat/necklace/pants/
  shoes/gloves/cloak accepted) or falls back to the armour type
  (helmet, necklace, cloak, backpack, belt, pants, gloves, boots,
  ring). Cosmetic pieces need no set_limbs() and never carry or stop
  damage.
- Items with neither a protective marker nor a resolvable slot
  (shields, odd legacy NM3 types) still use the old limb-based path.

Appearance override:
- set_appearance_name("A Coalition Dead Boy") on a protective armor
  piece makes strangers see that string instead of the wearer's
  race-based room-listing name while it is worn on whole_body. Stored
  verbatim (include the article you want). Strangers-only: players who
  know the wearer still see their name. Implemented in std/armour.c
  and std/user.c query_appearance_override() (computed live from worn
  gear; no property, nothing to clean up on removal/destruction).
- Wired as proof of concept on dead_boy_armor.c and predator_armor.c.
  Sunaj Assassin armor, Armor of the Beast, and JR-15 do NOT exist yet
  and still need creation once lore/MDC values are decided.
