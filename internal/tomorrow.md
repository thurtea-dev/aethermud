First, read CLAUDE.md and confirm you are following its rules before doing anything else.

We are not investigating the introduction system output; the generic player-to-player race labels are intended, and wizard visibility is different by design.

The next task is a foundation-quality audit for chargen/start-state completeness, with minimal-scope fixes only.

Context:

Chargen now gives players their starting primary and secondary skills automatically.

Players can type skills to see what they currently have.

Players can type pskills to see primary skills available to their race/RCC/OCC.

Players can type sskills to see secondary skills available to their race/RCC/OCC.

Players then choose desired skills via roleplay-wizard tell/request flow; this is intentional.

Audit goals:

Trace the full chargen/start pipeline for race, RCC/class, and OCC setup.

Identify where starting equipment, starting inventory/items, starting skills, starting spells, starting psionics, and any other automatic starting grants are assigned.

Verify that the current chargen flow still grants the intended automatic starting primary/secondary skills and does not leave dead/legacy selection paths partially active.

Audit the player-facing skills, pskills, and sskills commands end-to-end:

where each command lives,

who can use it,

what data source it reads,

whether the output matches the current chargen design.

Find concrete mismatches, omissions, broken references, or obvious player-facing failures across race/RCC/OCC startup support.

Produce a prioritized list of the smallest safe fixes needed to finish the foundation QoL pass.

Constraints:

Local Fedora checkout only; do not give VPS commands or do anything VPS-side.

No broad redesigns or speculative cleanup.

Prefer audit first, then propose the smallest fixes.

Do not commit anything unless you find real issues and I approve the changes.

Deliverable:

A concise audit report with exact file/function paths.

A short “foundation remaining” checklist grouped by:

starting equipment/items

starting skills

starting spells/psionics

player commands (skills, pskills, sskills)

any race/RCC/OCC combinations obviously incomplete or inconsistent

End with: “recommended next fix” as a single smallest logical next task.