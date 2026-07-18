# HOWTO: lib/secure/

## Add a new daemon and have it exist at boot

1. Write the daemon under `/daemon/` (preferred; only genuinely
   privileged daemons belong under `/secure/daemon/`).
2. Add its path to `secure/cfg/preload.cfg`. Without this it is not
   loaded at boot and `DAEMON_NAME->call()` targets may fail until
   something loads it lazily.
3. Add a `#define MY_D "/daemon/my_daemon"` style macro in
   `secure/include/daemons.h` so callers use the macro, not a string.
4. Daemons are update-safe: `update /daemon/my_daemon.c` in-game
   reloads them for all callers (call_other resolves fresh each call).

## Grant or check privileges

- Groups live in `secure/cfg/groups.cfg` as `(GROUP) member member`
  lines. A group with no members is valid.
- In code, gate with the predicate functions: `archp()`,
  `creatorp()`, `admin_wizp()`, `coding_wizp()`, or
  `member_group(who, "GROUP")` for raw group checks.
- The first account registered on a fresh install is offered admin
  automatically (see the bootstrap clause in `std/user.c
  set_position()` and login.c).

## Work with player saves (carefully)

- Path: `secure/save/users/<first_letter>/<name>.o`, plain text
  key/value.
- Read-only inspection is fine for debugging. Never copy a real
  player's save or password hash into a test account; create
  throwaways through normal chargen instead.
- Deleting a player goes through the `rid` admin command (moves the
  save to `save/users/rid/`), never manual file deletion.

## Add a simul_efun

One function per file in `SimulEfun/`, filename matching the function.
It becomes globally callable from every LPC object after a reboot.
Keep them tiny and dependency-free; they run for everything.

## Reload rules for this tree

- `secure/daemon/*.c` (master.c especially): treat as reboot-only.
- `secure/cmds/**`: full reboot (CLAUDE.md).
- `secure/cfg/*.cfg`: read at boot (preload) or on demand
  (groups.cfg via master.c load_access); a reboot is the safe way to
  apply changes.
