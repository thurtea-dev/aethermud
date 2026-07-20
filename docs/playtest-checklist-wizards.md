# Playtest checklist: WIZARD round - 2026-07-19

Every test in this file needs staff rank (creatorp or archp) or acts
on staff-only content. Run as thurtea (or a throwaway wizard account)
with one throwaway MORTAL online for the gating and stranger checks;
call it TESTCHAR below. Player-side tests live in
[playtest-checklist-players.md](playtest-checklist-players.md).

Prerequisites: server running, FULL REBOOT since the last pull of any
`/std/` file (user.c, living.c, room.c, armour.c, combat.c, and
everything else under `/std/`) — `update` or `warmboot` is not enough;
see section 3 for what happens when this gets skipped. Mismatches go to
`/domains/Praxis/adm/master_gap_report.txt`.

## 1. repairchar and makechar (admin-only, target online)

Syntax:

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

## 2. Test wing (from your workroom, exit `wing`)

Setup:
1. [ ] In the hall, `list` at Renn. Expected: 13 entries - one
       weapon, one protective armor, one implant, nine cosmetic
       slot samples, and a rebreather mask.
2. [ ] Buy everything: `buy rifle`, `buy dead boy`, `buy optic eye`,
       `buy cap`, `buy amulet`, `buy shirt`, `buy backpack`,
       `buy belt`, `buy trousers`, `buy gloves`, `buy boots`,
       `buy ring`, and a second `buy ring`.

Cosmetic slots:
3. [ ] Wear each cosmetic piece. Expected: all nine slots fill
       (head, neck, shirt, back, belt, legs, hands, feet, ring), no
       limb messages, `eq` shows each in its slot.
4. [ ] Wear the second signet ring. Expected: ring2. A third ring is
       refused: both ring slots full.
5. [ ] Buy and wear a second cap while one is worn. Expected:
       refused; one item per slot.

Protective armor and appearance override:
6. [ ] `wear armor` (Dead Boy). Expected: whole-body wear alongside
       the cosmetic pieces (no conflict).
7. [ ] With TESTCHAR (a stranger) able to see you in a public room:
       they see "A Coalition Dead Boy" in the listing, not your
       name; anyone who knows you still sees your name.

Damage order (arena, `arena` from the hall):
8. [ ] Attack the sparring drone (legacy hp target) with anything.
       Expected: normal damage, dies, respawns on next reset.
9. [ ] Attack the rift crawler with a NON-mega-damage attack.
       Expected: shrugged off entirely.
10. [ ] Attack the rift crawler with the C-12 rifle. Expected: its
        30 MDC depletes and it dies.
11. [ ] Wearing the Dead Boy armor, take hits (second staff
        character, or let a target swing). Expected: armor MDC pool
        depletes FIRST; your own pools untouched until it empties.
12. [ ] `cast armor of ithan`, then take hits. Expected order:
        Ithan barrier, then worn armor pool, then body pools.

Sever/restore (narrative severing, admin command):
13. [ ] `sever list <target>` for valid limbs, then `sever <limb>`
        on an ordinary limb. Try `sever head` too. Expected: head,
        torso, and whole body are all REFUSED; the ordinary limb
        severs.
14. [ ] `look` at the victim; have them type `body` and `limbs`.
        Expected: severed limb shown in all three outputs, no stat
        or damage change (narrative only).
15. [ ] `restore <limb>`. Expected: limb back, outputs clean.

## 3. Ring of Dominion (regression retest, post `admin_ring` fix)

Background: after the `admin_ring` cosmetic slot was added to
`std/armour.c` and `ring_of_dominion.c`'s `rifts_slot` was changed to
`"admin_ring"`, `wear ring` made the ring vanish from inventory with no
on-wear message and no error shown. Root cause: the driver had not been
fully rebooted since the `std/armour.c` change landed, so the connected
session was still running pre-`admin_ring` code — `resolve_cosmetic_slot()`
fell through to the old limb-based path, and `ring_of_dominion.c` (which
never called `set_limbs()`) passed an undefined limb array into
`equip_armour_to_limb()`. Fixed by (a) a full reboot to load the current
`std/armour.c`, and (b) a defensive `set_limbs(({ "right hand" }))` added
to `ring_of_dominion.c` so any future fall-through fails as an ordinary
limb-based refusal instead of silently destructing/dropping the item.

