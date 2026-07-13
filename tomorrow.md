Check CLAUDE.md first and confirm you're following the rules there before doing anything else.

Cursor made edits to lib/doc/help/user/riftsmud (help scrub), scripts/ws-bridge.py, and mud.sh (bridge hardening: non-blocking I/O, 256 KiB MAX_FRAME cap, pinned pgrep pattern) while you were mid-task on the same prompt. Review all three files now:

Confirm lib/doc/help/user/riftsmud matches CLAUDE.md's tone/formatting conventions for player-facing help files, and confirm no other RiftsMUD brand mentions remain anywhere under lib/doc/help/.

Confirm your own partial edits (faq, start, staff, roleplay, admin, domain, coding help files) are intact and weren't overwritten or duplicated by Cursor's pass.

Review ws-bridge.py and mud.sh's new changes for correctness and safety, the same way you reviewed the original bridge — confirm the non-blocking retry logic and frame cap don't introduce new bugs, and confirm the pgrep pin works as intended.

Report back clean or flag any issues before this is considered done.
