# lib/cmds/

Player-facing and staff command files. Every file here defines one
command: `_name.c` implements the `name` command via a `cmd_name()`
function. The command daemon (`/daemon/command.c`) looks these files up
fresh on every command, so they are update-safe (no reboot needed after
edits, just `update /cmds/...` in-game).

- `mortal/` - commands every player gets (say, who, cast, psi, prowl,
  radio, store, ...). The bulk of the game's verbs.
- `adm/` - admin commands (makewiz, setrole, playerwipe, ...). More
  privileged admin commands live in `/secure/cmds/adm/` instead.
- `creator/` - coding-wizard commands, including the QCS builder
  dispatcher (`_qcs.c` and its `_qcs_*` helpers).
- `ambassador/`, `hm/` - legacy Nightmare III rank commands
  (ambassador file tools, high mortal commands like title).
- `soul/` - emote/social commands.
- `guild/`, `race/`, `skills/`, `database/`, `qcs/` - smaller
  special-purpose command sets.

See HOWTO.md here for the step-by-step of adding a command, and WHY.md
for how this directory relates to `/secure/cmds/`.
