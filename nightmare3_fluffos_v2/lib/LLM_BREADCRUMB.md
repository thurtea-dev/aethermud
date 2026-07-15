# LLM breadcrumb - lib/

1. Read `/CLAUDE.md` at the repo root first - nearly every rule in it
   applies to code somewhere under this directory.
2. Read `README.md` in this directory (above).
3. Check the parent `nightmare3_fluffos_v2/LLM_BREADCRUMB.md` (and the
   repo root's) before assuming this directory's gotchas are
   self-contained.
4. Related: CLAUDE.md's "Rifts-Specific Architecture" and "Mudlib
   Structure" sections map subsystem names to file paths directly.

## Gotchas specific to this directory

- C89 rules apply strictly across every `.c` file in this tree: all
  variable declarations at the top of the function block, `/* */`
  comments only (no `//`), no mixed declarations and code. This causes
  real compile errors on FluffOS 2.9, not just style nitpicks.
- Functions must be declared before use (or forward-declared with a
  prototype) - calling a function defined later in the same file is a
  compile error here, unlike modern LPC. This has caused whole-boot
  failures before (see CLAUDE.md rule 17).
- `std/` changes require a full driver restart (`./mud.sh stop && ./mud.sh
  start`) to take effect for anything already loaded - `update`/warmboot
  do not reload inherited base classes for existing objects. This is not
  limited to user.c/living.c/room.c/armour.c/combat.c.
- Player saves live at `/secure/save/users/<letter>/<name>.o` - never
  `/save/players/`.
- Every subdirectory here (except vendored driver code elsewhere in the
  tree) carries its own README.md/LLM_BREADCRUMB.md pair - check the one
  in the specific subsystem directory you're working in for narrower
  gotchas than what's listed here.
