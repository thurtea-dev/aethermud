# tools/

Scripted playtest and verification utilities that drive a live, running
MUD over telnet (port 1122) to exercise real gameplay flows end to end.
These are not game code and are not run automatically - they're used
manually to verify a change actually works in the running game, similar
in spirit to the `verify` skill.

- `playtest_create_chars.py` - creates fresh playtest characters via
  telnet chargen. Does not touch existing accounts; all new names, a
  shared known password.
- `section8_verify.py`, `section8_verify3.py`, `section8_verify4.py` -
  successive passes of a chargen-and-in-world-command verification
  script (walkthrough, combat, bank, regression checks). Each file is a
  distinct pass, not a version history to clean up - check each file's
  docstring for what it specifically covers.
- `spell_shortcut_verify.py` - verifies the bare spell-name cast
  shortcut works correctly.
- `migrate-static.pl` - a one-off Perl script for migrating MudOS
  `static` keyword usage for modern FluffOS.
- `__pycache__/` - generated Python bytecode cache, not source. Ignore
  it and don't document it.
