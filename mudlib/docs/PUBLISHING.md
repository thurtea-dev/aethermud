# Publishing Checklist

Use this before making the repository public or sharing with outside developers.

## 1. Verify ignore rules

Confirm these are **not** tracked by git:

```bash
git status --ignored
```

Must be ignored:

- `nightmare3_fluffos_v2/bin/mudos.cfg`
- `nightmare3_fluffos_v2/lib/secure/cfg/groups.cfg`
- `nightmare3_fluffos_v2/lib/secure/save/**/*.o`
- `nightmare3_fluffos_v2/lib/daemon/save/**/*.o`
- `nightmare3_fluffos_v2/lib/log/*`
- `nightmare3_fluffos_v2/lib/realms/*/`
- `nightmare3_fluffos_v2/bin/driver` (build locally)
- `zip-files/`

If any were committed earlier:

```bash
git rm --cached nightmare3_fluffos_v2/bin/mudos.cfg
git rm --cached nightmare3_fluffos_v2/lib/secure/cfg/groups.cfg
git rm -r --cached nightmare3_fluffos_v2/lib/secure/save/
git rm -r --cached nightmare3_fluffos_v2/lib/daemon/save/
git rm -r --cached nightmare3_fluffos_v2/lib/log/
```

Do not delete local files; only remove from git index.

## 2. Scan for machine-specific paths

```bash
grep -r '/home/thurtea' --include='*.md' --include='*.cfg' --include='*.sh' .
grep -r '/home/thurtea' nightmare3_fluffos_v2/lib --include='*.c'
```

Committed files should use `/path/to/nm3` or placeholders only. Local `mudos.cfg` and `groups.cfg` are gitignored.

## 3. Confirm templates exist

- `nightmare3_fluffos_v2/bin/mudos.cfg.example`
- `nightmare3_fluffos_v2/lib/secure/cfg/groups.cfg.example`

## 4. Clean-clone smoke test (manual)

On a second directory or machine:

```bash
git clone <repo-url> aethermud-test
cd aethermud-test
chmod +x mud.sh scripts/*.sh
./mud.sh init-config
cd nightmare3_fluffos_v2/fluffos-2.9-ds2.08
./configure nm3 && make && make install
cd ../..
./mud.sh doctor
./mud.sh start
telnet localhost 1122
```

Create a character matching your `groups.cfg` admin name. Confirm login and `colorize on`.

## 5. Optional content cleanup (not required for boot)

These may remain in source but are personal or legacy:

- `lib/domains/Praxis/npcs/thurtea_hawk.c` (personal pet NPC template)
- `lib/realms/thurtea/` on disk (gitignored; not published)
- Admin command help may use generic `playername` examples

Game content completeness (races, OCCs, spells, etc.) is tracked separately in `lib/domains/Praxis/adm/master_gap_report.txt`.

## 6. Announcement-ready docs

Root docs should agree on the bootstrap flow:

- [README.md](../README.md) quickstart
- [INSTALL.md](INSTALL.md) full setup
- [SHARING.md](SHARING.md) local vs shared files
- [RUNNING.md](RUNNING.md) daily ops

## 7. Legal note

AetherMUD is a fan recreation of Palladium Books Rifts content. Confirm you are comfortable sharing under your chosen license and that upstream FluffOS/NM3 terms are respected.
