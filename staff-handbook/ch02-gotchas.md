# Chapter 02 - Known Gotchas and Conversion Quirks

Part of the AetherMUD Staff Handbook. See [index.md](index.md) for the full chapter list.

Read this chapter before touching any code. These are the mistakes that
have already cost time. Each entry describes the trap, the correct
behavior, and why it matters.

---

## LPC Code Rules

### No // Comments

FluffOS 2.9 does not accept C++-style comments. All comments must use
C-style syntax.

Wrong:
```c
// this sets the player's race
```

Correct:
```c
/* this sets the player's race */
```

A file with // comments will fail to compile. The error shows in
log/runtime but the cause is not always obvious at a glance.

---

### C89 Variable Declarations - Always at the Top

All variable declarations must appear at the top of the function block,
before any code statements. This is strict C89 compliance required by
FluffOS 2.9. Mid-function declarations cause compile errors that are
hard to trace.

Wrong:
```c
void do_thing() {
    write("Hello\n");
    int x;      /* declaration after a statement - compile error */
    x = 5;
}
```

Correct:
```c
void do_thing() {
    int x;      /* declaration at top, before any statements */
    write("Hello\n");
    x = 5;
}
```

This applies even to short functions. Always declare first.

---

### No Mixed Declarations and Code

A related rule: do not interleave declarations and executable lines.
Declare all variables for a block at the top of that block, then write
the code. The FluffOS 2.9 parser does not accept mixing.

---

### Forward Declarations - Functions Used Before They Are Defined

If a function is called before its definition in the same file, you must
add a forward declaration at the top. Without it, the driver may fail
with an unresolved symbol or call the wrong version.

Wrong (bar is defined after foo, no prototype):
```c
void foo() {
    bar();
}
void bar() {
    write("bar\n");
}
```

Correct (with forward declaration):
```c
void bar();   /* prototype at top of file */

void foo() {
    bar();
}
void bar() {
    write("bar\n");
}
```

Alternatively, use string-form call_out to defer the call to runtime,
which resolves the function by name and bypasses the declaration order
requirement entirely.

---

### Non-Standard Include Paths Use Quoted Full Paths

Standard lib headers in the include search path can use angle brackets:
```c
#include <std.h>
#include <rooms.h>
```

Custom headers that are not in the search path must use a quoted full
path from lib root:
```c
#include "/domains/Praxis/my_header.h"
```

Never use angle brackets for a path outside the standard include directory.
The driver will not find the file and the error message is not always clear.

---

### MDC Creature Setup

There are three separate operations for making an MDC creature. All three
are required and must use the correct calls.

Wrong:
```c
set_mdc(100);             /* no such function on rifts_npc */
set_stats("is_MDC", 1);   /* wrong property key */
```

Correct:
```c
set_property("mdc_creature", 1);  /* marks creature as MDC type */
set_stats("MDC", 100);            /* current MDC pool */
set_stats("max_MDC", 100);        /* max MDC pool */
```

`set_mdc()` exists on rifts_vehicle.c only. Monsters do not have it.

---

### Persistent Player Data Uses setenv/getenv

`set_property` and `query_property` store runtime data only. It is lost
when the player logs out. For data that must survive reconnect (race,
OCC, alignment flags, known psionics, creation step, etc.) use
`setenv` and `getenv`.

Wrong (data lost on relog):
```c
this_player()->set_property("rifts_occ", "headhunter");
```

Correct (data saved and restored with the player object):
```c
this_player()->setenv("rifts_occ", "headhunter");
```

Read with: `this_player()->getenv("rifts_occ")`

All per-player Rifts state (race, OCC, alignment, ISP, creation_step,
psionic flags, etc.) is stored as env vars, not properties.

---

### Deferred Calls with call_out

When a function needs to run after the current execution finishes (after
move() completes, after the call stack returns, etc.) use call_out with
a string name:

```c
call_out("end_psi_effect", 60);       /* no args, 60-second delay */
call_out("end_psi_effect", 60, ob);   /* with an argument */
call_out("end_psi_effect", 0);        /* next driver cycle, no real delay */
```

The string form resolves the function at runtime, which also bypasses
the forward-declaration order requirement. A delay of 0 defers to the
next driver cycle without waiting any real time.

---

## Working with Code

### Do Not Use Shell Search Commands First

When looking for something in the codebase, read the relevant file
directly rather than running find, grep, or ls sweeps. The file
structure is known. Check CLAUDE.md for where things live.

Example: to find how races are defined, read /daemon/rifts.c directly.
Do not run grep -r across the lib first.

This rule exists because shell sweeps on large trees consume quota and
often return too much noise to be useful.

---

### Make the Smallest Possible Patch

Fix the specific problem. Do not refactor surrounding code, rename
unrelated variables, or clean up nearby style issues unless the task
explicitly asks for it. A one-line fix should be one line.

If a broader fix is clearly necessary to make the specific fix correct,
make it - but explain why the scope expanded.

---

### Read Files Before Editing

Always read a file before making changes to it. Do not guess the current
content based on earlier reads. The file may have changed since you last
saw it.

---

## Rifts-Specific Traps

### Player Save Path

Player saves are at `/secure/save/users/<letter>/<name>.o`.
Not at /save/players/. That path does not exist. Code that writes
to /save/players/ silently fails without an obvious error.

