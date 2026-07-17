# Mercenary group system - design plan (planning only)

Status: **planning only, no implementation started.** Written 2026-07-17.
Revisit once the current chargen/foundation gaps are resolved (see
`master_gap_report.txt` OPEN items: spell/psionic auto-grant coverage,
skill-invocation syntax gaps). Do not build against this doc until it has
been reviewed again and open questions below are answered.

## Goals

- Give level 4+ players a way to found a persistent, player-led group
  (a mercenary company / faction) distinct from the existing transient
  adventuring party system (`cmds/mortal/_party.c`) and from clan/OCC
  unlock flavor (`cmds/mortal/_clan.c`, Aerihman/Sunaj).
- Groups own a physical, discoverable asset: a hidden base tied to a
  real room in the existing world, gated by simple item possession
  rather than a permissions system.
- Membership should be visible and revocable through an ordinary game
  object (the emblem), not just a database flag - a player can prove or
  lose membership by what they are carrying.
- Start narrow (3 groups, 3 bases, one gating mechanism) and expand
  later once the pattern is proven.

## Relationship to existing systems (check before building)

- `cmds/mortal/_party.c`: the classic Nightmare temporary adventuring
  party (form/join/leave/add_member/leader). Session-scoped, not
  persistent, no base or emblem concept. The new system should not
  reuse `party` verbs or its data structures - keep the two concepts
  clearly separate in naming (e.g. `group`/`mercgroup`, not `party`).
- `cmds/mortal/_clan.c`: lore-gated OCC unlock (join Aerihman to access
  Sunaj Assassin). Different purpose (character-build gate, not a
  persistent player-led organization) but same general shape (a
  membership flag granting access to something). Worth a design glance
  for prior art on how membership state is currently stored per-player,
  even though the mechanism (clan flag vs. carried emblem object) will
  differ here.
- `cmds/guild/` exists as a command directory but is currently empty -
  not in use, not a conflict, but confirms no existing "guild" system to
  collide with.

## Founding a group

- Requires character level 4+ to found (per the level gate given).
- Founding creates: one founder, one group record (name, founding date,
  founder, co-leader slot, member list), and reserves one of the three
  initial secret bases (Chi-Town, Splynn, New Camelot - see below).
- Open question: is founding player-initiated via a command (e.g.
  `group found <name>`), or staff-approved/staff-triggered given there
  are only 3 base slots to start? See open questions.

## Roles

- **Founder** - can add and remove members, can promote a member to
  co-leader, presumed to be the only role that can dissolve the group
  (exact dissolution rules are an open question below).
- **Co-leader** - can add new members (per the spec as given). Whether
  a co-leader can *remove* members, promote/demote, or is capped at one
  co-leader is an open question.
- **Member** - holds an emblem, gets base access while holding it, no
  administrative rights.

## The emblem object

Expected properties (first pass, to refine during implementation
planning):

- `group_name` - which group this emblem belongs to, used by the base
  entrance check to match emblem to base.
- `group_id` or similar stable identifier, separate from the display
  name, in case a group is ever renamed.
- Non-stackable, non-tradeable by default (a traded/stolen emblem
  granting base access to a non-member would defeat the gating model
  unless that is an intentionally desired smuggling/theft angle - flag
  as an open question).
- Should be visibly identifiable (`set_short()`/`examine`) as belonging
  to a specific group, both for roleplay flavor and so other players
  can recognize a member on sight.
- Destroyed or returned to a neutral holding state when a member is
  removed (see remove flow) rather than left as a loose orphaned item
  in the world.

## Add-member flow

1. Founder or co-leader targets a player present with them (exact
   command syntax TBD, but per this repo's established default syntax
   convention it should be `<verb> <target>` - e.g. `groupadd grunt` -
   not a `groupadd at grunt`/prefixed form; see the syntax audit
   already on record for this session).
2. System validates: target is not already a member of this or another
   group (open question: can a player belong to more than one group at
   once - see below), and the acting player actually holds
   founder/co-leader rank for this group.
3. On success: clone and hand the target player a group emblem object
   carrying that group's `group_name`/`group_id`, and add the target to
   the group's member list.

## Remove-member flow

- A member leaves by **dropping or removing (unwearing/uninventorying)
  the emblem** - the spec explicitly ties membership loss to no longer
  carrying the object, not to a separate `groupquit` command (though a
  `groupquit` command that also strips the emblem could exist as a
  convenience wrapper around the same underlying check - open
  question).
