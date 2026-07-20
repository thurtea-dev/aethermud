# AetherMUD

AetherMUD is a TTRPG-inspired MUD: a text-based multiplayer online world
built on the Nightmare III mudlib and the FluffOS 2.9 driver. The game
system is Palladium Books' Rifts RPG, faithfully adapted for real-time
multiplayer play. AetherMUD is a fan project inspired by (and distinct
from) a classic early-2000s Rifts MUD; it is not affiliated with or
endorsed by Palladium Books.

## What is this?

**A MUD** (Multi-User Dungeon) is a shared, persistent game world you play
entirely in text, over telnet or a client like Mudlet. You type commands
(`look`, `cast energy bolt at grunt`, `rift splynn`), the world answers in
prose, and other players inhabit the same rooms with you.

**Rifts** is a tabletop RPG set on a post-apocalyptic Earth where a golden
age ended in nuclear fire and the returning surge of magical energy tore
open rifts: portals to other worlds. Ley lines crackle across the
landscape, the human supremacist Coalition States wage war on magic and
"D-bees" (dimensional beings), dragons are playable, and a soldier in
powered armor can share a campfire with a Ley Line Walker. AetherMUD
recreates that world: Mega-Damage combat, PPE and ISP energy pools,
psionics, techno-wizardry, and rift travel between continents.

## Features

Measured from the live code (2026-07-20):

- **62 playable races**, from human to Great Horned Dragon, secondary
  vampire, psi-stalker, Atlantean, and mutant animals
- **66 Classes** (Occupational Character Classes / OCCs in the
  mudlib): Coalition military, Juicers, Crazies, Cyber-Knights,
  Glitter Boy pilots, Ley Line Walkers, Techno-Wizards, Mind Melters,
  and more
- **158 skills**, **115 spells**, **51 psionic powers**
- **~430 playable rooms** across Praxis, Chi-Town, Tolkeen, Horton,
  New Camelot, and NGR Germany, plus stub/placeholder zones (Lazlo,
  Lone Star, Puerto Angel, demon plane). Splynn, Alvurron, and
  Atlantis content lives under `domains/Praxis/areas/splynn/`, not as
  separate top-level domains
- **174 player commands**, MDC/SDC combat with attacks-per-melee,
  rift travel between continents via the NPC Moxim, radios, banking,
  slave markets, black markets, and a two-level in-game help system

## Quick start

Requires a C toolchain (gcc, make, bison, flex). Full setup:
[docs/INSTALL.md](docs/INSTALL.md).

```bash
git clone <repo-url> aethermud && cd aethermud
chmod +x mud.sh scripts/*.sh
./mud.sh init-config          # local mudos.cfg + groups.cfg from templates
cd nightmare3_fluffos_v2/fluffos-2.9-ds2.08
./configure nm3 && make && make install
cd ../..
./mud.sh doctor               # verify config and binaries
./mud.sh start
telnet localhost 1122
```

Preload takes 1-3 minutes on first boot. Connect with Mudlet or any telnet
client on port 1122. In-game, type `colorize` for ANSI color.

The first account you register will be offered admin rank automatically,
since no admin exists yet on a fresh install - answer `y` at the prompt.

## Repository layout

```
mud.sh                     Start/stop/status/doctor wrapper (never kill directly)
nightmare3_fluffos_v2/     FluffOS 2.9 driver source + the LPC mudlib (lib/)
docs/                      Setup, operations, contributor, and planning docs
docs/reference/            Bulk Rifts reference material for developers
internal/                  Personal / machine-specific working notes
staff-handbook/            In-depth staff reference (16 chapters)
www/                       Website: game info, staff guides
scripts/, tools/           Setup helpers and scripted playtest utilities
CLAUDE.md                  Mandatory coding rules for the mudlib
```

## Documentation

| Doc | Purpose |
|-----|---------|
| [HOWTO.md](HOWTO.md) | Root how-to: run, edit, where to put work |
| [WHY.md](WHY.md) | Root context: layout and staff-power decisions |
| [docs/INSTALL.md](docs/INSTALL.md) | First-time setup on a fresh Linux box |
| [docs/RUNNING.md](docs/RUNNING.md) | Start/stop, ports, logs, reload vs reboot |
| [docs/DEVELOPMENT.md](docs/DEVELOPMENT.md) | Contributor workflow and LPC rules |
| [docs/SHARING.md](docs/SHARING.md) | Local config vs tracked files |
| [docs/PUBLISHING.md](docs/PUBLISHING.md) | Pre-publication checklist |
| [docs/ASSESSMENT.md](docs/ASSESSMENT.md) | Measured state-of-the-project audit |
| [docs/zone-expansion-plan.md](docs/zone-expansion-plan.md) | Zone growth priorities |
| [docs/mercenary-groups-plan.md](docs/mercenary-groups-plan.md) | Mercenary group system design (planning only) |
| [docs/legacy-riftsmud-reference.md](docs/legacy-riftsmud-reference.md) | Legacy RiftsMUD design memories (reference) |
| [docs/playtest-checklist.md](docs/playtest-checklist.md) | Playtest passes and roster |
| [staff-handbook/](staff-handbook/index.md) | Internal staff reference |
| [CLAUDE.md](CLAUDE.md) | Coding rules for the lib |

Per-directory `README.md` / `HOWTO.md` / `WHY.md` pairs also exist under
`nightmare3_fluffos_v2/lib/{cmds,domains,std,secure,realms}/`,
`staff-handbook/`, and `www/`.

## Staff notes

- Mortal to Apprentice: `makewiz <player>` (gives the apprentice kit: `kit`)
- Apprentice to full role: domain/coding review, then `setrole <player> <role>`
- Bulk LPC reload: in-game `warmboot` (notifies online admins)
- `/std/` or simul-efun changes: `./mud.sh restart`

## Legal

AetherMUD is a non-commercial fan recreation. Rifts and Palladium Books
are trademarks of Palladium Books, Inc.; no affiliation or endorsement is
implied. Driver and mudlib carry their upstream FluffOS / Nightmare III
terms.
