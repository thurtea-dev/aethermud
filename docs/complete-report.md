# AetherMUD Workspace: Complete Inventory & Rebuild-Base Report

Generated 2026-08-01. Scope: every directory under `/home/thurtea/work`,
with a deep dive into all copies of the AetherMUD codebase (a Rifts-RPG
conversion of the Nightmare III mudlib on FluffOS 2.9), including the
quarantined VPS production snapshot. Read-only survey — nothing was
modified.

## Verdict

**Use `/home/thurtea/work/am` as the rebuild base.** It is the newest,
most-committed, most feature-complete copy, and git history confirms it
is a strict superset of the live production VPS snapshot (see below) plus
6 additional local commits the VPS never received.

---

## 1. Directory inventory

| Path | Size | Files | Last touched | What it is |
|---|---|---|---|---|
| `am` | 125M | 8,964 | 2026-07-31 | **Winner.** Active git working copy of AetherMUD (driver + mudlib + docs) |
| `AetherMUD-Backups` | 869M | 22,368 | 2026-07-31 | Container for backup/quarantine snapshots (broken down in §2) |
| `am-main` | 30M | 3,484 | 2026-07-30 | Static, non-git snapshot of an older `am` state |
| `aethermud` (top-level) | 104K | 13 | 2026-07-03 | Static HTML staff/website pages only — no mudlib |
| `aethermud-website-backup` | 2.1M | 38 | 2026-07-30 | Static HTML website only — no mudlib |
| `LLM` | 9.1G | 3,351 | 2026-07-31 | Unrelated: a Python LLM client/GUI tool project |
| `Llm GUI` | 77M | 4,618 | 2026-06-25 | Unrelated: another LLM tooling project; contains 2 stray LPC files (`rifts_pet.c`, `trained_hawk.c`) with no surrounding mudlib |

Only `am`, `AetherMUD-Backups/*`, and `am-main` contain an actual mudlib
(`nightmare3_fluffos_v2/` — the FluffOS driver + LPC game code). Everything
else is website chrome or unrelated tooling.

---

## 2. Inside `AetherMUD-Backups` (869M, 22,368 files)

```
AetherMUD-Backups/
  aethermud/                              full mudlib snapshot, git repo
  aethermud-rebuild-2026-07-29/           full mudlib snapshot, git repo (diverged branch)
  quarantine-aethermud-2026-07-29/
    backups/
      aethermud-backups/                  tarball + bare mirror of .git (2026-07-25)
      aethermud-git-reference/            full mudlib snapshot, git repo (2026-07-11, old)
      production-aethermud/               208MB tarball from a WSL->Fedora migration (2026-07-11)
    loose-runtime-state/                  empty
    old-working-copy/                     empty
    unknown-extra/Latest-AetherMUD/       empty
    vps-copy/aethermud-live-VPS/          <- the actual production server snapshot, git repo
```

This whole tree is the residue of a prior cleanup session (dated
2026-07-29): several duplicate/legacy copies were swept into
`quarantine-aethermud-2026-07-29/` while `am` was kept as the live
working copy. `loose-runtime-state`, `old-working-copy`, and
`unknown-extra/Latest-AetherMUD` are empty — nothing was actually left in
them.

### Git relationship between the copies

All the git-tracked copies below share the same history up to a common
ancestor commit `8711108` (Jul 28 21:52). I fetched each into a temporary
remote against `am` to check ancestry directly (not just by commit count):

