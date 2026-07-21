# RIFTS MUD DEVELOPER REFERENCE CHART
# Source: information.md + daemon files | Developer use only | No player-facing text

---

## 1. RACES

### Dog Pack (Mutant Dog) -- RCC
- **Type**: SDC
- **Base Attributes**: 3D6 all 8; +1D4 PE, +1D4 PS, +2D6 Spd (universal dog bonuses)
- **Base Body Pools**: SDC +20 (base); HP: P.E. + 1D6 (+1D6/level); PPE: 3D6; ISP: ME + 1D6x10, +10/level
- **Psionic Type**: Master (save: 10+)
- **Special Abilities**:
  - Sense Psychic/Magic: passive 50ft; active track 40%+4%/level per 1000ft; 400ft+50ft/level range active
  - Sense Supernatural: 62%+2%/level; 100ft/level passive; 1000ft+100ft/level active
  - Track by Scent: 40%+4%/level per 1000ft roll; trail up to 4 days old; -50% penalties in darkness/blind
  - Keen Hearing: included in initiative/combat bonuses
  - Combat bonuses: +20 SDC, +2 initiative, +1 strike/parry/dodge; bite 1D6 SDC; carry/exertion x2 human
  - Auto psionics: Sense Evil, Sense Magic, Sixth Sense, Empathy (receive only), +1 Sensitive (choice)
- **Available OCCs**: CS Grunt (half "other" skill selections) or Vagabond per canon; [others by GM]

### Mind Melter -- RCC
- **Type**: SDC (human)
- **Base Attributes**: Standard 3D6; IQ 10+ / ME 10+ strongly suggested, not required
- **Base Body Pools**: HP: P.E. + 1D6 (+1D6/level); SDC: 1D6 base (practitioner of magic roll; +OCC/skill bonuses); PPE: 2D4 (permanent); ISP: ME + 3D6x10, +10/level
- **Psionic Type**: Master (save: 10+)
- **Special Abilities**:
  - Full psionic access: all four categories including Super
  - Starting: Alter Aura (self), See Aura, Sixth Sense, Mind Block + 3 from each of Sensitive/Physical/Healing/Super
  - Restricted until L3: Mind Wipe, Psi-Sword, Possess Others
  - Ley line: +50% range/duration near line; x2 range/duration/damage within 1 mile of nexus; TK force field MDC x2 at nexus
  - ISP recovery: 2/hour active; 12/hour sleep or meditation
- **Available OCCs**: N/A (RCC)

### Dragon -- RCC
- **Type**: MDC (hatchling; skin is mega-damage substance, impervious to SDC weapons)
- **Base Attributes** (hatchling dice; mature ~600 years):
  - Great Horned Dragon: IQ 5D6, MA 4D6, ME 5D6, PS 6D6, PP 4D6, PE 5D6, PB 6D6, Spd 4D6
  - Fire Dragon: IQ 4D6, MA 4D6, ME 4D6, PS 5D6, PP 4D6, PE 5D6, PB 5D6, Spd 5D6
  - Ice Dragon: IQ 4D6, MA 4D6, ME 4D6, PS 5D6, PP 4D6, PE 5D6, PB 5D6, Spd 5D6
  - Thunder Lizard: IQ 4D6, MA 4D6, ME 5D6, PS 5D6, PP 4D6, PE 5D6, PB 4D6, Spd 6D6
- **Base Body Pools** (hatchling): Great Horned/Thunder Lizard 1D4x100+50 MDC; Fire/Ice 1D4x100 MDC; PPE/ISP varies by sub-race (see Dragon OCC below)
- **Special Abilities** (hatchling, common unless noted): Nightvision 90ft; see the invisible; metamorphosis (cat to max own size, 2 hr/level); teleport (5 miles, % roll varies by sub-race); bio-regenerate 1D4x10 MDC/5 min; sense ley lines/nexus/other dragons 20 miles (direction only); magic knowledge from L3 (2 spells/level); H2H Basic +1 attack. Sub-race: fly speed/breath weapon/resistances vary (Great Horned fly 70mph, fire breath 2D6 MD; Fire fly 50mph, impervious to fire, fire breath 6D6 MD; Ice fly 50mph, impervious to fire, ice breath 5D6 MD; Thunder Lizard no fly, resistant to fire/cold half damage)
- **Available OCCs**: N/A (RCC -- Dragon treated as OCC=None; race detection in rifts_start_d.c)

---

## 2. OCCS

Note: Most OCCs appear only in XP tables in this source. Full skill/gear detail exists only for Shifter, Techno-Wizard, and Mind Melter. XP level ranges included for developer reference.

### CS Grunt / Dog Pack
- **XP Range**: 0 - 375,600 (L15)
- **Skills** (from rifts_start_d.c, cs grunt / cs dead boy case):
  - WP Energy Rifle 70%, Military Etiquette 55%, Pilot Hovercraft 52%, Radio Basic 55%
  - H2H: Expert (melee/attack +5)
- **Starting Gear** (from rifts_start_d.c): dead_boy_armor, cp30_laser, 2x e_clip, military_radio
- **Credits**: 2D6 x 100 (200-1200)
- **PPE/ISP**: N/A unless Dog Pack RCC

### CS Technical Officer / CS Military Specialist / Shifter
- **XP Range**: 0 - 389,961 (L15)

#### Shifter (full detail)
- **OCC Skills**: Astronomy (+20%), Mathematics: Basic (+15%), Lore: Demon (+20%), Lore: Faerie (+15%), Language x2 (+15%), Land Navigation (+10%), Wilderness Survival (+5%)
- **H2H**: Must pick as "other" skill; Basic=1 pick, Expert=2, Martial Arts/Assassin=3
- **OCC Related Skills**: 6; +2 at L3, +2 at L6, +1 at L9, +1 at L12
- **Skill Restrictions**: No Mech; Medical: Holistic Medicine or Paramedic only; Physical: any except Boxing/Wrestling/Gymnastics; Pilot: any except Robot/Jets/Tanks
- **Secondary Skills**: 6 (no bonuses)
- **Starting Gear** (from rifts_start_d.c): knife, leather_jacket, ritual_components
- **Starting Credits**: 1D6 x 100 (per daemon); tabletop source says 1D6x1000 + 2D6x1000 black market
- **PPE**: No bonus in daemon; base PPE assigned at character creation (PE attribute + rolls)
- **ISP**: N/A
- **Cybernetics**: None; avoids; max 3 sensory/optic implants
- **OCC Spells** (from daemon do_occ_spells): sense evil, sense magic, dimensional pocket, call familiar

