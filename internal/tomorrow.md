Read CLAUDE.md and confirm you are following all rules there
before doing anything else.

Batch 3 is now written and deployed (11 files: Electrical +
Creative, with basic_electronics intentionally held back due to
the data conflict). Do not redo that work.

Next task: investigate basic_electronics so we can resolve it
cleanly before batch 4.

Investigate only, no edits yet:

1. Show the exact conflicting data for basic electronics in
   rifts_skills.c:
   - every case label / alias involved
   - start %, per-level %, max %, category
   - any duplicate or contradictory entries

2. Check whether basic_electronics is referenced anywhere in
   actual code or docs:
   - query_skill / add_skill_points / improve_skill calls
   - any help files, checklists, docs, website pages, or OCC
     skill lists that mention it

3. Tell me whether this is:
   - a true duplicate-definition bug,
   - an alias collision with another skill,
   - or just an ambiguous naming issue for the help topic.

4. Propose the smallest safe resolution for the help-file pass:
   - whether to create a help file now,
   - what filename it should use,
   - what skill values it should cite,
   - or whether the underlying daemon data must be fixed first.

5. Also give me the new running total after batch 3:
   - how many skill help files now exist,
   - how many remain,
   - and which planned batch is next after basic_electronics is
     resolved.

Show me the full picture and your recommended resolution before
writing anything.