| Copy | HEAD commit | Commit date | Commits total | Relationship to `am` |
|---|---|---|---|---|
| **`am`** | `cd2a283` | 2026-07-30 15:14 | 230 | — (reference) |
| `AetherMUD-Backups/aethermud` | `33275bc` | 2026-07-29 16:45 | 228 | Strict ancestor of `am` (2 commits behind) |
| `AetherMUD-Backups/aethermud-rebuild-2026-07-29` | `1f49c69` | 2026-07-29 22:00 | 225 | **Diverged**, not an ancestor — has 1 commit `am` lacks (a squashed/rearranged version of `am`'s `33275bc`+`77e71e3`, plus scaffolding `.gitkeep` files), but is missing `am`'s final 5 commits |
| `quarantine.../vps-copy` (live production) | `7a9d506` | 2026-07-29 17:06 | 224 | **Diverged**, not an ancestor — has 1 commit `am` lacks, but it's file-mode-only noise (see below); missing `am`'s final 6 commits |
| `quarantine.../aethermud-git-reference` | `f360536` | 2026-07-11 15:09 | — | Old ancestor, ~3 weeks stale |
| `am-main/am-main` | (not a git repo) | files dated 2026-07-30 | — | Plain directory copy, no history; mudlib `lib/` has only 2,647 files vs `am`'s 2,753 |

**Bottom line: `am` is strictly ahead of every other copy in git terms**
except for one trivial commit each on the vps-copy and rebuild branches.

---

## 3. Deep dive: the live VPS snapshot vs `am`

This is the part you asked me to check specifically — whether the
production server (`aethermud.com`, quarantined at
`AetherMUD-Backups/quarantine-aethermud-2026-07-29/vps-copy/aethermud-live-VPS/`)
had any live hotfixes that never made it back into `am`.

**Short answer: no.** The VPS's one commit that `am` doesn't have
(`7a9d506`, "commit", 2026-07-29 17:06) touches these 7 files:

```
commit.sh
internal/TRANSFER-CHECKLIST.md
mud.sh
nightmare3_fluffos_v2/fluffos-2.9-ds2.08/configure
scripts/check-setup.sh
scripts/init-local-config.sh
tools/migrate-static.pl
```

All 7 changes are `old mode 100755` -> `new mode 100644` — i.e. the
executable bit was stripped on every one of them (a side effect of
however that copy was transferred/archived), **zero content changes**.
There is no real hotfix on the VPS that's missing from `am`.

I also diffed working-tree state (`git status --porcelain`) in both
copies to check for *uncommitted* changes that might represent live
hand-edits on the server:

- `vps-copy` has 10 modified files, all `.bat`/Windows build scripts and
  a compiled `driver.exe` — build artifacts, not gameplay code, and not
  unique (see next line).
- `am` has the same 10 files modified **plus** `configure`,
  `commit.sh`, `internal/TRANSFER-CHECKLIST.md`,
  `scripts/check-setup.sh`, `scripts/init-local-config.sh`,
  `tools/migrate-static.pl`, and an untracked `setup.sh`.

So `am`'s working tree is a superset of the VPS's working tree too —
nothing was hand-patched on the live server that isn't already present
(or superseded) locally.

### What `am` has that the VPS snapshot doesn't (6 commits, Jul 29-30)

| Commit | Date | Summary |
|---|---|---|
| `79815e5` | Jul 29 13:17 | Website reorg: moved the old `www/` site into `www/OldAetherMUD/`, rewrote `www/index.html` and `www/staff.html` |
| `4ad1480` | Jul 29 14:59 | Skill fixes: `_bump.c`, `_drain.c`, `_mend.c`, `_missile.c`, `_pick.c`, `_raise.c`; removed stale `internal/tomorrow.md` |
| `77e71e3` | Jul 29 16:04 | `daemon/news.c` and `secure/daemon/wiztools.c` changes |
| `398f236` | Jul 29 16:27 | Further work on `domains/adm/wiz_tools/demotion_menu_d.c` |
| `33275bc` | Jul 29 16:45 | `cmds/mortal/_skills.c` and `demotion_menu_d.c` refinements |
| `c623101` | Jul 30 14:22 | Trimmed `CLAUDE.md` session-history bloat (82 -> 4 lines of that section), path cleanup |
| `cd2a283` | Jul 30 15:14 | FluffOS driver source tweaks (`array.c`, `configuration`, `func_spec.cpp`, `main.c`, `packages/contrib.c`, `qsort.h`); deleted a stray 813KB `driver.old` binary |

None of this is destructive or divergent — it's straightforward forward
progress after the VPS snapshot was taken.

**Practical implication:** if you rebuild from `am`, you are rebuilding
from something strictly newer than what was ever live on
`aethermud.com`. You lose nothing by ignoring the VPS copy and the other
backups.

---

## 4. Feature completeness snapshot (as measured in `am/docs/ASSESSMENT.md`)

This is a code-measured (not guessed) audit dated 2026-07-17, cross-checked
against reference docs, still accurate to `am`'s current state give or
take the 6 commits above. **Overall: ~80-85% of a playable, faithful
Rifts recreation.**

| System | Completeness | Notes |
|---|---|---|
| OCC / RCC (classes/races) | ~90% | 66 OCCs, 62 races; all core Rifts book classes present, plus expansions (Atlantis, Sunaj, NGR/Triax, warlock, necromancer) |
| Magic & psionics | ~75% | 115 spells, 51 psionic powers, 157 skills; combat/utility magic solid, ritual-flavored magic (summoning, curses, creation rituals) is the open tail — ~55-65 book invocations unimplemented |
| Combat | ~85% | Full MDC/SDC gate, APM system, elemental damage typing, vehicle armor HP; per-round initiative is a deliberate simplification, not a bug |
| Zones & rooms | ~60% of the *expansion plan's* target | ~440 playable rooms live across Praxis, Chi-Town, Tolkeen, Horton, New Camelot, NGR, Splynn/Alvurron; Stormshire (+80 rooms) not started; Lone Star and Splynn Preserves partially expanded |
| Commands | ~90% | 174 mortal, 33 admin, 74 creator commands; gap report found no genuinely missing mortal verbs |
| Player systems (chargen/saves/inventory) | ~85% | Full chargen flow verified end-to-end; insanities intentionally out of scope; score UI missing a couple of cosmetic status indicators |

### Known technical debt (ranked by risk, from the same audit)

1. ~~`domains/Praxis/standardOld/` legacy NM3 rooms throwing runtime errors~~ — **resolved 2026-07-19**, moved to `attic/`
2. `secure/daemon/events` throws destructed-function-pointer errors referencing `domains/Praxis/supply2` — needs a liveness guard, still open
3. `log/crashes` shows 15 "Process terminated" entries Jul 10-12 — likely manual stop/starts, unconfirmed
4. ~~Casting resource-loss ordering bug~~ — **resolved and verified live 2026-07-13**
5. Editor droppings in the repo (`#s_centre2.c#`, `*.c~`, `*.backup`, `services.old`) — harmless, just noise
6. Documentation drift: some counts in older docs/reference files are stale vs. the measured figures above; `master_gap_report.txt` body may still list superseded MISSING entries
7. `CLAUDE.md`'s "no `//` comments" rule is violated by 146 files that compile fine — rule is either stale or a latent portability trap for a future modern-FluffOS move
8. Zero TODO/FIXME markers in the lib — genuinely clean on that axis

---

## 5. Other candidates, briefly

- **`am-main/am-main`** — a plain (non-git) copy, files dated one day
  before `am`'s final commits, with a smaller mudlib (2,647 vs 2,753
  files in `lib/`). Superseded by `am`; no unique content found.
