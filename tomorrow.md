# tomorrow.md - session handoff (written overnight 2026-07-08)

You asked for a summary of what we worked on, where things stand, and next
steps. Everything below happened in the 2026-07-08 session (started from the
"where are we" status check).

## TL;DR

Section 8 (live verification sweep) is CLOSED: the final scripted run
finished **44 PASS / 0 FAIL**. Doing it properly surfaced and fixed EIGHT
real bugs, several of them gameplay-blocking. The MUD is running on port
1122 with all fixes live and test characters cleaned up. The end-to-end
suite covering chargen, skills-category aliasing, bank, combat, psi-sword,
and two-player radio is `tools/section8_verify4.py` - rerun it any time.
One cosmetic fix for your "a staff of creation" report is also in.

## Bugs found and fixed this session (all live after reboot)

1. `daemon/rifts.c` - `lower_case(0)` crash (the `score` error in your last
   playtest, `is_smuggler_occ` at line 1835). Any character without an OCC
   (mid-chargen, wizards) crashed `score`. Added safe helpers
   `query_occ_string()`/`query_race_string()` and routed all 11 unsafe call
   sites through them. Lesson re-learned: FluffOS 2.9 requires prototypes
   before use; added them at the top of the file.

2. `domains/Praxis/setter.c` - the chargen guide NPC tells new players to
   type `region americas`, but `region_cmd()` was a stub that always
   rejected with "Type: start americas...". New players following the guide
   literally could not proceed. `region <zone>` now delegates to
   `start_cmd()` so both forms work.

3. `domains/Praxis/obj/bounty_board.c` - `set_prevent_get(1)` (int arg)
   threw "Bad argument 1 to set_prevent_get()" during `create()`. Because
   Krasna square's `reset()` clones the board, **entering Krasna square
   failed entirely** ("You go nowhere at all") - the bank/post/shops route
   was unreachable. Now passes a proper string.

4. `std/living/body.c` - `pick reroll` at the OCC step crashed with
   `*Division by zero` for EVERY player. Cause: `user.c` gives players a
   `whole_body` pseudo-limb that is in no RACE_D limb table, so
   `query_max_dam()` returns 0 and `augment_body()` divides by it. The
   initial roll never crashed only because race is unset at step 2.
   `augment_body()` now preserves `whole_body` semantics (max_hp+1) and
   clamps missing-limb divisors to the unknown-race default (2). Also
   guarded `return_target_limb()` against an empty limb array.

5. `std/monster.c` - many Rifts-era monsters (coalition_grunt, the alvurron
   set, brodkil, catacomb_wight, deevil_scout, atlantean_guardian, ...)
   never call `set_body_type()`, so they had EMPTY limb arrays and any
   combat round against them crashed (`Array index out of bounds` in
   `return_target_limb`). `create()` now defaults every monster to a human
   body; a later `set_body_type()` rebuilds it. (My first version of this
   fix broke /std/monster compilation - missing prototype, FluffOS 2.9
   again - which for one boot meant NO NPCs loaded anywhere and chargen
   couldn't teleport players out of the void. Fixed and verified; the void
   Gamemaster is back and chargen completes.)

6. `cmds/mortal/_inventory.c` - your mudlet report: "You are carrying: a
   staff of creation." should read "A staff of creation." Root cause: item
   shorts are stored lowercase by lib convention ("a combat knife"), the
   room display capitalizes each line, but the inventory command printed
   shorts verbatim. Inventory now capitalizes each line, so ALL items match
   room-display style. (The wiz_tools shorts themselves were left lowercase
   on purpose - that IS the lib convention; the display layer was the bug.)

7. `domains/Praxis/bank.c` - `deposit 50` was treated as 50 BLACK credits
   ("Insufficient black credits on hand"). FluffOS sscanf counts `%d` as
   matched even when a trailing literal fails, so `"%d black"` also
   matched plain "50". Parser rewritten to capture the trailing word with
   `%s` and compare it explicitly. Verified live: balance 0 -> deposit 50
   -> 50 -> withdraw 10 -> 40 -> close account returns the credits.

8. `cmds/mortal/_exchange.c` - same sscanf trap, worse effect: `exchange
   50 black to credits` matched the `"%d credits to black"` pattern first
   and would convert in the WRONG DIRECTION. Same %s-capture fix.
   (FYI: `obj/misc/donation.c` has the same pattern with "%d gold", but
   gold is the only currency there so behavior is unchanged; it is also a
   legacy-reskin candidate anyway.)

## Verification results (scripted, tools/section8_verify4.py)

Confirmed working end to end on the live game:
- Full chargen (city rat, rogue scholar, titan cyber-knight) via real
  telnet sessions, including the 5-roll reroll flow at the OCC step.
- Mid-chargen `score` with no OCC (regression for bug 1) - clean.
- Skills category alias fix (`daemon/rifts_skills.c`, was pending from last
  session, went live with the reboots): `skills technical` lists
  archaeology/astronomy/botany/anthropology/geology for a Science OCC;
  `skills espionage` lists seduction/escape artist/trap detection/
  cardsharp/palming for a Rogue OCC. CONFIRMED both.
- Radio between two simultaneous connections: both tuned to channel 5,
  message received as `[Radio 5] A human: hello ...`. CONFIRMED.
- autododge/autoparry toggles with APM mechanics in the response. CONFIRMED.
- Psi-sword activation as a Cyber-Knight (10 ISP, 2d6 MDC). CONFIRMED.
- Bank: full flow - `read sign`, `show id` -> ATM card, balance 0,
  deposit 50 -> 50, withdraw 10 -> 40, close account. CONFIRMED.