### Cyber-Knight / Juicer / Crazies
- **XP Range**: 0 - 401,700 (L15)

#### Cyber-Knight (from rifts_start_d.c)
- **Skills**: WP Sword 65%, WP Vibroblade 55%, Horsemanship 55%, H2H Cyber-Knight (melee/attack +6)
- **Starting Gear**: short_sword, knight_shield (NewCamelot domain), chain_mail, knife, backpack
- **Credits**: 2D6 x 100 (200-1200)
- **PPE/ISP**: No bonus in daemon (N/A)

#### Juicer (from rifts_start_d.c)
- **Skills**: WP Energy Rifle 65%, Athletics 50%, Acrobatics 50%, H2H Martial Arts (melee/attack +7)
- **Starting Gear**: environmental_body_armor, vibro_sword
- **Credits**: 1D6 x 100 (100-600)
- **PPE/ISP**: N/A

#### Crazy (from rifts_start_d.c)
- **Skills**: WP Energy Rifle 60%, WP Energy Pistol 60%, Athletics 50%, Acrobatics 55%, H2H Martial Arts
- **Starting Gear**: [default: leather_jacket, knife] -- no specific gear case in daemon
- **Credits**: 500 (default fallthrough)
- **PPE/ISP**: N/A

### Borg / Head Hunter / Glitter Boy
- **XP Range**: 0 - 402,600 (L15)

#### Borg (from rifts_start_d.c)
- **Skills**: [default: WP Energy Pistol 50%, WP Energy Rifle 50%, H2H Basic] -- no OCC skill case
- **Starting Gear**: full_borg_conversion bionic installed via BIONIC_D; built_in_mdc property 80
- **Credits**: 2D6 x 100 (200-1200)
- **PPE/ISP**: N/A

#### Head Hunter (from rifts_start_d.c)
- **Skills**: WP Energy Rifle 65%, WP Energy Pistol 60%, Military Etiquette 50%, Detect Ambush 45%, Tracking 40%, Wilderness Survival 40%, First Aid 50%, H2H Combat 3 (melee/attack +4)
- **Starting Gear**: c18_pistol, vibro_knife, environmental_body_armor, handcuffs
- **Credits**: 4D6 x 100 (400-2400)
- **PPE/ISP**: N/A

#### Glitter Boy Pilot (from rifts_start_d.c)
- **Skills**: Pilot Power Armor 72%, Weapon Systems 60%, WP Energy Rifle 65%, Radio Basic 55%, Military Etiquette 45%, First Aid 45%, H2H Expert (melee/attack +5)
- **Starting Gear**: GB_Glitter_Boy power armor, c18_pistol, vibro_knife, personal_radio
- **Credits**: 2D6 x 100 (200-1200)
- **PPE/ISP**: N/A

### CS RPA Elite / Body Fixer
- **XP Range**: 0 - 339,000 (L15)

#### CS RPA Elite (CS SAMAS RPA Pilot) (from rifts_start_d.c)
- **Skills**: Pilot Power Armor 70%, Weapon Systems 60%, WP Energy Rifle 65%, Radio Basic 60%, Military Etiquette 55%, Detect Ambush 40%, H2H Expert (melee/attack +5)
- **Starting Gear**: PA-06A_SAMAS power armor, cp30_laser, e_clip, military_radio
- **Credits**: 2D6 x 100 (200-1200)
- **PPE/ISP**: N/A

#### Body Fixer (from rifts_start_d.c)
- **Skills**: Medical Doctor 60%, First Aid 75%, Pathology 50%, H2H Basic (melee/attack +2)
- **Starting Gear**: knife, leather_jacket, first_aid_kit, medical_bag
- **Credits**: 3D6 x 100 (300-1800)
- **PPE/ISP**: N/A

### City Rat / Vagabond
- **XP Range**: 0 - 326,500 (L15)

#### City Rat (from rifts_start_d.c)
- **Skills**: Streetwise 46%, Pick Locks 30%, Pick Pockets 25%, Prowl 35%, Concealment 20%, H2H Basic
- **Starting Gear**: knife, leather_jacket, personal_radio
- **Credits**: 1D6 x 100 (100-600)
- **PPE/ISP**: N/A

#### Vagabond (from rifts_start_d.c)
- **Skills**: Streetwise 36%, Prowl 30%, Beg 40%, Disguise 30%, WP Energy Pistol 50%, H2H Basic
- **Starting Gear**: knife, leather_jacket, 2x rations, rope
- **Credits**: 1D4 x 100 (100-400)
- **PPE/ISP**: N/A

### Cyber-Doc / Rogue Scholar / Rogue Scientist
- **XP Range**: 0 - 389,500 (L15)

#### Cyber-Doc (from rifts_start_d.c)
- **Skills**: [default: WP Energy Pistol 50%, WP Energy Rifle 50%, H2H Basic] -- no OCC skill case in daemon
- **Starting Gear**: knife, leather_jacket, first_aid_kit, medical_bag
- **Credits**: 4D6 x 100 (400-2400)
- **PPE/ISP**: N/A

#### Rogue Scholar (from rifts_start_d.c)
- **Skills**: [default: WP Energy Pistol 50%, WP Energy Rifle 50%, H2H Basic] -- no OCC skill case in daemon
- **Starting Gear**: knife, leather_jacket, personal_radio, field_journal
- **Credits**: 2D6 x 100 (200-1200)
- **PPE/ISP**: N/A

#### Rogue Scientist (from rifts_start_d.c)
- **Skills**: [default: WP Energy Pistol 50%, WP Energy Rifle 50%, H2H Basic] -- no OCC skill case in daemon
- **Starting Gear**: knife, leather_jacket, science_kit
- **Credits**: 2D6 x 100 (200-1200)
- **PPE/ISP**: N/A

