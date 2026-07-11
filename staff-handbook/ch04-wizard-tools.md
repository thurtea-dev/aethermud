# Wizard and Admin Tools

Five tool objects live under `/domains/adm/wiz_tools/`. Each one checks your
position when you use it, not just whether you are holding it, so picking up
the wrong tool from a chest will refuse to work for your role.

| Tool | Command(s) | Who can use it |
|---|---|---|
| Staff of Demotion | `promote`, `demote` | admin only |
| Staff of Dominion | `dominate` | admin, domain |
| Staff of Creation | `create`, `modify`, `add`, `delete`, `about`, `more`, `clone`, `purge` | admin, coding |
| RP Skill Tool | `tool`, `rptool` | admin, domain, rp |
| Tattoo Gun | `inscribe` | admin, domain, rp |

## Staff of Demotion

Menu driven: type `promote` or `demote` to open it. Options: promote a
mortal to a wizard rank, demote a wizard back to mortal (this clears their
tools and role tag), set player level, set player race, set player OCC, set
player stats, grant or remove a title, teleport to a player, summon a
player, force-quit (boot) a player, purge a player file, shut down the MUD
with a reason, and reload a daemon (rifts, occ, rifts_skills, or
rifts_combat).

## Staff of Dominion

Menu driven: type `dominate`. Options: create a domain directory, assign a
domain to a coding wizard, list domains, set a domain description, go to a
domain's start room, grant building access (logged), revoke building
access (logged), list players, and reset loaded rooms.

## Staff of Creation

Grants the QCS builder verbs used to write rooms, NPCs, weapons, and armor:
`create`, `modify`, `add`, `delete`, `about`, `more`, plus `clone` and
`purge`. See ch16 for the full builder workflow and examples.

**clone** (from `tanstaafl_base.c`):

- Resolves relative filenames against your current `cd` path, then against
  your QCS realm directories, so `clone bandit.c` works while building.
- Non-living clones try your inventory first, then the room.
- Living clones (NPCs) always go to the room you are standing in. The
  command prints where the object landed (inventory or room short name).
- Uses `move() == MOVE_OK` (0 means success).

**purge**:

- Removes a non-living object from the current room by id.
- Cannot purge living objects.

## RP Skill Tool

Menu driven: type `tool` or `rptool`. Options: select a player, view their
skills, grant a skill, remove a skill, grant a language, remove a language
(native languages cannot be removed), and view the skill request queue.

## Tattoo Gun

Menu driven: type `inscribe`. For Atlantean and Tattooed Man characters:
select a player, view their tattoos, grant a tattoo, remove a tattoo.

## Getting Your Tools

Tools are granted the moment you are promoted, whether through the Staff of
Demotion (option 1) or `setrole <player> <admin|domain|coding|rp>`. They are
also restored automatically every time you log in, including after a full
reboot, so under normal circumstances you never need to ask for them again.

Your auto-created workroom also holds a mailbox and a reference book
matching your role; type `read book` for a quick command summary. Some
hand-built workrooms (the admin workroom, for one) also keep a staff
supplies chest stocked with a spare of every tool and every role's
handbook: `look in chest`, `take admin handbook from chest`.

## Recovering Missing Tools

If a tool is missing from your inventory:
- Log out and back in. Login re-checks your position and re-grants any
  tool missing for your role.
- If that does not fix it, an admin can run `setrole <you> <your role>` to
  force a resync.
- As a last resort, take a spare from a staff supplies chest if your
  workroom has one. It still checks your position when used, so a spare
  only works if it matches your actual role.

## The call Command

The `call` command lets wizard staff invoke any function on any loaded object directly from the command line. It is the fastest way to inspect or modify a live object without writing a script.

**Syntax**

```
call <object>
call <object>;<function>
call <object>;<function>;<arg>
call <object>;<function>;<arg>;<arg>
```

With no function, `call` dumps the object using the default query functions. Both `<object>` and `<function>` accept array forms (e.g. `users`).

**Common patterns**

| What you want to do | call syntax |
|---|---|
| Dump object state | `call <name>` |
| Check a player's level | `call <name>;query_level` |
| Set HP | `call <name>;set_stats;HP;200` |
| Set MDC | `call <name>;set_stats;MDC;300` |
| Grant mdc_creature | `call <name>;set_property;mdc_creature;1` |
| Set an env var | `call <name>;setenv;autododge;1` |
| Read an env var | `call <name>;getenv;autododge` |
| Call on all users | `call users;query_name` |
| Call on a daemon | `call /daemon/rifts_combat;query_version` |

**Warning:** `call` bypasses all safety checks. A wrong function name silently fails. A wrong argument type or value can crash the target object or corrupt player save data. Test on yourself or on a disposable test NPC before running on a live player. Use dedicated admin commands (`setrole`, `setfaction`, etc.) where they exist instead of `call`.

**See also:** `refs`, `update`, `force`, `eval`
