Check CLAUDE.md first and confirm you're following the rules there before doing anything else.

Before batch 4, resolve one urgent item from content-fixes-needed.md: getting-started and communication/channels give contradictory instructions about the tell command (one says player-to-player, the others say staff-contact only). Check _tell.c to determine the actual behavior, then fix whichever help file(s) are wrong. This is the first file new players read, so it needs to be correct before playtesting starts. Report what you found and which file was wrong.

Then proceed with batch 4 of the formatting pass: the next ~35 user help files alphabetically (kill/levels/ley-line range through the m's), with the underscore rule now enforced by the conformance checker. Same process as prior batches — report clear vs inferred content, append new stale/wrong findings to content-fixes-needed.md rather than fixing them.

Do not touch any .c source files except to read _tell.c for verification. Do not run any git commands.

End your response with: files changed, whether a reboot is needed (should be no), and the next objective.