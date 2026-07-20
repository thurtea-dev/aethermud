# Playtest checklist - 2026-07-19 (bug-fix and polish pass)

Step-by-step live tests for everything fixed or built in the recent
sessions: chargen accept gate, reconnect placement, staff-area start
guard, Moxim rift destinations, the whole-body damage model, and the
new test wing. Work through it in order; each numbered step is one
in-game action. Check the box when the observed result matches the
expected result. Anything that does not match goes to
`master_gap_report.txt`.

This replaces the 2026-07-10 checklist (stale since the 2026-07-12
player wipe; its still-pending zone walk-throughs survive in section
8). The old `pt*` roster with expected score values is still in
[playtest-roster.md](playtest-roster.md); recreate any of those
characters quickly with `makechar` (section 6).

Prerequisites: local server running (`./mud.sh status`, port 1122),
one admin login (thurtea), one throwaway mortal (create in section 1,
reuse in sections 2, 3, and 5). A FULL REBOOT must have happened
after pulling these changes (std/user.c changed); `update` is not
enough.

---

## 1. Chargen full walkthrough (new account)

Create a fresh throwaway account (any name, normal registration).

1. [ ] At the zone prompt, type `look` first. Expected: normal
       command works; the prompt is not consumed (news-pager fix).
2. [ ] Type `americas`. Expected: "The Americas. You will begin on
       the outskirts of Praxis." and the STEP 2 attribute banner.
3. [ ] Type `human` (out of order). Expected: refused; you are told
       to roll, not silently advanced.
4. [ ] Type `roll`. Expected: an 8-attribute block (IQ ME MA PS PP
       PE PB Spd) plus "Type accept ... or reroll (4 of 4 rerolls
       remaining)."
5. [ ] Write down two attribute values. Type `reroll`. Expected: a
       NEW block; at least some values differ from what you wrote
       down (fresh dice, not a reprint), and rerolls remaining drops
       to 3.
6. [ ] Type `human` again, before accepting. Expected: still
       blocked; "Accept your attributes first."
7. [ ] Type `accept`. Expected: "Attributes accepted", final block
       reprinted, race list shown.
8. [ ] Type `reroll` after accepting. Expected: refused (the roll is
       locked once accepted).
9. [ ] Type `human`. Expected: race description, reshaped
       attributes, then the STEP 4 alignment menu.
10. [ ] Type `scrupulous`. Expected: "Alignment chosen: Scrupulous",
        then the OCC list (STEP 5).
11. [ ] Type `list`. Expected: available OCCs listed. At NO point in
        this whole flow should any elective or secondary skill menu
        appear.
12. [ ] Type `vagabond`. Expected: OCC applied, starting package
        granted, rift-arrival text, you land outside Praxis, score
        sheet prints, and only then does unread news appear.
13. [ ] Type `score` and `skills`. Expected: OCC package skills are
        present without you ever picking one.

## 2. Reconnect and login-start tests (mortal + admin)

Use the section 1 throwaway (call it TESTCHAR below).

1. [ ] As TESTCHAR, walk a few rooms away from the start room (any
       ordinary Praxis room).
2. [ ] Kill the client connection (close the window; do NOT type
       quit).
3. [ ] Reconnect and log back in. Expected: you resume in the SAME
       room, and anyone watching sees "TESTCHAR has rejoined our
       reality."
4. [ ] Repeat once more but type `quit` instead, then log in.
       Expected: you also return to the room you quit in (quit saves
       the logout room).
5. [ ] As thurtea, poison the mortal's start:
       `eval find_player("TESTCHAR")->set_primary_start("/realms/thurtea/workroom")`
6. [ ] Have TESTCHAR quit and log back in. Expected: they do NOT
       appear in the workroom; they land at Praxis square and their
       saved start is repaired (verify:
       `eval find_player("TESTCHAR")->query_primary_start()` shows
       the square, not the workroom).
7. [ ] As TESTCHAR, type `start here` in an ordinary room. Expected:
       the command no longer exists for mortals (removed 2026-07-19;
       an ordinary unknown-command response, no syntax help). As
       thurtea, `start here` still works (staff command).

## 3. Moxim rift destinations

As any character with credits (admins travel free). From anywhere,
`rift <destination>` opens a portal; `enter rift` travels. After each
arrival, confirm a live Moxim is standing in the arrival room, then
use him (or the global command) for the next hop.

1. [ ] `rift americas` -> Chi-Town boulevard. Moxim present: yes/no.
2. [ ] `rift atlantis` -> Splynn market. Moxim present: yes/no.
3. [ ] `rift europe` -> New Camelot square. Moxim present: yes/no.
4. [ ] `rift lazlo` -> Lazlo market. Moxim present: yes/no.
5. [ ] `rift tolkeen` -> Tolkeen square. Moxim present: yes/no.
6. [ ] `rift praxis` -> Monument Square. Moxim present: yes/no.
7. [ ] In one arrival room, `say destinations` near Moxim. Expected:
       his destination table matches the global `rift` list.

Note: Moxim clones spawned before the last full reboot carry the old
destination table. If any hop lands in a gate/welcome room instead
of the rooms above, the server has not been rebooted since the pull.

## 4. Test wing: wear slots, damage model, spells, sever

As thurtea, from your workroom type `wing`.

Setup:
1. [ ] In the hall, type `list` at Renn. Expected: 12 entries - one
       weapon, one protective armor, one implant, and nine cosmetic
       slot samples.
2. [ ] Buy everything: `buy rifle`, `buy dead boy`, `buy optic eye`,
       `buy cap`, `buy amulet`, `buy shirt`, `buy backpack`,
       `buy belt`, `buy trousers`, `buy gloves`, `buy boots`,
       `buy ring`, and a second `buy ring`.

