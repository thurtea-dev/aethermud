# lib/ cleanup manifest - 2026-07-19

Discovery and reorganization pass over `nightmare3_fluffos_v2/lib/`.
Dead/legacy content was MOVED (never deleted) to
`nightmare3_fluffos_v2/attic/`, a sibling of `lib/` that the driver
cannot reach (the driver mounts `lib/` as its root). Original
`lib/`-relative paths are preserved inside `attic/`.

Verification method used for every item: grep of all `*.c`, `*.h`,
and `*.cfg` under `lib/` for the path string and any macro that
expands to it, plus a check of the command daemon's rehash list
(`daemon/command.c`), the command-dir macros in
`secure/include/dirs.h`, `secure/cfg/preload.cfg`,
`secure/cfg/socket.cfg`, and a grep of all player save files under
`secure/save/users/` for saved locations pointing at the paths.

## 1. Moved to attic/ (zero references confirmed)

### domains/Praxis/standardOld/ (133 rooms, ~712K, 138 tracked files)
- Why: pre-Rifts NM3 fantasy rooms. docs/ASSESSMENT.md section 7
  documents them as "legacy NM3 rooms, not Rifts content, throwing
  runtime errors" (recurring Bad argument 1 to call_other() in
  reset() for pit, spider_pit, app_room, crypt) and recommends
  "unlink and archive the tree". This move is that archive step.
- Checked: zero references anywhere in lib code or config; zero
  player saves reference any standardOld path; no exit in any live
  room points into the tree. The old CLAUDE.md note that it was
  "live and erroring" predates this check; with no inbound
  references and no saved player locations, nothing can load it
  after the next full reboot, and the reset error spam ends.

### domains/Examples/ (~88K, 22 tracked files)
- Why: stock Nightmare/FluffOS example code (virtual room servers
  exaA/exaB, sample search rooms). Never adapted for AetherMUD.
- Checked: only self-references inside the directory; no reference
  from secure/daemon/master.c, std/virtual, or any config.

### cmds/soul/atmos.c (directory cmds/soul/ removed, was 1 file)
- Why: the old NM3 soul-database editor. The live emote/soul system
  is `daemon/soul.c` + `daemon/soul/*.l` (called from std/living.c);
  atmos.c is a leftover editor for the obsolete flat soul.db format.
- Checked: no DIR_ macro points at cmds/soul; not in the command
  rehash list; no file loads or clones it. (The "atmos" NPC in
  domains/Praxis/realty.c is an unrelated name collision.)

