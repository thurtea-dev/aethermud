# Top-level object variables feature slice: plan

Status: planning only, no implementation yet.

Scope: add support for top-level (per-object, sometimes loosely called
"global") variable declarations, the LPC construct where a bare
`[modifiers] type [*] name;` (or a comma-separated list of names)
appears outside any function body. Confirmed as the blocker identified
by the eleventh boot attempt:

```
[object] compile error in .../secure/daemon/master.c: parse error:
expected "(" in function declaration parameters at line 66 (got ";")
```

Raw line 18 (preprocessed line 66, same file/line mapping convention as
every prior slice doc: this driver preprocesses master.c with `cpp`
before lexing, so boot-log line numbers are from that expanded unit,
not the editor's raw line numbers; this document uses raw numbers
throughout except where matching a boot log verbatim). This was already
predicted by name in the ternary-operator slice's own Step 4 scan, which
looked ahead past its own scope; this slice re-confirms and scopes it
properly rather than taking that prediction on faith.

## Step 1: re-confirmed shapes of every top-level variable declaration

Re-scanned `master.c` directly with a brace/paren-depth tracker (not a
plain grep, since indentation is not a reliable signal of scope) rather
than trusting the ternary slice's forward-looking note unchanged. Found
exactly four top-level statements, all in one unbroken block at raw
lines 18 through 21, immediately after the `#include`/`#include
"master.h"` block and immediately before `create()`'s opening brace at
raw line 23:

```
18: static private object __Unguarded;
19: static private string __PlayerName;
20: static private object __NewPlayer;
21: static private mapping __Groups, __ReadAccess, __WriteAccess;
```

That is four declaration statements naming six distinct object
variables total (line 21 alone declares three: `__Groups`,
`__ReadAccess`, `__WriteAccess`). No other top-level variable
declaration exists anywhere else in the file: the depth tracker's only
depth-zero, non-brace-adjacent statements in the entire 625-line file
are these same four lines; every other depth-zero position in the file
is the boundary between one function definition and the next.

None of the six carry a declaration-time initializer (no `type name =
expr;` shape). All are bare declarations, optionally comma-separated.
This matters because real LPC (confirmed against the reference driver
below) does allow an initializer at object-variable-declaration time;
master.c simply does not use that shape, so this slice does not need to
implement it to unblock the file, and does not, by design (see the
Parser section).

`master.h` was re-confirmed to have zero top-level variable
declarations, unchanged from the range-indexing and ternary slices'
own findings: it is a pure 52-line prototype list, every line matching
`[modifiers] type [*] name(params);`, all function prototypes.

## Reference driver cross-check: FluffOS's own disambiguation and storage

Per the task's instruction, checked
`/home/thurtea/aethermud/nightmare3_fluffos_v2/fluffos-2.9-ds2.08`
(read-only reference, a real production LPC driver) directly rather
than inferring from general C-family intuition, on both questions this
slice depends on.

**How the real compiler tells a variable declaration from a function
declaration.** `grammar.y`'s `def` nonterminal has these two
alternatives (among others), both sharing the same `type` prefix:

```
def:
    type optional_star identifier
        { ... }
    '(' argument ')'
        { ... }
    block_or_semi
        { ... }
|   type name_list ';'
        { ... }
| ...
```

This is a yacc/LALR(1) grammar, but the disambiguation it relies on is
exactly one token of lookahead past `type optional_star identifier`:
`(` predicts the function alternative, and nothing else can validly
follow that same prefix except `,` (another name in the list), `;`
(end of a single declaration), or `=` (an initializer) predicting the
variable-list alternative. All four continuations are disjoint at one
token, which is why a single-token lookahead is sufficient here and
not a coincidence of this driver's grammar shape. This directly
confirms the design below (peek one token past the shared
modifiers/type/star/name prefix) is not a novel or risky approach; it
is the same disambiguation a real, production LPC grammar relies on,
expressed as explicit token-checking instead of a parser generator's
lookahead table.

**How per-object variable storage is represented.** `object.h`:

```c
struct object {
    ...
    svalue_t variables[1];      /* All variables to this program */
    /* The variables MUST come last in the struct */
};
```

