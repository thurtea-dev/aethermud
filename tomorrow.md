Good news: the reboot happened (apparently the server crashed and 
auto-restarted), and the MDC fix is confirmed live — my prompt now 
shows MDC:5000/5000 PPE:2000/2000 ISP:2000/2000>, matching score 
exactly. The crash handler message also fired correctly with the new 
wording.

But I need to know what caused the crash. Check log/runtime and 
log/errors/ for whatever happened right before "Driver: AetherMUD has 
crashed." Tell me:

1. What was the actual cause of the crash? Was it related to any of 
   tonight's changes (encumbrance removal, std/user.c MDC fix, the 
   command daemon rehash, or anything else), or something unrelated?

2. Is this a one-time event or something that could recur? If it's 
   tied to tonight's work, what's the fix?

3. Confirm the server is currently stable post-crash — run a check 
   similar to before (log/errors for fresh compile failures, 
   secure/save/users/ for exactly one save file, no resurrected pt* 
   saves).

4. Now that the reboot happened organically, is the command-daemon 
   verb-hash issue from before (prompt command not registering) 
   worth fixing preemptively for next time — should we add the 
   distinct_array cleanup to command.c's rehash() now while we're 
   thinking about it, or leave it since it's harmless?

Report back before making any further changes.