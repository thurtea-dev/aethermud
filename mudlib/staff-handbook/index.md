# AetherMUD Staff Handbook

Internal reference for Admin, Domain, Coding, and RP Wizards.
Not player-facing. Do not place in /doc/help/.

This handbook documents how the mud works, how to operate it, and
what the conversion rules are for the Rifts 2.1 recreation running
on Nightmare III / FluffOS 2.9. It is meant to be read alongside
CLAUDE.md, which is the authoritative rule file for the codebase.
When the two conflict, CLAUDE.md wins.

---

## Table of Contents

| # | Chapter | Status | Audience |
|---|---------|--------|----------|
| 01 | [Server Operations](ch01-server.md) | Ready | All |
| 02 | [Known Gotchas and Conversion Quirks](ch02-gotchas.md) | Ready | All |
| 03 | Staff Ranks and Responsibilities | In progress - needs admin input on rank definitions | Admin |
| 04 | [Wizard and Admin Tools](ch04-wizard-tools.md) | Ready (clone/purge documented) | Admin, Coding |
| 05 | Character Creation Flow | In progress - needs rifts_start_d.c read | All |
| 06 | Races and RCCs | In progress - needs rifts.c read for Dragon stats | RP, Coding |
| 07 | OCCs | In progress - several OCC skill blocks missing from daemon | RP, Coding |
| 08 | MDC vs SDC | In progress - needs rifts_combat.c enforcement check | Coding, RP |
| 09 | Combat Notes | In progress - APM system not yet implemented | Coding |
| 10 | Skills and Advancement | In progress - per-level advancement gap to document | Coding, RP |
| 11 | Spells | In progress - needs rifts_spells_d.c targeted read | RP, Coding |
| 12 | Psionics | In progress - count confirmed 50, all implemented in daemon; chapter content not yet written | RP, Coding |
| 13 | PPE and ISP | In progress - regen heartbeat not yet implemented | Coding |
| 14 | Alignments | Ready to write - reference-chart.md has full data | RP |
| 15 | Insanities and Addiction | Ready to write - reference-chart.md has full data | RP |
| 16 | [Builder Guidelines](ch16-builder.md) | Ready (QCS + clone/purge) | Domain, Coding |

---

## How to Use This Handbook

- Read ch01 and ch02 before touching any code or in-game commands.
- RP Wizards: focus on ch05 through ch15.
- Domain Wizards: focus on ch01, ch02, ch16.
- Coding Wizards: focus on ch01, ch02, ch05, ch08 through ch13.
- Admin Wizards: all chapters, especially ch03 and ch04.

Chapters marked "In progress" have known source gaps. Do not treat
them as complete until the status updates to "Ready."

---

## Source Files

These are the primary sources used to write this handbook:

- CLAUDE.md - project rules, known mistakes, architecture overview
- See also: [SHARING.md](../docs/SHARING.md) - local config and gitignore
- reference-chart.md - developer reference for game mechanics
- report-new.md - recent session progress (handbook supplements this file)
- /daemon/rifts.c - race definitions
- /daemon/rifts_start_d.c - starting packages and creation flow
- /daemon/rifts_skills.c - skill base percentages and per-level values
- /daemon/rifts_spells_d.c - spell definitions and PPE costs
- /daemon/rifts_psionics_d.c - psionic definitions and ISP costs
- /daemon/rifts_combat.c - combat hooks and MDC enforcement
- /cmds/adm/ - admin commands
- /cmds/creator/ - creator/builder commands
- /domains/adm/wiz_tools/ - RP wizard tools
- /doc/help/wiz/ and /doc/help/creator/ - existing wizard docs
