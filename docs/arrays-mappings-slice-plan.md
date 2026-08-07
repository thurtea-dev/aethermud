# Arrays and mappings feature slice: plan

Status: planning only, no implementation yet.

Scope: add real support for LPC array literals (`({ ... })`), mapping
literals (`([ ... ])`), indexing as both a read expression and an
assignment target (including chained indexing like `lines[i][0]`), the
array-typed variable declaration syntax (`string *lines;`), and string
and array concatenation via `+` (folded in from an originally separate
gap, see the dedicated decision section below). This mirrors the size
and structure of the earlier control-flow slice (if/while), touching
the AST, parser, VM, and codegen together.

## AST changes (Ast.hpp)

1. `VarDeclStmt` needs an `isArray` field, same pattern already used
   for `Param::isArray` and `FunctionDecl::returnTypeIsArray`:
   ```cpp
   struct VarDeclStmt : AstNode {
       std::string type;
       bool isArray = false;
       std::string name;
       AstPtr initializer;
   };
   ```
   This unblocks `string *lines;` inside `load_access()`, which is a
   different parse path (statement-level) from the parameter/return
   type fix already done, and was not covered by that earlier change.

2. New expression node for array literals:
   ```cpp
   struct ArrayLiteralExpr : AstNode {
       std::vector<AstPtr> elements;
   };
   ```
   Elements are arbitrary expressions (string literals, variable
   references, nested calls, even nested array literals), observed
   directly in master.c, for example `({ "one", "thing" })` and
   `({ ob }) + previous_object(-1)`.

3. New expression node for mapping literals:
   ```cpp
   struct MappingLiteralExpr : AstNode {
       std::vector<std::pair<AstPtr, AstPtr>> entries;
   };
   ```
   Only the empty form `([])` appears in master.c today, but the node
   should support key:value pairs generically since non-empty mapping
   literals exist elsewhere in the mudlib and an empty-only node would
   just move the same gap one file later.

4. New expression node for indexing (read context):
   ```cpp
   struct IndexExpr : AstNode {
       AstPtr target;
       AstPtr index;
   };
   ```
   Chained indexing like `lines[i][0]` is just an `IndexExpr` whose
   `target` is itself another `IndexExpr`, no special casing needed.

5. New statement node for indexed assignment (write/lvalue context).
   `AssignStmt` today is `{ std::string name; AstPtr value; }`, a
   plain-identifier-only lvalue; repurposing `name` to hold an
   arbitrary target expression would break its existing contract, so a
   parallel node is cleaner:
   ```cpp
   struct IndexAssignStmt : AstNode {
       AstPtr target;
       AstPtr index;
       AstPtr value;
   };
   ```
   This covers both observed cases in `load_access()`:
   `ref[fl] = explode(ac, ":");` and `ref[fl] = ({});`.

## Parser changes (Parser.cpp)

1. `parseVarDeclStatement()`: after reading the base type keyword,
   check for an optional `*` before the identifier and set
   `decl->isArray`, exactly mirroring the fix already applied in
   `parseFunction()` and `parseParamList()`.

2. Literal disambiguation in `parsePrimary()`. Plain `(` already means
   "parenthesized subexpression" (existing code, lines 100-105).
   `([` and `({` need to be recognized before that generic path
   triggers, since `(` is the first character of all three forms:
   - `(` followed by `[` -> mapping literal: consume `(`, `[`, then a
     comma-separated `expr : expr` list (or nothing for empty), then
     `]`, `)`.
   - `(` followed by `{` -> array literal: consume `(`, `{`, then a
     comma-separated expression list (or nothing for empty), then
     `}`, `)`.
   - `(` followed by anything else -> existing grouped-expression path,
     unchanged.
   This needs one token of lookahead past the `(`, which `peekAt(1)`
   already supports.

3. Chained indexing as a read expression belongs in `parsePostfix()`,
   alongside the existing `->` handling: after parsing the primary
   expression, loop while the next token is `[`, consuming
   `[ expr ]` and wrapping the expression built so far in an
   `IndexExpr`. Looping (not a single check) is what makes
   `lines[i][0]` work without extra cases.