A flexible array member, allocated once per object at the size the
compiled program says it needs (`program_t::num_variables_total`,
`program.h`), addressed purely by integer index, not by name, at
runtime. `compiler.c`'s `define_variable()` assigns each newly
declared global variable the next sequential index
(`ihe->dn.global_num = n`) into that array, the same way
`add_local_name()` assigns each local/parameter the next sequential
local slot (`ihe->dn.local_num`). This is materially the same shape
already stubbed in this driver: `LpcObject::variables()` is already a
`std::vector<Value>`, index-addressed, currently constructed empty and
never resized or touched by anything (see Step 2). The real driver
confirms this stub's shape was the right one to have built; this slice
is the first to actually size and use it.

**How a bare identifier resolves to local vs. object variable at
compile time.** `grammar.y`'s `expr4: L_DEFINED_NAME` action (the
general case of referencing an already-known identifier in an
expression):

```c
if ((i = $1->dn.local_num) != -1) {
    ... CREATE_OPCODE_1($$, F_LOCAL, ...);
} else if ((i = $1->dn.global_num) != -1) {
    ... CREATE_OPCODE_1($$, F_GLOBAL, ...);
} else {
    ... yyerror("Undefined variable ...");
}
```

One identifier table shared across the whole compile
(`ident_hash_elem_t`), where each entry tracks both a `local_num`
(active only while inside the function currently being compiled, reset
to -1 on scope exit) and a `global_num` (set once when the object
variable is declared, persists for the whole file). Local wins over
global when both are set, i.e. a local or parameter shadows an
object variable of the same name; otherwise it falls back to the
object variable; otherwise it is a genuine compile error. This is
exactly the two-tier lookup this slice's CodeGen design below
implements (`locals_` checked before `objectVars_`), confirmed against
the real driver rather than assumed from general scoping intuition.

## Step 2: current state of `LpcObject`'s variable storage

Read `LpcObject.hpp`/`.cpp` directly. The stub is present and matches
the shape described above:

```cpp
class LpcObject : public std::enable_shared_from_this<LpcObject> {
public:
    ...
    std::vector<Value>& variables() { return variables_; }
    ...
private:
    ...
    std::vector<Value> variables_;
    ...
};
```

`variables_` is a `std::vector<Value>` (not a name-keyed map), default-
constructed to size 0 in the constructor's member-init list and never
touched anywhere else in the codebase: `variables()` is not called from
`ObjectManager.cpp`, `VM.cpp`, `CodeGen.cpp`, or anywhere else (grepped
directly, zero call sites). It is exactly what the plan needs to build
on: a flat, index-addressed per-object slot array, the same shape
`locals` already uses inside `VM::run()`, just never sized or wired up.

## Step 3: how `parseProgram()` should distinguish a variable declaration from a function declaration

Read `parseProgram()`/`parseFunction()` directly:

```cpp
std::unique_ptr<Program> Parser::parseProgram() {
    auto program = std::make_unique<Program>();
    while (!atEnd()) {
        program->functions.push_back(parseFunction());
    }
    return program;
}
```

`parseFunction()` unconditionally consumes modifiers, a type, an
optional `*`, and a name, then unconditionally calls
`expectText("(", ...)`, which is exactly why raw line 18 throws: after
consuming `static private object __Unguarded`, the next token is `;`,
not `(`.

Rather than write a second, separate lookahead pass that duplicates
`parseFunction()`'s modifier/type/star/name consumption just to peek
ahead, the proposal factors that shared prefix out into its own
method, called once, with a single branch after it:

```cpp
struct DeclPrefix {
    std::string type;
    bool isArray;
    std::string name;
};

Parser::DeclPrefix Parser::parseDeclPrefix(const std::string& context) {
    while (check(TokenType::Keyword) && isModifierKeyword(peek())) {
        advance();
    }
    Token typeTok = expect(TokenType::Keyword, context + " type");

    bool isArray = false;
    if (checkText("*")) {
        advance();
        isArray = true;
    }

    Token nameTok = expect(TokenType::Ident, context + " name");
    return DeclPrefix{typeTok.text, isArray, nameTok.text};
}
```