- This implies membership must be re-validated continuously (or at
  least on every base-access attempt) by checking current inventory
  for the emblem, rather than trusting a stored "is member" flag that
  could go stale the moment the emblem is dropped elsewhere.
- Open question: does dropping the emblem *outside* the base (e.g. in
  a shop) immediately and silently end membership, or is there a grace
  period / notification to the founder? Given the spec's plain wording
  ("leaves the group by dropping or removing the emblem") the simplest
  read is: immediate and automatic, no confirmation step.

## Base access gating

- Each of the three initial group bases has one hidden entrance in an
  existing room, which only opens for a player who is currently
  carrying that group's emblem.
- Worked example given: the **New Camelot garden fountain** has a
  hidden entrance that opens only for a carrier of that group's
  emblem. This is an existing room (New Camelot is already built per
  `zone-expansion-plan.md`, 30 rooms) - the base entrance is a new
  interactive feature added to an existing room, not a new zone.
- The other two bases: a hidden entrance somewhere in **Chi-Town** and
  somewhere in **Splynn** (both already-built zones per
  `zone-expansion-plan.md`), exact room/object not yet chosen.
- Gating mechanism, first pass: an `enter`/`search`/movement-hook check
  on the hidden-entrance object or exit that calls something like
  `present("group emblem", this_player())` (or a dedicated
  `query_property`/inventory check for the matching `group_id`) before
  allowing passage, mirroring the "no state stored on the player, only
  checked via carried object" model the spec describes.
- Open question: is base access single-group-exclusive (an emblem for
  Group A only opens Group A's base), which seems to be the intent, or
  could there be shared/rival-base mechanics later (raiding another
  group's base, etc.) - explicitly out of scope for this first pass
  unless you say otherwise.

## Open questions (must be answered before implementation)

1. **Founder loss:** what happens to a group's base access, roster, and
   ability to add/remove members if the founder is removed from the
   game (playerwipe, banned) or simply goes inactive for a long period?
   Does the co-leader auto-promote to founder? Is there a staff
   intervention path? Does the base become permanently inaccessible if
   both founder and co-leader vanish?
2. **Size cap:** is there a maximum member count per group? Unbounded
   membership could make a secret base meaningless as an exclusive
   space.
3. **Group cap / growth:** the spec says "the first three mercenary
   groups" will get bases in Chi-Town, Splynn, and New Camelot - does
   the system support a 4th+ group founding later without a base (pure
   social/administrative group), or is founding hard-capped at 3 until
   more secret bases are built? If capped, how is "founding" gated once
   3 exist - first-come-first-served, staff-assigned, or something
   else?
4. **Co-leader powers:** can a co-leader remove members and/or promote
   others, or only add? Can there be more than one co-leader? Can the
   founder demote/replace a co-leader?
5. **Multi-group membership:** can a player hold emblems for more than
   one group at once, or is membership exclusive (joining a new group
   auto-drops the old emblem)?
6. **Emblem transfer/theft:** is the emblem tradeable, stealable, or
   droppable-and-pickupable by a non-member for a one-time base
   "break-in", or should it be bound (no-drop except for the
   intentional leave action, no-trade always)? This materially changes
   how secure the "secret base" concept actually is.
7. **Dissolution:** can a group be dissolved entirely (by the founder,
   by staff, by attrition to zero members)? What happens to the base
   room and any remaining emblems when that happens?
8. **Command surface:** exact verb set and syntax (e.g. `group found`,
   `groupadd`, `groupkick`, `group who`) - not decided here, but should
   follow the plain `<verb> <target>` convention already established
   for spells/skills in this codebase rather than requiring prefixed
   forms.
9. **Persistence format:** where does group/roster data live - a new
   daemon (e.g. `daemon/mercenary_groups_d.c`), or player-object
   properties plus a small save file? Not decided; should follow
   existing daemon conventions (see `daemon/` preload rules in
   CLAUDE.md) once implementation planning starts.

## Explicitly out of scope for this first pass

- Base interior design/room content (not addressed at all here - this
  doc only covers the entrance-gating mechanic, not what the base
  contains).
- PvP, base raiding, or rival-group mechanics.
- Any group beyond the first three (see open question 3).
- Reusing or modifying the existing `_party.c` temporary-party system.
