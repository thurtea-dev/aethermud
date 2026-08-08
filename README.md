# AetherMUD

A custom C++ LPC driver and a Rifts/Palladium themed mudlib that runs
on it.

## Layers

- driver/ - the LPC driver. Lexer, parser, codegen, bytecode VM,
  object management, efun table, networking, and scheduler.
- mudlib/ - the game content. Races, OCCs, skills, spells, psionics,
  combat, character creation, rooms.
- docs/STATUS.md - current implementation status.

## Status

Login, account creation, and character creation work end to end into
a real starting room with a working look command. call_out and
heart_beat are not yet firing. See docs/STATUS.md for details.

## Build

    cmake -B driver/build -S driver
    cmake --build driver/build
    driver/build/lpcdriver etc/driver.cfg