- Real combat: Titan Cyber-Knight vs the SDC coalition grunt in the test
  zone (one move north of the Rift arrival room) - hits landed both ways,
  autododge visibly fired ("You dodge Coalition grunt's attack!"), grunt
  killed. CONFIRMED.

Final full-suite result: **44 PASS / 0 FAIL** (with a clean runtime log
check built into the suite). Re-run any time with the MUD up:
`python3 tools/section8_verify4.py` (creates aethverifya/b/c via real
telnet chargen, cleans up its saves afterward).

## Notes / observations for you

- **Cyber-Knight is nearly unrollable as a human.** Reqs are PS 12, PE 12,
  MA 10, ME 12 on straight 3d6 with 5 total rolls - under 5% odds. The test
  had to use a Titan (attribute dice bonuses) to qualify reliably. If you
  want human Cyber-Knights to be viable, consider relaxing the reqs or
  granting a bonus die. Your call - flagging, not changing it.
- The user-account thurtea was online during one of the reboots tonight
  (your `home` command actually surfaced the /std/monster break - thanks).
  Your workroom is fine; it was the monster inherit that failed.
- Test chars (aethverifya/b/c, aethprobe) are cleaned from
  secure/save/users/a/. thurtea and splynncryth untouched.

## Where we are overall (report.md section numbers)

- Section 8 (live verification): effectively DONE except two manual items
  below. All scripted items pass.
- Section 9 (parity extras): unchanged - player-run shops + website parity
  (help browser, who page) still open.
- Section 10 (modern driver port): unchanged, paused in archive/.
- playtest-checklist.md: the 8 race test characters and the 3 wizard
  promotions (dacon/mcguyver/sola) have NOT been created yet - those need
  you online as admin. Note: the `update /daemon/rifts_skills` step at the
  top of that checklist is no longer needed (fix went live via reboot).

## Next steps (suggested order)

1. Skim the final run log (see above) - if any FAIL lines remain, they are
   the first thing to look at. Then tick the remaining Section 8 boxes in
   report.md and note the eight fixes in master_gap_report.txt.
2. Your manual playtest pass (playtest-checklist.md): 8 race characters,
   3 wizard promotions, and eyeball the fixed flows in person - especially
   `pick reroll` during chargen and combat vs the test-zone grunt
   (north of the Rift arrival room).
3. Decide on the Cyber-Knight attribute-requirement question above.
4. Section 9 remainder (shops + website parity) - prompt block is in
   report.md.
5. Long tail: Section 10 driver port (archive/, no rush).

Good night. The server is up, clean, and all eight fixes are live.

---

# Addendum - evening session, 2026-07-08

Everything below happened after the overnight handoff above, in the
evening playtest session.

## Project relocated and renamed on disk

- Picked the canonical copy: the live project (formerly `/home/thurtea/am`)
  is the most complete by far. The `LPC-Resources` candidates were either
  empty (`Rifts_LPC_Final_*`), flattened file piles (`Simple`), raw backup
  collections (`Clean_Core`, `Core`), or July 5 snapshots that predate all
  of this week's fixes (`Ultimate_*`).
- Renamed `/home/thurtea/am` to `/home/thurtea/aethermud`. Updated the
  hardcoded paths in `nightmare3_fluffos_v2/bin/mudos.cfg`,
  `archive/bin-modern/fluffos.cfg`, and all docs. Verified with a full
  clean boot from the new path.
- Moved `LPC-Resources` (all the old candidate dirs) to
  `/home/thurtea/lpc-archive`. Nothing in it is needed to run the game.
- Migration tarball for the new Fedora Workstation 44 box:
  `/home/thurtea/aethermud-20260708.tar.gz` (root dir `aethermud/`).
  Remember to rebuild the driver from `fluffos-2.9-ds2.08/` source on the
  new machine; the binary in `bin/` was built for WSL.

## Bug 9: bare spell-name casting never worked (your windrush report)

Typing `windrush` (or any spell name) as a command returned "What?".
Two real defects, both fixed and live:

1. `daemon/chat.c` - `do_chat()` returned 0 at the `channel_allowed()`
   gate before ever reaching the `try_spell_shortcut()` fall-through.
   Since `channel_allowed()` also fails for all non-creators, no mortal
   could EVER trigger the spell shortcut. The shortcut attempt now runs
   when the channel gate rejects the verb.
2. `cmds/mortal/_cast.c` - the shortcut only parsed `windrush at grunt`.
   It now also accepts the classic RiftsMUD forms: bare `windrush`,
   `windrush grunt`, and multi-word names longest-first
   (`energy bolt grunt`). Also: `cast <spell> at me` (or `self`, or your
   own name) now self-targets; `present()` does not resolve the caster.

Caveat found while testing: a spell whose first word is a real command
(`heal wounds` vs the psionic `heal` command) is intercepted by that
command first. That is correct precedence, not a bug; use `cast heal
wounds` for the spell.

Verified end to end with a new scripted suite,
`tools/spell_shortcut_verify.py` (telnet chargen of a human Mystic, who
starts with windrush): **13 PASS / 0 FAIL** covering bare cast, bare
multi-word cast, classic `windrush grunt` against the test-zone grunt,
`cast ... at me` self-heal, the original `cast` syntax, and the "What?"
fall-through for gibberish. Runtime log clean, test saves removed.
