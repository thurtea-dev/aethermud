# AetherMUD

AetherMUD pairs two things built together: a custom C++ LPC driver, and a
Rifts/Palladium-themed mudlib (a Nightmare III base under active Rifts
conversion) that runs on it.

## Layers

- `driver/` — the C++20 LPC driver itself: `compiler` (lexer, parser,
  codegen), `vm` (stack bytecode interpreter, `Value` variant), `object`
  (`LpcObject`/`ObjectManager`, inherit resolution, compile-time privs),
  `efun` (the registered efun table), `net` (non-blocking TCP, per-connection
  state), and `scheduler` (the heartbeat/call_out tick loop). Unit-tested
  (`ctest`), grounded throughout in FluffOS 2.9-ds2.08 reference source for
  language and runtime semantics rather than guessed.
- `mudlib/` — the LPC mudlib itself (Nightmare III base, in active Rifts
  conversion): races, OCCs, skills, spells, psionics, combat, chargen, and
  the rest of the game content and rules.
- `docs/` — `STATUS.md` is the authoritative running log of what the driver
  has implemented, confirmed working, and left as a known stub; the
  `driver-comparisons/` set cross-checks the driver against FluffOS, MudOS,
  and DGD.

## Driver status

As of 2026-08-07: ~91 registered efuns, 293 passing unit tests, and a
fresh account confirmed live end to end through the complete chargen flow
(login, account creation, zone, attribute rolling, race, alignment, OCC
pick, automatic starting-equipment grant) into a real starting room with a
working `look` command. Closures (`(: :)` bare-name, general inline lambda,
string-constant, and `(*fp)(args)` forms), `add_action`/`enable_commands`
command dispatch, `catch()`, and simul_efun resolution are all implemented.
The largest remaining gap is the scheduler: `call_out`/`heart_beat` are
accepted and validated but nothing time-based actually fires yet. See
`docs/STATUS.md` for the full detail and citations.

## Build instructions

    cmake -B driver/build -S driver
    cmake --build driver/build
    driver/build/lpcdriver etc/driver.cfg
