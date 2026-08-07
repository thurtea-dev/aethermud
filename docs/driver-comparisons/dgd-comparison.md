# AetherMUD C++ LPC Driver vs DGD (Dworkin’s Generic Driver)

**Our driver:** `/home/thurtea/mudstack/aethermud-repo/driver` (features from `docs/STATUS.md` only)  
**Reference:** DGD — open-source since 1.4 (AGPL); docs and kfun list from dworkin.nl, chattheatre/lpc-doc, Phantasmal/DGD FAQ, and the published kfun index (~99 documented kernel functions). Source: https://github.com/dworkin/dgd. DGD is a *from-scratch reimplementation* of the LPMud idea, not a MudOS/FluffOS code fork; LPC dialect and runtime diverge substantially.

---

## 1. Language / architecture overview

### DGD

| Layer | Design |
|---|---|
| Implementation | Originally C; 1.6+ rewritten in C++. Optional JIT extension module; Hydra is a closed-source multi-core variant. |
| Compile path | Built-in LPC compiler → compact bytecode VM (bytecode versions evolved through 2.x). Objects can be **recompiled in place** while the server runs; upgrades migrate instances. |
| VM | Small bytecode interpreter; tasks (“timeslices”) run to completion or hit `rlimits` (ticks / stack). Optional JIT to native code. |
| Object model | **Persistent objects** (named, network/timer capable, explicit `destruct`) and **lightweight objects (LWOs)** (value-like, copy-on-timeslice-boundary semantics). Every object auto-inherits the **auto object**; the **driver object** handles boot, connections, path translation, compile hooks. No MudOS-style simul_efun object — auto object + kfun shadowing fills a similar role. |
| Persistence | **Natively disk-based**: objects swap to disk; **statedumps/snapshots** capture full runtime; **hotboot** into a new server binary without dropping connections. |
| Concurrency | Event-driven **sequential tasks** (Node-like). No overlapping LPC threads. **Atomic functions** roll back state (and buffered I/O) on error — transaction semantics unique in the LPC family. |
| Dialect | “DGD LPC”: closer to early LPMud in some ways; `nil` instead of `0` for many absences; `rlimits`; `atomic`; `parse_string`; limited/optional closures (`-DCLOSURES`). Backward-leaning toward LPMud 2.4.5 / partial 3.1.2 — **not** MudOS Nightmare LPC. |

### Our driver

C++20 FluffOS/MudOS-oriented subset: stack VM, master + simul_efun objects, TCP connect/logon/`input_to`, memory-resident objects, explicit `save_object`/`restore_object` rather than world snapshots. Single-threaded poll loop with stub timers. No LWOs, no atomic rollback, no statedump, no auto/driver object split in the DGD sense (we have FluffOS master + simul_efun).

**Bottom line:** DGD optimizes for *decades-long persistent process evolution*. Our driver optimizes for *running an existing FluffOS mudlib*. They share “LPC on a bytecode VM with objects and call_out,” then diverge.

---

## 2. Feature-by-feature comparison

| Feature | Our driver (STATUS.md) | DGD |
|---|---|---|
| **Closures / function pointers** | MudOS-style bare `(: name, args… :)` only (partial). | Historically **no** closures (2003-era comparisons). Modern builds may enable **`-DCLOSURES`** (`function` keyword); not the FluffOS `(: :)` grammar this mudlib uses. |
| **catch / throw** | `catch(expr)` implemented; **`throw()` not implemented**. | `catch` (expr and richer forms with `:` handlers in kernel lib patterns); `error()` kfun; atomic rollback interacts with errors differently than MudOS throw. |
| **Inheritance** | Multi-level `inherit`, flattened vars, `::` calls, cycle detection. | Multiple inheritance; **recompile/upgrade** rules constrain what can be recompiled in place (inheritable vs non-inheritable discipline in Kernel Library). |
| **Arrays / mappings** | Supported with STATUS-noted operator gaps. | Supported; **copy-on-timeslice** when shared across object arenas — semantic difference from MudOS shared references. |
| **sscanf** | Partial format set. | `sscanf` kfun present (full DGD semantics). Also distinctive **`parse_string`** grammar kfun. |
| **simul_efuns** | Real simul_efun object + tier-3 lookup + `efun::`. | **No simul_efun object.** **Auto object** is implicitly inherited by all; kfuns can be shadowed by auto/afuns. |
| **heart_beat** | Apply recognized; **never called** (stub). | **No dedicated heart_beat apply** in the MudOS sense. Recurring work is done with **`call_out`** (and mudlib conventions). |
| **call_out scheduling** | Efun exists; **scheduler does not fire**. | First-class **`call_out` / `remove_call_out`** kfuns; timing rules documented (order among equal delays not guaranteed; long vs ms callouts). |
| **Efun / kfun count** | ~**58** efuns. | ~**99** documented kfuns in the public index (math, ASN.1, crypto hashes, files, network, compile, dump_state, …) — smaller surface than FluffOS, larger than ours, differently shaped. |
| **Threading / coroutines** | Single-threaded loop. | Sequential tasks; Hydra adds speculative parallel timeslices with rollback. No LPC coroutines. |
| **Persistence** | Per-object `save_object`/`restore_object` (custom format). | World-level **statedump**, swapout, hotboot; objects immortal-by-default across restarts. |
| **Master / connect** | FluffOS master `connect`/`logon`/`compile_object`. | **Driver object** callbacks (`path_read`/`path_write`, compile hooks, connection handling) + auto object — different boot API. |
| **Virtual / compile** | `master()->compile_object` for missing `.c` paths. | `compile_object` kfun + driver hooks; runtime upgrade of existing instances. |

