# Builder Guidelines

## Who Can Build

You need position `creator` (coding wizard) or `arch`/`head arch` (admin),
and you must be holding the Staff of Creation. Both are granted
automatically on promotion and restored automatically on every login.

You must also be standing inside your own realm: `/realms/<yourname>/`.
Files are written under:

```
/realms/<yourname>/area/room/
/realms/<yourname>/area/npc/
/realms/<yourname>/area/weap/
/realms/<yourname>/area/armor/
```

## Direct Commands

Type these directly, with no prefix:

```
create room <direction> <file>
create npc <file>
create weapon <file>
create armor <file>
modify here <property> <value>
modify <file> <property> <value>
add exit <direction> <path>
add <npcfile> to here
delete <object>
about <file|here>
more <file|here>
clone <path>
purge <id>
```

The older prefixed form still works exactly the same way and reaches the
same code: `build <verb> ...` (also `qcs <verb> ...` for admins). Prefer
the direct verbs above for everyday building.

## Practical Example

```
create room east test1
east
modify here short Room One
create npc guy
modify guy short a mean-looking orc
add hammer to guy
add guy to here
about guy
more here
delete fighter
```

`create room east test1` creates `test1.c` in your realm's room directory
and wires a two-way exit between your current room and the new one.
`create npc guy` creates `guy.c` in your realm's npc directory. `modify
guy short a mean-looking orc` edits that file's short description.
`add guy to here` patches a `reset()` spawn for `guy` into the current
room file. `about guy` prints the raw source of `guy.c`; `more here`
pages through the raw source of the current room file.

## Verifying Files Were Written

Every `create`, `modify`, `add`, and `delete` prints a line telling you
what happened, for example `QCS: wrote and updated
/realms/yourname/area/npc/guy.c`. If a write fails you get an explicit
`QCS: write_file failed for ...` or `QCS: file not found` instead of a
silent no-op.

## Reloading

QCS recompiles the file it just wrote as part of the same command, so a
plain `create`, `modify`, `add`, or `delete` already takes effect
immediately; you do not need to run `update` afterward. Use `update
<path>` yourself only if you hand-edited a file outside of QCS (with `ed`,
for example) and need to force a reload.

## Notes

- `about` and `more` fall back to any absolute file path if the name you
  give does not resolve inside your realm, so they also work as a quick
  way to read source outside your own area.
- `delete`, `add`, and the `modify <file> ...` form only operate on files
  inside your own realm.
- `clone` and `purge` are separate commands granted by the Staff of
  Creation; they are not gated behind `build`.

## clone and purge

```
clone bandit.c          /* relative name in your realm */
clone /domains/Praxis/monsters/highway_bandit.c
purge crate
```

**clone** resolves the path in this order:

1. `absolute_path(your cwd, name)` from ambassador `cd`
2. `qcs_resolve_realm_file(you, name)` for files under `/realms/<you>/area/`

Placement rules:

| Object type | Destination |
|-------------|-------------|
| Living (NPC, player) | Current room |
| Non-living | Inventory if it fits, else current room |

Success messages name the destination. If placement fails, the clone is
destructed and you get an error.

**purge** removes a non-living object from the room by id. It cannot remove
NPCs or players.

## Short descriptions for new content

When you `modify <npc> short ...`, start with a lowercase article unless the
word is a proper noun:

```
modify bandit short a ragged bandit
modify cp30 short a CP-30 laser pistol
```

This keeps `get`, `drop`, and room listing text grammatically correct. See
ch02 for why.
