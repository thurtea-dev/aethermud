# attic/

Holding area for dead and legacy mudlib content moved out of `lib/`
during the 2026-07-19 cleanup pass. Nothing in here is loaded by the
driver: this directory is a sibling of `lib/`, and the driver mounts
`lib/` as its filesystem root, so no in-game command, include path,
inherit, or clone can reach these files.

Directory structure mirrors the original `lib/`-relative paths, so
`attic/domains/Praxis/standardOld/pit.c` was
`lib/domains/Praxis/standardOld/pit.c` before the move. To restore
anything, move it back to the same relative path under `lib/`.

See `docs/lib-cleanup-manifest.md` at the repo root for what was
moved, why each item was flagged, and what was checked before moving
it. Review before deleting anything permanently.
