# WHY: staff-handbook/

## Why a handbook separate from docs/ and www/

Three doc surfaces, three audiences:

- `docs/` answers "how do I run and develop this repo" (setup, git
  workflow, ops). Developer-facing.
- `www/` is the polished website: player recruitment plus staff role
  guides rendered as HTML, copied manually to the VPS web root.
- `staff-handbook/` is the deep game-mechanics reference for people
  who already have staff access: how MDC actually interacts with SDC,
  what the chargen state machine does, what each wizard tool grants.

Merging them was considered and rejected: the website needs to stay
presentable and stable, while the handbook needs to be cheap to update
every time a daemon changes. Markdown in the repo, next to the code it
describes, keeps the update cost near zero.

## Why chapters mirror the game's subsystems

Each chapter maps roughly one-to-one onto a daemon or mechanic
(rifts_combat.c, rifts_skills.c, the PPE/ISP pools, chargen's
setter.c). That mapping is deliberate: when a subsystem changes, there
is exactly one chapter to re-verify, and a chapter can cite its source
files for re-checking.

## Why it documents behavior, not tabletop rules

AetherMUD adapts Palladium Rifts; it does not implement it verbatim
(real-time combat, MUD economy, level caps). Early drafts that quoted
tabletop rules caused staff to "correct" working code toward the book.
The standing policy: the handbook describes the lib as built, flags
deviations from tabletop explicitly, and defers to code when in doubt.
