# HOWTO: staff-handbook/

## Find the right chapter fast

| Task | Read |
|------|------|
| Set up or operate the server | ch01-server, then `docs/RUNNING.md` |
| Avoid known lib traps | ch02-gotchas (and CLAUDE.md, which wins) |
| Understand the promotion ladder | ch03-staff-ranks |
| Use staff tools (staves, kit) | ch04-wizard-tools |
| Debug chargen | ch05-chargen-flow |
| Look up a race / OCC | ch06-races / ch07-occs |
| Combat and damage questions | ch08-mdc-sdc, ch09-combat |
| Ability systems | ch10-skills, ch11-spells, ch12-psionics |
| Energy pools | ch13-ppe-isp |
| Alignment / insanity rules | ch14-alignments, ch15-insanities |
| Build content | ch16-builder (and `help build` in-game) |

`index.md` is the canonical table of contents.

## Update a chapter

1. Edit the chapter's `.md` file directly; plain Markdown, no build
   step.
2. Match repo prose rules: no em dashes, American spelling, spaced
   command names (never underscores) in anything a reader will type.
3. If the change reflects a code change, name the source file in the
   chapter (e.g. "see /daemon/rifts_combat.c") so the next editor can
   re-verify against code.
4. Update `index.md` only when adding, removing, or renaming a
   chapter.

## Add a new chapter

Copy the header style of an existing chapter, name it
`ch<NN>-<topic>.md` continuing the sequence, add it to `index.md`, and
list it in this directory's `README.md`.

## Keep it honest

This handbook documents how systems ACTUALLY behave in the lib, not
Palladium tabletop rules as written. When the two differ, describe the
MUD's behavior and note the deviation. When you find the handbook and
the code disagreeing, the code is the truth and the handbook gets
fixed.