4. Indexed assignment as a statement. `parseStatement()`'s current
   dispatch to `parseAssignStatement()` only fires for
   `Ident '='` (line 297: `check(TokenType::Ident) && peekAt(1).text ==
   "="`). It needs a second, similar lookahead branch for
   `Ident '[' ... ] '='` so `ref[fl] = ...;` is recognized as the start
   of an indexed assignment rather than falling through to a bare
   expression statement. The simplest implementation: reuse
   `parsePostfix()` to parse the target (which already knows how to
   build up `IndexExpr` chains via the loop in point 3), then check
   whether the result is an `IndexExpr` followed by `=`; if so, build
   an `IndexAssignStmt` from its `target`/`index`, otherwise fall
   through to the existing plain-expression-statement path. This
   avoids duplicating the indexing-parse logic between read and write
   contexts.

## VM changes (Value.hpp, Bytecode.hpp, VM.cpp)

What already exists today, confirmed by reading the current source
before proposing anything new:

- `Value.hpp` already declares `Array { std::vector<Value> items; }`
  and `Mapping { std::vector<std::pair<Value, Value>> entries; }`, and
  `ValueVariant` already includes `std::shared_ptr<Array>` and
  `std::shared_ptr<Mapping>` as possible value alternatives. No changes
  needed here, the runtime value representation is already in place.
- `Bytecode.hpp`'s `OpCode` enum already declares `MakeArray`,
  `MakeMapping`, `Index`, `IndexAssign` (plus `Add`, `Sub`, `Mul`,
  `Div`, `Mod`, `Call`, `CallApply`, `Dup`, none of which this slice
  needs). These are genuine stubs: `CodeGen.cpp` never emits them
  today, and `VM.cpp`'s execution switch has no `case` for any of
  them, they fall through to the `default` branch, which throws
  `NotImplementedError`. So the opcode surface is already designed and
  named, only the VM handling and the codegen emission are missing.

What needs to be added to `VM.cpp`'s execution switch:

- `MakeArray`: pop `argCount` values off `localStack` (same pattern
  `CallEfun` already uses to gather its args), build a
  `std::make_shared<Array>()` with those values as `items` in order,
  push it as a `Value`.
- `MakeMapping`: pop `argCount * 2` values off `localStack` in
  key/value pairs, build a `std::make_shared<Mapping>()`, push it.
- `Index`: pop the index value and the target value. If target holds
  an `Array`, treat the index as an integer and bounds-check into
  `items`. If target holds a `Mapping`, linear-scan `entries` for a
  key equal to the index value (via the existing `valuesEqual`
  helper) and push the matching value, or push `Value{}` (void) if not
  found, matching normal LPC mapping-miss behavior. Anything else is a
  runtime `LpcRuntimeError`.
- `IndexAssign`: pop the value, the index, and the target. For an
  `Array` target, bounds-check and overwrite the element in place. For
  a `Mapping` target, find-or-insert the key/value pair. Since
  `Array`/`Mapping` are held via `shared_ptr`, mutating in place is
  sufficient, no need to push a new copy back into the local slot.

## CodeGen changes (CodeGen.cpp, CodeGen.hpp)

- `emitExpr()` gains two new branches: `ArrayLiteralExpr` emits each
  element expression in order then `MakeArray` with `argCount` set to
  the element count; `MappingLiteralExpr` emits each key then each
  value (or interleaved key/value, matching whatever `MakeMapping`'s
  VM implementation expects) then `MakeMapping` with `argCount` set to
  the entry count.
- `emitExpr()` gains an `IndexExpr` branch: emit `target`, emit
  `index`, then `Index`.
- A new `emitIndexAssignStmt()` (declared in `CodeGen.hpp` alongside
  the existing `emitAssignStmt()`), called from `emitStatement()` for
  `IndexAssignStmt`: emit `target`, emit `index`, emit `value`, then
  `IndexAssign`.
- `emitVarDeclStmt()` needs no change: `isArray` is metadata for the
  parser/AST only, `declareLocal()` and slot allocation are already
  purely name-based and do not care about the declared type or its
  array-ness, the same way `Param::isArray` was already confirmed
  inert to codegen in the previous slice.

