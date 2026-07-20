# RiftsMUD Memories and Brainstormed Ideas

**This is reference material only. Nothing in this file is an active
task.** It is a record of raw player/staff memories about the original
RiftsMUD (the predecessor to AetherMUD) plus some brainstormed future
feature ideas, organized into sections for easier lookup. Future
sessions should treat this as background context to consult when
making content or design decisions, not as a backlog or to-do list.
Source: verbal memories recorded in
`nightmare3_fluffos_v2/RiftsMUD Memories.txt` (the historical archive
file referenced in CLAUDE.md), reorganized here by topic.

## 1. Visual/UI conventions

- Room exits were bold green. Very little color throughout room
  descriptions otherwise.
- The `score` command was wrapped in an ASCII border.
- Status effects shown in color on the status display: thirst being
  quenched was blue; there was a similar one for having done drugs,
  which would say "stoned" in blue.
- The only other places color showed up: green from the radio, red/
  white from OOC, and blue from tells.
- There used to be multiple `score` commands.
- There was also a separate `equipment` command that showed what you
  were wearing/had equipped.
- HUDs were very similar to the Dead Souls ones, but simpler: just a
  green line like "A Glitterboy power armor is standing here." etc.

## 2. Race/RCC mechanics

- Great Horned Dragons had MDC only, not HP + SDC.
- Great Horned Dragons could `metamorph human` or `metamorph <race>` to
  change shape and appearance, and this let them fit into human sized
  armor.

## 3. Magic/psionics

- Psionics included telepathy.
- Spells included swimasafish and windrush.
- There was a hydra enemy you could cast windrush on, and they would
  end up in another room, letting you stay behind. That destination
  room had a rock you could push to reveal a jagged obsidian blade (a
  very strong sword). There was also a metal hilt in the same vein:
  when charged with PPE it would produce a flame that lasted for a
  duration then go out, and it was also a strong sword.

## 4. Notable unique items and locations

- The jagged obsidian blade was behind a pushable rock, in the room a
  hydra ends up in after being hit with windrush.
- A metal hilt sword: charging it with PPE makes a flame appear on it
  for a duration, then the flame goes out. Also a strong sword.
