Check CLAUDE.md and confirm you're following its rules before doing anything.

I need a bulletproof first-admin setup that works reliably from a fresh git clone, since the VPS deployment gets wiped and re-cloned from GitHub periodically and I can't manually inspect save files each time to verify admin rights.

Investigate and then implement (you may edit files, but do NOT run any git add/commit/push commands — leave changes staged/uncommitted for me to review and commit myself):

1. Review the current admin bootstrap mechanism (groups.cfg seeding via scripts/init-local-config.sh, and the position-based check in secure/include/config.h / creatorp()/archp()). Identify every point where this could fail silently on a brand-new clone — e.g., groups.cfg not being generated, ADMIN_NAME not being set, the placeholder YOUR_ADMIN_NAME staying in the file, or the daemon/master.c group loading happening before or after player login in a way that could race.

2. Make the setup self-healing and foolproof for first boot: if groups.cfg still contains the YOUR_ADMIN_NAME placeholder, or if no admin exists in groups.cfg at all when the driver boots, automatically grant admin/wizard privileges to whichever player name is defined by an ADMIN_NAME environment variable or config value — and if that's not set, grant it to the very first player who successfully creates a new character on that fresh install. Make this idempotent so it doesn't re-trigger once an admin already exists.

3. Update scripts/check-setup.sh so it actually FAILS (not just warns) if the admin bootstrap state is ambiguous or broken before the driver is allowed to start, so this is caught before deployment rather than discovered after connecting.

4. Document exactly how this works in CLAUDE.md and README.md so both of us have a clear reference: what env var or config controls the admin name, what happens on first boot, and how to verify it worked after a fresh clone.

Show me a summary of every file you changed and why, and confirm nothing was committed or pushed. I will review, test locally, then commit and push myself before deploying to the VPS.