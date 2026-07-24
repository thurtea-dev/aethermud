Read CLAUDE.md and confirm you are following all rules there
before doing anything else.

Two tasks this session, in order.

TASK 1: Website factual/tone fix (no game code changes)

The website currently describes Praxis as the main starting
zone. That is incorrect. The actual main starting zones are
Chi-Town, Splynn, and Camelot. Find every place on the website
(www/ and any HomePages.c / gateway files that render marketing
or landing content) that states or implies Praxis is the main
starting zone, and correct it to name Chi-Town, Splynn, and
Camelot as the starting zones instead.

While you're in those files, do a tone pass:
- Remove marketing buzzwords and filler language (e.g. vague
  hype phrases, unnecessary superlatives, empty adjectives).
- Rewrite remaining copy to be factual, concise, and
  professional in tone — plain statements about setting,
  mechanics, and starting zones, not sales language.
- Do not invent new lore or features. Only trim/rewrite existing
  claims; do not add unverified claims.
- Keep changes scoped to prose/copy. Do not touch layout, CSS,
  or gateway logic.

Show me a diff-style before/after for every changed block before
finalizing, file by file.

TASK 2: Splynn Preserves — next room batch (approved focus)

Your own investigation ranked Splynn Preserves zone depth as the
highest-confidence next focus (roadmap.html Phase A, 
zone-expansion-plan.md target of +30-50 rooms, 17 of the target
already on disk). This is approved as the next work item.

Steps:
1. Read docs/zone-expansion-plan.md in full and pull the exact
   remaining room list/theme for the Preserves — don't skim.
2. List the 17 existing preserve_*.c rooms and identify the
   logical next 3-5 rooms to add, based on the plan's theme and
   how the existing rooms connect/exit into each other.
3. Propose those 3-5 rooms: name, short description direction,
   how each connects to existing exits, and any new
   monster/NPC/object needs (reuse preserve_beast.c pattern where
   sensible).
4. Do NOT write any files yet. Present the proposal and wait for
   my approval, same review-before-write rhythm as the skill help
   batches.

End with a short status line: task 1 diff ready for review /
task 2 proposal ready for review.