---

## 3. Notable gaps (ours relative to DGD)

These are “DGD has it, we don’t” — **not** a backlog we must copy to run AetherMUD:

1. **Persistence & hotboot** — statedumps, swap, year-long uptimes without mudlib save discipline.
2. **Atomic functions** — transactional rollback of LPC state and buffered I/O on error.
3. **LWO / per-object memory arenas** — DGD’s copy-at-timeslice-end model.
4. **In-place object upgrade** — recompile and migrate live instances.
5. **Auto/driver object architecture** — different privileged API than master/simul_efun.
6. **Working `call_out`** — DGD and MudOS both need this; we still stub it (gap shared with FluffOS comparison).
7. **`parse_string`, ASN.1, dump_state, editor kfuns** — DGD specialities unused by FluffOS mudlibs.
8. **Optional DGD closures** — still would not parse Nightmare `(: :)` sources without a separate frontend.

Gaps the other way (we/MudOS have, classic DGD lacks or treats differently): MudOS `function`/`(: :)` ecosystem, `heart_beat` apply, simul_efun object, enormous efun packages, FluffOS-compatible save strings — i.e. **our mudlib would not run on stock DGD without a port**.

---

## 4. Deliberate design differences

| Choice | Ours | DGD | Match / diverge |
|---|---|---|---|
| **Dialect & mudlib target** | FluffOS/MudOS Nightmare LPC | DGD LPC + Kernel Library / Phantasmal / SkotOS-style stacks | **Hard diverge** — intentional |
| **Privileged objects** | `master` + `simul_efun` | `driver` + `auto` | Same *role* (boot, security, global API), different *shape* |
| **Tiered efun fallback** | local → inherit → simul → efun | Name lookup through inherit/auto + kfun table; shadowing instead of simul object | Related idea, **different mechanism** |
| **Timers** | Stubbed MudOS-style `call_out` + future `heart_beat` | `call_out` only as kernel primitive | We should match **MudOS**, not drop heartbeats because DGD did |
| **Error model** | MudOS-like catch → string/`0`; no atomic | catch + **atomic** rollback | DGD’s atomic is a major design bet we are not taking |
| **Persistence** | Explicit save efuns | Snapshot the world | **Diverges**; STATUS already chose a custom save format for nested Values, still FluffOS-*shaped* API |
| **From-scratch rewrite** | Yes (C++20) | Yes (independent of LPMud code) | Shared *greenfield* spirit; opposite compatibility goals |

Our tiered runtime call chain is a FluffOS/MudOS fidelity choice. DGD’s auto-inherit + kfun shadowing solves “global API in LPC” without a simul_efun file — elegant, but wrong for `secure/SimulEfun/SimulEfun.c`.

---

## 5. Performance / architecture tradeoffs if we keep building

- **Do not pivot toward DGD persistence** unless the product goal changes. Statedumps and LWO arenas would force a mudlib rewrite and abandon FluffOS save/login assumptions already partially working (STATUS live account creation).
- **Steal ideas, not dialect:** DGD’s *task length limits*, careful buffered I/O commit points, and “keep the kernel small, put policy in LPC” are worth studying. Atomic functions are powerful but expensive to retrofit onto a MudOS object graph with shared array refs.
- **call_out completeness** is the overlapping must-have with DGD *and* FluffOS; implement MudOS semantics first.
- **JIT / Hydra-style parallelism** is irrelevant until the interpreter is feature-complete; DGD invested there after decades of API stability.
- **Kfun count ~100 vs FluffOS 300–600:** DGD proves a mud can be small at the C++ boundary if the mudlib (auto/kernel) is rich. Our ~58 efuns can grow MudOS-ward without needing DGD’s full math/ASN surface.

---

## Sources

- `docs/STATUS.md`
- https://www.dworkin.nl/dgd/ (features, timeline, Hydra)
- https://chattheatre.github.io/lpc-doc/dgd/unusual.html (tasks, atomic, disk-based, snapshots)
- https://chattheatre.github.io/lpc-doc/dgd/kfuns.html (~99 kfuns)
- Phantasmal DGD LPC FAQ (`call_out`, `catch`)
- Historical Mateese note (DGD: small feature set, disk-based, no closures — context for older DGD; modern optional `-DCLOSURES`)
- https://github.com/dworkin/dgd
