# Playtest checklist: PLAYER round - 2026-07-19 (updated 2026-07-21)

Every test in this file is run as a normal mortal character. Nothing
here needs creatorp() or any admin command; where a step needs an
admin to act ON the player (a summon), it says so. Staff-side tests
live in [playtest-checklist-wizards.md](playtest-checklist-wizards.md).

Prerequisites: server running (`./mud.sh status`, port 1122), a FULL
REBOOT since the last std/user.c pull, and one or two fresh throwaway
player accounts created in section 1. Mismatches go to
`/domains/Praxis/adm/master_gap_report.txt`.

## 1. Chargen full walkthrough (new account)

1. [ ] At the zone prompt, type `look` first. Expected: normal
       command works; the prompt is not consumed (news-pager fix).
2. [ ] Type `americas`. Expected: "The Americas. You will begin at
       the edge of Chi-Town." and the STEP 2 attribute banner. (Text
       changed 2026-07-21 with the new zone start rooms; it no
       longer says "outskirts of Praxis".)
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
        granted, rift-arrival text, you land in `chitown_start`
        ("Waystation, Praxis-Chi-Town Corridor") - not Praxis proper -
        score sheet prints, and only then does unread news appear.
        See section 2 for the full start-room check.
13. [ ] Type `score` and `skills`. Expected: OCC package skills are
        present without you ever picking one.

## 2. Zone start rooms and orientation NPCs (Phase 1 naming pass, 2026-07-21)

Run this once per zone with a fresh throwaway character; you do not
need to complete all three in one session.

1. [ ] Americas: finish chargen with `americas`. Expected: you land in
       `chitown_start` ("Waystation, Praxis-Chi-Town Corridor"), with
       `rift_survivor` ("survivor") and `chargen_guide` ("Gamemaster")
       both present. `say hello` to each.
2. [ ] From `chitown_start`, go `north`. Expected: `chitown_gate`
       ("Chi-Town, Main Gate"). Go `waystation` (not `south`, which is
       claimed by `chitown_burbs`). Expected: back at `chitown_start`.
       This exit was missing before 2026-07-21 - a real one-way dead
       end - so confirm it actually works, not just that it exists.
3. [ ] From `chitown_start`, go `south`. Expected: back at
       `rifts_welcome`, unaffected by this pass.
4. [ ] Europe: finish chargen with `europe`. Expected: you land in
       `newcamelot_start` ("New Camelot, Gatehouse Interior"), with
       two `royal_knight_guard`s and a `camelot_guide` ("Gamemaster",
       New Camelot-flavored appearance text) present. `say hello` to
       the guide.
5. [ ] From `newcamelot_start`, confirm `north`/`south`/`east` reach
       `camelot_square`/`europe_road`/`camelot_gate`, and each of
       those has a working exit back.
6. [ ] Infamy/ambush gate at `newcamelot_start` (needs an admin to
       push a throwaway character's infamy above 300 or coalition
       standing above 500, then re-enter the room): expected "You are
       not welcome within these walls," followed about a second later
       by two knights spawning and attacking. A normal-standing
       character should instead get "Hail, traveler. Welcome to New
       Camelot." and a one-time-per-day `new_camelot` faction bump on
       first entry.
7. [ ] Atlantis: finish chargen with `atlantis`. Expected: you land in
       `splynn_entry`, unaffected by this pass (still the same room it
       always was).

## 3. Logout and reconnect behavior

1. [ ] Walk a few rooms away from the start room (any ordinary
       Praxis room). Type `quit`, then log back in. Expected: you
       resume in the SAME room you quit in.
2. [ ] Walk somewhere else ordinary. Kill the client connection
       (close the window; do NOT type quit). Reconnect and log in.
       Expected: you resume in the SAME room, and anyone watching
       sees "<Name> has rejoined our reality."
3. [ ] Staff-room refusal message (needs an admin): have an admin
       summon you into a staff room (their workroom), then type
       `quit` there. Expected quit-time message: "This location
       cannot be kept as your login start; you will resume at your
       previous one." Log back in. Expected: you resume at your
       previous valid room, never the staff room.
4. [ ] Type `start`. Expected: an ordinary unknown-command response.
       The mortal self-set-start command was removed 2026-07-19;
       there should be no "Syntax: start here" output.

## 4. Moxim rift network (six hops)

Costs 500 credits per hop (200 to Praxis); make sure the character
has funds, or run this round with whatever credits chargen granted
and stop when broke. `rift <destination>` opens a portal from
anywhere; `enter rift` travels. After each arrival, confirm a live
Moxim stands in the arrival room.

1. [ ] `rift americas` -> Chi-Town boulevard. Moxim present: yes/no.
2. [ ] `rift atlantis` -> Splynn market. Moxim present: yes/no.
3. [ ] `rift europe` -> New Camelot square. Moxim present: yes/no.
4. [ ] `rift lazlo` -> Lazlo market. Moxim present: yes/no.
5. [ ] `rift tolkeen` -> Tolkeen square. Moxim present: yes/no.
6. [ ] `rift praxis` -> Monument Square. Moxim present: yes/no.
7. [ ] In one arrival room, `say destinations` near Moxim. Expected:
       his table matches the global `rift` list.