### cmds/database/soul.db
- Why: data file read ONLY by the atmos.c editor above; the live
  soul daemon compiles from daemon/soul/*.l instead.
- Checked: SOUL_DB macro (secure/include/databases.h) was referenced
  by atmos.c only. NOTE: the `#define SOUL_DB` line in databases.h
  now dangles harmlessly; left in place (see section 3).
- NOT moved from the same directory: cmds/database/where.db - it is
  live data for the mortal `where` command (WHERE_DB).

### tmp/test_load.c
- Why: untracked local scratch file, zero references.
- Note: untracked in git, so this move is local-only and will not
  propagate to the VPS via pull.

### cmds/mortal/_start.c (moved 2026-07-19, retirement decision)
- Why: mortal "start here" (self-set login start) retired by owner
  decision after three playtest false alarms. Quit and link-death
  both resume the player at their logout room now (std/user.c), so
  the command's only residual value was pinning a start different
  from the logout room. Staff keep the ability via cmds/hm/_start.c
  (untouched).
- Checked: zero references in lib *.c/*.h/*.cfg (setter.c's chargen
  "start" verb is its own add_action, unrelated); no doc/help topic
  or mention; no www/ mention; user.c set_start_here() and
  save_logout_start()/staff_area_start()/valid_start_room() are
  callee-side only and keep working for quit, link-death, and the
  hm command. docs/playtest-checklist.md had two steps invoking the
  mortal command; both rewritten the same day to test the removal
  instead.
- Cache note: daemon/command.c's rehash table lists the command
  until the daemon reloads; clean immediately after pull with
  `update /daemon/command.c`, or let the pending full reboot (for
  the same-day std/user.c change) handle it.

## 2. Flagged but NOT moved - needs your decision

### cmds/skills/ - all 66 legacy NM3 class-ability commands
- fireball, backstab, bless, demonfire, heal, turn, warshout, etc.
- ZERO of the 66 files reference RIFTS_D or any rifts daemon; they
  all check legacy NM3 class skills (query_skill("healing"),
  query_class() == "monk", mp costs) that no Rifts character has.
- NOT moved because they are REACHABLE: DIR_CLASS_CMDS
  (= /cmds/skills) is in daemon/command.c's base rehash list and in
  help.c's "*abilities" category, so every one is a live command for
  every player. Retiring them requires a code change (remove
  DIR_CLASS_CMDS from the rehash + living search path), which is a
  behavior change, not a zero-reference move.
- This is also the answer to the Wizard skill question - see
  section 4.

### std/hm/castle.c
- Orphaned (nothing inherits or clones it) BUT: it belongs to the
  half-missing high-mortal castle feature (see CASTLE_D below), and
  a "castle area" build is planned next session; leaving it until
  you decide whether the HM castle system lives or dies.

### CASTLE_D dangling daemon (live bug, report only)
- secure/daemon/advance.c line ~134 calls
  CASTLE_D->enable_high_mortal(this_player()), but CASTLE_D expands
  to /daemon/castle.c which DOES NOT EXIST. A player advancing into
  high mortal will hit a runtime error on that call. Not fixed this
  session (out of scope); fix is either restoring a castle daemon or
  removing/guarding the call.

### secure/cfg/preload.cfg stale entries
- `/adm/daemon/astronomy_d` and `/adm/daemon/race_d` point into the
  EMPTY lib/adm/ tree. The real daemons are /daemon/astronomy.c and
  /daemon/race.c (per daemons.h macros) and load on demand. The two
  stale lines fail at every boot (silently or into logs). Left
  untouched: editing preload.cfg changes boot behavior. Recommend
  deleting both lines or repointing them at /daemon/astronomy and
  /daemon/race.

### news/ legacy class news files (cleric, fighter, mage, monk,
  rogue, kataan, child)
- daemon/news.c's class_news() reads news/<class> for the player's
  legacy class. Post-conversion no player should have one, but the
  code path is live, so these data files are conditionally
  referenced. Harmless; retire when class_news() itself is retired.

### domains/Praxis/realty.c ("Atmos, the Patron of High Mortals")
- Referenced by two live Praxis rooms (unnamed1.c, s_centre4.c), so
  it stays. Flagged for theming: it is the NM3 estate agent NPC
  ("Atmos is a geek.", high-mortal estates flavor), not Rifts
  content. The estates subsystem behind it (daemon/estates.c,
  preloaded; empty lib/estates/ dir; write.cfg entries) is likewise
  live-but-NM3. Converting or retiring estates is a design decision.

### Empty stock directories (left in place)
- lib/estates/, lib/ftp/, lib/save/, lib/adm/ (+adm/tmp),
  lib/cmds/guild/, lib/cmds/race/: all empty. Each is still named by
  a live macro or config (DIR_GUILD_CMDS and DIR_RACE_CMDS in
  dirs.h/help.c/living.c search paths, estates in write.cfg and
  security.c, /adm in preload.cfg per above). Git does not track
  empty directories, so moving them would be local-only and would
  risk get_dir()/hash errors for code that expects them to exist.
- lib/tmp/bb/: bulletin-board temp dir (secure/include/bboard.h).

### secure/include/databases.h stale defines
- SOUL_DB now points at a moved file (harmless; no remaining users).
  Most other *_DB defines point at /adm/db/* which does not exist
  either (ACCESS_DB, RACES_DB, etc.) - the whole header is mostly
  NM3 residue; only WHERE_DB is used by live code (cmds/mortal/
  _where.c). Candidate for a later trim; left untouched because it
  is an include used by a live command.

### secure/include/objects.h stale define
- OB_HM_ROOM points at /std/hm/hm_room.c which does not exist; zero
  users. Harmless dangling define; noted for a later trim.

## 3. Checked and confirmed ACTIVE (not candidates, do not move)

- domains/newcamelot, tolkeen, Lazlo: live Rifts zones, reachable
  via cmds/mortal/_rift.c, daemon/rifts_start_d.c, and the NGR gate.
- domains/demon_plane: reachable from domains/Praxis/areas/
  nexus_dimensional_tear.c (by design, "placeholder sliver").
- domains/wizards/ (flat, no subdirs locally): live staff area -
  hallway, lounge, coding workshop, admin chamber, training dummies,
  vendors. The legacy per-wizard dir /domains/wizards/thurtea/
  exists ONLY on the VPS (untracked there); retiring it is a manual
  VPS-side step you already have on your list from the 2026-07-18/19
  workroom fix.
- lib/www/: the in-mud HTTP server content - daemon/http.c is wired
  via secure/cfg/socket.cfg. Distinct from the repo-root www/ staff
  guides. Active infrastructure.
- cmds/qcs/: DIR_QCS_CMDS is in std/living.c's search path logic,
  help.c, and warmboot. Active.
- cmds/database/where.db, news/, tmp/bb/, obj/ (credcard.c and
  credit_chip.c are used by rifts_start_d.c and std/user.c;
  obj/items and obj/tattoo_weapons are live).
- realms/splynncryth/, realms/thurtea/: live wizard sandboxes
  (gitignored by design except thurtea/workroom.c); not touched.

## 4. Wizard / head-arch "You do not have the skill to do that"

Verdict: NOT a skill-grant scoping limitation and NOT a privilege
bug - it is the unconverted legacy command layer.

That exact string occurs in exactly one file: cmds/skills/_heal.c
(the NM3 class heal ability). Its gate is:

    healing = query_skill("healing");   /* NM3 class skill */
    ...
    if(healing < 5) -> "You do not have the skill to do that."

query_skill() (std/living/skills.c) reads the same skills mapping
the Rifts system uses, but no Rifts race/OCC package ever grants a
skill literally named "healing" - that is an NM3 class skill. So
every Rifts character, from a fresh vagabond to a level-15 Wizard
head arch, gets healing == 0 and the refusal. Creator/arch status is
irrelevant: the check is a skill-points check with no creatorp()
bypass, and being a Wizard grants racial abilities, not NM3 class
skills. All 66 commands in cmds/skills/ share this pattern (zero are
Rifts-aware), so any of them will refuse everyone the same way.

Fix options (for a later session, your pick): retire DIR_CLASS_CMDS
from the command rehash entirely (my recommendation - the Rifts
ability layer already lives in cast/psi/racial commands), or convert
the few worth keeping (heal, sneak, lockpick have Rifts analogues)
to rifts_skills checks.

## 5. Deployment notes

- All moved items except tmp/test_load.c were git-tracked; after you
  commit, the VPS pull applies the same moves there. The attic/ new
  files (README, breadcrumb) and this manifest are new tracked files.
- No live object holds a reference into the moved paths, so nothing
  breaks at runtime on either box when the moves land. The
  standardOld reset-error spam on the VPS stops at the next full
  reboot after the pull (already-loaded blueprints persist until
  then).
- Empty directories vacated by the moves (lib/cmds/soul) were
  removed locally; git handles this automatically on pull since it
  tracks only files.
