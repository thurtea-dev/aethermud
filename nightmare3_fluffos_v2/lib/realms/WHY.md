# WHY: lib/realms/

## Why wizards get private directories

Two reasons, both old MUD wisdom the project kept:

1. Blast radius. New staff learn by breaking things. A realm is a
   sandbox where a broken room strands one wizard, not players; nothing
   in the world links into /realms/, so half-finished content can never
   be wandered into.
2. Ownership and review. The apprentice pipeline (makewiz, build
   tasks, review via the Staff of Creation queue, then setrole) needs a
   place where an apprentice's work is unambiguously theirs to be
   reviewed before an admin promotes the content, and them.

## Why QCS writes here and only here

The Quick Creation System refuses to operate outside the wizard's own
realm. That is a deliberate safety property: a generated-file bug or a
misunderstood modify can only ever rewrite files under
`/realms/<you>/area/`, never a domain. The `qcs_in_realm()` check plus
the sanitized filename rules make the worst case "your own draft is
wrong".

## Why realms are not part of the world

Domain content is the reviewed, permanent game; realm content is
drafts. Keeping the boundary hard (no exits from domains into realms)
means the world map never silently depends on someone's personal
directory, and demoting or removing a wizard never breaks a zone.

## Why realm directories persist after demotion

Deleting a demoted wizard's realm would destroy possibly-good draft
content and any workroom customization they might return to. Disk is
cheap; the directories stay until an admin decides otherwise. The
demotion flow removes powers and tools, not files.
