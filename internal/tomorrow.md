Read CLAUDE.md first and confirm you're following the rules there before doing anything else.

We still have a real deployed split-brain bug with my workroom, and the
previous triage did NOT finish the actual fix.

Evidence from live post-deploy testing:
- After reboot/login, I land in:
  /domains/wizards/thurtea/workroom
  which is the old legacy room with only the reference manual
- When I manually go to:
  /realms/thurtea/workroom
  I get the newer room I actually expect to be canonical
- There is still no chest visible in the room I actually log into
- So the real issue is not just chest visibility; login/start-room resolution
  is still sending me to the legacy /domains/wizards/<name>/workroom path

Your task this session:
1. Trace exactly why my live login is still placing me in
   /domains/wizards/thurtea/workroom after reboot, despite the earlier claim
   that /realms/<name>/workroom is canonical and std/user.c migrates old
   wizards/* paths to realms/*.
2. Identify all code paths and saved data involved in determining my login
   destination / home / start room:
   - user save vars
   - login.c path selection
   - std/user.c setup or migration shims
   - any wizard setup helpers
3. Determine whether the problem is:
   - my existing save still carrying a legacy path that the migration shim
     does not actually catch,
   - the migration logic matching the wrong prefix,
   - a separate home/start-room variable bypassing the intended rewrite,
   - or some other path source entirely.
4. Fix it so my actual login lands in the canonical
   /realms/thurtea/workroom, not /domains/wizards/thurtea/workroom.
5. Then address the chest in the correct canonical workroom path:
   - verify whether /realms/thurtea/workroom.c is actually tracked in git or
     still ignored/untracked due to .gitignore
   - if it is still being ignored, fix the repo so THIS specific workroom file
     can be tracked and deployed safely without opening the floodgates on all
     realms files
   - ensure the canonical /realms/thurtea/workroom actually contains the
     supplies chest clone/reset logic
6. Also report plainly whether the legacy
   /domains/wizards/thurtea/workroom is still needed for anything, or whether
   it is just stale dead content that should be retired later.

Important constraints:
- Stay local only; do not tell me to SSH or run manual VPS-side commands in
  the prompt.
- Do not touch unrelated QCS durability work yet.
- Do not touch starting-equipment tasks this session.
- Keep this focused on making the actual deployed login/workroom/chest path
  sane and consistent.
- If the earlier "chest deployed" / "canonical path solved" claims were wrong,
  say so plainly in the report.

After the fix, give the usual session-end report with extra clarity:
- root cause
- exact files changed
- whether save migration is one-time or persistent
- whether a full reboot is required
- whether this is safe to test locally before I deploy