Cosmetic slots:
3. [ ] Wear each cosmetic piece. Expected: all nine slots fill
       (head, neck, shirt, back, belt, legs, hands, feet, ring), no
       limb messages, `eq` shows each in its slot.
4. [ ] Wear the second signet ring. Expected: it goes to ring2. A
       third ring (buy again) is refused: both ring slots full.
5. [ ] Buy and wear a second cap while one is worn. Expected:
       refused; one item per slot.

Protective armor and appearance override:
6. [ ] `wear armor` (Dead Boy). Expected: it wears on the whole
       body alongside the cosmetic pieces (they do not conflict).
7. [ ] Bring TESTCHAR (mortal) somewhere they can see you (a public
       room; the wing will eject them). Expected: TESTCHAR sees "A
       Coalition Dead Boy" in the room listing, not your name;
       anyone who knows you still sees your name.

Damage order (arena, `arena` from the hall):
8. [ ] Attack the sparring drone bare-handed or with any weapon.
       Expected: it takes damage normally (legacy hp target) and
       dies; it respawns on the next room reset.
9. [ ] Attack the rift crawler with a NON-mega-damage attack.
       Expected: its mega-damage hide shrugs it off.
10. [ ] Attack the rift crawler with the C-12 rifle. Expected: its
        MDC (30) depletes and it dies.
11. [ ] Wearing the Dead Boy armor, let a target hit you (or have a
        second staff character hit you). Expected: the armor's MDC
        pool depletes FIRST; your own MDC/SDC/HP untouched until the
        armor pool empties.
12. [ ] `cast armor of ithan` on yourself, then take hits. Expected:
        the Ithan barrier absorbs BEFORE the worn armor pool; your
        body pools move last. Watch the order: barrier, armor, body.

Sever/restore (narrative severing):
13. [ ] `sever list TESTCHAR-or-self` to see valid limbs, then
        `sever <limb>` on a target limb (not head, torso, or whole
        body - those must be refused; try `sever head` and expect
        refusal).
14. [ ] `look` at the victim, and have them type `body` and `limbs`.
        Expected: the limb shows as severed in all three outputs,
        with no damage or stat change (narrative only).
15. [ ] `restore <limb>`. Expected: limb back, outputs clean.

Mortal gating:
16. [ ] Have TESTCHAR try to enter the wing (walk them to your
        workroom and through `wing`, or `trans` them into the hall).
        Expected: the containment ward ejects them to Praxis square
        in both cases.
17. [ ] As TESTCHAR, quit while inside the wing (if they can even
        stand there long enough before the ward ejects them).
        Expected: on next login they resume at their previous valid
        start, never inside the wing.

## 5. repairchar and makechar reference

Syntax (both admin-only, target must be online):

```
repairchar <player>                  dump chargen/stat state
repairchar <player> clear            clear stuck chargen flags
repairchar <player> finish           force creation_step=done
repairchar <player> reroll           re-roll racial attributes/pools
repairchar <player> skills           re-grant OCC skill package
repairchar <player> setstat <A> <n>  set one attribute (IQ ME MA PS PP PE PB Spd)

makechar <player> <race> <occ|none> <alignment>
         [zone=<name|/path>] [gear=default|none]
         [skills=a;b] [spells=a;b] [psi=a;b]
```

Copy-paste tests (TESTCHAR online):

1. [ ] `repairchar TESTCHAR` - expected: full state dump, all
       attributes non-zero, creation_step done.
2. [ ] `repairchar TESTCHAR setstat PS 20` - expected: confirmation;
       `repairchar TESTCHAR` shows PS 20.
3. [ ] `makechar TESTCHAR human vagabond scrupulous zone=americas` -
       expected: full rebuild summary, TESTCHAR moved to the Praxis
       welcome area with gear, and a rebuild notice shown to them.
4. [ ] `makechar TESTCHAR titan none anarchist gear=none` -
       expected: RCC-only rebuild, no equipment granted, zone
       unchanged.

## 6. Tone/style spot-check (strings added this week)

Read each in-game and confirm: professional tone, no slang, no long
dashes, American spelling.

1. [ ] Chargen STEP 1-5 banners and the accept/reroll prompts
       (setter room).
2. [ ] "TESTCHAR has rejoined our reality." (reconnect, section 2).
3. [ ] The containment ward ejection line (test wing, section 4).
4. [ ] Renn's and Sela's response lines (test wing hall; ask Sela
       about logs, procedure, damage, armor, spells).
5. [ ] The test wing room descriptions and all sample item
       descriptions (each tag reads SAMPLE: <SLOT>).
6. [ ] Moxim's flavor lines on each rift (section 3).

## 7. After the pass

- Bugs and mismatches: append to
  `/domains/Praxis/adm/master_gap_report.txt`.
- When everything above passes locally, commit, pull on the VPS, and
  FULL-reboot the VPS (std/user.c is in this change set).

## 8. Backlog: zone eyeball walk-throughs (carried from 2026-07-10)

Still worth one pass each when convenient; details in git history of
this file (2026-07-10 version):

- [ ] Chi-Town: arcade shops, ISS checkpoint, maintenance hatch,
      sewer hydra, D-bee hostility without fake ID.
- [ ] Tolkeen: gate, square, market, barracks, mage quarter, crypt
      (ward golem, shade, vault charm), siege edge.
- [ ] Horton: hovertrain, Main Street, outskirts spokes, quarry
      lurker, hermit cabin, forest ley glade and wolf den.
- [ ] Staff systems: makewiz/apprentice kit/setrole flow, mentor
      bond persistence, permission boundaries (old section 8).