**This is exactly the failure mode the FULL REBOOT prerequisite at the
top of this file exists to prevent. Any `/std/` file change needs a real
`mud.sh stop && mud.sh start`, never just `update` or `warmboot` — those
only reload the blueprint, not the code already baked into
already-connected sessions and already-loaded objects. If you're ever
unsure whether a reboot actually happened after a `/std/` edit, check
`log/runtime`'s timestamp against the most recent commit touching that
file (`git log -1 -- std/<file>.c`); if the commit is newer than the
last boot, the running driver has not picked it up yet.**

1. [ ] Confirm the ring is present, unworn, in inventory (`inventory`
       or `i`).
2. [ ] `wear ring`. Expected: an on-wear message appears (either "You
       slide the gold ring onto your finger..." if `admin_wizp`, or
       "The ring stays cold and inert on your hand." if not). The ring
       must still be listed afterward as "a plain gold ring (worn)" -
       not vanished.
3. [ ] Confirm the five wiz-tools were cloned into inventory (staff of
       demotion, staff of dominion, staff of creation, RP-Wiz skill
       tool, tattoo-gun) - skip any you already had before wearing the
       ring.
4. [ ] `eq` or `look` at yourself: confirm the ring shows on its own
       slot, distinct from `ring_left`/`ring_right`.
5. [ ] While the ring is worn, also wear a regular ring (e.g. the test
       wing's signet ring) and/or a wedding band if one is available.
       Expected: no conflict either direction - the ring of dominion
       occupies `admin_ring`, never `ring_left`/`ring_right`, so both
       should be wearable together with no refusal message from
       either side.
6. [ ] `remove ring`. Expected: "The ring cools, and the tools it
       granted fade from your hands." Tools freshly cloned by the ring
       are stripped; any tool you already had before wearing it
       remains.
7. [ ] `askring <question>` while worn (try "promote", "build",
       "domain", "tattoo", "skill", "reboot", "save"). Expected: a
       relevant canned answer for each. `askring` while NOT worn:
       "The ring is silent. It only answers while worn."

## 4. Gating: mortals must reach none of this

1. [ ] As TESTCHAR, type `repairchar TESTCHAR`, `makechar x y z w`,
       `sever list`, and `playerwipe x`. Expected: every one gets an
       ordinary unknown-command response or an access refusal;
       nothing executes.
2. [ ] Walk TESTCHAR to your workroom and through `wing`. Expected:
       the containment ward ejects them to Praxis square.
3. [ ] `trans` TESTCHAR directly into the hall or arena. Expected:
       ejected the same way on arrival.
4. [ ] Have TESTCHAR quit while briefly inside the wing (if they can
       type it before ejection). Expected: on next login they resume
       at their previous valid start, never inside the wing.
5. [ ] As TESTCHAR, type `start` and `start here`. Expected:
       ordinary unknown-command response (mortal command retired
       2026-07-19). As thurtea, `start here` still works
       (cmds/hm/_start.c, staff only).

## 5. Poisoned-start repair (admin-side check)

1. [ ] Poison the mortal's start:
       `eval find_player("TESTCHAR")->set_primary_start("/realms/thurtea/workroom")`
2. [ ] Have TESTCHAR quit and log back in. Expected: they land at
       Praxis square, and
       `eval find_player("TESTCHAR")->query_primary_start()` now
       shows the square, not the workroom.

## 6. Tone spot-check (staff-side strings)

1. [ ] Test wing room descriptions, ward ejection line, and every
       sample item description (each tag reads SAMPLE: <SLOT>).
2. [ ] Renn's and Sela's response lines (ask Sela about logs,
       procedure, damage, armor, spells).
3. [ ] repairchar/makechar output blocks and the rebuild notice the
       target sees.

## 7. After the pass

Bugs to `master_gap_report.txt`. When both rounds pass locally:
commit, pull on the VPS, full-reboot the VPS.