### Wilderness Scout / Operator
- **XP Range**: 0 - 328,000 (L15)

#### Wilderness Scout (from rifts_start_d.c)
- **Skills**: Wilderness Survival 65%, Track Animals 55%, Hunting 50%, Identify Plants 45%, Swimming 50%, Climbing 55%, Navigation 40%, WP Rifle 60%, Falconry 40%, H2H Combat 4 (melee/attack +5)
- **Starting Gear**: leather_jacket, vibro_knife, personal_radio, 2x rations, rope, trained_hawk
- **Credits**: 2D6 x 100 (200-1200)
- **PPE/ISP**: N/A

#### Operator (from rifts_start_d.c)
- **Skills**: Mechanical Engineer 50%, Electrical Engineer 45%, Pilot Hover Vehicle 52%, Salvage 45%, H2H Basic
- **Starting Gear**: c18_pistol, leather_jacket, 2x rations, tool_kit
- **Credits**: 3D6 x 100 (300-1800)
- **PPE/ISP**: N/A

### Burster / Psi-Stalker / Mystic
- **XP Range**: 0 - 400,600 (L15)

#### Burster (from rifts_start_d.c)
- **Skills**: [default: WP Energy Pistol 50%, WP Energy Rifle 50%, H2H Basic] -- no OCC skill case in daemon
- **Starting Gear**: [default: leather_jacket, knife] -- no gear case in daemon
- **Credits**: 500 (fixed default)
- **PPE/ISP**: ISP +20 from race psionics (do_race_psionics); no PPE bonus

#### Psi-Stalker (from rifts_start_d.c)
- **Skills**: [default: WP Energy Pistol 50%, WP Energy Rifle 50%, H2H Basic] -- no OCC skill case in daemon
- **Starting Gear**: [default: leather_jacket, knife] -- no gear case in daemon
- **Credits**: 500 (fixed default)
- **PPE/ISP**: ISP +20 from race psionics; psionics: mind block, object read, sense evil, sixth sense

#### Mystic (from rifts_start_d.c)
- **Skills**: Lore Magic 50%, Meditation 60%, H2H Basic
- **Starting Spells**: sense evil, sense magic, heal wounds, light target, globe of daylight, windrush, swimasafish
- **Starting Gear**: knife, leather_jacket
- **Credits**: 1D6 x 100 (100-600)
- **PPE/ISP**: PPE +20 bonus; ISP +30 bonus

### Mind Melter / Ley Line Walker
- **XP Range**: 0 - 395,920 (L15)

#### Mind Melter (full detail)
- **OCC Skills**: American + 2 languages (90%); 2 pilot skills (+5%); 6 "other" skills (+10%); H2H Basic (upgradeable: Expert=1 pick, Martial Arts/Assassin=2 picks)
- **Secondary Skills**: 8; +4 at L3, +4 at L8
- **Skill Restrictions**: Physical: any except Acrobatics/Wrestling; Science: Math only; no Electrical except Basic; no Mech except Automotive
- **Starting Gear**: Extensive clothing/wardrobe, light MDC armor only, sleeping bag, backpack, canteen, goggles, air filter/gas mask, knife, 1 week food rations; 1 energy weapon + 1 non-energy weapon (player choice); non-military vehicle (hover/jet pack/motorcycle/TW vehicle)
- **Starting Credits**: 4D6x100 credits + 2D4x1000 black market items
- **PPE**: 2D4 (spent on psionic development)
- **ISP**: ME + 3D6x10; +10/level; master psionic
- **Cybernetics**: None; avoids; medical only
- **OCC Skills (daemon, do_occ_skills)**: Detect Psionics 55%, Meditation 60%, H2H Basic
- **OCC Psionics (daemon, do_occ_spells)**: telepathy, empathy, sense evil, sixth sense, object read, mind block, telekinesis, bio-regeneration; ISP +50 bonus

#### Ley Line Walker (from rifts_start_d.c)
- **Skills**: Lore Magic 60%, Spell Knowledge 70%, H2H Basic
- **Starting Spells**: cloud of smoke, blinding flash, thunderclap, globe of daylight, levitation, magic net, energy bolt, sense evil, sense magic
- **Starting Gear**: knife, leather_jacket
- **Credits**: 1D6 x 100 (100-600)
- **PPE/ISP**: PPE +30 bonus; ISP N/A

### Techno-Wizard
- **XP Range**: 0 - 400,000 (L15)
- **Attribute Requirements**: IQ 12+, ME 12+
- **OCC Skills**: Literacy (+10%), Radio: Basic (+10%), Carpentry (+10%), Computer Operation (+5%), Computer Programming (+5%), Computer Repair (+10%), Basic Electronics (+15%), Automotive Mechanics (+10%), Read Sensory Equipment (+10%), Mathematics: Basic (+20%), Land Navigation (+5%), Language x2 (+15%)
- **H2H**: Must pick as "other" skill
- **OCC Related Skills**: 7; +2 at L3, +2 at L6, +1 at L9, +1 at L12
- **Skill Notes**: Electrical/Mechanical each count as 2 selections; Physical: any except Boxing/Wrestling/Acrobatics
- **Secondary Skills**: 5 (no bonuses)
- **Starting Gear**: Work overalls, jumpsuit, goggles, magnifying glass, flashlights (pocket + large), 6 flares, mini-tool kit, knapsacks, binoculars, air filter/gas mask, laser distancer, disc recorder, hand computer; light MDC armor (2 magic features, not invisibility/impervious-to-energy); magic energy rifle + pistol; wing board or tree trimmer; magic-converted vehicle (1 extra magic feature)
- **Starting Credits**: 1D6x100 credits + 1D4x1000 black market + 2D4x1000 in quartz crystals/gems
- **PPE**: 2D4x10 + PE attribute; +2D6/level; draws from ley lines/nexus as normal
- **ISP**: 4D6 + ME (minor psionic); +1D4/level; save vs psionics 12+
- **Psionic Powers**: Mind Block, Speed Reading, Total Recall, Telemechanics (fixed)
- **Starting Spells (L1)**: Blinding Flash, Globe of Day Light, Ignite Fire, Fuel Flame, Fire Bolt, Call Lightning, Energy Bolt, Energy Field, Impervious to Energy, Telekinesis; +3 spells L1-4 at L3
- **Spell Casting Penalty**: All ranges/durations/MDC/SDC/damage HALVED when cast as spell (not through TW device)
- **Magic Bonuses**: +2 save vs Horror Factor; +1 save vs magic at L3/L7/L10/L13; +1 spell strength at L4/L8/L12
- **Cybernetics**: None; avoids except medical