- **`AetherMUD-Backups/aethermud-rebuild-2026-07-29`** — a diverged
  branch that squashed some of the same Jul 29 work differently and added
  placeholder `.gitkeep`/`.keep` files for empty save-data directories,
  but never picked up `am`'s final 5 commits (skill fixes, website
  reorg, CLAUDE.md trim, driver tweaks). Not ahead of `am` in any way
  that matters.
- **`aethermud-git-reference`** and **`production-aethermud`** (tarball)
  — both dated 2026-07-11, roughly three weeks stale. Historical only.
- **Top-level `aethermud/`** and **`aethermud-website-backup/`** — these
  are just the marketing/staff website (`www/`), already present *inside*
  `am/www/` (and its predecessor now lives at `am/www/OldAetherMUD/`).
  Nothing here isn't already in `am`.

---

## 6. Recommendation

Rebuild from **`/home/thurtea/work/am`** as-is. It is:

- The most recent (230 commits, last commit 2026-07-30 15:14)
- A strict superset of the actual production server that was ever live
- The most feature-complete by every measured axis in §4
- The only copy with current, actively-maintained documentation
  (`README.md`, `CLAUDE.md`, `docs/ASSESSMENT.md`,
  `docs/zone-expansion-plan.md`, `staff-handbook/`)

No content from the VPS quarantine copy, `am-main`, or the rebuild branch
needs to be merged in — nothing there is missing from `am`. The other
directories under `/home/thurtea/work` (backups, website copies, LLM
tooling) are safe to ignore for the rebuild and can eventually be
archived or deleted once you're confident the rebuild is underway (not
done automatically here — that's your call).