---

### Log Path Double-Slash Bug

Log paths must be literal strings. Do not concatenate LOG_DIR with a
path that already includes /log/.

Wrong:
```c
LOG_DIR + "/log/setrole"   /* results in /log//log/setrole */
```

Correct:
```c
"/log/adm/setrole"         /* literal string, no concatenation */
```

---

### Warmboot Does Not Reload Core Std Files

After editing std/user.c, std/living.c, std/room.c, std/armour.c,
or std/combat.c, a full reboot is required. Warmboot leaves the old
compiled version running. The code change has no effect until the next
full reboot. See ch01-server.md for the full reload matrix.

---

### Do Not Describe NPCs or Objects in set_long

NPCs and objects placed via reset() appear automatically in room
descriptions below the exits line. Do not hardcode their names or
descriptions in set_long(). Doing so causes them to appear twice and in
the wrong position (above exits).

---

### Smell and Listen Must Not Auto-Display

Text set with set_smell() and set_listen() must only appear when a
player types smell or listen. Never include smell/listen content in
set_long() and never call them during room display.

---

### Room Description Display Order

The correct display order is fixed and must not be changed:

1. Room description
2. Exits line (bold green)
3. NPCs
4. Objects

This is implemented in std/user.c describe_current_room(). Do not
revert or alter this order.

---

### No Ambient Messages in Rooms

Do not add set_ambient_messages() or start_ambient() to any room file.
The rifts_ambient mixin was removed intentionally. Ambient messages
interrupted players and are not wanted.

---

### Em Dashes Are Banned

Do not use em dashes (the long dash character, U+2014) anywhere:
- in-game strings (set_long, set_short, write, tell_object)
- help files and player-facing documents
- code comments
- staff docs and CLAUDE.md

Use a hyphen (-), comma, colon, or rephrase instead.

---

### Wiz Tools Location

Admin and wizard tools live at `/domains/adm/wiz_tools/`.
Not at /domains/Praxis/wiz_tools/. That path does not exist.

---

### move() Returns MOVE_OK (0) on Success

`move()` uses the codes in `/secure/include/move.h`. `MOVE_OK` is **0**.
A nonzero return is an error.

Wrong (treats success as failure):
```c
if(ob->move(env)) {
    write("Placed.\n");
}
```

Correct:
```c
if(ob->move(env) == MOVE_OK) {
    write("Placed.\n");
}
```

The Staff of Creation `clone` command uses this check. Living clones (NPCs)
must go to the wizard's current room, not inventory, or they will not appear
in `look` / `present` and seem invisible.

---

### set_short() Uses Lowercase Articles

Object shorts are embedded mid-sentence in messages like "You take ..." and
"You see ...". Use a lowercase article at the start:

Wrong:
```c
set_short("A staff of creation");
set_short("An RP-Wiz skill tool");
```

Correct:
```c
set_short("a staff of creation");
set_short("an RP-Wiz skill tool");
```

Room listings capitalize via `std/container.c` (`capitalize()` on display).
Proper nouns inside the short stay capitalized (CP-30, Dead Boy, etc.).

QCS `modify <file> short ...` should follow the same rule for NPCs, weapons,
and armor you create.

---

### Wiz Tools Are Invisible in Room Listings

Objects under `/domains/adm/wiz_tools/` call `set_invis(1)` on purpose.
A dropped staff of creation will not appear in the room object list but can
still be taken by name (`get staff`, `get creation`).

### Setrole Log Path

The setrole command logs to `/log/adm/setrole` as a literal string.
Do not change this to use LOG_DIR concatenation.

---

## Reload Responsibility

In-game reloads (update commands) are run by the admin from the Mudlet
client. If you finish a coding session, produce a reload list at the end:

```
update /daemon/rifts_skills.c
update /cmds/mortal/_score.c
```

Do not attempt to run update commands yourself unless you are operating
as admin and have been explicitly asked to. All files on disk do not take
effect in-game until the admin runs the reload.

---

## Spelling Rules

Use American English in all string literals and comments:
armor, color, defense, offense, center, favor.

Never rename std/armour.c or any armour.h header. Never rename these
functions: equip_armour_to_limb, remove_armour_from_limb,
set_armour_type, query_armour_type. Never rename the ARMOUR macro in
std.h. The British spelling is intentional in those identifiers for
historical NM3 compatibility. Apply American spelling only in
user-facing strings and comments.

---

## Help File Format

Help files have no extension. The correct path is:

```
/doc/help/user/score
```

Not /doc/help/user/score.hlp. Before creating a new help entry, check
the existing files in the same directory and match that format exactly.

---

## Do Not Touch Completed Systems Without a Specific Task

The following systems are complete and stable. Do not modify them
unless explicitly asked to fix a reported bug or add a described feature:

- Character creation flow (race list, OCC list, stat roll, setter)
- GHD body pools, spells, PPE, psionics, metamorph
- Alignment storage and display (rifts_alignment env var)
- Score MDC/SDC split and PPE/ISP conditional display
- Wizard race package
- Psionic shortcuts (telepathy, mindblock, telekinesis, empathy, etc.)
- NPC language filter bypass
- Addiction system daemon and its hooks
- Admin tools: grantrace, grantskills
- RP Wizard tools: rp_skill_tool, tattoo_gun, skill_slip
