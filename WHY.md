# WHY: AetherMUD repo root

## Why this layout

The repo root is intentionally thin. Game code lives two levels down under
`nightmare3_fluffos_v2/` so the FluffOS driver source and the LPC mudlib
stay a self-contained tree that can be built, packaged, and reasoned about
without scattering C and LPC across the checkout.

Tracked docs at the root stay limited: overview (`README.md`), mandatory
coding rules (`CLAUDE.md`), navigation breadcrumb (`LLM_BREADCRUMB.md`),
and these HOWTO/WHY entry points. Everything else belongs under `docs/`,
`docs/reference/`, `staff-handbook/`, or `internal/` so contributors can
tell authoritative project docs from personal notes.

## Why Nightmare III + FluffOS 2.9

AetherMUD is a Rifts conversion of an existing Nightmare III mudlib on
FluffOS 2.9, not a greenfield MUD. That choice preserves the classic
early-2000s Rifts MUD feel this project descends from, and it locks in
driver constraints (C89 LPC, inherit baked at compile time, warmboot
limits on `/std/`) that the coding rules exist to respect.

## Why CLAUDE.md sits at the root

Assistants and humans both start here. Putting the mandatory rules next to
the README means every session can open one file and know: no live testing
by the assistant, no ambient messages, spawn-in-reset, American spelling
in strings, and the rest. Subdirectory breadcrumbs always point back up
to this file first.

## Why staff tools and domains are separated

Wizard tools live under `domains/adm/wiz_tools/`, not inside Praxis. Realm
sandboxes live under `lib/realms/`. Player-facing zones live under
`lib/domains/<zone>/`. That split keeps experimental builder work from
leaking into the live world map, and keeps demotion from deleting draft
content someone might still want.

## Why demote and rid are different powers

Demote (Staff of Demotion) returns a wizard to their pre-promotion mortal
identity. Rid (arch + LAW) deletes a player file. Collapsing those into
one verb would make a routine staff correction indistinguishable from a
permanent wipe, so they stay separate on purpose.
