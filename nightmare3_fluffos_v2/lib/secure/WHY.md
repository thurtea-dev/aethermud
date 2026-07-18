# WHY: lib/secure/

## Why a separate secure tree exists

The FluffOS driver asks the master object to approve every file read
and write an LPC object attempts. The Nightmare III answer, kept here,
is directory-based trust: code under `/secure/` is the code allowed to
touch saves, accounts, and privileged state, and everything else is
sandboxed by `master.c`'s valid_read/valid_write. Putting a file here
is a security decision, not an organizational one; that is why most new
work belongs in `/daemon/`, `/cmds/`, or `/domains/` instead.

## Why PRIVS is off and bootstrap works by file identity

The driver's PRIVS mechanism is compiled out (`#undef` in options.h),
so `valid_apply()` can never approve anyone by privilege string. The
first-admin bootstrap in `std/user.c set_position()` instead
authenticates its caller by file identity (previous_object() must be
the login object) plus a save-file scan proving no admin exists yet.
That was a deliberate fix for the chicken-and-egg problem of a fresh
install with zero staff; do not "simplify" it into a privs check.

## Why groups.cfg tolerates empty groups

`master.c load_access()` parses `(GROUP) members...` with sscanf, and
FluffOS's sscanf stops early when a trailing literal has nothing left
to match. A member-less `(AMBASSADOR)` line therefore needs a second
bare-parenthes parse pass, which is intentional: empty groups are how
retired ranks keep their ACL entries without members. CLAUDE.md rule 18
documents the whole trap family.

## Why tell and reply live under secure/cmds/mortal/

Inherited placement from upstream Nightmare III, where intermud tells
touched network services considered privileged. Moving them would gain
nothing and would invalidate muscle memory about where they are, so
they stay; the cost is remembering that editing them requires a full
reboot like everything else under `secure/cmds/`.

## Why player saves are plain text under save/users/

One file per player, human-readable, first-letter sharded. Trivial to
inspect when debugging, trivial to back up, no database dependency for
a small MUD. The tradeoff (anyone with shell access reads everything)
is accepted; the mitigations are the repo-level rule against touching
real saves in development and password hashing in the save itself.
