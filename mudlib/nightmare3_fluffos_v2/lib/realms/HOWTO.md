# HOWTO: lib/realms/

## From fresh promotion to first room

1. Get promoted: an admin runs `makewiz <you>`. This creates
   `/realms/<you>/` with a workroom, grants the Staff of Creation (for
   coding-track staff), and your shell starts homed here (`ls` works
   immediately).
2. Go home: type `workroom` in-game.
3. Build: from inside your realm, use QCS (`help build`):

   ```
   build create room north practice_room
   build modify here short A practice chamber
   build modify here long A bare chamber for learning the builder.
   build create npc dummy
   build modify dummy name training dummy
   build add dummy to here
   ```

4. Inspect what QCS wrote: `build more here`, or `cat` / `ed` the
   files under `/realms/<you>/area/`.
5. Hand-polish: generated files are plain LPC. Edit with `ed`, then
   `update` the file in-game and check `/log/catch`.

## Customize your workroom

`/realms/<you>/workroom.c` is yours. Keep the CLAUDE.md room rules
(spawns in reset() with present() guards, nothing spawned described in
set_long) so your workroom stays a good example rather than a bad
habit.

## Graduate content into the world

When a realm area is ready for players, an admin copies the files into
the target domain, fixes the exit paths (realm paths must not leak
into domain rooms), updates each file in-game, and links it from an
existing domain room. The realm copy stays as your draft space.

## If ls says "No current directory"

Your cwd initializes to your realm at login (and at promotion). If you
land without one (e.g. your realm directory was removed), bare `cd`
returns you home, and `cd /realms/<you>` always works while the
directory exists.
