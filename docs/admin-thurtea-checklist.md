# Thurtea Admin/Wizard Self-Verification Checklist

Purpose: self-check items for Thurtea's own admin/wizard character only
(staff commands, admin tools, wiz-tools chest, staff areas, staff-only
visibility, reboot/update procedures). This is separate from
`docs/playtest-checklist-players.md` and `docs/playtest-checklist-wizards.md`,
which cover general playtesting content -- do not merge this file into
either of those.

Built from a codebase sweep on 2026-07-20. Items are what currently
exists on disk; check them off as you personally confirm each one
in-game. Per project policy, this file was generated without any live
testing -- everything below is a claim to verify, not a confirmed result.

## 1. Identity & Privilege Baseline

- [ ] `secure/cfg/groups.cfg` lists `thurtea` under `(SECURE)` and
      `(ASSIST)`. `(AMBASSADOR)` is currently empty.
- [ ] `who` shows your own line tagged `[Admin-Wiz]`
      (`cmds/mortal/_who.c` `get_role_tag()`: `wiz_role` env var
      `"admin"` maps to this exact tag).
- [ ] `score` reflects arch-level stats consistent with your level 15
      godling account (per prior session verification, 2026-07-15).

## 2. Staff Movement & Areas

- [ ] `wizz` (`secure/cmds/creator/_wizz.c`, creator-gated) teleports
      you to `/domains/wizards/lounge` from anywhere. File confirmed
      present on disk.
- [ ] `workroom` (`cmds/mortal/_workroom.c`, creator-gated) teleports
      to your personal workroom. Resolution order: `home_room`
      property, then `/realms/<name>/workroom`, then
      `/domains/wizards/<name>/workroom`. Yours resolves to
      `/realms/thurtea/workroom.c` (confirmed present on disk).
- [ ] **Reminder**: `/realms/` is gitignored (see prior session memory
      "Realms gitignore split-brain") -- your workroom never deploys
      via git, and the VPS copy can diverge from local. Don't assume a
      local workroom edit is live on the VPS, or vice versa.
- [ ] From the workroom, exit `wing` reaches
      `domains/wizards/thurtea_test/test_hall.c` (staff-only test
      wing, per that directory's own README). Confirm both
      `test_hall.c` and `test_arena.c` eject non-creator players and
      carry `no teleport`.
- [ ] Confirm `/domains/wizards/` is blocked as a mortal login start
      room (`staff_area_start()` in `std/user.c`) -- a fresh mortal
      character should never land here by accident.

## 3. Staff Test Wing Contents (domains/wizards/thurtea_test/)

- [ ] `test_hall.c` spawns Renn (quartermaster vendor) and Sela
      (archivist, for dialogue-system testing).
- [ ] `test_arena.c` spawns `sparring_drone.c` (legacy NM3 hp-pool
      target) and `rift_crawler.c` (30 MDC Rifts target) for
      damage-model testing.
- [ ] `quartermaster.c` sells one sample item per wear slot at token
      prices: weapon (C-12 rifle), whole-body protective armor (Dead
      Boy, appearance-override demo), cybernetic implant (optic eye),
      and one cosmetic item per slot (cap/head, amulet/neck, shirt,
      backpack/back, belt, trousers/legs, gloves/hands, boots/feet,
      ring).
- [ ] **KNOWN BROKEN as of this session (2026-07-20)**: 8 of the 9
      cosmetic wear-slot samples (`field_cap.c`, `faction_amulet.c`,
      `canvas_shirt.c`, `utility_belt.c`, `canvas_trousers.c`,
      `work_gloves.c`, `travel_boots.c`, `signet_ring.c`) call
      `set_armour_type()`, a function that does not exist anywhere in
      the current codebase (`std/armour.c` only defines
      `set_type()`/`query_type()`). Only `backpack.c` (back slot) is
      unaffected. The cosmetic-slot self-test kit cannot currently
      verify the wear-slot system end-to-end. This was found during
      this session's Task 1 gap sweep and is not fixed yet (discovery
      only, pending approval).