## Efuns needed to unblock master.c

Only these three are required to get `load_access()` and its callers
compiling and running, registered in `EfunTable.cpp` the same way
`write`, `clone_object`, etc. already are:

- `sizeof(mixed x)`: returns `Array::items.size()` or
  `Mapping::entries.size()` as an int, depending on which alternative
  `x`'s variant holds; 0 for anything else (matches LPC's permissive
  `sizeof`).
- `explode(string str, string sep)`: splits `str` on `sep`, returns an
  `Array` of the resulting substrings as `Value(std::string)` entries.
- `keys(mapping m)`: returns an `Array` containing each key from
  `Mapping::entries`, in insertion order.

`sscanf()` is called in the same function (`load_access`, both
`sscanf(lines[i], "(%s) %s", fl, ac)` and `sscanf(lines[i], "(%s)",
fl)`) and will be the next thing blocking master.c immediately after
this slice lands. It is deliberately excluded from this plan: it is
format-string parsing with reference-parameter multiple returns (`fl`
and `ac` are output parameters, not just inputs), which is a distinct
and non-trivial feature in its own right, unrelated to array/mapping
support, and deserves its own slice.

## Decision: '+' (concatenation) folded into this slice

Originally flagged as an adjacent gap to handle separately. Decision:
fold it into this slice instead, since array literal usage already
found in master.c requires it directly (`({ob})+previous_object(-1)`
at line 280 of the preprocessed output), and string concatenation via
`+` appears constantly throughout the file, for example
`"/secure"+"/cfg"+"/read.cfg"` in every one of the
`new_read`/`new_write`/`new_groups` functions already looked at.
Handling arrays/mappings without `+` would just relocate the next wall
a few lines further, the same problem already avoided by scoping
arrays/mappings as their own slice instead of a smaller stopgap.

The `:` mapping key separator needs no separate treatment beyond what
is already planned: it is consumed as part of the mapping literal
parsing work in the Parser changes section above (the `expr : expr`
entries inside `([ ... ])`), not a standalone lexer or precedence
concern the way `+` is.

Confirmed current status before proposing changes: `+` is not in the
lexer's symbol whitelist today (`( ) { } ; , - = ! < > *`, checked
directly against the current `tokenize()` dispatch), and `BinOp` has
no addition/concatenation member today (`enum class BinOp { Eq, Neq,
Lt, Lte, Gt, Gte };`), so this is a real gap, not already covered.

### Lexer changes for '+'

Add `c == '+'` to the existing dispatch whitelist in `tokenize()`,
alongside `*`'s recent addition, so it becomes a `Symbol` token
instead of throwing the new unrecognized-character error. No multi
character combination is needed (LPC's `+=`/`++` are not observed
anywhere in master.c and are out of scope here), so this is a plain
one-character addition to the existing `else if` condition, the same
shape as the `*` change already made.

### Parser changes for '+'

Add `Add` to `BinOp`:
```cpp
enum class BinOp { Eq, Neq, Lt, Lte, Gt, Gte, Add };
```

Addition/concatenation needs its own precedence level, binding tighter
than comparison and looser than postfix/indexing, matching normal
expression-grammar structure and where `+` sits relative to `==`/`<`
in the examples seen (`sizeof(stack = ({ob})+previous_object(-1))`
must group the `+` before anything else applies to its result). Insert
a new `parseAdditive()` between `parseComparison()` and
`parsePostfix()` in the existing precedence chain:

```
parseExpr -> parseEquality -> parseComparison -> parseAdditive -> parsePostfix
```

`parseComparison()` currently calls `parsePostfix()` directly as its
operand parser (both at the top of the function and inside its loop);
both call sites change to call `parseAdditive()` instead.
`parseAdditive()` itself follows the same loop-while-operator-matches
shape already used by `parseComparison()` and `parseEquality()`:

