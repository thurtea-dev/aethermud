# Starting Equipment Audit (Race / OCC)

Date: 2026-07-18. Sources compared: `daemon/rifts_start_d.c`
`give_equip()` (what players actually receive), `daemon/occ.c`
`starting_equipment` text (what chargen promises on screen), and
Palladium Rifts tabletop starting gear. Item availability checked
against `/domains/Praxis/equipment/`.

## How gear is granted

- All gear is keyed on OCC in `give_equip()`. Every one of the 62 OCCs
  in `daemon/occ.c` has a case; nothing falls through by accident.
- Races got no gear of their own before this audit. Only 19 of the 62
  races are OCC-eligible, so the other 43 (dog boy, both psi-stalkers,
  burster, dragons, vampires, faeries, mutant animals, etc.) always
  received the generic default: leather jacket, knife, 500 credits.
- New in this pass: `give_race_equip()` in `rifts_start_d.c` runs
  instead of `give_equip()` when the OCC is "none". It currently has
  one case (dog boy) and falls back to the default package for all
  other races. New RCC gear should be added there, not in the OCC
  switch.
- Every energy weapon ships with a full internal charge (e.g. C-18
  holds 20 shots), so missing spare e-clips is a quality-of-life gap,
  not a hard blocker.

## Fixed this pass (top 5 most-played)

1. **Human** (via the OCC "none" / default package): added backpack and
   two rations so an OCC-less start is survivable. Was: jacket, knife,
   500 credits.
2. **Dog boy**: new race-gear case. Now receives Coalition grunt armor,
   CP-30 laser pistol, spare e-clip, vibro-knife, military radio, and
   2d6 x 10 credits (the CS issues gear, not cash). Was: generic
   default.
3. **Juicer** (and ninja/delphi/hyperion variants, shared case): added
   C-18 pistol, C-12 rifle, and two spare e-clips to match the promised
   "two ranged weapons"; credits raised from 1d6 x 100 to 3d6 x 100.
   Kept armor and vibro-sword.
4. **Ley line walker / ley line rifter**: split out of the shared case
   with shaman/air warlock. Added backpack, field journal (spellbook
   stand-in), spell components, and a C-18 sidearm per the occ.c text
   ("spellbook, arcane focus, and concealed sidearm"); credits raised
   to 2d6 x 100. Shaman and air warlock keep their old package
   unchanged.
5. **Cyber-knight** (shared with knight (europe), royal knight):
   replaced chain mail (SDC-only: 60 SDC, AR 14, worthless against MDC
   weapons) with environmental body armor (90 MDC), and added a C-18
   energy sidearm per the occ.c text ("light armor ... one ranged
   weapon"). Cyber-knights on an MDC world previously started in
   armor that could not stop a single MD hit.

## Remaining gaps, prioritized

### High (playable class starts materially under-equipped)

- **cs psi-stalker / wild psi-stalker**: FIXED 2026-07-18. CS
  psi-stalker gets the CS-issued kit (grunt armor, CP-30, e-clip,
  vibro-knife, military radio, 2d6 x 10 credits, same as dog boy);
  wild psi-stalker gets wilderness gear (leather jacket, vibro-knife,
  C-12 rifle, e-clip, backpack, 2 rations, 1d6 x 100 credits).
- **burster**: FIXED 2026-07-18. Environmental body armor (closest
  fire-resistant MDC suit in the lib), C-18 pistol, e-clip, backpack,
  2 rations, 2d6 x 100 credits.
- **cs grunt / cs dead boy**: occ.c text promises C-20 pistol, C-12
  rifle, and four grenades; actual grant is dead boy armor, CP-30
  pistol, two e-clips, radio. Missing the rifle and grenades (grenade
  files exist under `equipment/grenades/`). Also no C-20 item exists;
  either build one or reword occ.c.
- **cs ranger**: text promises energy pistol plus sniper rifle; gets
  CP-40 rifle only. Add a pistol.
- **headhunter / bounty hunter**: text promises pistol and rifle
  ("two modern ranged weapons" / "laser pistol, rifle"); both get only
  the C-18 pistol. Add a rifle to each.
- **mind melter** (race variant of the OCC): OCC case gives leather
  jacket only, which matches the occ.c "no weapon" text, but the
  tabletop RUE mind melter does get light armor and a sidearm.
  Decide which source wins; currently the thinnest kit in the game.

### Medium (promised-but-missing flavor items)

- **mystic**: text promises a concealed ranged weapon; gets none.
- **shifter**: text promises a sidearm; gets none (has ritual
  components).
- **techno-wizard**: text promises "one techno-wizard creation of
  choice"; no TW item is granted (none exists yet at equipment root).
- **operator**: text promises "a well-maintained vehicle";
  `hover_cycle.c` exists at the equipment root and could be granted.
- **borg**: text promises integrated weapons, vibro-blade, and a
  maintenance toolkit; gets only the conversion plus credits. Add
  vibro_knife.c and tool_kit.c at minimum.
- **glitter boy pilot / cs samas rpa pilot / robot pilot**: no spare
  e-clip for the sidearm; minor since weapons ship charged.
- **gunfighter**: two C-18 pilots, no spare e-clips; same note.
- **assassin group (assassin / master assassin / sunaj assassin)**:
  leather jacket only for armor; sunaj in particular should have MDC
  armor per tabletop.

### Low / informational

- **Dead case in `give_equip()`**: `case "kankoran":` sits in the
  wilderness scout block, but kankoran is a race, never an OCC value,
  so that label can never match (give_equip receives the OCC string).
  Kankoran players with no OCC get the default package. Either delete
  the label or move kankoran gear to `give_race_equip()`.
- **Races correctly gearless**: dragons, faeries, sprites, vampires
  (tabletop gives them nothing; their bodies are the equipment). The
  default package now includes a backpack and rations, which is
  slightly silly for a dragon hatchling but harmless.
- **Kittani (race)**: OCC-eligible and has two dedicated Kittani OCCs
  with good gear; a Kittani who skips OCC gets the default package,
  which is acceptable.
- **occ.c starting_equipment text vs reality**: the chargen screen
  shows the occ.c text, so every gap above is also a player-visible
  broken promise. When fixing an OCC, update whichever side (grant or
  text) is wrong so they agree.

## Next pass suggestion

Work the High list top to bottom; psi-stalkers and burster are pure
additions to `give_race_equip()` with existing item files and need no
new content, so they are the cheapest wins.