### Dragon
- **XP Range**: 0 - 800,000 (L15)
- **Skills** (race-based, from rifts_start_d.c do_occ_skills case "none" with dragon race):
  - Lore Magic 75%, Lore Demons & Monsters 65%, Fly 98%, H2H Dragon (melee/attack +9)
- **Starting Gear**: [default: leather_jacket, knife] -- no dragon-specific gear case in daemon
- **Credits**: 500 (fixed default)
- **PPE/ISP** (from rifts_start_d.c do_race_spells):
  - Great Horned Dragon: 200 PPE base; +40 ISP; spells L1-5 (globe of daylight, blinding flash, cloud of smoke, levitation, magic net, thunderclap, fire ball, invisibility simple, carpet of adhesion, fire bolt, armor of ithan, energy field, shadow meld, fly, impervious to energy)
  - Fire/Ice/Thunder Lizard Dragon: 100 PPE base; spells (globe of daylight, blinding flash, cloud of smoke, levitation, magic net, fire ball, fire bolt, armor of ithan, fly)
  - Dragon Hatchling: 40 PPE base; spells (globe of daylight, blinding flash, cloud of smoke, levitation)
- **Note**: Dragon stats (Type, Attributes, Body Pools, Special Abilities) not found in daemon files; check rifts.c or rifts_races.c

---

## 3. SKILLS

Skill base percentages from rifts_skills.c (start value). All advance at +5%/level unless noted.
H2H skills are not tracked as a percentage; they grant melee/attack bonuses (see do_hth values in rifts_start_d.c).
"bonus ability" means the skill grants combat or attribute bonuses rather than a percentage roll.

Dog Pack military skills (base % from rifts_skills.c + OCC bonus):

| Name | Base % | OCC Bonus (Dog Pack) | Category |
|------|--------|---------------------|----------|
| Intelligence | 32% | +6% | Espionage |
| Radio: Basic | 45% | +10% | Communications |
| Pilot Hovercraft | 50% | +10% | Pilot |
| Read Sensory Equipment | 30% | +10% | Technical |
| Weapon Systems | 40% | +10% | Military |
| Climbing | 40% | +10% | Physical |
| Running | bonus | -- | Physical |
| Land Navigation | 36% | +10% | Wilderness |
| Wilderness Survival | 40% | +10% | Wilderness |
| W.P. Energy Pistol | 10% | -- | W.P. |
| W.P. Energy Rifle | 10% | -- | W.P. |
| W.P. (choice) | 10% | -- | W.P. |
| Hand to Hand: Martial Arts | bonus | -- | Physical |

Additional skills referenced (base % from rifts_skills.c + core rulebook):

| Name | Base % | Category | Notes |
|------|--------|----------|-------|
| Anthropology | 20% | Science | -- |
| Archaeology | 20% | Science | -- |
| Art | 35% | Technical | -- |
| Astronomy | 25% | Science | -- |
| Automotive Mechanics | 25% | Mechanical | Standard Palladium mechanical skill base |
| Basic Electronics | 30% | Electrical | -- |
| Biology | 30% | Science | -- |
| Botany | 25% | Science | -- |
| Boxing | bonus ability | Physical | -- |
| Carpentry | 25% | Mechanical | Standard Palladium mechanical skill base |
| Chemistry | 30% | Science | -- |
| Chemistry Analytical | 25% | Science | -- |
| Computer Hacking | 15% | Rogue | -- |
| Computer Operation | 40% | Technical | -- |
| Computer Programming | 30% | Technical | -- |
| Computer Repair | 35% | Electrical | -- |
| Concealment | 20% | Rogue | -- |
| Gymnastics | 50% | Physical | -- |
| Hand to Hand: Basic | bonus ability | Physical | -- |
| Hand to Hand: Expert | bonus ability | Physical | -- |
| Hand to Hand: Assassin | bonus ability | Physical | -- |
| Holistic Medicine | 35% | Medical | -- |
| Language (any) | 50% | Technical | -- |
| Literacy | 60% (base grant) | Technical | -- |
| Lore: Demon | 25% | Technical | -- |
| Lore: Faerie | 25% | Technical | -- |
| Mathematics: Advanced | 45% | Science | -- |
| Mathematics: Basic | 45% (base grant) | Science | -- |
| Navigation | 40% | Pilot Related | -- |
| Palming | 20% | Rogue | -- |
| Paramedic | 40% | Medical | -- |
| Photography | 35% | Technical | -- |
| Pick Locks | 30% | Rogue | -- |
| Pick Pockets | 25% | Rogue | -- |
| Pilot Robot | 60% | Pilot | -- |
| Prowl | 25% | Rogue | -- |
| Robot Combat: Basic | bonus ability | Military | +1 strike/parry at L2; +1 attack at L6/L12; body block 1D6 MD |
| Robot Combat: Elite | bonus ability | Military | Varies by platform; see combat training tables |
| Streetwise | 20% | Rogue | -- |
| Surveillance Systems | 30% | Communications | -- |
| TV/Video | 30% | Communications | Standard Palladium communications base |
| Wilderness Survival | 40% | Wilderness | -- |
| Wrestling | bonus ability | Physical | No % roll; grants combat bonuses only |
| Writing | 25% | Technical | -- |
| W.P. Archery and Targeting | bonus ability | W.P. Ancient | -- |

---

## 4. SPELLS

Spell levels: filled below from rifts_spells_d.c level comment tags. Spells without level tags in daemon marked with *.
PPE costs from chart source retained as-is; cross-reference daemon for actual cost.

