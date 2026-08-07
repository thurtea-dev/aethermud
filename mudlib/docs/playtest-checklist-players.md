# Playtest checklist: PLAYER round - 2026-07-19 (updated 2026-07-24)

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

Other unique-item locks (added 2026-07-22):

13. [ ] Ghostly katana: in the Catacomb Side Chamber (Splynn
        catacombs), take the katana from the pedestal. Expected: on
        every later reset the pedestal stays empty; the katana never
        respawns for anyone once first taken.
14. [ ] Dragonfire lance: in Camelot Castle - Royal Armory (New
        Camelot), take the lance from the iron stand. Expected: on
        every later reset the stand stays empty; the lance never
        respawns for anyone once first taken.
15. [ ] Armor talisman (Rocky's Bar, Splynn): give Rocky 3 items with
        "salvage" in the name. Expected: on the 3rd, Rocky hands over
        an armor talisman. Immediately repeat with 3 more salvage
        items. Expected: Rocky refuses ("Already gave one of those
        out. Not making two.") - this one is a 3-day cooldown, not a
        permanent lock like the katana and lance above.

Vendor fixes (added 2026-07-22):

16. [ ] At Lone Star's Supply Depot, `buy long e-clip` (500 credits)
        and `buy long clip`. Expected: both succeed and hand over a
        long e-clip (60 shots). Before the fix, both words resolved
        to a broken item path and the vendor said "I'm out of that
        right now" even though `list` showed it for sale.
17. [ ] At the NGR Triax Equipment Depot, repeat with `buy long
        e-clip` / `buy long clip` (520 credits). Expected: same
        success as above.
18. [ ] At La Cantina in Puerto Angel, `list` at Elena. Expected: she
        now sells silver cross, wooden stake, silver dagger, and holy
        water vial (she was a plain non-vendor NPC before). `buy holy
        water` (60 credits) and confirm you receive a vial.

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

Peace skill (ISP-gated, added 2026-07-22):

6. [ ] As the psychic (Mystic), with at least 10 ISP and hostile
       NPCs attacking you, type `peace`. Expected: "You call for
       peace," 10 ISP spent, and each attacker's chance to break off
       depends on your ISP against their level.
7. [ ] As the caster (Ley Line Walker - "magic" flag only, no
       "psychic" flag), type `peace`. Expected: "Your training
       doesn't include psychic pacification." No ISP spent.
8. [ ] As the psychic with fewer than 10 ISP, type `peace`. Expected:
       "Not enough ISP to project a calming field." No ISP spent, no
       attackers affected.

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
8. [ ] `ooc <message>` in a room with another player present.
       Expected: you see "You [OOC]: <message>" and they see
       "<Name> [OOC]: <message>", the [OOC] tag in red and the
       message in white (changed 2026-07-22 from the old plain
       "(ooc)" tag).

## 8. Tone spot-check (player-visible strings)

Professional tone, no slang, no long dashes, American spelling:

1. [ ] Chargen STEP 1-5 banners and accept/reroll prompts.
2. [ ] "<Name> has rejoined our reality." (section 3).
3. [ ] The quit-time refusal line (section 3 step 3).
4. [ ] Moxim's six flavor lines (section 4).
5. [ ] The Gamemaster/`camelot_guide`, `rift_survivor`, and New
       Camelot gate-check lines (section 2).

## 9. Look-by-race (std/user.c `id()` change, 2026-07-24, needs full reboot)

Players are now also identifiable by race/RCC name as a look target,
in addition to their character name. Needs two throwaway characters
of the SAME race online together (e.g. two humans, or a human and a
Great Horned Dragon for the first two steps).

1. [ ] As a Great Horned Dragon (or any race), stand in a room with
       another player. Have them `look at <your race name>` (e.g.
       `look at great horned dragon`). Expected: shows your normal
       look-description, identical to `look at <your name>`.
2. [ ] `look at <race>` still works exactly like before for NPCs and
       for `look at <yourname>` - this change is additive only.
3. [ ] Two players of the SAME race in one room. `look at <race>`
       with no number. Expected: resolves to whichever of the two is
       first in room order (same first-match behavior as `look at
       knight` already has with the two Royal Knight guards - no
       error, no random pick, no crash).
4. [ ] Same room, `look at <race> 2`. Expected: resolves to the OTHER
       player of that race (standard ordinal disambiguation, e.g. the
       same syntax that already works for `get sword 2` or similar).
5. [ ] If you have access to `metamorph` (Great Horned Dragon or
       changeling): `metamorph human`, then have a stranger (someone
       who does NOT know you via `introduce`) try
       `look at great horned dragon`. Expected: fails to find you -
       they only find you via `look at human` while disguised. Someone
       who already knows you, or staff, can still find you either way.

## 10. Splynn Preserves: loop closure + depth (batch, 2026-07-24)

Structural fix: the Preserves graph was a pure tree with 8 dead ends
and no loop. This batch adds one loop-closing connector and depth on
three of the shallowest dead ends (R20-R23).

1. [ ] From `preserve_nest` (top of the switchback), go `west`.
       Expected: `preserve_saddle` ("Wind-Scoured Saddle").
2. [ ] From `preserve_saddle`, go `south`. Expected: `preserve_ridge`
       (Overgrown Ridge) - confirms the loop actually closes rather
       than dead-ending.
3. [ ] From `preserve_ridge`, go `north` back to `preserve_saddle`,
       then `east` back to `preserve_nest`. Expected: both directions
       work, completing the loop in either direction.
4. [ ] From `preserve_hivedeep` (Hive Depths), go `down`. Expected:
       `preserve_hivecore` ("Hive Heart"), with a Kydian Overlord and
       a Xiticix warrior both present. Go `up` to confirm the way
       back to Hive Depths.
5. [ ] From `preserve_catwalk` (High Catwalk), go `down`. Expected:
       `preserve_nook` ("Rope Nook"). Go `up` to confirm the way back.
6. [ ] From `preserve_cagerow` (Cage Row), go `south`. Expected:
       `preserve_cagepit` ("Lower Cage Pit"), with a captive and a
       Splugorth minion present. Go `north` to confirm the way back.
7. [ ] Room descriptions: confirm `preserve_nest`, `preserve_ridge`,
       `preserve_hivedeep`, `preserve_catwalk`, and `preserve_cagerow`
       each mention their new second exit in their `look` text, not
       just in the exits line.

## 11. Lone Star: barracks/supply/perimeter depth (batch, 2026-07-24)

Matches world.html's "complex still expanding" description with three
new depth spurs.

1. [ ] From `lone_star_barracks` (Dog Boy Barracks), go `north`.
       Expected: `lone_star_bunkroom` ("NCO Bunkroom"), with a Dog Boy
       guard present.
2. [ ] From the bunkroom, go `east`. Expected: `lone_star_armory`,
       with a Dog Boy guard present. Go `west` to confirm the way
       back to the bunkroom, then `south` back to the main barracks.
3. [ ] From `lone_star_supply` (Supply Depot), go `east`. Expected:
       `lone_star_motorpool` ("Motor Pool"). Go `west` to confirm the
       way back.
4. [ ] From `lone_star_perimeter` (Outer Perimeter), go `north`.
       Expected: `lone_star_escape_tunnel` ("Breached Maintenance
       Tunnel") - a dead end past the torn grating. Go `south` to
       confirm the way back to the perimeter.
5. [ ] Confirm none of the three new spurs (bunkroom/armory,
       motorpool, escape tunnel) affect the existing gate-plaza-lab-
       containment-perimeter loop - all five original rooms should
       still connect exactly as before.

## 12. Backlog: zone eyeball walk-throughs (carried forward)

- [ ] Chi-Town: arcade shops, ISS checkpoint, maintenance hatch,
      sewer hydra, D-bee hostility without fake ID.
- [ ] Tolkeen: gate, square, market, barracks, mage quarter, crypt
      (ward golem, shade, vault charm), siege edge.
- [ ] Horton: hovertrain, Main Street, outskirts spokes, quarry
      lurker, hermit cabin, forest ley glade and wolf den.

## 13. Character Select spacing (login screen, three account states)

Rendered by `show_char_menu()` in `secure/std/login.c`. Test all three
states; the blank lines and the conditional `delete` line are the
point. Reach it by logging in to an account (not a character).

1. [ ] **Empty account** (brand new account, no characters yet).
       Expected, exactly:

       ```
       (blank line)
       === Character Select ===
       ------------------------
       (blank line)
       (No characters on this account yet.)
       (blank line)
         new. Create a new character
       (blank line)
       Choose a character number, or 'new':
       ```
       Confirm the `delete` line is ABSENT (it is suppressed when the
       account has no characters).
2. [ ] **Single character, offline.** Expected: same frame, with
       `  1. <Name>` in place of the "(No characters...)" line, and the
       `delete <number or name>. Delete a character (asks to confirm)`
       line now PRESENT below `new.`.
3. [ ] **Multiple characters, one of them online** (log one in from a
       second client, then reach Character Select from the first).
       Expected: numbered list `1.`, `2.`, ... in order, with
       `  (online)` appended to the logged-in one only. Confirm the
       two-space indent is consistent across all rows and that the
       `(online)` marker does not shift the alignment of other rows.
4. [ ] Confirm no double blank lines anywhere in any of the three
       states, and exactly one blank line between the `------` rule
       and the first list row.

## 14. Magic net: capture and dissolve messages

The dissolve text was simplified 2026-07-26 to drop a PPE reference.
`std/obj/magic_net.c` is cloned, never inherited, so `update` is
enough for new nets - but a net already in flight keeps the old text
for up to its 120-second lifetime, so let any pre-existing net expire
before testing.

1. [ ] As a caster who knows the spell, `cast magic net at <target>`
       (or `magicnet <target>`). Expected: the capture messages fire
       and the target is bound - cannot move or attack, but is still
       damageable.
2. [ ] With no target given, `cast magic net`. Expected: the syntax
       help, `Cast at whom?  Syntax: cast magic net at <target>` /
       `Or: magicnet <target>`. Confirm no PPE was spent on the failed
       cast (check PPE in `score` before and after).
3. [ ] Net a target, then leave it alone for the full 120 seconds.
       Expected dissolve text: `The magic net fades away and
       dissolves.` Confirm the phrase "sparks of PPE" does NOT appear.
4. [ ] Net a target, then attack the netted target until the net's 30
       MDC is gone. Expected: `The magic net shreds and falls away!`
       and the victim is freed - this is the damage path, distinct from
       the timeout path in step 3.
5. [ ] Confirm the freed victim can move and attack again in both
       cases (timeout and shredded).

## 15. Lone Star expansion (batch, 2026-07-26, 11 -> 23 rooms)

Supersedes section 11's coverage, which only reached the three
2026-07-24 spurs. Two closed loops are the structural point here.

1. [ ] **Kennel wing.** From `lone_star_barracks`, go `east`.
       Expected: `lone_star_kennel_yard` ("Kennel Yard"). From there
       `north` to `lone_star_kennels` ("Kennel Rows") - expect a
       kennel master NPC and two juvenile Dog Boys. Back `south`, then
       `east` to `lone_star_training_ring` ("Training Ring"). Confirm
       `west` returns to the yard.
2. [ ] **Incubation core loop.** Walk
       plaza -> `north` lab -> `east` `lone_star_gene_vault` ->
       `north` `lone_star_observation` -> `down`
       `lone_star_incubation` -> `south` `lone_star_containment` ->
       `south` lab. Expected: the loop closes with no dead end.
3. [ ] Walk the same loop in reverse (lab -> containment -> incubation
       -> `up` observation -> `south` gene vault -> `west` lab).
       Expected: every exit is bidirectional.
4. [ ] From `lone_star_incubation`, go `west` to
       `lone_star_cold_storage`. Expected: a **failed specimen**
       present (level 9, 45 MDC - noticeably tougher than the mutant
       subject). Confirm `east` returns to the gallery.
5. [ ] **Escape tunnel loop.** From `lone_star_gate` go `north` to
       perimeter, `north` to `lone_star_escape_tunnel`, `north` to
       `lone_star_tunnel_junction`, `north` to
       `lone_star_culvert_mouth`, then `southeast`. Expected: back at
       `lone_star_gate` - the loop closes outside the wire.
6. [ ] From `lone_star_tunnel_junction`, go `west` to
       `lone_star_sump` ("Flooded Sump"). Expected: a dead end;
       sometimes a salvage item on the floor. Confirm `east` returns.
7. [ ] **Motor pool loop closer.** From `lone_star_motorpool`, go
       `northeast`. Expected: `lone_star_perimeter`. From perimeter,
       `southwest` returns to the motor pool. This exit is new; it was
       a dead end in the 2026-07-24 batch.
8. [ ] **Canteen vendor.** From `lone_star_gate` go `south` to
       `lone_star_support_row`, then `east` to `lone_star_canteen`
       ("The Dry Kennel"). Run `list`, then `buy water` and `buy
       meal`. Expected: purchases succeed against credits and the
       items arrive in inventory.
9. [ ] **Juvenile Dog Boy.** In the kennels or training ring, `look`
       at a `dog boy pup`. Expected: level 3, non-aggressive - it must
       NOT attack on sight. Confirm attacking one is possible but
       requires you to start it.
10. [ ] Confirm the two rule-8 text fixes: `look` in
       `lone_star_lab` and `lone_star_plaza` must NOT describe the
       scientist or the Dog Boy guard in the room description text -
       they should appear only in the NPC list below the exits line.
       In the plaza, confirm the description no longer states the air
       smells of antiseptic and ozone (that belongs to `smell` only).

## 16. Support Row registration sweep (2026-07-26)

Needs a **non-human mortal** character (the check is
`WANTED_D->is_cs_enemy()`: human, Dog Boy, CS psi-stalker, staff, CS
faction members, and anyone at friendly coalition standing all pass
and will never trigger it). Room is `lone_star_support_row`.

1. [ ] As a non-human mortal, enter Support Row and **wait**. Expected:
       after 20-60 seconds a patrol arrives, and the Dog Boy demands
       papers with a `Type 'show papers' to comply.` hint. Confirm
       nothing fires if you walk straight through in under 20 seconds.
2. [ ] **Comply.** Type `show papers`. Expected: detention - you are
       moved to `lone_star_containment`, `A cell door cycles shut
       behind you.`, and you keep ALL carried and worn gear (nothing
       is stripped). Confirm +50 infamy and a coalition standing drop.
3. [ ] From containment, escape via the new route: `east` to
       perimeter, `north` tunnel, `north` junction, `north` culvert
       mouth, `southeast` to the gate. Expected: a complete escape
       path with no dead end.
4. [ ] **Refuse.** Trigger a fresh sweep and do nothing for the full
       30-second window. Expected: `"Non-compliance. Take it down."`
       and the patrol attacks (level 7, 35 MDC).
5. [ ] **Walk out.** Trigger a sweep, then leave the room during the
       30-second window. Expected: a clean escape - no combat, no
       infamy, no standing change, and the patrol is gone when you
       return.
6. [ ] **Dead Boy armor bypass.** Wear `dead_boy_armor` (appearance
       "A Coalition Dead Boy") and enter Support Row as the same
       non-human character. Expected: NO sweep is ever armed. Then
       trigger a sweep without the armor and put the armor on during
       the 20-60s delay - expected: the patrol arrives, looks you over,
       and waves you past instead of demanding papers.
7. [ ] **Dry Kennel refuge.** Trigger a sweep, then go `east` into the
       canteen. Expected: the patrol does not follow, and the canteen
       is a no-attack room.
8. [ ] **Cooldown.** After a sweep resolves, re-enter Support Row
       repeatedly for the next 5 minutes. Expected: no second sweep.
9. [ ] **Cooldown is not exploitable.** Step into Support Row and
       straight back out BEFORE any patrol arrives, several times.
       Expected: the sweep still arms on a later visit - stepping in
       and out must NOT buy 5 minutes of quiet.
10. [ ] **No patrol stacking.** Trigger a sweep, let it go to combat,
       flee, and return. Expected: at most one patrol Dog Boy in the
       room, never two.

## 17. Metamorph held form (fix, 2026-07-26)

**Read this first: metamorph cannot be verified on a staff character.**
`std/user.c` `query_apparent_race()` returns the TRUE race whenever the
viewer is the character themselves, is staff, or already knows them -
and `creatorp(this_object())` means a staff character's disguise is
inert for every viewer, mortals included. Use a mortal shapeshifter
observed by a second mortal who has not been introduced to them.
Needs a FULL REBOOT (`std/living.c` and `std/user.c` both changed).

1. [ ] As a mortal Great Horned Dragon, run `metamorph human`.
       Expected: the "massive form flows inward" text, followed by
       `Anyone who does not already know you now sees a human.`
2. [ ] Run `metamorph human` a second time. Expected: `You already
       appear to be a human.` and NO success text - this is the
       false-success bug that was fixed.
3. [ ] Run `metamorph great horned dragon` while in true form.
       Expected: `You already appear to be a great horned dragon.`
4. [ ] `score`. Expected: `Race: Great Horned Dragon (as Human)` while
       metamorphed, reverting to plain `Race: Great Horned Dragon`
       after `metamorph true`.
5. [ ] `look` at yourself while metamorphed. Expected: the true race
       on the "You look over the..." line PLUS `Currently holding the
       form of a human.`
6. [ ] As a **second mortal who has not been introduced** to the
       shapeshifter, `look` at them. Expected: `You look over the male
       human.` and NO "Currently holding the form of" line - that
       line must never leak to a stranger.
7. [ ] As that same stranger, `look at human`. Expected: resolves to
       the disguised character. Then `look at great horned dragon`.
       Expected: does NOT resolve - a stranger must not be able to
       confirm the disguise by guessing the true race word.
8. [ ] As the shapeshifter, `look at human` on yourself. Expected:
       resolves. This failed before the fix.
9. [ ] `metamorph true`. Expected: the revert text, the form line gone
       from `look`, and `score` back to the plain race.
10. [ ] If the shapeshifter is staff, confirm the command warns:
       `Your staff status overrides this: your true race is still
       shown to everyone. Test this on a mortal character.`

## 18. After the pass

Bugs to `master_gap_report.txt`. When both this and the wizard round
pass locally: commit, pull on the VPS, full-reboot the VPS.