```cpp
AstPtr Parser::parseAdditive() {
    AstPtr left = parsePostfix();

    while (checkText("+")) {
        advance();
        auto right = parsePostfix();
        auto bin = std::make_unique<BinaryExpr>();
        bin->op = BinOp::Add;
        bin->left = std::move(left);
        bin->right = std::move(right);
        left = std::move(bin);
    }

    return left;
}
```

Declared in `Parser.hpp` alongside the existing `parseComparison`.

### CodeGen/VM changes for '+'

`CodeGen::emitBinaryExpr()` already switches on `BinOp` to choose an
`OpCode` (currently `Eq`/`Neq`/`Lt`/`Lte`/`Gt`/`Gte`); add
`case BinOp::Add: op = OpCode::Add; break;`. `OpCode::Add` is already
declared in `Bytecode.hpp` (confirmed earlier, part of the same
already-declared-but-unimplemented group as `MakeArray`/`MakeMapping`/
`Index`/`IndexAssign`), so no `Bytecode.hpp` change is needed, only a
new `case OpCode::Add` in `VM.cpp`'s execution switch, following the
same pop-two-push-one shape the existing comparison opcodes use.

Priority order, per the decision: string concatenation and array
concatenation are the confirmed real requirements from actual master.c
usage, both must work. Numeric addition should be included too since
it falls out naturally from the same opcode (the operand types just
determine which branch runs), but it is not separately confirmed as
required by master.c today, so it is a natural side effect to include,
not a separately prioritized requirement. Proposed runtime behavior
for `OpCode::Add`, checked by `Value` variant alternative:
- both `std::string`: concatenate, push the combined string.
- both `std::shared_ptr<Array>`: build a new `Array` whose `items` is
  the left array's items followed by the right array's items (append,
  not in-place mutation, since LPC array `+` is not destructive), push
  it.
- both `int64_t`, both `double`, or one of each: numeric addition,
  push the result (promote to `double` if either operand is `double`,
  matching normal LPC numeric-promotion behavior).
- any other combination: runtime `LpcRuntimeError`, no silent
  coercion.

## Minimal test case (step 2)

Mirrors the `mudlib_stub` pattern used for the control-flow slice: a
tiny object added under `mudlib_stub/`, exercised both as a unit test
(parser and codegen assertions) and as a live boot/telnet check.

Proposed file, `mudlib_stub/obj/array_check.c`:

```c
void create() {
    mixed *items;
    mapping scores;
    int total;

    items = ({ "sword", "shield", "potion" });
    scores = ([]);
    scores["sword"] = 10;

    total = sizeof(items);

    write("items: " + total + "\n");
    write("first: " + items[0] + "\n");
    write("sword score: " + scores["sword"] + "\n");
}
```

This exercises, in order: an array-typed local variable declaration
(`mixed *items;`), a non-empty array literal, an empty mapping
literal, indexed assignment into a mapping, `sizeof()` on the result,
indexed read from the array (`items[0]`), and indexed read from the
mapping (`scores["sword"]`), plus string concatenation via `+` in
every `write()` call. It deliberately does not touch `sscanf`, keeping
it scoped to exactly what this slice adds (arrays, mappings, indexing,
and `+`, per the decision above to fold concatenation in). Since `+`
is now in scope for this same slice rather than a separate one, the
test's `write()` calls exercising string concatenation are no longer
an open dependency, they are covered by the same implementation pass
as the array/mapping work.

As a unit test (`tests/test_lexer.cpp`, following existing style):
parse the object's source, assert the array literal produces an
`ArrayLiteralExpr` with 3 elements, assert the indexed assignment
produces an `IndexAssignStmt`, run it through `CodeGen::generate()`
and assert the resulting instruction stream contains `MakeArray`,
`MakeMapping`, `Index`, and `IndexAssign` opcodes.

As a live-verification target: boot the driver against `mudlib_stub`
with this object's `create()` invoked (either as the master object
temporarily, or cloned and called the same way `master.c` already
clones `simple_login.c` today), and confirm the `write()` output shows
the expected item count, first item, and mapping lookup, proving the
VM opcodes actually execute correctly end to end, not just that the
program compiles.

This becomes the target to get green before retrying the real mudlib
boot against `master.c`.