| Name | Level | PPE Cost | Effect (one line) |
|------|-------|---------|-------------------|
| Armor of Ithan | 3 | 10 | Force field MDC shell around target |
| Befuddle | 2 | 3 | Confused/disoriented; -2 strike/parry/dodge; half attacks; -20% skills; 2 min/level; 1 target/100ft; save vs magic |
| Blinding Flash | 1 | 1 | Bright flash; temporary blindness |
| Breathe Without Air | 3 | 5 | Self can breathe in airless/toxic environment |
| Call Lightning | 6 | 15 | Lightning bolt; energy damage |
| Chameleon | 2 | 6 | Blend with surroundings when motionless |
| Detect Concealment | 2 | 6 | Negates concealment spells; reveals mystically hidden objects in 30ft area |
| Dimensional Portal | 15 | 1000 | Open rift to another dimension; 30 sec/level (2 min/level ritual); 1000+ lbs/level can pass |
| Energy Bolt | * | 5 | Energy projectile attack (in daemon but no level tag) |
| Energy Field | 4 | 10 | Protective energy barrier |
| Escape | 5 | 8 | Slip free of bonds or restraints |
| Fire Bolt | 3 | 7 | Fire projectile attack |
| Fly as the Eagle | 10 | 25 | Flight at 100 mph; 30 min/level; self or one other |
| Fuel Flame | 3 | 5 | Double fire size in 10ft area |
| Globe of Day Light | * | 2 | Creates sustained light source (in daemon but no level tag) |
| Globe of Silence | 7 | 20 | 10ft radius absorbs all sound; 6 melees/level; referenced in TW Psionic Mind Shield |
| Ignite Fire | 4 | 6 | Start fire on combustible material |
| Impervious to Energy | 5 | 20 | Immune to energy; still vulnerable to physical/magic/rail guns/explosives |
| Impervious to Fire | 3 | 5 | Immune to fire damage |
| Invisibility Superior | 7 | 20 | Full invisibility (daemon has Invisibility Simple at L2 only) |
| Invulnerability | 7 | 25 | Temporary invulnerability (daemon has Invincible Armor at L7; may be different spell) |
| Levitation | 1 | 5 | Float/rise vertically |
| Life Drain | 10 | 25 | Drain 4D6 HP; caster gains half; save 16 PE |
| Magic Net | 1 | 7 | Ensnare target; range 200ft |
| Mystic Portal | 7 | 60 | Create a traversable portal |
| Negation | 8 | 30 | Dispel/cancel active magic (core rulebook: Negate Magic; TW devices reference as Negation) |
| See Aura | 1 | 6 | Reveal aura properties (in daemon as psionic, not spell; may be psionic-only) |
| See the Invisible | 1 | 4 | See invisible entities (in daemon as psionic, not spell; may be psionic-only) |
| Shadow Meld | 4 | 10 | Merge with shadows; become hidden |
| Superhuman Speed | 5 | 10 | Enhanced movement speed |
| Superhuman Strength | 5 | 10 | Enhanced physical strength |
| Talisman | 13 | 500 | Empower object with one spell (levels 1-8); 3 uses; rechargeable at 50 PPE + spell cost per charge |
| Telekinesis | 3 | 8 | Move objects with mental force |
| Teleport: Superior | 15 | 600 | Teleport self + up to 1000 lbs/level; range 300 miles/level; familiar location 99%, described 58%, unknown 20% |
| Tongues | 3 | 12 | Instant comprehension/speech of any language |
| Wind Rush | * | 20 | Air/wind manipulation (in daemon as "windrush" but no level tag) |

---

## 5. PSIONICS

**ISP Recovery**: 2/hour active; 6/hour meditation (= 2 hours sleep equivalent); 12/hour sleep
**Saves**: Non-psionic: 15+; Minor/Major psionic: 12+; Master psionic: 10+
**Ley Line Effect**: +50% range/duration within 2 miles of line; x2 range/duration within 1 mile of nexus; energy attack damage x2 at nexus; TK force field MDC x2 at nexus
**Attacks per melee**: Equal to physical H2H attacks (2-5 typically)

### Sensitive Psionics

| Name | ISP | Range | Effect |
|------|-----|-------|--------|
| Empathy | 4 | 100ft | Receive emotional state from others; 2 min/level; save vs magic if unwilling; "receiver only" for Dog Pack |
| Mind Block | 4/period | Self | Block all psi/mental; +1 save vs psi/mental; 10 min/level |
| Object Read | 6 (+4 present) | Touch | Impressions 56%+2%/level; Images 48%+2%/level; Present 38%+2%/level; duration 2D6 min |
| Presence Sense | 4 | 120ft area | Sense supernatural/magic presence; near (<50ft) or far (>90ft); number estimate; 2 min/level |
| See Aura | 6 | 60ft (LOS) | XP level estimate (low/med/high); magic present; psionic present; PPE level; possessing entity; aberration; duration 2 melees |
| See the Invisible | 4 | 120ft | See invisible/naturally-invisible creatures and entities; 1 min/level |
| Sense Evil | 2 | 140ft area | Supernatural evil: number (1/few/several/many), intensity, location (15ft/50ft/60-140ft); 2 min/level |
| Sense Magic | 3 | 120ft area | Near (<20ft) or far (120ft); pinpoint source; detect enchanted items; detect active spellcasting; 2 min/level |
| Sixth Sense | 2 | 90ft | Auto-trigger only; imminent life threat within 60 sec; +6 initiative, +2 parry, +3 dodge; no sneak attack surprise |
| Speed Reading | 2 | Self | 30 pages/min; 15/min for technical texts; 3 min/level |
| Telepathy | 4 | 60ft (read) / 140ft (send) | Read surface thoughts 1 person at a time OR send brief message; 2-way only between two telepaths; save conditional; 2 min/level |
| Total Recall | 2 | Self | Perfect permanent memory of all text read; recall costs 3 ISP for exact detail |

### Physical Psionics

