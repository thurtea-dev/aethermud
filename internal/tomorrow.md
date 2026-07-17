## Prompt 1, send to Claude Code on Fedora, LOCAL

Fix the highest-impact, hardest-failure gap first (magic race grants), since it's a small, contained fix compared to the skill-invocation gap, and it unblocks 9 races that currently cannot use their signature ability at all.

```
Before doing anything else, read CLAUDE.md and confirm you are following all rules stated there.

Propose a fix only. Show me the diff, do not apply it yet.

query_race_spell_grant() in daemon/rifts_start_d.c does not cover the 9 races flagged faerie_magic or nature_magic in daemon/rifts.c query_race_flags() (common faerie, common pixie, frost pixie, green wood faerie, night-elves faerie, silver bells faerie, tree sprite, water sprite, brownie). These races currently get no starting spells and no starting PPE despite being flagged as innately magical.

Research what spell list and PPE amount would be appropriate for each of these two groups under the source material, keep it minimal and small for now since the user plans to expand later, then propose the exact case additions to query_race_spell_grant() needed to grant a small starting spell list and correct PPE for all 9 races. Show the full diff. Do not touch query_race_psionic_grant() or any other function.
```

## Prompt 2, send to Claude Code on Fedora, LOCAL

Capture the mercenary group design as a planning document, no code yet.

```
Before doing anything else, read CLAUDE.md and confirm you are following all rules stated there.

This is a documentation task only. Do not write any code files.

Create a new planning document (check whether docs/zone-expansion-plan.md or a new file under docs/ is the better home, follow existing doc conventions in this repo) capturing this design for a future mercenary group system:

Level 4 players can found a mercenary group. The founder and a co-leader can add new members. Adding a member grants that player a group emblem object. A member leaves the group by dropping or removing the emblem. The first three mercenary groups will have secret faction bases hidden in Chi-Town, Splynn, and New Camelot. As one example, the New Camelot garden fountain has a hidden entrance that only opens for a player carrying a group emblem.

Write this as a design spec: goals, the emblem object's expected properties, the add and remove member flow, how base access should be gated by emblem possession, and open questions that still need answers before implementation (for example, what happens to a group's base access if the founder is removed or goes inactive, whether groups have a size cap, whether there can be more than three groups eventually).

Do not propose any code changes or begin implementation. This is planning only, to be revisited once current foundation gaps are resolved.
```

Once both come back, we will decide whether to also tackle the psionic shortcut gap and the broader skill-invocation gap before circling back to actually build the mercenary system, and only then update the README files to reflect the real, settled state of the project.
