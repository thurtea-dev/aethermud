# Playtest checklist - 2026-07-10

**STALE (2026-07-12 player wipe): the `pt*` accounts this checklist uses
were all deleted. Recreate them (see `tools/playtest_create_chars.py`)
before working through the sections below.**

Fresh characters were created for this pass. **Do not** use thurtea,
splynncryth, or any older account. Roster + password:
[playtest-roster.md](playtest-roster.md).

Shared password for all `pt*` playtest characters: `playtestaa`

Server: `./mud.sh status` (port **1122**). Telnet: `localhost 1122`.

---

## 0. Design change already applied

**Cyber-Knight attribute requirements** lowered from PS/PE/ME 12 (+ MA 10)
to **PS 10, PE 10, MA 10, ME 10** (Palladium book floors) in
`daemon/occ.c` and `doc/help/user/cyber_knight`.

Verified: **ptcknight** (human Cyber-Knight) created successfully after
one attr reroll. Under the old reqs this was nearly impossible on human
3d6.

---

## 1. Race eyeball characters (READY)

Log in each, run `score`, `eq`, try racial commands, note anything weird.

| Login | Race | What to check | Done |
|-------|------|---------------|------|
| ptdragona | Great Horned Dragon | MDC ~high, breath/fly/metamorph if present | [ ] |
| ptdragonb | Great Horned Dragon | Same; compare MDC variance | [ ] |
| ptvampa | Secondary Vampire | MDC, sunlight notes, visible_race human | [ ] |
| ptvampb | Secondary Vampire | Same | [ ] |
| ptatlana | Atlantean (Nomad) | Tattoos / PPE, OCC skills | [ ] |
| ptatlanb | Atlantean (Vagabond) | Compare to Nomad package | [ ] |
| pttitana | Titan (Vagabond) | Size / SDC, strength feel | [ ] |
| pttitanb | Titan (Headhunter) | Combat gear, attr bonuses | [ ] |

### Known chargen quirk to confirm

FIXED 2026-07-10: race pick now applies `do_rifts_rolls()` via
`apply_rifts_race_attributes()` in `setter.c`. Dragons/vampires
(ptdragona/b, ptvampa/b) were recreated - GHD now shows PS 50+ / high
MDC, not human 3d6. Eyeball and confirm racial commands still.

---

## 2. Wizard promotion candidates (READY - need admin)

Characters exist as mortals. **You** (thurtea or splynncryth) must promote
them while each target is **online**.

`makewiz` creates **Apprentice** (`[Apprentice]` on who) and gives the
**apprentice kit** (`kit`). They pick a track, submit a task, domain/coding
review, then you `setrole` when marked ready.

| Login | Target role | Commands (target online) | Done |
|-------|-------------|--------------------------|------|
| ptdacon | Domain-Wiz | `makewiz ptdacon` -> they use `kit` (domain track) -> later `setrole ptdacon domain` | [ ] |
| ptcoder | Coding-Wiz | `makewiz ptcoder` -> `kit` (coding track) -> later `setrole ptcoder coding` | [ ] |
| ptrpwiz | RP-Wiz | `makewiz ptrpwiz` -> `kit` (rp track) -> later `setrole ptrpwiz rp` | [ ] |

Confirm `who` shows `[Apprentice]` after makewiz, then the role tag after
setrole. Domain review: `domain` option 10. Coding queue: `review`.

---

## 3. Cyber-Knight smoke (READY)

| Login | Check | Done |
|-------|-------|------|
| ptcknight | `score` shows Cyber-knight + ISP; `help cyber-knight` shows reqs 10/10/10/10 | [ ] |
| ptcknight | Psi-sword activate / short fight north of Rift arrival | [ ] |

---

## 4. Section 8 live checks (any two pt* mortals)

Scripted suite already passed these; eyeball once as a human:

- [ ] Bank: Praxis bank room, `show id`, deposit/withdraw/balance
- [ ] Combat: weak grunt, autododge/autoparry on
- [ ] Radio: two chars, `radio on`, `radio tune 5`, message received

---

## 5. Chi-Town fortified city (eyeball when convenient)

From the gate / boulevard (Moxim is here):

- [ ] Northeast into shopping arcade - `list`/`buy` at ration, arms, outfitter
- [ ] ISS checkpoint (plaza northeast) - Psi-Stalker post, SAMAS hangar, kennels
- [ ] Residential hatch down to maintenance (secret city-side entrance)
- [ ] Sewer hydra: `push rock` after clearing hydra - obsidian blade + flame hilt
- [ ] D-bee alt without fake ID: confirm Dog Boy / grunt / patrol hostility

---

## 6. Tolkeen (new - Moxim `rift tolkeen`)

Liberation Square has its own Moxim. Global `rift` / Moxim both list
`tolkeen` (cmds/mortal/_rift.c + moxim.c).

- [ ] Gate → square → market / barracks / mage quarter / inn
- [ ] South from gate: approach → scorched field → siege edge (CS scout)
- [ ] Crypt down from square: ward golem → shade → vault ward charm
- [ ] TW vendor / mage / innkeeper interact; buy or talk once

---

## 7. Horton wilderness (hovertrain from Chi-Town burbs)

- [ ] Station north to Main Street (exit fix); store/inn
- [ ] Outskirts west orchard / north trail / east forest
- [ ] Trail: creek+marsh, scrub+bandit camp, farmstead spokes
- [ ] Quarry floor lurker; hermit cabin talk (`say quarry`)
- [ ] Forest: logging road → ley glade → grove → wolf den

---

## 8. After playtest

- Note bugs in chat or append to `master_gap_report.txt`
- Zone growth priorities: [zone-expansion-plan.md](zone-expansion-plan.md)
- Recreate chars anytime: `python3 tools/playtest_create_chars.py`
  (refuses if a save already exists - delete saves first if re-running)