| Name | ISP | Range | Effect |
|------|-----|-------|--------|
| Alter Aura | 2 | Self | Alter own aura appearance/properties |
| Death Trance | 1 | Self | Appear dead; slows metabolism |
| Ectoplasm | Varies | Varies | Create ectoplasmic matter |
| Impervious to Cold | 2 | Self | Resist cold damage |
| Impervious to Fire | 4 | Self | Resist fire damage |
| Impervious to Poison | 4 | Self | Resist poison/toxins |
| Levitation | Varies | Self | Float vertically |
| Mind Block | 4/period | Self | Block psi/mental; +1 save vs psi/mental; 10 min/level |
| Nightvision | 4 | Self | See in darkness |
| Resist Fatigue | 4 | Self | Resist fatigue/exhaustion |
| Resist Hunger | 2 | Self | Resist hunger |
| Resist Thirst | 6 | Self | Resist thirst |
| Summon Inner Strength | 4 | Self | Temporary attribute/endurance boost |
| Telekinesis | Varies | Varies | Move objects with mental force (physical category) |

### Healing Psionics

| Name | ISP | Range | Effect |
|------|-----|-------|--------|
| Bio-Regenerate (self) | 6 | Self | Heal self 2D6 HP or 3D6 SDC after 1 min concentration; no scarring; every other minute |
| Deaden Pain | 4 | Touch / 3ft | Negate pain or anesthetic; 1 hr/level; 2 min trance |
| Detect Psionics | 6 | Self (30ft/level) | Sense psionic presence; weak/medium/powerful; 1 min/level |
| Exorcism | 10 | Touch / 8ft | Expel possessing entity; 30 min prep + 6D6 min rite |
| Healing Touch | 6 | Touch | Restore 1D8 HP or 2D6 SDC to another; 2 min trance |
| Increased Healing | 10 | Touch / 3ft | Double HP/SDC recovery rate for 2D4 days; 1D6 hr trance |
| Induce Sleep | 4 | Touch / 6ft | Restful sleep 1 hr/level; save +5 if unwilling/in combat |
| Psychic Diagnosis | 4 | Touch / 3ft | Pinpoint pain, disease, possession |
| Psychic Purification | 8 | Touch | Destroy drugs/poison/toxins in body; 6D6 min trance |
| Psychic Surgery | 14 | Touch | Repair serious internal injury (see core rulebook) |
| Resist Fatigue | 4 | Self | Resist fatigue/exhaustion |

### Super Psionics

| Name | ISP | Range | Effect |
|------|-----|-------|--------|
| Bio-Manipulation | 10 (+6 extend 4D4 min) | 160ft | One target per attack; 4D4 min; save standard. Blind: -9 strike/parry/dodge. Deaf: -6 parry/dodge rear, -3 all, lose initiative. Mute: -2 first melee. Pain: -6 strike/parry/dodge, 1 HP/min. Paralysis: incapacitated. Stun: -4 strike/parry/dodge, lose 1 attack/melee, half speed. Tissue: -1 strike/parry/dodge |
| Bio-Regeneration | 20 | Self | 4D6 HP + 4D6 SDC restored after 60 sec concentration; permanent |
| Electrokinesis | Varies | Varies | 1) Resist (4 ISP/3 min per level, 60kV=no damage, >60kV=half, self). 2) Discharge (2 ISP, touch/2ft, 1D6, instant). 3) Manipulate devices (4 ISP, 45ft+5ft/level, 12 functions/melee, 2 min/level). 4) Sense electricity (2 ISP/2min, 45ft+5ft/level, 55%+5%/level) |
| Empathic Transmission | 6 | 60ft | 1 target per attack; 2D6 min; save standard. Despair: 50% surrender, -2 parry/dodge. Confusion: -3 strike/parry/dodge, lose initiative. Fear: -3 strike/parry/dodge, 66% flee. Hate: +1 strike, -1 parry/dodge, 60% attack allies. Love: 60% halt hostility (not docile). Trust: believe psionic; +3 save on life-threat suggestions |
| Group Mind Block | 22 | 120ft area | Mind block all in area; undetectable without detect psionics; 10 min/level |
| Hydrokinesis | Varies | Varies | 1) Sense impurities (2 ISP/min, 6in, 70%+5%/level pure, 35%+5%/level type). 2) Boil 1 gal (3 ISP/gal, 8ft+2ft/level, 1 min). 3) Water spout (5 ISP, 20ft, 1 gal/level; boiling face: 2D4 damage, lose initiative/all attacks 1D6 melees, blind 3D6 melees). 4) Sense open water (free, 20ft, permanent; no underground/sealed) |
| Hypnotic Suggestion | 6 | 12ft (eye contact) | Calm/sleep/memory focus/implant idea/imagined sense; save standard; hours max |
| Mentally Possess Others | 30 | Touch | Full body control 5 min/level; caster enters coma; victim in trance, no memory; caster can return any time; save standard |
| Mind Block Auto-Defense | 14 (permanent) | Self | Auto mind block vs empathy/telepathy probes; alerts psionic; 14 ISP permanently removed on selection; no ongoing cost |
| Mind Bolt | Varies | 100ft/level | 6 ISP=1D6 SDC; 12 ISP=3D6 SDC; 20 ISP=6D6 SDC; 40 ISP=2D4 MD; +4 strike; +8 strike with additional 10 ISP; instant |
| Mind Bond | 10 | Touch | Both learn everything about each other (skills, secrets, phobias); full recall 3D4 hours; 15% recall per item after; gone at 1 month; alien/disturbed minds risk permanent insanity (roll insanity table); save standard |
| Mind Wipe | Varies | Touch (temples) | 1D4 days per 10 ISP expended; permanent: 50 ISP; full blank: 50 ISP + 4 ME permanent loss; 3 min process; save standard |
| P.P.E. Shield | 10 | Self | Block PPE drain from P.P.E. vampires/psi-stalkers/mages; 6 min/level |
| Psi-Shield | 30 | Self | 80 MDC shield; parry all H2H/robot/power armor/magic attacks; cannot parry energy blasts or projectiles; 5 min/level |
| Psi-Sword | 30 | Self | MD energy sword; L3: 4D6 MD; +2D6 at L4/L7/L9/L12/L15; adjustable from 2D6 to current max in 1D6 increments; 5 min/level; 15 sec (1 melee) to create; disappears if KO |
| Pyrokinesis | Varies | Varies | 1) Resist (2 ISP, self, half damage, 5 min/level). 2) Spontaneous combustion (2 ISP, 30ft, ignite combustibles). 3) Fuel flame (4 ISP, 30ft+5ft/level, double 10ft area). 4) Extinguish (4 ISP, 30ft+5ft/level, 15ft area). 5a) Create pillar/wall (20 ISP, 4D6 pillar or 6D6 wall, 72% ignite, 30ft+2ft/level). 5b) Fireball (25 ISP, 6D6, 30ft+2ft/level, +2 strike, instant). 6) Sense fire (2 ISP/2min, 100ft+5ft/level, 90%) |
| Telekinesis (Super) | 10+/100 lbs | 100ft/level | 10 ISP per 100 lbs; damage 1D4x10 SDC per 100 lbs (100 SDC = 1 MD); multi-objects: 1 per level (combined weight limit applies); 2 min/level |
| Telekinetic Force Field | 30 | Self or 40ft/level | Bubble up to 10ft area/level; 25 MDC/level; 10 min/level; can parry if erected before strike; multiple fields allowed (cost per); vanishes if KO |
| Telemechanics | 10 | Touch / 5ft | Non-AI machine: 80% skill (full schematic/operation). AI/computer: 88% skill + direct telepathic access to memory; 10 min + 2 min/level |

