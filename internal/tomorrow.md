Read CLAUDE.md first and confirm you're following the rules there before doing anything else.

This is a documentation-only task. No code changes.

I'm sharing raw memories and research about the original RiftsMUD (the
predecessor to this project) and some brainstormed future ideas. I want
this captured as organized reference material, not acted on yet.

Create a new file: internal/riftsmud-memories.md

Organize the content below into clear sections with headers, preserving
all specific details (numbers, percentages, command syntax, item names,
locations) exactly as given - do not paraphrase away specifics like
"98% obey, 65% squawk" or "100md rechargeable" or the black market
directions. Suggested section structure (adjust if a better structure
fits the material):

1. Visual/UI conventions (color usage: green room exits, minimal room
   description color, blue for status effects like thirst-quenched and
   "stoned," green/red-white/blue for radio/ooc/tells, ASCII-bordered
   score command, HUD style similar to Dead Souls, multiple score
   commands existed, separate equipment command)
2. Race/RCC mechanics (Great Horned Dragon MDC-only health model,
   metamorph human/metamorph <race> shapeshifting and armor-fit
   behavior)
3. Magic/psionics (telepathy, swimasafish, windrush and its
   hydra-into-other-room interaction)
4. Notable unique items and locations (obsidian blade behind a pushable
   rock, PPE-charged flaming metal hilt sword, ghostly katana of soul
   slaying in the catacombs with armor-ignoring MD/SDC-to-body damage,
   Sword of Atlantis location in the ocean rift, Rocky's bar in Splynn
   with the 100md rechargeable PPE armor talisman usable by all races
   except Hawrk-duhk types, the black market entrance directions south
   of Rocky's)
5. Pet/companion systems (falconry hawk acquisition via shopkeeper
   trade, corpse-eating behavior, obey percentage by skill level)
6. Moxim and the rift travel system (manual "rift <destination>"
   command, credit cost, temporary rift duration, multi-player use)
7. Recovery/healing pacing by race (Juicers minutes, dragons hours,
   humans slow)
8. Chargen/skill philosophy (auto-granted race/class abilities vs.
   wizard-reviewed skill requests; note on wanting a cleaner
   category-based skill picker that only shows what's actually
   available to picked)
9. Project history/origin notes (Chris did an estimated 30-35% of the
   FluffOS conversion, a head coder modified most systems, Macgyver did
   the majority of difficult work including OCC/RCC conversion and MDC,
   the note about how long a driver update took vs. how long it should
   take)
10. Brainstormed future feature ideas (smuggler credit conversion with
    improvable exchange rate and optional paid-service menu; unlockable
    secret classes similar to Sunaj; automating room-creation tooling
    for mob-driven Merc group base generation; in-game creation tables
    for Merc groups, cyborgs, androids, and the juicer process per the
    books; OCC respec at level 1 with a pfile flag to prevent repeat use
    and a note that this needs heavy power-creep testing)
11. Reference: original RiftsMUD marketing copy and world/area
    descriptions (Chi-town, Lone Star, Horton, Puerto Angel, Atlantis/
    Splynn/Alvurron/The Preserves, Europe/New Camelot/NGR) - preserve as
    historical/flavor reference, not as a to-do list
12. Reference: original RiftsMUD race and OCC list (full lists as given,
    marking which races could get an OCC)
13. Splynn map notes (partial, to be filled in over time - just capture
    what's given now: south route to Rocky's bar, black market entrance
    via south-all west-1 north-knock on pillar)

Add one clear note at the top of the file stating this is reference
material only, nothing here is an active task, and future sessions
should treat it as background context to consult, not a backlog.

Do not modify internal/tomorrow.md as part of this task, keep it
separate. Do not create or modify any .c files, any game content, or any
other file. This is purely a new markdown reference file.

After creating the file, report: file path, section count, confirm no
other files were touched, confirm no reboot or deploy needed since this
is documentation only.