`parseFunction()`'s existing body from `expectText("(", ...)` onward
becomes `parseFunctionRest(DeclPrefix prefix)`, unchanged in every
other respect (same prototype-vs-body handling, same param list
parsing). `parseProgram()` becomes the single dispatch point:

```cpp
std::unique_ptr<Program> Parser::parseProgram() {
    auto program = std::make_unique<Program>();
    while (!atEnd()) {
        DeclPrefix prefix = parseDeclPrefix("top-level declaration");
        if (checkText("(")) {
            program->functions.push_back(parseFunctionRest(std::move(prefix)));
        } else {
            auto decls = parseObjectVarDeclRest(std::move(prefix));
            for (auto& decl : decls) {
                program->objectVars.push_back(std::move(decl));
            }
        }
    }
    return program;
}
```

This consumes the shared modifiers/type/star/name prefix exactly once,
with no rewinding and no duplicated logic, matching the task's
instruction to reuse the existing modifier-skipping logic rather than
duplicate it. It mirrors the disambiguation the reference driver's own
grammar performs (see above): the one token immediately after the name
(`(` versus anything else) is sufficient and is checked exactly once.

`parseObjectVarDeclRest(DeclPrefix prefix)` handles the comma-separated
case (raw line 21) and explicitly rejects the initializer shape rather
than mis-parsing it or silently ignoring it:

```cpp
std::vector<std::unique_ptr<ObjectVarDecl>>
Parser::parseObjectVarDeclRest(DeclPrefix prefix) {
    std::vector<std::unique_ptr<ObjectVarDecl>> decls;

    auto makeDecl = [](const std::string& type, bool isArray,
                        const std::string& name) {
        auto decl = std::make_unique<ObjectVarDecl>();
        decl->type = type;
        decl->isArray = isArray;
        decl->name = name;
        return decl;
    };

    if (checkText("=")) {
        throw NotImplementedError(
            "object variable initializer at declaration time (\"" +
            prefix.type + " " + prefix.name + " = ...\")");
    }
    decls.push_back(makeDecl(prefix.type, prefix.isArray, prefix.name));

    while (checkText(",")) {
        advance();
        bool isArray = false;
        if (checkText("*")) {
            advance();
            isArray = true;
        }
        Token nameTok = expect(TokenType::Ident,
                                "object variable declaration name");
        if (checkText("=")) {
            throw NotImplementedError(
                "object variable initializer at declaration time (\"" +
                prefix.type + " " + nameTok.text + " = ...\")");
        }
        decls.push_back(makeDecl(prefix.type, isArray, nameTok.text));
    }

    expectText(";", "object variable declaration");
    return decls;
}
```