## 4. Wiz-Tools Chest (domains/adm/wiz_tools/staff_supplies_chest.c)

Auto-restocks on `reset()`; should always contain:

- [ ] 4 role handbooks (`wiz_reference_book.c` with `book_role` set):
      admin, domain, coding, rp.
- [ ] `staff_of_demotion.c`, `staff_of_dominion.c`,
      `staff_of_creation.c`, `rp_skill_tool.c`, `tattoo_gun.c`,
      `skill_slip.c`.
- [ ] Present in `wiz_tools/` but **not** stocked in the chest --
      confirm you know where these live if you need them:
      `apprentice_kit.c`, `mailbox.c` / `mailbox_welcome.c`,
      `ring_of_dominion.c`, `tanstaafl_base.c`. (See section 10 --
      unclear whether these should be chest-stocked too.)

## 5. Wizard Roles (cmds/adm/_setrole.c)

Four roles, each maps to a position string and title:

- [ ] admin -> position `arch`, title "Administrator $N"
- [ ] domain -> position `domain_wiz`, title "Domain Wizard $N"
- [ ] coding -> position `creator`, title "Coding Wizard $N"
- [ ] rp -> position `rp_wiz`, title "Roleplay Wizard $N"
- [ ] Confirm your own account carries the `admin`/`arch` role, and
      that `setrole` itself refuses non-privileged callers ("You don't
      have permission to set wizard roles.").

## 6. Character/World Admin Commands (cmds/adm/)

Arch-gated (`archp()` check found in source) -- highest tier, use with
care:

- [ ] `_force.c`, `_makewiz.c`, `_playerwipe.c`, `_setfaction.c`,
      `_warmboot.c`

Creator-gated, lower tier:

- [ ] `_advance.c`, `_archshout.c`, `_bench.c`, `_broadcast.c`,
      `_cache.c`, `_channel.c`, `_commands.c`, `_cref.c`,
      `_dumpallobj.c`, `_end.c`, `_fdinfo.c`, `_fixdemote.c`,
      `_give.c`, `_goto.c`, `_grantrace.c`, `_grantskills.c`,
      `_haven.c`, `_makechar.c`, `_monitor.c`, `_opcprof.c`,
      `_repairchar.c`, `_setocc.c`, `_setrcc.c`, `_setrole.c`,
      `_setskill.c`, `_sever.c`, `_stat.c`, `_trans.c`

Specific items worth a closer self-check:

- [ ] `grantrace <player>` / `grantskills <player>` -- confirm these
      call the same `RIFTS_START_D->grant_race_package()` /
      `grant_occ_skills()` "backfill tool" wrappers documented in
      `daemon/rifts_start_d.c` (public entry points explicitly built
      for this purpose). Good candidates to test on a throwaway
      character with a known race/OCC.
- [ ] `_sever.c` (sever/restore/list) -- narrative-only limb severing
      per the whole-body damage model (see CLAUDE.md, "Armor, Slots,
      and the Whole-Body Damage Model"). Confirm `whole_body`,
      `torso`, and FATAL limbs (head) correctly refuse severing.
- [ ] `_playerwipe.c` -- confirm this remains the only sanctioned way
      to delete a save file (per CLAUDE.md security notes: never
      delete saves directly).

## 7. Filesystem/Coding Admin Commands (secure/cmds/creator/)

- [ ] `_update.c` -- room-safe update (moves players to the void and
      back around a room reload; fixed 2026-07-13).
- [ ] `_clone.c`, `_move.c`, `_mv.c`, `_rm.c`, `_rmdir.c`, `_mkdir.c`
      -- filesystem manipulation, arch/creator gated per file.
- [ ] `_eval.c`, `_call.c` -- arch-gated; `_call.c` specifically
      blocks `query_email` unless the caller is `archp()` (fixed
      2026-07-17, closed an email-leak path any creator-level+ account
      could previously use).
- [ ] `_grant.c` -- grants domain read/write access to other creators.
      Note: this file logged a stale compile-time WARNING ("!= always
      true, int vs string" at line 27) as of the last recorded error
      log (2026-07-12); not independently confirmed broken this
      session, low priority, worth a look if `grant` ever behaves
      oddly.
- [ ] `_snoop.c`, `_dbxwhere.c`, `_dbxframe.c`, `_locate.c` --
      debugging tools.
- [ ] `_du.c`, `_dref.c`, `_eref.c`, `_ref.c`, `_refs.c`, `_tref.c` --
      reference-counting/disk-usage tools (Nightmare legacy).
- [ ] `_domains.c`, `_realms.c` -- domain/realm listing.
- [ ] `_wizz.c` -- teleport to the Wizard's Lounge (see section 2).
- [ ] `_sponsor.c`, `_changelog.c`, `_log.c`, `_imcp.c` -- misc.

## 8. Staff-Only Visibility (Introduction System)

The introduction system (`cmds/mortal/_introduce.c`) makes two players
learn each other's real name via `add_to_face_list()` /
`knows_player()`. Until introduced, `std/user.c query_display_name()`
shows a generic race-based label ("A stranger", "A human", etc.) or an
armor `appearance_name` override (e.g. Dead Boy armor) instead of the
real name.

- [ ] Confirm `creatorp(viewer)` bypasses this entirely:
      `query_display_name()` (`std/user.c` around line 2107) and
      `query_apparent_race()` (`std/user.c` around line 2082) both
      return the real name/race immediately for any creator-level
      viewer, regardless of introduction status or worn
      appearance-override gear. **This is intentional design, not a
      bug** -- confirmed explicitly this session.
- [ ] Self-check: as thurtea, look at an un-introduced test character
      wearing Dead Boy armor (appearance override) and confirm you see
      their real race/name, not "A Coalition Dead Boy".
- [ ] If you keep a non-creator alt for testing, confirm it sees the
      obfuscated version in the same scenario.

## 9. Reboot / Update Procedures (admin-specific reminders)

Full rules live in CLAUDE.md ("UPDATE VS FULL REBOOT", "Warmboot vs
full reboot") -- this section is a self-check pointer, not a
restatement:

- [ ] You know the difference between `update <path>` (single file,
      safe for daemons/cmds/one room), `warmboot`
      (`cmds/adm/_warmboot.c`, arch-gated bulk daemon/cmd/domain
      reload, no driver restart), and `mud.sh stop && mud.sh start`
      (the only way to reload `/std/` and `/secure/cmds/` changes for
      already-connected players and already-loaded objects).
- [ ] You never run `kill` directly -- always `mud.sh`.
- [ ] After any `/std/` or `/secure/cmds/` change, you personally
      relog (not just warmboot) before testing, since your own
      connected session keeps old inherited code otherwise.

## 10. Uncertain / Flag for Review

Items that plausibly belong on an admin-only checklist but I'm not
certain of scope or current correctness -- flagged rather than guessed:

- [ ] `apprentice_kit.c`, `mailbox.c` / `mailbox_welcome.c`,
      `ring_of_dominion.c`, `tanstaafl_base.c` (in `wiz_tools/`, not
      chest-stocked) -- unclear if these are meant to be handed out by
      an arch to specific roles, or are legacy/in-progress. Worth a
      decision on whether they belong in the chest too.
- [ ] `_grant.c`'s stale compile warning (section 7) -- probably
      harmless, not independently verified this session.
- [ ] `help staff` (`doc/help/user/staff`) and `help wiz staff`
      (`doc/help/wiz/staff`) both exist on disk -- confirmed present,
      but I did not check their content is current with the roles/tools
      described above. Worth a read-through.
- [ ] Ambassador tier (`(AMBASSADOR)` group in `groups.cfg`, currently
      empty) -- not populated, so not covered above. Add a section
      here if that ever changes.
