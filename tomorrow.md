Check CLAUDE.md first and confirm you're following the rules there before doing anything else.

Based on ASSESSMENT.md, do three small, isolated fixes — treat each as a separate scoped change, commit-ready but don't commit:

Fix the casting resource-loss ordering: in _cast.c and _psi.c, move target/range validation before PPE/ISP/APM deduction, so a failed cast (no target, out of range) costs nothing. Do not change any spell/psionic effect logic itself.

Update CLAUDE.md's stale content counts (races 51→61, OCCs 38→62, spells 113→116, skills 102→158) to match the measured figures from ASSESSMENT.md.

Fix playtest-checklist.md's reference to "dominate option 10" — it should say "domain" to match the actual live verb on staff_of_dominion.c.

Do not touch standardOld/, the pending rehash commit, or anything else outside these three items. Report back what changed in each file, and confirm the pending daemon/command.c rehash fix from earlier tonight is still sitting uncommitted and untouched.