- The ghostly katana of soul slaying loads in the catacombs (same as
  the obsidian blade's area). It ignores armor and does MD or SDC,
  depending on the target, directly to the body.
- The Sword of Atlantis was in the ocean rift.
- In Splynn, in the bar, Rocky had a 100md rechargeable (with PPE)
  armor talisman. You could activate it in inventory without wearing
  it, and it worked on every race except Hawrk-duhk and those like it.
- Also in Splynn: go south like you're going to Rocky's, then all
  west, then 1 north, then knock on the pillar. This opens up the
  black market, where you can buy predator type armors that make you
  invisible, net guns, etc.

## 5. Pet/companion systems

- Falconry: if you had the falconry skill and traded the right item to
  a shopkeeper, they would give you a pet hawk.
- The hawk would follow you and eat the corpses of enemies you killed.
- It would listen to say commands depending on your falconry skill
  level: at 98% it would almost always obey; at 65% it wouldn't always
  and would just squawk.

## 6. Moxim and the rift travel system

- Moxim was an NPC that would open rifts between the 3 areas.
- You had to manually enter the rifts; there was a `rift <destination>`
  command.
- You handed Moxim credits with a command instead of saying something
  to him, and it worked great: essentially `rift <target>` and he took
  the money.
- Moxim would utter something and open a rift; you could enter it, and
  someone else could enter it too.
- The rift would collapse after a short duration.

## 7. Recovery/healing pacing by race

- Juicers can heal in a few minutes.
- Dragons heal in like an hour or two.
- Humans heal slow as hell (slow af).

## 8. Chargen/skill philosophy

- The way RiftsMUD worked: you'd pick your race and/or class, and
  everything you automatically came with (so long as it was coded into
  the game) got immediately added.
- Then you had to wait for a wizard, and go through lists to see what
  else you could have, etc.
- Noted want: a nice, clean skill picker that would just list the
  categories, and only display what you can actually have, would be
  simple and preferable to the old wizard-review process.

## 9. Project history/origin notes

- It was frustrating that Chris couldn't figure out how to update the
  driver on a MUD he supposedly wrote the majority of the code for;
  it took over a month for what would be a FluffOS update that would
  take Tsath or Malc a few minutes, or take the note-taker a few hours
  to a day or two, tops.
- Turns out Chris did about 30-35% of the conversion; he had a head
  coder who modified everything, and Chris mostly did the stuff the
  coding wizards did and followed the examples for the most part.
- Macgyver did the vast majority of the difficult work, including the
  OCC and RCC conversions, MDC, etc.

## 10. Brainstormed future feature ideas

- An ability for smugglers that lets them convert credits back and
  forth between regular and black market credits, letting them set an
  exchange rate they can improve based on level. Could add a menu
  based system so they could offer it as a paid service.
- Adding more secret classes similar to Sunaj that you can unlock for
  your account by doing certain things in the game.
- Modifying either makeroom.c or the room creation tool, and wiring it
  to a mob for automation of Merc group base generation.
- Probably the most difficult idea, but adding some of the creation
  tables to the game like Merc group creation, and making cyborgs and
  androids and such, adding the juicer process in game like it is in
  the books.
- Eventually some way to start your OCC at level 1 and pick a new one,
  maybe with a flag added to the pfile to prevent players from doing it
  twice. Would probably need heavy testing to prevent too much power
  creep.

## 11. Reference: original RiftsMUD marketing copy and world/area descriptions

Preserved as historical/flavor reference, not as a to-do list.

### Welcome text

Welcome to RiftsMUD 2.1. RiftsMUD is a multi-user dungeon based on
Palladium Books' Rifts role-playing game. It allows players to select
from over 50 races, 50 Occupational Character Classes, 150 skills, and
countless magic spells, psionics, and abilities. The game includes
locations such as Chi-town, New Camelot, Splynn, and other worlds.
Come see why RiftsMUD is the longest running and best Rifts based MUD
on the web!

The Third World War has crippled the earth, killing millions. Their
energy pours back into the earth erupting the magical rivers ley lines
with devastating consequences. Where these lines cross, nexus points
open destructive rifts, gateways to unknown lands. These mystic
portals unleash the nightmarish monsters of thousands of worlds upon
our homeland. Technology and magic now clash for the fate of humanity
in this apocalyptic landscape.

RiftsMUD is currently running on the mighty server Tonster thanks to
Arzach. The lib is custom coded by the administrator.

### The Americas

The Americas consist of the present day North America, Central
America, and South America. North America is home to two large
conflicting forces: the Federation of Magic and the Coalition States.
Outside these territories lie vast amounts of thick wilderness and
danger. Blood thirsty vampires rule most of Mexico. Moving into South
America, think jungles, swamps and mountains make up most of the
terrain. A few pockets of civilization scattered throughout the
continent. Rare 20th Century technology makes for good yet dangerous
treasure hunting.

(Artwork credit noted in the source: Splynncryth - "psi-stalker")

### Areas

**Chi-town:** The capital of the Coalition States. CS troops, mutant
dog boys, psi-stalkers, SAMAS pilots, grunts, heavily patrol the area.
Dangerous for the unprepared D-bee. Small shanty towns surround the
fortified city where crime is high. Not the best place to stay. The CS
patrolling there lets everyone fight for themselves.

**Lone Star:** The Coalition's headquarters for mutant creation and
research. High CS patrols within the area. A decent livable city, if
human. Many D-bees are captured and used for experiments. Another
place where D-bees shouldn't explore without proper gear.

**Horton:** A small human town. Seems friendly enough and welcomes
anyone. Placed in a location surrounded by forest, dangerous wild
creatures have a tendency to wander in and prey on the weakly defended
townsfolk. Only way in and out besides days of traveling through the
wilderness is by hovertrain.

**Puerto Angel:** Once home to humans, vampires now roam free. Better
bring a cross and some water! Though tolerant to most folk, they will
fight to the death if attacked. Some have traveled as far north as the
burbs of Chi-Town looking for prey. The beaches next to the ocean make
a wonderful resting spot.

**Coming Soon (per the original marketing copy):**
- Addition to the Burbs as well as the fortified city itself: secret
  entrances, quests, new mobs, many new shops, a few of the things yet
  to come.
- Stormshire, one of the great places to visit in the magic zone: a
  huge city and wilderness areas to explore, secrets, new items, mobs,
  a must see.

**Atlantis:** The legendary continent of Atlantis has mysteriously
appeared on Earth again. However, it is no longer ruled by the wise and
benevolent Atlanteans, but by a ruthless alien intelligence race known
as the Splugorth. Lord Splynncryth and his minions now claim the land,
turning it into a mysterious and magical land with monstrous denizens.

Splynn is the capital and best known for The Market, an
inter-dimensional shopping area where just about anything and
everything can be found, from weapons, power armor, to slaves.

Alvurron is the city of the gargoyles. Only the brave-of-heart
adventure into this city where the winged beasts fly overhead and
cavernous dwellings are scattered around.

The Preserves is Lord Splynncryth's great hunting grounds on the west
side of the continent. It is filled with ravenous monsters of all
sorts and even nomadic tribes of lesser creatures (such as humans) roam
here. Perhaps it is time to take a hunting party into this savage
area. Be prepared and pack plenty of e-clips.

Atlantis highlights include:
- Slave trading: the active ability to buy and own slaves.
- The Market: and its countless services offered.
- Dozens of secrets that only the smart and able will discover.

It is rated:
- Services: ****
- Difficulty: *****
- Mob numbers: ***

**Africa:** under construction.
**Australia:** under construction.
**Japan:** under construction.

**Europe:** Welcome to Europe. Here is where you will find the City of
New Camelot. Who knows, perhaps the King will make an appearance,
flanked by his loyal servant Mrrlyn. Here you can explore the royal
gardens and browse through various shops along market street, just be
careful, some people say that the gardens are alive. Now off to what is
left of the pre-rifts country Germany, which is now ruled by the NGR.
The technology here is almost unbeatable compared to many of the other
weapons manufacturing industries. Be on the lookout though, some
soldiers harbor the same feelings towards D-bees as the dreaded
Coalition States do.

## 12. Reference: original RiftsMUD race and OCC list

Full lists as given in the source archive. An asterisk (*) denotes
ability to get an O.C.C.

### Currently Available Races

- Algor Frost Giant *
- Atlantean *
- Basilisk
- Bearman *
- Bogie
- Brownie
- Burster
- Changeling *
- Common Faerie
- Common Pixie
- Conservator
- Coyle *
- Dog Boy
- Dwarf *
- Elf *
- Equinoid
- Fire Dragon
- Frost Pixie
- Gargoyle
- Goblin *
- Great Horned Dragon
- Green Wood Faerie
- Gurgoyle
- Hawrke Duhk (NEW!)
- Hawrk-ka (NEW!)
- Human *
- Ice Dragon
- Jotan *
- Kankoran *
- Mind Melter
- Night-Elves Faerie
- Nimro Fire Giant *
- Ogre *
- Orc *
- Pogtal - Dragon Slayer
- (CS) Psi-stalker
- Rahu-man *
- Ratling *
- Secondary Vampire
- Silver Bells Faerie
- Thunder Lizard Dragon
- Titan *
- Tree Sprite
- Troll *
- Water Sprite
- Werebear
- Weretiger
- Werewolf
- Wild Psi-stalker (NEW!)
- Wild Vampire

### Currently Available OCCs

- Atlantean Nomad (NEW!)
- Atlantean slave (NEW!)
- Body Fixer
- Bounty Hunter
- City Rat
- CS Grunt
- CS Ranger
- CS Military Specialist
- CS SAMAS RPA Pilot
- CS Technical Officer
- Cyber-Doc
- Cyber-Knight
- Forger
- Freelance Spy
- Glitter Boy Pilot
- Gifted Gypsy
- Headhunter
- ISS Peacekeeper
- ISS Specter
- Juicer
- Knight (Europe)
- Master Assassin
- Ninja Juicer
- NTSET Protector
- Operator
- Pirate (S.A.)
- Professional Thief
- Rogue Scholar
- Royal Knight
- Sailor (S.A.)
- Smuggler
- Special Forces (Merc)
- Vagabond
- Wilderness Scout

**Special limited OCCs:**
- Ley Line Walker
- Maxi - Man
- Sunaj Assassin (NEW!)

## 13. Splynn map notes (partial, to be filled in over time)

- South route to Rocky's bar: this is the reference point used for the
  black market directions below.
- Black market entrance: from the point of going south like you're
  heading to Rocky's, go all west, then 1 north, then knock on the
  pillar. This opens the black market, which sells predator type
  armors that make you invisible, net guns, etc.
- Nothing further mapped yet; add to this section as more of Splynn's
  layout is recalled.
