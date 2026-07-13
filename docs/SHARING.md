# Sharing and Local Setup

What to commit, what to keep local, and what every new developer must personalize.

## First-time setup (new checkout)

```bash
cd /path/to/nm3
chmod +x mud.sh scripts/*.sh
./scripts/init-local-config.sh      # creates mudos.cfg + groups.cfg
./scripts/check-setup.sh            # verify paths and binaries

cd nightmare3_fluffos_v2/fluffos-2.9-ds2.08
./configure nm3 && make && make install

cd /path/to/nm3
./mud.sh start
telnet localhost 1122
```

Or run `./mud.sh doctor` after init (same checks as check-setup.sh).

## Files every developer must personalize

| File | Template | Action |
|------|----------|--------|
| `nightmare3_fluffos_v2/bin/mudos.cfg` | `bin/mudos.cfg.example` | Set absolute `mudlib directory` and `binary directory` |
| `lib/secure/cfg/groups.cfg` | `lib/secure/cfg/groups.cfg.example` | Replace `YOUR_ADMIN_NAME` with your login name |

Use `./scripts/init-local-config.sh` to generate both from templates.

Optional env overrides when running:

```bash
export MUD_BASE=/path/to/nightmare3_fluffos_v2
export MUDOS_CFG=/path/to/custom/mudos.cfg
./mud.sh start
```

## Do not share as-is

These are listed in `.gitignore` and should stay local:

| Path | Reason |
|------|--------|
| `lib/secure/save/**/*.o` | Player saves, postal, daemon save blobs |
| `lib/daemon/save/**/*.o` | Runtime daemon persistence |
| `lib/domains/**/data/*.o` | Domain runtime data |
| `lib/secure/cfg/groups.cfg` | Personal admin assignments |
| `bin/mudos.cfg` | Machine-specific absolute paths |
| `lib/log/*` | Runtime logs |
| `lib/tmp/**` | Transient mudlib data |
| `lib/realms/*/` | Personal wizard sandboxes |
| `bin/mud.pid`, `bin/addr.pid` | Runtime state |
| `bin/driver`, `addr_server`, `portbind` | Build locally with `make install` |
| `zip-files/` | Local archive bundle, not required to run |

## Safe to share (tracked in repo)

- All LPC source under `lib/` (except ignored paths above)
- `bin/mudos.cfg.example`, `groups.cfg.example`
- FluffOS source under `fluffos-2.9-ds2.08/`
- Documentation (`README.md`, `INSTALL.md`, `PUBLISHING.md`, etc.)
- Build driver locally: `make install` in the fluffos tree

## Before publishing the repo

See [PUBLISHING.md](PUBLISHING.md) for the full release checklist.

1. Confirm `.gitignore` excludes saves, logs, local config, realms, and binaries
2. Run `grep -r '/home/thurtea'` on tracked paths; fix any hits in committed files
3. Confirm `groups.cfg` and `mudos.cfg` are not tracked (only `.example` files)
4. Run a clean-clone smoke test (init-config, build, doctor, start, connect)

## Admin bootstrap on fresh install

1. Create character with the name you put in `groups.cfg`
2. `./mud.sh restart` after editing groups.cfg
3. Log in; SECURE group grants master-level access
4. Optional: `makewiz <name>` and `setrole <name> admin` for wizard tooling

There is no automatic first-player-admin in current code.

## Related docs

- [INSTALL.md](INSTALL.md) - full build and install
- [RUNNING.md](RUNNING.md) - daily operations
- [DEVELOPMENT.md](DEVELOPMENT.md) - contributor workflow
