# WHY: lib/cmds/

## Why commands are standalone files

This layout is inherited from the Nightmare III mudlib: one file per
verb, looked up by the command daemon at call time. The payoff is that
commands are hot-reloadable (nothing inherits them, so `update` fully
replaces the behavior) and independently testable. The cost is a large
flat directory per rank; the underscore prefix exists purely so the
filename is never itself a valid verb.

## Why there are two command trees

`/cmds/` and `/secure/cmds/` split on trust. Files under `/secure/` sit
inside the security boundary enforced by the master object: they may
touch saves, accounts, and privileged state, and per CLAUDE.md they
need a full reboot to reload safely. When adding a command, put it in
`/cmds/` unless it genuinely needs secure-tree privileges; history has
left a few oddities (tell and reply are under `/secure/cmds/mortal/`)
that must be respected when searching for existing verbs.

## Why the rank subdirectories

Nightmare III granted command paths by position (player, high mortal,
ambassador, creator, admin), and the login/user code still assembles a
player's command path from their rank. AetherMUD keeps the mechanism
but has collapsed most of the old ladder: mortals and staff are the two
ranks that matter, `hm/` and `ambassador/` survive mostly as legacy
homes for a few still-used tools (title, file shells).

## Why some commands are data-driven instead

Spells, psionics, and skills are NOT one-file-per-verb: `_cast.c` and
`_psi.c` are thin dispatchers over the daemons (`rifts_spells_d.c`,
`rifts_psionics_d.c`), and bare spell-word shortcuts fall through the
user's cmd hook into `try_spell_shortcut()` / `try_psi_shortcut()`.
That keeps 116 spells from being 116 command files, at the price of the
NEED_TARGET_EFFECTS bookkeeping documented at the top of both
dispatchers.
