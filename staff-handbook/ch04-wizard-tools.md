# Wizard and Admin Tools

The staff tools live under `/domains/adm/wiz_tools/`. Each role-granted tool
checks your position when you use it, not just whether you are holding it, so
picking up the wrong tool from a chest will refuse to work for your role.

| Tool | Command(s) | Who can use it |
|---|---|---|
| Staff of Demotion | `promote`, `demote` | admin only |
| Staff of Domains | `domain` | admin, domain |
| Staff of Creation | `create`, `modify`, `add`, `delete`, `about`, `more`, `clone`, `purge`, `build`, `review` | admin, coding |
| RP Skill Tool | `tool`, `rptool` | admin, domain, rp |
| Tattoo Gun | `inscribe` | admin, domain, rp |
| Apprentice Kit | `kit`, `apprentice` | apprentice (and admin) |
| Skill Slip | `read`, `approve`, `deny` | granted to RP wizards |

Beyond these, every workroom is stocked with support objects that are not
role-gated the same way: a **mailbox** (`read`, `take`, `empty`), a
**wizard reference book** (`read book`), and — in some hand-built workrooms —
a **staff supplies chest** holding spares of every tool and handbook.

## Staff of Demotion

Menu driven: type `promote` or `demote` to open it. Options: promote a
mortal to a wizard rank, demote a wizard back to mortal (this clears their
tools and role tag), set player level, set player race, set player OCC, set
player stats, grant or remove a title, teleport to a player, summon a
player, force-quit (boot) a player, purge a player file, shut down the MUD
with a reason, and reload a daemon (rifts, occ, rifts_skills, or
rifts_combat).

## Staff of Domains

Menu driven: type `domain`. Options 1-10:

1. **Create new domain directory** — makes `/domains/<name>/` with `areas/`,
   `monsters/`, and `obj/` subdirectories (matching the layout every live
   domain uses) and seeds a minimal `areas/start.c` start room so option 5
   works immediately. Edit or replace that room as you build.
2. **Assign domain to a coding wizard** — logged only. Ownership is recorded
   in `/log/adm/domain_log`; it does not itself grant write access (see below).
3. **List all domains.**
4. **Set domain description** — writes `/domains/<name>/.desc`.
5. **Teleport to a domain's start room** — looks for `areas/start`, then
   legacy `room/start` or `rooms/start`.
6. **Grant building access to a player** — logged only.
7. **Revoke building access** — logged only.
8. **List players currently in a domain.**
9. **Reset all loaded rooms in a domain** — probes `rooms/`, `room/`, and
   `areas/`.
10. **Apprentice submissions** — a submenu to list the domain and coding
    review queues, read a submission, approve, reject, forward to coding,
    accept a coding build as-is, and mark an apprentice ready for `setrole`.

Domain names accept letters, digits, and underscores only and must start
with a letter (no slashes, dots, or spaces). Existing domains are resolved
case-insensitively, so `praxis`, `Praxis`, and `PRAXIS` all reach the real
`Praxis` directory.

**Building access is not automatic.** Options 2, 6, and 7 record the request
to `/log/adm/domain_log` but do **not** change write permissions on their
own. To actually give a player write access to a domain, an admin runs the
`grant` command:

```
grant write to <player> on /domains/<domain>/
```

That records the grant in `/domains/<domain>/adm/access.c`, which the master
consults (via `check_domain`) on every write into that domain. There is no
`ungrant` command — to revoke, an admin edits that `access.c` or calls its
`remove_access()`. (Broad, all-domains access for a whole staff group is a
separate thing, controlled by group membership in `/secure/cfg/groups.cfg`;
the old `master.c valid_write` reference the staff used to print was wrong.)
A future update may wire the staff to apply grants directly; until then,
contact an admin after using option 6 or 7.

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

## Apprentice Kit

Granted to apprentices on promotion. Type `kit` (or `apprentice`) to open it.
It guides temporary staff through a track task in one of three tracks — RP,
domain, or coding — and submits the finished work into the review pipeline.
Domain and admin wizards review those submissions through the Staff of
Domains, option 10 (Apprentice submissions). Coding-track submissions that
are approved or forwarded land in the coding queue for the creation staff to
review with the Staff of Creation's `review` verb.

## Skill Slip

A lightweight approval object handed to RP wizards. Type `read` to view a
pending skill request, then `approve` or `deny` it. It is the quick path for
acting on a single request without opening the full RP Skill Tool queue.

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