---

## 6. ALIGNMENTS

| Name        | Numeric Value | Description                                      |
|-------------|---------------|--------------------------------------------------|
| Principled  | 900           | Always keeps word; never harms innocent; respects law |
| Scrupulous  | 600           | Values life; lies only to evil; bends law for greater good |
| Unprincipled| 200           | Selfish but not evil; never harms innocent; never kills for pleasure |
| Anarchist   | -100          | Self-gratification first; lies/cheats; won't kidnap/kill innocents |
| Miscreant   | -500          | Self-serving; lies/cheats all; harms innocents; no loyalty |
| Aberrant    | -900          | Evil with personal code; keeps word; torture for info only |
| Diabolic    | -1200         | No honor; kills innocents for pleasure; betrays everyone |

---

## 7. INSANITIES

### Random Insanity Table (d100)
| Roll | Result |
|------|--------|
| 1-10 | Alignment reversal (good->evil, evil->good) |
| 11-20 | Neurosis (roll neurosis sub-table) |
| 21-25 | Compulsive liar |
| 26-30 | Kleptomaniac |
| 31-40 | Obsession (roll obsession sub-table) |
| 41-50 | Phobia (roll phobia sub-table) |
| 51-60 | Psychosis (roll psychosis sub-table) |
| 61-70 | Fear of Animals (neurosis, no sub-roll needed) |
| 71-80 | Recluse (prefers isolation) |
| 81-90 | Affective Disorder (roll affective disorder sub-table) |
| 91-00 | Fear of Dark (neurosis, no sub-roll needed) |

### Affective Disorders (d100)
| Roll | Mechanical Effect |
|------|-----------------|
| 1-19 | Loud noises: cower and freeze [no numeric penalty listed in source] |
| 20-35 | Sticky textures: avoid at any length [no numeric penalty] |
| 36-54 | Cleanliness compulsion: must clean area [no numeric penalty] |
| 55-75 | Violence outrage: 72% berserk vs perpetrator; while berserk: +1 strike, +2 damage |
| 76-88 | Music hatred: attempt to destroy source [no numeric penalty] |
| 89-00 | Language intimidation: cannot speak; sign language or written only |

### Neurosis (d100)
| Roll | Mechanical Effect |
|------|-----------------|
| 1-18 | Fear of Dark: gibbering collapse in darkness [incapacitated; no specific combat penalty listed] |
| 19-34 | Fear of Animals: flee from small furry creatures [no numeric penalty beyond panic] |
| 35-49 | Compulsive liar: cannot tell truth [behavioral only] |
| 50-64 | Paranoid (Invasion): acute paranoia vs supernatural/alien/off-world humans [behavioral] |
| 65-85 | Fear of Heights: paralyzed above 2nd story; fine if ground not visible |
| 86-00 | Fear of Success: -15% all skills, -3 to strike/dodge/parry/damage in high-pressure moments |

### Psychosis (d100)
| Roll | Mechanical Effect |
|------|-----------------|
| 1-15 | Hysterical Blindness under pressure: 1-89% chance per high-pressure situation |
| 16-28 | Paranoid: trusts no one [behavioral] |
| 29-49 | Manic-Depressive: depression week (-5% all skills); mania week (+5% all skills); 30% develop alcoholism |
| 50-73 | Schizophrenia: passive/jumpy; hears voices; 50% develop alcoholism or drug addiction |
| 74-94 | Mindless Aggression (semi-functional): 72% berserk when frustrated/angry/upset; 3-18 min confinement to recover |
| 95-00 | Mindless Aggression (non-functional): continuous berserk; 1 lucid day/week; talk way out of confinement |
| 86-00 | (on psychosis sub-table) Psychiatrist obsession: treat everyone; demand fees [behavioral] |

Note: Entries 74-85 and 86-00 in source psychosis table overlap due to source formatting; see raw table at source lines 803-814.

### Phobia Panic Response (d100, on encounter)
| Roll | Mechanical Effect |
|------|-----------------|
| -- | 40% chance companions can prevent panic if nearby and calming |
| 1-25 | Pass out 2D4 minutes; unconscious |
| 26-80 | Flee at top speed; ignores companions; fights only to escape; cannot stop until escaped |
| 81-00 | Paralyzed; cannot move/fight/use skills/dodge; cry/whimper/scream; until source removed or dragged away |

### Phobia List (d100)
Undead, Reptiles, Scientists, Mutants, Snakes, Ghosts, Confining Enclosures, Skeletons, Darkness, Graveyards, Abandoned Buildings, Giant Creatures, Basements, Corpses, Users of Magic, Spiders, Tombs, Cats, Heights, Dogs, Contamination, Psychic Strangers, Insects, Flying, Bats, Water, Computers, Psychic Healing, Open Spaces, Supernatural Entities

