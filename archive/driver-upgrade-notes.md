# FluffOS 2.9 to v2025.1205.0 Driver Upgrade Notes

Parallel port track for AetherMUD. Live game unchanged on port **1122** (FluffOS 2.9).
Modern driver build: `/home/thurtea/aethermud/fluffos-modern` (tag **v2025.1205.0**).
Porting mudlib copy: `/home/thurtea/aethermud/lib-modern` (rsync from live lib).
Modern config: `/home/thurtea/aethermud/bin-modern/fluffos.cfg` (telnet **1124**, websocket **1131**).

---

## Build (2026-07-07)

- Cloned `https://github.com/fluffos/fluffos` tag `v2025.1205.0` into `fluffos-modern`.
- Fedora 44 deps: `git cmake gcc-c++ make bison flex openssl-devel mariadb-connector-c-devel`
  `sqlite-devel libevent-devel jemalloc-devel libicu-devel pcre-devel zlib-devel`.
- CMake + build succeeded; driver at `fluffos-modern/build/src/driver`.
- Symlinks in `bin-modern/driver` and `bin-modern/lpcc`.

Config notes:
- `binary directory` and `swap file` lines are **obsolete** in modern FluffOS (warnings only).
- No external `addr_server` needed (built-in DNS in modern driver).
- `maximum local variables : 30` rejected; driver reset to default **64**.

---

## Error catalog (by cause)

### Stricter parsing

| File | Error | Fix |
|------|-------|-----|
| `secure/SimulEfun/to_object.c:49` | Split return type/name (`static object` newline `e(...)`) | Merged to single-line declarations |

### Changed semantics (warnings, not yet blocking)

| File | Warning | Planned fix |
|------|---------|-------------|
| `secure/SimulEfun/absolute_path.c:27,34` | Negative range end `[0..-2]` | Use `[0..<2]` |
| `secure/SimulEfun/time.c:14` | `date()` prototype mismatch | Align prototype with implementation |
| Various SimulEfun | Unused locals, unknown `\` escape | Cleanup optional |

### Removed / obsolete config

| Item | Notes |
|------|-------|
| `binary directory` | Obsolete line; driver ignores |
| `swap file` | Obsolete line; driver ignores |
| `address server ip/port` | Not used in modern Config.example |
| `port number : 1122` | Replaced by `external_port_N : telnet N` |
| Telnet 1123 / websocket 1130 | `bind error: Address already in use` on boot - something else on the box already held one of these (not the live 1122 game). Moved to telnet **1124** / websocket **1131** to unblock trials; `bin-modern/fluffos.cfg` reflects this. Root cause of the 1123/1130 conflict was not investigated further since the workaround was cheap. |

### Removed / stricter type

| File | Error | Fix |
|------|-------|-----|
| `secure/daemon/master.h:8`, `master.c:69` | `L_TYPE_MODIFIER`/`unexpected '['` on `static` variable declarations | Migrated `static` -> `protected`/`nosave` per the project-wide `static` migration (see "Fixes applied" below) |
| `std/room/senses.c:114,136` | `status smell_things(string str)` / `status do_listen(string str)` -> `syntax error, unexpected L_IDENTIFIER` | **NOT YET FIXED.** The `status` return-type keyword (MudOS-era boolean alias for `int`) does not parse as a function return type in modern FluffOS's grammar. Fix: change both to `int`. Checked `lib-modern` for siblings - `senses.c` is the only file using `status` as a return type, so this is a two-line fix. This was the last error in the most recent boot attempt (port 1124) and the log capture ends right after it, i.e. boot did not reach "ready for connections" in that run. |

### Removed efun

_(none confirmed yet beyond the `status` return-type issue above)_

---

## Fixes applied in lib-modern

1. **to_object.c** - single-line function headers for `e()` and `to_object()`.
2. **`static` migration** - project-wide pass changing `static` to `protected`
   (functions) / `nosave` (variables), including `secure/daemon/master.h`/
   `master.c` which were blocking boot in attempts 4-5.
3. **`mapping ref` parameter rename** - `ref` is a reserved word in modern
   FluffOS; renamed the parameter at each call site that used it.
4. **`to_object.c` helper rename** - internal helper renamed `resolve_env`
   to avoid a naming collision surfaced by the stricter modern parser.

---

## Boot status

- Attempt 1-3: Failed loading SimulEfun (`to_object.c` syntax). **Fixed** (see
  fix #1 above).
- Attempt 4-5: Failed loading `/secure/daemon/master` (`static` access
  modifier syntax). **Fixed** (see fix #2 above).
- Attempt 6: `bind error: Address already in use` on ports 1123/1130.
  **Worked around** by moving to 1124/1131.
- Port-1124 run (`driver-boot-1124.log`, 2026-07-07 17:00): SimulEfun and
  master now load; preload proceeds much further (through `/daemon/banish.c`,
  `/std/user/editor.c`, `/std/room/exits.c`, `/std/room/items.c`) with only
  warnings (unused locals, multiple access modifiers, a function-pointer
  warning) until it hits the **unresolved** `/std/room/senses.c:114` `status`
  return-type error above. The log capture ends immediately after that error
  - it did not reach "ready for connections" in this run. Contrary to any
  earlier summary claiming a full clean preload on this driver, **the modern
  boot is not yet fully clean**: fix the `status` return-type issue (and any
  siblings found by grepping for other `status <name>(` headers in the lib),
  then re-run and confirm preload actually completes and the driver accepts
  a connection before updating this status again.

---

## Regression test checklist (post-cutover)

- [ ] Login / reconnect
- [ ] CharGen (race, OCC, electives, secondary skills, starting package)
- [ ] Movement and room resets
- [ ] Combat (SDC + MDC, ranged, flee)
- [ ] Spells (`cast`, PPE, ley line bonus)
- [ ] Psionics (ISP, disciplines)
- [ ] Skills / OCC commands
- [ ] Mail (send, read, folders)
- [ ] Boards / news
- [ ] Shops (buy/sell, credits)
- [ ] Save / restore (`save`/`quit`, reload character)
- [ ] Quest_d objectives and completion
- [ ] Wanted_d / bounty board
- [ ] Radio / rift travel
- [ ] WebSocket client (port 1130)

---

## Cutover plan (draft)

1. Complete lib-modern boot with zero preload failures on port 1124.
2. Run full regression checklist above on lib-modern.
3. Scripted telnet smoke + manual spot checks on MDC combat and chargen.
4. Copy validated `lib-modern` over live lib during maintenance window **or** swap mud.sh to point at modern driver + lib-modern.
5. Retire `addr_server` from mud.sh (modern driver only).
6. Enable websocket port in production config.
7. Update mud.sh with `mud-modern.sh` parallel script until cutover.

### CLAUDE.md rules to relax after cutover

| Rule (2.9 workaround) | After modern FluffOS |
|-----------------------|----------------------|
| C89 only (`//` comments forbidden) | May use `//` comments in new code |
| No mid-block variable declarations | May declare variables where needed (C99-ish LPC) |
| Split function headers workaround | Single-line headers still fine; multiline types may work |
| `reset()` spawns only | **Keep** - still best practice for NM3 mudlib |
| MDC NPC stat pattern | **Keep** - game design rule, not driver rule |
| Full reboot after `/std/` edits | **Keep** - inherit semantics unchanged |

---

## What remains

- Complete boot iteration until master preloads cleanly.
- Fix any further compile errors in preload chain.
- Telnet login smoke test on port 1124.
- Full regression pass before production cutover.