If any hop lands in a gate/welcome room instead of the rooms above,
the server has not had a full reboot since the destination fix.

Ocean access (added 2026-07-19):

8. [ ] Near any Moxim, `say blue rift`. Expected: his hint about a
       blue rift on the Chi-Town boulevard that leads to open ocean,
       NOT the destination table.
9. [ ] Without breathing gear, walk to the Ocean Rift north of the
       Splynn docks and go `down`. Expected: you are forced back to
       the surface with a message naming the three ways to survive
       (rebreather, oxygen implant, Swim as a Fish).
10. [ ] Buy a rebreather at the Chi-Town outfitter (1200 credits),
        `wear rebreather`, return, and go `down` again. Expected: you
        pass into the Shallows, and can continue down to the Deep
        Zone and the Rift Floor.
11. [ ] Wait on the Chi-Town boulevard across a few resets until "a
        rift glowing deep blue" appears (roughly one reset in four;
        it collapses after two minutes). `enter rift`. Expected: you
        arrive at the Ocean Rift surface room off Splynn.
12. [ ] Sword of Atlantis: if it is still on the shelf at the Ocean
        Rift, take it (mind the sea serpent). Expected: on every
        later reset the shelf stays empty; the sword never respawns
        for anyone once first taken.

## 5. Combat, armor pools, and cosmetic wear (player gear)

Use ordinary shops (Praxis outfitter/arms dealers, or the Chi-Town
arcade) and a weak zone enemy (a lone highway bandit or similar
low-level target outside Praxis).

1. [ ] Buy and wear any protective body armor a shop sells.
       Expected: it wears on the whole body; no limb prompts.
2. [ ] Buy and wear cosmetic pieces available in shops (a backpack
       at minimum; any hat/belt/boots the outfitters stock).
       Expected: each occupies its slot, shows in `eq`, and a second
       item of the same slot is refused.
3. [ ] Fight the weak enemy while wearing the protective armor.
       Expected: the armor's pool depletes FIRST; your HP/SDC/MDC
       only move after the armor pool empties.
4. [ ] `remove` each piece. Expected: clean removal, `eq` empties,
       no errors.
5. [ ] Confirm no attack ever reports hitting a named limb of yours;
       damage is whole-body only.

## 6. Spellcasting and psionics (common OCC sample)

Create (or reuse) one caster and one psychic through normal chargen,
e.g. a Ley Line Walker and a Mystic.

1. [ ] As the caster, `cast energy bolt at <target>` on a zone
       enemy. Expected: damage lands, PPE drops by the spell cost.
2. [ ] Cast a targeted spell with NO target present (e.g.
       `cast energy bolt` alone in a room). Expected: the cast
       fails WITHOUT spending PPE (resource-loss fix).
3. [ ] `cast armor of ithan`. Expected: barrier up; take a hit and
       watch the barrier absorb before armor and body.
4. [ ] As the psychic, use a damage psionic (e.g. `psi mind bolt at
       <target>`). Expected: damage lands, ISP drops.
5. [ ] Repeat the no-target check with a targeted psionic. Expected:
       no ISP spent on the failed attempt.

## 7. General command sweep

1. [ ] `score` - all pools shown, no zero attributes, alignment and
       OCC correct.
2. [ ] `skills` - OCC package skills listed with percentages.
3. [ ] `eq` and `inventory` - consistent with what you carry/wear.
4. [ ] `body` and `limbs` - display fine, no severed limbs, no
       errors.
5. [ ] `languages` - native languages listed per race.
6. [ ] `help` - categories list; `help combat` and one topic open
       correctly; `help index` shows the flat list.
7. [ ] `smell` and `listen` in a room that sets them - text appears
       ONLY on explicit command, never in the room description.

## 8. Tone spot-check (player-visible strings)

Professional tone, no slang, no long dashes, American spelling:

1. [ ] Chargen STEP 1-5 banners and accept/reroll prompts.
2. [ ] "<Name> has rejoined our reality." (section 3).
3. [ ] The quit-time refusal line (section 3 step 3).
4. [ ] Moxim's six flavor lines (section 4).
5. [ ] The Gamemaster/`camelot_guide`, `rift_survivor`, and New
       Camelot gate-check lines (section 2).

## 9. Backlog: zone eyeball walk-throughs (carried forward)

- [ ] Chi-Town: arcade shops, ISS checkpoint, maintenance hatch,
      sewer hydra, D-bee hostility without fake ID.
- [ ] Tolkeen: gate, square, market, barracks, mage quarter, crypt
      (ward golem, shade, vault charm), siege edge.
- [ ] Horton: hovertrain, Main Street, outskirts spokes, quarry
      lurker, hermit cabin, forest ley glade and wolf den.

## 10. After the pass

Bugs to `master_gap_report.txt`. When both this and the wizard round
pass locally: commit, pull on the VPS, full-reboot the VPS.