### Obsessions
- Roll 1-50: Love/Desire; 51-00: Hate/Destroy
- Foci (d100): Timeliness, High-tech, Women/Men, Wealth, Secrecy, Specific individual, Specific object/animal, Appearance, Danger, Food, Alcohol, Gambling, Solitude, Crime-busting

### Insanity Cure Mechanics
| Type | Minimum Therapy | Results |
|------|----------------|---------|
| Affective Disorder/Neurosis | 3 months | 1-29 no effect; 30-69 half cured (48% recurrence); 70-00 total cure |
| Psychosis | 6 months | 1-33 no effect; 34-68 becomes phobia; 69-00 total cure |
| Phobia | 3 months exposure | 1-39 no effect; 40-88 lessened (functional); 89-00 intensified (50% paralysis on encounter) |

---

## 8. DRUG/ALCOHOL EFFECTS

### Alcohol: Intoxicated Personality (d100)
| Roll | Mechanical Effect |
|------|-----------------|
| 1-10 | +2 damage |
| 11-20 | +1 initiative, -1 parry, -1 dodge |
| 21-30 | +2 initiative |
| 31-40 | -2% all skills |
| 41-50 | +1 initiative |
| 51-60 | -5% all skills |
| 61-70 | -2% all skills when SOBER |
| 71-80 | -5% all skills |
| 81-90 | -2 initiative |
| 91-00 | INTOXICATED: +5% all skills; SOBER: -10% all skills |

### Alcohol: Totally Drunk (25% of waking time)
- -4 initiative
- -2 strike, -2 parry, -2 dodge
- Speed x0.5
- -12% all skills

### Alcohol Withdrawal
| Period | Mechanical Penalties | Relapse % |
|--------|---------------------|-----------|
| Week 1 | -5% all skills, -2 initiative, -2 strike, -2 parry, -2 dodge | 40% |
| Week 2 | -2% all skills, -2 initiative, -1 strike, -1 parry, -1 dodge | 20% |
| Weeks 3-6 | -2 initiative | 13%/week |
| Months 1-6 | No penalty listed | 15% under extreme pressure |
| After 6 months | No penalty | 5% under extreme pressure; 43% re-addiction per drink |

---

### Drugs: Intoxicated Personality (d100)
| Roll | Mechanical Effect |
|------|-----------------|
| 1-10 | +1 initiative |
| 11-20 | -5 parry, -5 dodge, -5 roll with punch/fall |
| 21-30 | +1 initiative |
| 31-40 | -4 parry, -4 dodge, -2 strike |
| 41-50 | -4 parry, -4 dodge; 50% friend/foe confusion; may not recognize danger |
| 51-60 | -6% all skills |
| 61-70 | -4% all skills when NOT on drug |
| 71-80 | -8% all skills |
| 81-85 | 43% hallucination flashback under pressure (same effects as 41-50) |
| 86-91 | -2 initiative, -2% all skills |
| 92-00 | HIGH: +1 strike, +1 parry, +1 dodge, +6% all skills; SOBER: -1 strike, -1 parry, -1 dodge, -12% all skills |

### Drug General Notes
- Onset: 15-20 minutes
- Duration: 45 min to 2 hours
- Lingers in system: 48 hours after high

### Drug Withdrawal (Cold Turkey)
| Period | Mechanical Penalties | Relapse % if Drug Available |
|--------|---------------------|----------------------------|
| Week 1 | -35% all skills, -8 all combat skills | 75% |
| Week 2 | -20% all skills, -4 all combat skills | 50% |
| Week 3 | -10% all skills, -2 strike, -2 parry, -2 dodge | 28% |
| Week 4 | Detoxified; psychological addiction only | 14% |
| Months 1-6 | No stat penalty | 10% under extreme pressure |
| After 6 months | No stat penalty | 3% under extreme pressure |

---

## 9. MUD ADAPTATION NOTES
Source: daemon code review. These are gaps or conversion decisions for real-time MUD play.

### a. APM (Actions Per Melee)
Tabletop uses 15-second melee rounds; APM of 2-9 depending on H2H type (rifts_start_d.c do_hth values: basic=2, expert=5, combat3=4, combat4=5, martial arts=7, cyber-knight=6, commando=6, dragon=9, vampire=8).
Suggested MUD conversion: divide 15 seconds by APM to get seconds-per-action (H2H basic = 7.5s, martial arts = ~2s). Needs implementation in rifts_combat.c as a per-character action timer.

### b. Skill Percentages (Per-Level Advancement)
rifts_skills.c stores "per_level" value (typically 5%) for every tracked skill.
Per-level skill advancement is NOT automatically applied at level-up in any daemon reviewed; a level-up hook in user.c or a skills daemon needs to iterate player skills and apply per_level increments.

### c. PPE Regeneration
No PPE regen heartbeat found in any daemon reviewed.
Tabletop rate: 2 PPE/hour rest, 10 PPE/hour sleep, doubled near ley line. Needs a periodic regen daemon or heartbeat hook tied to player rest state.

### d. ISP Regeneration
No ISP regen heartbeat found. Mind Melter source note in the race section states 2 ISP/hour active, 12 ISP/hour sleep.
Same implementation needed as PPE regen; can share one heartbeat that checks both pools and player state.

### e. MDC Damage Scale
MDC creature system exists (mdc_creature property, is_rifts_race() check in spell code, MDC vs HP path in all damage handlers reviewed).
Whether combat enforces that SDC weapons deal no meaningful MDC damage is not confirmed from daemon files reviewed; check rifts_combat.c. Without enforcement, all weapons deal equal damage regardless of MDC/SDC type -- a significant balance gap.

### f. Coalition Alignment Restrictions
Alignment system is fully implemented (rifts_alignment env var, 7-tier numeric system).
No OCC-specific alignment restriction enforcement found in daemon files reviewed. CS soldiers should be restricted to Principled/Scrupulous/Anarchist at creation; check rifts_occ.c or the setter flow for where to add this gate.