Each comma-separated name gets its own independent optional `*`
(matching the reference grammar's `new_name: optional_star
identifier`, not just inheriting the first name's array flag), even
though raw line 21 does not exercise that. This is the same kind of
no-extra-cost defensive correctness the ternary slice's
right-associativity was: the grammar naturally requires per-name
handling once a loop exists at all, so it is built correctly rather
than left subtly wrong for a shape master.c happens not to use.

The initializer shape (`type name = expr;`) is deliberately not parsed.
It is real, standard LPC (confirmed above via the reference grammar's
`new_name: optional_star identifier L_ASSIGN expr0` alternative), but
master.c does not use it anywhere (Step 1), so this slice does not
implement it; it throws a specific, named `NotImplementedError` rather
than a generic parse error or a silent misparse, so a future file that
does use it fails loudly and identifiably rather than being
misinterpreted. Recorded as a confirmed deferred item below, not
silently expanded into this slice's scope.

**AST changes**, alongside the `DeclPrefix` above (`Parser.hpp`/
`.cpp` only, not part of the public AST):

```cpp
struct ObjectVarDecl : AstNode {
    std::string type;
    bool isArray = false;
    std::string name;
};

struct Program : AstNode {
    std::vector<std::unique_ptr<FunctionDecl>> functions;
    std::vector<std::unique_ptr<ObjectVarDecl>> objectVars; // new
    std::vector<std::string> inherits;
};
```

No `Lexer.hpp`/`.cpp` changes at all: every token this construct needs
(identifiers, type/modifier keywords, `*`, `,`, `=`, `;`) is already
lexed by every prior slice. Confirmed directly, not assumed, the same
way the ternary slice confirmed `:` needed no lexer change.

**Declaration order is not enforced, deliberately more permissive than
real LPC.** The reference driver's single-pass compiler requires a
global variable to be declared (textually earlier in the file) before
any function that references it, because `ihe->dn.global_num` is only
set at the point `define_variable()` runs. This driver's architecture
already fully separates parsing from codegen (`parseProgram()` builds
the complete `Program` AST before `CodeGen::generate()` ever runs), so
CodeGen can gather every top-level `ObjectVarDecl` from the whole
`Program` in one pass before generating any function body (see below),
making declaration order irrelevant regardless of where in the file a
variable is declared relative to the functions that use it. This is a
deliberate simplification enabled by an architectural difference this
driver already has, not an oversight; it is strictly more permissive
than real LPC, never less, so it cannot cause a real site to
mis-compile, and master.c's own real declarations already come first
in the file regardless.

## Step 4: runtime design

**Storage allocation, at object load/clone time.** `CompiledProgram`
(`Bytecode.hpp`) gains one new field:

```cpp
struct CompiledProgram {
    std::vector<Instruction> code;
    std::vector<FunctionEntry> functions;
    std::vector<std::string> stringPool;
    std::vector<std::string> objectVarNames; // new
    std::vector<std::string> inherits;
};
```

`CodeGen::generate()` gains a pre-pass, before any function is
compiled, that walks `program.objectVars` once and assigns each a
sequential slot, mirroring the reference driver's
`define_variable()`/`variable_index_offset` scheme:

```cpp
CompiledProgram CodeGen::generate(const Program& program) {
    CompiledProgram result;
    out_ = &result;

    objectVars_.clear();
    for (const auto& varDecl : program.objectVars) {
        if (objectVars_.count(varDecl->name)) {
            throw LpcRuntimeError(
                "codegen: object variable \"" + varDecl->name +
                "\" already declared");
        }
        int slot = static_cast<int>(objectVars_.size());
        objectVars_[varDecl->name] = slot;
        result.objectVarNames.push_back(varDecl->name);
    }

    for (const auto& fn : program.functions) {
        ... // unchanged
    }
    ...
}
```

New `CodeGen` private member: `std::unordered_map<std::string, int>
objectVars_;`, alongside the existing per-function `locals_` (which
keeps being cleared per function exactly as today; `objectVars_` is
populated once per `generate()` call and never cleared during it,
matching its per-object, not per-function, lifetime).

Real LPC's softer behavior on a duplicate top-level declaration is to
warn and alias one variable to hide the other (`compiler.c`,
`define_variable()`'s "Redeclaration of global variable" path); this
slice throws a hard `LpcRuntimeError` instead, matching this driver's
own existing precedent (`declareLocal()` already throws outright on a
duplicate local, no warn-and-continue path exists anywhere in this
codebase today). master.c has no duplicate top-level names (Step 1), so
this never fires against the real file; it is a deliberate, simpler
choice than replicating real LPC's leniency, not an oversight.

Object storage itself is sized once, when the object is constructed,
not per function call (matching the reference driver's object struct,
sized once at object-creation time from
`program_t::num_variables_total`). The natural place is inside
`LpcObject`'s own constructor, not at each of `ObjectManager`'s two
construction call sites (`loadObject()` and `cloneObject()`), so that
correctness is structural (every `LpcObject` is correctly sized the
moment it exists, with nothing for any caller to remember) rather than
something two call sites today, and any future third one, would each
have to duplicate:

```cpp
LpcObject::LpcObject(std::string filename, std::shared_ptr<CompiledProgram> program)
    : filename_(std::move(filename)), program_(std::move(program)) {
    variables_.resize(program_->objectVarNames.size());
}
```

Default-constructed `Value{}` per slot (`monostate`), matching how a
declared-but-not-yet-assigned LPC object variable reads as `0`; the VM's
existing `isTruthy()`/`valuesEqual()` already treat `monostate`
correctly (falsy, only equal to another `monostate`), so no VM-level
change is needed for that part. `ObjectManager.cpp`'s two construction
sites need zero changes.

**Identifier resolution: local vs. object variable.** Parameters and
locals already share one namespace and one slot space in this
codegen today (`CodeGen::generate()` already calls
`declareLocal(param.name)` for every parameter before generating a
function's body, so a parameter is simply an early local slot). So the
real disambiguation this slice adds is exactly two-way, not three-way:
`locals_` (which already covers both true locals and parameters) versus
the new `objectVars_`, checked in that order, matching the reference
driver's local-wins-over-global precedence confirmed above. A local or
parameter that happens to share a name with an object variable
correctly shadows it with zero special-casing needed, since
`declareLocal()` only ever checks for a collision within its own
per-function `locals_` map, never against `objectVars_`.

```cpp
enum class VarKind { Local, ObjectVar };
struct ResolvedVar { VarKind kind; int slot; };

CodeGen::ResolvedVar CodeGen::resolveVariable(const std::string& name) const {
    auto localIt = locals_.find(name);
    if (localIt != locals_.end()) {
        return ResolvedVar{VarKind::Local, localIt->second};
    }
    auto objVarIt = objectVars_.find(name);
    if (objVarIt != objectVars_.end()) {
        return ResolvedVar{VarKind::ObjectVar, objVarIt->second};
    }
    throw LpcRuntimeError("codegen: undeclared variable \"" + name + "\"");
}
```

This replaces `resolveLocal()`, whose only two call sites both need the
same two-way branch:

```cpp
// emitExpr()'s VarRefExpr case:
if (auto* ref = dynamic_cast<const VarRefExpr*>(&expr)) {
    ResolvedVar var = resolveVariable(ref->name);
    OpCode op = (var.kind == VarKind::Local) ? OpCode::PushLocal : OpCode::PushObjectVar;
    out_->code.push_back(Instruction{op, var.slot, 0});
    return;
}

// emitAssignStmt():
void CodeGen::emitAssignStmt(const AssignStmt& stmt) {
    ResolvedVar var = resolveVariable(stmt.name);
    emitExpr(*stmt.value);
    OpCode op = (var.kind == VarKind::Local) ? OpCode::StoreLocal : OpCode::StoreObjectVar;
    out_->code.push_back(Instruction{op, var.slot, 0});
}
```

`emitIndexAssignStmt()` needs no change at all: it already calls
`emitExpr(*stmt.target)` for its target, which already goes through the
shared `emitExpr()` `VarRefExpr` dispatch above, so `__Groups[grp] =
x`-shaped code (an object-variable array/mapping being index-assigned)
is correctly handled as a byproduct, confirmed by reading
`emitIndexAssignStmt()` directly rather than assumed.

**New opcodes: `PushObjectVar`/`StoreObjectVar`, not a reuse of
`PushLocal`/`StoreLocal`.** Considered reusing the existing pair with
some operand-range convention (e.g. negative operands meaning "object
variable slot"), but rejected: the VM's `run()` loop would need two
separate backing storage arrays (`locals` and `obj->variables()`)
regardless, and overloading one opcode's operand to mean two different
storage spaces depending on its sign or range is exactly the kind of
fragile implicit convention this codebase has consistently avoided
elsewhere (compare `RangeIndex` being a distinct opcode from `Index`
rather than a special operand value on `Index`, from the range-indexing
slice). Two new, unambiguous opcodes match that precedent:

```cpp
enum class OpCode : uint8_t {
    PushConst,
    PushInt,
    PushLocal,
    StoreLocal,
    PushObjectVar,   // new
    StoreObjectVar,  // new
    Add, Sub, Mul, Div, Mod,
    ...
};
```

**VM changes: threading the current object into `run()`.** Read
`VM.hpp`/`.cpp` directly: `VM::run()` currently has no notion of the
object it is executing at all.

```cpp
Value VM::run(const CompiledProgram& program, const FunctionEntry& fn,
              std::vector<Value> args);
```

`callFunction()` looks up `obj->program()` and calls `run(program, fn,
args)`, never passing `obj` itself down. This is a real, load-bearing
gap this slice has to close, not a hypothetical: `PushObjectVar`/
`StoreObjectVar` need to read and write `obj->variables()`, and `run()`
has no way to reach it today. Proposed signature:

```cpp
Value VM::run(const CompiledProgram& program, const FunctionEntry& fn,
              std::vector<Value> args, const std::shared_ptr<LpcObject>& obj);
```

`callFunction()` already has `obj` in scope (it is its own first
parameter) and already null-checks it (`if (!obj) return Value{};`)
before ever calling `run()`, so by the time `run()` executes, `obj` is
guaranteed non-null; threading it through is a one-line change at
`callFunction()`'s single call site. `applyMaster()` needs no separate
change: it already calls `callFunction(master, ...)`, so it is covered
automatically.

`run()`'s reentrancy is worth confirming explicitly, since a naive
"VM member variable holding the current object" design would be wrong:
`CallEfun` can invoke `clone_object`, which calls
`ObjectManager::cloneObject()`, which calls `vm_->callFunction(obj,
"create", {})` again, recursively, while the outer `run()` frame's
`while` loop is still active on the C++ call stack. Passing `obj` as a
plain parameter to `run()` is correctly scoped across this reentrancy
for free, since each nested `run()` invocation gets its own `obj`
argument on the C++ call stack; no explicit stack data structure is
needed, confirmed by tracing the actual call path rather than assumed.

Two new `VM::run()` switch cases, mirroring `PushLocal`/`StoreLocal`'s
existing bounds-check style exactly:

```cpp
case OpCode::PushObjectVar: {
    auto& vars = obj->variables();
    if (instr.operand < 0 || static_cast<size_t>(instr.operand) >= vars.size()) {
        throw LpcRuntimeError("PushObjectVar: bad object variable slot index");
    }
    localStack.push_back(vars[instr.operand]);
    ++ip;
    break;
}

case OpCode::StoreObjectVar: {
    auto& vars = obj->variables();
    if (instr.operand < 0 || static_cast<size_t>(instr.operand) >= vars.size()) {
        throw LpcRuntimeError("StoreObjectVar: bad object variable slot index");
    }
    if (localStack.empty()) {
        throw LpcRuntimeError("StoreObjectVar: stack underflow");
    }
    vars[instr.operand] = localStack.back();
    localStack.pop_back();
    ++ip;
    break;
}
```

Persistence falls out for free: `obj->variables()` returns a reference
to the same `LpcObject` instance's storage on every call, so a write in
one function call is visible to every later call against that same
object, exactly matching real per-object state semantics, with no
extra design needed beyond indexing into it directly.

**Inheritance is a confirmed non-issue for this slice.** `inherit` is
not implemented anywhere in this driver today (grepped `Parser.cpp`/
`CodeGen.cpp` directly: `Program::inherits` exists as a field but is
never populated or read by anything), and master.c itself contains no
`inherit` statement. So inherited object variables (a real, separate
LPC concern in the reference driver's `copy_variables()`) are out of
scope here as a pre-existing gap this slice does not touch, not a new
gap this slice introduces.

## Step 5: `this_object()` status

Confirmed out of scope for this slice, and confirmed unnecessary for it
to work: object variable reads/writes only need the executing object's
`variables()` vector, which this slice threads directly into `run()` as
a parameter; nothing about `PushObjectVar`/`StoreObjectVar` requires
the LPC-level `this_object()` efun to return anything real.

This slice does make `this_object()` meaningfully closer to
implementable, though, as a direct byproduct rather than a coincidence:
today `VM::run()` has no notion of "the current object" at all, which
is the actual reason `this_object()` is stubbed (`EfunTable.cpp`
registers it as an unconditional `return Value{};`, since nothing
available inside an efun lambda's `(VM&, args)` signature can reach the
object executing the call). This slice closes exactly that gap for
`run()` itself. What would remain afterward is smaller and better
scoped than it is today: either exposing the `obj` parameter `run()`
now has through some accessor `EfunTable::call()`'s efun lambdas can
reach (their signature is currently `(VM&, std::vector<Value>&)`, no
object parameter), or giving `VM` an explicit "current object" stack
pushed/popped at `run()` entry/exit (needed because, per the
reentrancy point above, a single plain member variable would be
overwritten incorrectly by a nested `run()` call before the outer one
resumes). Not proposed as part of this slice: `this_object()` needs no
new opcode and no AST/parser change of its own, only this remaining
VM/efun-signature plumbing, so building it now would be scope creep
onto an unrelated efun rather than something this slice's own real
blocking sites need.

## Test plan (mirrors the prior slices)

Parser-level tests in `test_lexer.cpp`:
- Parse `object __Unguarded;` at top level; assert `Program::objectVars`
  has one entry with `type == "object"`, `name == "__Unguarded"`,
  `isArray == false`, and `Program::functions` is empty.
- Parse the real comma-separated shape, `mapping __Groups, __ReadAccess,
  __WriteAccess;`; assert `Program::objectVars` has three entries, all
  `type == "mapping"`, names in order.
- Parse a top-level declaration immediately followed by a function
  declaration (`object foo; void create() { }`); assert one entry in
  `objectVars` and one in `functions`, confirming the dispatch correctly
  falls through to `parseFunctionRest()` when `(` follows the name.
- Parse a function declaration to confirm the refactor did not change
  its own behavior: reuse one of the existing real-shape fixtures
  already covered by `testTwoModifiersBeforeReturnTypeParseAsPrototype`
  et al. and assert it still produces the same `FunctionDecl` shape as
  before (regression coverage for the `parseDeclPrefix`/
  `parseFunctionRest` split).
- Parse `mixed *items;` at top level; assert `isArray == true` on the
  resulting `ObjectVarDecl`.
- Parse a top-level declaration-time initializer, `int x = 5;`; assert
  it throws `NotImplementedError`, confirming the deliberate deferral is
  enforced rather than silently mis-parsed.
- Parse the real four-statement block from raw lines 18-21 verbatim (all
  four lines together) and assert `objectVars.size() == 6` with the
  exact six names in order, reproducing the real blocking shape
  directly rather than only smaller synthetic fragments.

Codegen-level tests:
- Codegen a program with one object variable and a function that reads
  it (`object ob; mixed get() { return ob; }`); assert the instruction
  stream contains a `PushObjectVar` with operand `0` and
  `compiled.objectVarNames == {"ob"}`.
- Codegen a program with one object variable and a function that writes
  it (`int x; void set(int v) { x = v; }`); assert a `StoreObjectVar`
  with operand `0`.
- Codegen a program with both a local and an object variable of the
  same name (`int x; void probe() { int x; x = 1; }`); assert the
  local shadows the object variable, i.e. the emitted opcode for `x`
  inside `probe()` is `StoreLocal`, never `StoreObjectVar`.
- Codegen a program declaring two object variables with the same name;
  assert it throws `LpcRuntimeError`.
- Codegen a program referencing an undeclared identifier (covers the
  existing `resolveLocal`-throws test, re-run against the renamed/
  extended `resolveVariable` to confirm the "neither local nor object
  variable" case still throws correctly).

VM-level tests (`runProbe`-style, extended since `runProbe` currently
compiles a single free-standing `probe()` function with no companion
object-variable declarations; this slice's VM tests need their own
small helper that compiles a whole program with object variables and
multiple functions, then calls a named one, since object-variable state
needs to persist across separate `VM::callFunction()` calls to be
tested meaningfully):
- Declare one `int` object variable, one function that stores into it,
  a second function that reads it; call the writer then the reader
  against the same `LpcObject` instance; assert the reader observes the
  writer's value, confirming persistence across separate calls against
  one object.
- Same shape against a second, independently constructed `LpcObject`
  from the same compiled program; assert its object variable starts at
  the default (`monostate`/falsy), confirming storage is per-object
  instance, not shared across every object compiled from the same file.
- A function that both declares a same-named local and reads/writes the
  object variable in sequence (shadow, then a separate statement
  referencing the object variable under a different local name or after
  the local's scope no longer matters for this driver's whole-function
  local scoping); confirms shadowing at the VM level, not just at the
  codegen-inspection level above.
- Reproduce raw line 21's real shape at the VM level: `mapping
  __Groups;` as an object variable, a function that does `__Groups =
  (["a":1]); return __Groups["a"];`, asserting the mapping literal and
  index-read both work correctly against object-variable storage, not
  just local storage (confirms `MakeMapping`/`Index` compose correctly
  with `PushObjectVar`/`StoreObjectVar`, no special-casing needed there
  since both already operate purely on stack values regardless of where
  those values came from).

## `mudlib_stub` live-verification object

New `obj/object_var_check.c`, reproducing the real shape directly:
top-level `object`, `string`, and `mapping` declarations (mirroring
`__Unguarded`/`__PlayerName`/`__Groups`), a `create()` that assigns
into them, and a second function called afterward (via a second
`clone_object()` + `call_other()`, the same pattern
`master.c`'s own `create()` already uses for `simple_login.c`) that
reads them back and asserts the values survived, the same
assert-and-`write()` pattern `range_index_check.c` and
`ternary_check.c` already use. Not written or run as part of this
planning pass, per this repository's standing verification boundary;
proposed here for what that step should cover once the user asks for
implementation.

## Sequencing after this slice

**Predicted next parse-time blocker, confirmed by re-reading the file
structurally (not yet re-run live, since nothing is implemented yet):**
raw line 64, the file's first `for` loop:

```
64:    for(i=0; i<maxi; i++) {
```

`for` is a `Keyword` token already, but `parseStatement()` has no
branch for it (confirmed by re-reading `parseStatement()` directly,
unchanged since the ternary slice's own Step 4 found this), and
`parseFunction()`/`parseProgram()` never call anything named `parseFor`.
Parsing is sequential top to bottom; raw line 64 comes well before any
other still-open gap in the file, so once top-level variable
declarations parse correctly, `for` becomes the very next parse-time
stop, not a deeper or runtime issue. This is a restatement of the
ternary slice's own finding, carried forward unchanged since nothing
between that slice and this one touched `for` or `++`/`--`, not a new
discovery.

**Separately, a runtime (not parse-time) gap this slice's own
investigation surfaced, worth naming even though it sits behind the
`for` gap in file order:** `master.c`'s `create()` calls `new_read()`,
`new_write()`, and `new_groups()` as bare same-object function calls
(no `call_other`, no `->`). `parsePrimary()` turns any `name(args)` into
a `CallExpr` regardless of whether `name` is a real user-defined
function in the same file or an efun, and `CodeGen::emitCallExpr()`
always emits `CallEfun` for it; `EfunTable::call()` throws `"undefined
efun: " + name` for any name not registered as a core efun. Same-object
function-to-function calls are not implemented anywhere in this driver
today (confirmed by reading `emitCallExpr()`/`EfunTable.cpp` directly),
so once `for`/`++` are eventually resolved and this file's functions
actually start executing, calling `new_read()` from `create()` will
fail at runtime with an "undefined efun" error, not a parse error. Not
this slice's concern (it is unrelated to object variables and sits
behind two other confirmed gaps), but recorded here so the slice that
eventually reaches it does not have to rediscover it from scratch, the
same way this document is itself carrying forward the ternary slice's
own findings.

Confirmed still out of scope, unchanged from prior slices' notes:
`sscanf()`, range/array-indexing as an assignment target, open-ended
ranges, negative range bounds, `switch`/`do`/`break` (all confirmed
absent from master.c by the ternary slice, unchanged), inherited object
variables (no `inherit` support exists at all yet, Step 4 above).
Newly confirmed by this slice: declaration-time initializers on
top-level variables (`type name = expr;`, deliberately rejected with a
named `NotImplementedError` rather than silently mis-parsed, see the
Parser section), and same-object function-to-function calls (this
slice's own investigation above, needed before `create()` itself can
actually run to completion against the real master.c, once `for`/`++`
are separately resolved).
