# Logical operators feature slice: plan (||, &&, and unary !)

Status: planning only, no implementation yet.

Scope: add real support for logical OR (`||`), logical AND (`&&`),
with correct short-circuit evaluation, and unary logical NOT (`!`).
This is the blocker identified by the sixth boot attempt against the
real mudlib: `secure/daemon/master.c` line 65, inside `load_access()`,
the same function the arrays/mappings slice was already unblocking:

```c
if(!lines[i] || lines[i] == "" || lines[i][0] == '#') continue;
```

`sscanf()`, called two lines later in the same function, remains
excluded from this slice, same as the arrays/mappings slice already
excluded it. It is still its own future slice.

## Short-circuit evaluation is a correctness requirement, not an
## optional nicety

STATUS.md's existing note (item 8, "needed before more complex mudlib
conditionals become ergonomic") undersells this. For the actual line
that triggered this blocker, short-circuiting is required for
correctness, not just convenience:

```c
if(!lines[i] || lines[i] == "" || lines[i][0] == '#') continue;
```

If `lines[i]` is null (a real possibility from `explode()` results) or
otherwise empty, the first condition (`!lines[i]`) or the second
(`lines[i] == ""`) is already true, and evaluation must stop there.
`lines[i][0]` in the third condition would index into an empty or
absent string, which should never execute in that case. A naive
"evaluate both operands, then combine with AND/OR" implementation
would evaluate `lines[i][0]` unconditionally and crash or misbehave on
exactly the inputs this line exists to guard against. So:

- `||` must stop evaluating its right operand once the left operand is
  already truthy.
- `&&` must stop evaluating its right operand once the left operand is
  already falsy.

This is why the CodeGen section below builds `||`/`&&` out of jump
instructions instead of a plain arithmetic-style opcode: an opcode
that pops two already-evaluated operands and combines them cannot
short-circuit, the right operand would already have been evaluated
(and its side effects, including any crash, already happened) before
the combining opcode ever runs.

## Current-state findings, confirmed by reading the source before
## proposing anything new

- **Lexer whitelist** (`Lexer.cpp::tokenize()`'s dispatch): currently
  `( ) { } [ ] : ; , - = ! < > * +`. Neither `|` nor `&` is present, so
  either character throws `lexer: unrecognized character` today. This
  is the exact blocker hit by the sixth boot attempt.
- **Two-character operator pattern** (`Lexer.cpp::lexSymbol()`):
  `==`, `!=`, `<=`, `>=` are each formed by checking the second
  character after consuming the first, falling back to the
  single-character token otherwise (e.g. a lone `-` still lexes fine
  as `"-"` today even though nothing consumes bare `-` downstream yet,
  it would just eventually surface as "parse error: expected
  expression" if the parser ever encountered it standalone). `||` and
  `&&` follow exactly this same shape.
- **`BinOp` enum** (`Ast.hpp`): currently
  `{ Eq, Neq, Lt, Lte, Gt, Gte, Add }`. No `Or`/`And` members.
- **Unary `!`: confirmed NOT implemented anywhere.** Searched
  `Parser.cpp`, `Ast.hpp`, `CodeGen.cpp`/`.hpp`, and `VM.cpp` for any
  "unary", "Not", or standalone `!`-handling: the only existing use of
  `!` in the whole compiler is as half of the `!=` two-character token
  in `lexSymbol()`. There is no AST node, no parser rule, no codegen
  branch, and no VM opcode for prefix `!`. This is a second, smaller
  gap bundled into the same blocking line (`!lines[i] || ...`), not
  something already built earlier that just needs wiring up. It is
  included in this slice's scope below, alongside `||`/`&&`, since
  both gaps sit on the exact same line and blocking a slice on it
  again immediately after this one would be wasted effort.
- **`isTruthy(const Value&)`** already exists (`Value.hpp`/`Value.cpp`)
  and is already used by `JumpIfFalse` in `VM.cpp`. It treats
  `monostate` (void/null), zero int, zero double, empty string, and
  null object/array/mapping shared_ptr as falsy, everything else
  truthy. This is exactly the semantics `!lines[i]` needs (catching
  both a null `lines[i]` and, separately, an empty-string one) and is
  directly reusable for both unary `!`'s runtime behavior and the
  short-circuit jump codegen below. No new truthiness logic needed.
- **`OpCode::Dup`** is already declared in `Bytecode.hpp` but has no
  `case` in `VM.cpp`'s execution switch (falls to `default`, throws
  `NotImplementedError`). This is the same "declared but stubbed"
  situation `MakeArray`/`Index`/etc. were in before the arrays slice.
  The short-circuit codegen below depends on `Dup`, so this slice must
  give it a real VM implementation.
- **`JumpIfFalse`/`Jump`/`Pop`** are already fully implemented in
  `VM.cpp` (used by `if`/`while` codegen already). No changes needed
  to any of these three.

## AST changes (Ast.hpp)

1. `BinOp` gains `Or` and `And`:
   ```cpp
   enum class BinOp { Eq, Neq, Lt, Lte, Gt, Gte, Add, Or, And };
   ```
   No new node type for `||`/`&&` themselves: they reuse `BinaryExpr`
   exactly the way `Add` did. Only `CodeGen` treats `Or`/`And`
   differently from the other `BinOp` members (see CodeGen section).

2. New node and enum for unary `!`:
   ```cpp
   enum class UnaryOp { Not };

   struct UnaryExpr : AstNode {
       UnaryOp op = UnaryOp::Not;
       AstPtr operand;
   };
   ```
   `UnaryOp` has a single member today, deliberately mirroring the
   shape of `BinOp` so a later unary operator (e.g. unary minus, if
   ever needed) slots into the same node without a redesign.

## Lexer changes (Lexer.cpp)

1. Add `c == '|'` and `c == '&'` to the character whitelist in
   `tokenize()`'s dispatch, the same one-line-per-character addition
   already used for `*` and `+`.

2. `lexSymbol()` gains two more two-character checks, same shape as
   the existing four:
   ```cpp
   if (c == '|' && peek() == '|') {
       advance();
       return Token{TokenType::Symbol, "||", startLine};
   }
   if (c == '&' && peek() == '&') {
       advance();
       return Token{TokenType::Symbol, "&&", startLine};
   }
   ```

3. A lone `|` or `&` (not doubled) still lexes fine as its own
   single-character `Symbol` token, same as a lone `-` does today.
   Nothing in the parser will ever expect a bare `|` or `&`, so if one
   is ever encountered it surfaces as the existing "parse error:
   expected expression" message, not a new lexer error. Bitwise `|`
   and `&` are out of scope for this slice: no real master.c usage of
   either has been found, consistent with how every other slice here
   has been scoped to confirmed usage rather than the full LPC
   grammar.

4. `!` itself needs no lexer change. It is already a valid standalone
   `Symbol` token (`lexSymbol()` already falls through to the
   single-character case whenever it is not immediately followed by
   `=`). The unary-`!` gap is entirely in the parser/codegen/VM not
   consuming a bare `!` token as a prefix operator, not in the lexer.

## Parser changes (Parser.cpp, Parser.hpp)

Precedence, loosest to tightest, standard C-family grouping, and the
ordering needed for the target line to parse correctly (`==` must bind
tighter than `||`, and `!` must bind to only its immediate operand,
not spill across a following `||`):

```
parseExpr -> parseLogicalOr -> parseLogicalAnd -> parseEquality
          -> parseComparison -> parseAdditive -> parseUnary
          -> parsePostfix -> parsePrimary
```

1. `parseExpr()` changes from calling `parseEquality()` directly to
   calling `parseLogicalOr()`.

2. New `parseLogicalOr()`, same loop-while-operator-matches shape
   already used by `parseComparison`/`parseEquality`/`parseAdditive`:
   ```cpp
   AstPtr Parser::parseLogicalOr() {
       AstPtr left = parseLogicalAnd();

       while (checkText("||")) {
           advance();
           auto right = parseLogicalAnd();
           auto bin = std::make_unique<BinaryExpr>();
           bin->op = BinOp::Or;
           bin->left = std::move(left);
           bin->right = std::move(right);
           left = std::move(bin);
       }

       return left;
   }
   ```

3. New `parseLogicalAnd()`, identical shape, operator text `"&&"`,
   `BinOp::And`, calling `parseEquality()` as its operand parser
   (`parseEquality()` itself is unchanged, it still starts the
   existing chain down through `parseComparison` -> `parseAdditive`).

4. New `parseUnary()`, inserted between `parseAdditive` and
   `parsePostfix`:
   ```cpp
   AstPtr Parser::parseUnary() {
       if (checkText("!")) {
           advance();
           auto operand = parseUnary(); // right-associative: "!!x" chains
           auto notExpr = std::make_unique<UnaryExpr>();
           notExpr->op = UnaryOp::Not;
           notExpr->operand = std::move(operand);
           return notExpr;
       }
       return parsePostfix();
   }
   ```

5. `parseAdditive()`'s two existing calls to `parsePostfix()` (top of
   the function and inside its loop) both change to call
   `parseUnary()` instead. This is the same "swap the operand parser
   for the next tighter level" edit already made twice before when
   `parseComparison`/`parseAdditive` were inserted into the chain.

6. `Parser.hpp` declares `parseLogicalOr`, `parseLogicalAnd`, and
   `parseUnary` alongside the existing precedence-chain methods.

With this chain, the target line parses as intended:
`!lines[i] || lines[i] == "" || lines[i][0] == '#'` groups as
`(!lines[i]) || (lines[i] == "") || (lines[i][0] == '#')`.

## CodeGen changes (CodeGen.hpp, CodeGen.cpp): the short-circuit part

This is the one place a naive "evaluate both operands, then combine"
pattern is wrong, per the correctness requirement above. `Or`/`And`
get dedicated jump-based emission instead of a new arithmetic-style
opcode, reusing the same `emitJumpPlaceholder`/`patchJumpToHere`
machinery `emitIfStmt`/`emitWhileStmt` already use, so this looks like
a small, self-contained `if`, not a new kind of expression opcode.

`emitExpr()`'s existing `BinaryExpr` branch is extended to check the
operator first:
```cpp
if (auto* bin = dynamic_cast<const BinaryExpr*>(&expr)) {
    if (bin->op == BinOp::Or || bin->op == BinOp::And) {
        emitLogicalExpr(*bin);
    } else {
        emitBinaryExpr(*bin);
    }
    return;
}
```
`emitBinaryExpr()`'s switch is untouched: `Or`/`And` never reach it,
so its existing `default: throw` stays a correct safety net, not a
dead branch that silently needs updating.

New private method `emitLogicalExpr(const BinaryExpr& bin)`:

For `BinOp::Or` (`left || right`): if `left` is truthy, short-circuit
immediately, the result is `left`'s own value, `right` is never
evaluated. If `left` is falsy, discard it, evaluate `right`, the
result is `right`'s own value (whatever its own truthiness turns out
to be).
```
emit left                     ; [leftVal]
Dup                            ; [leftVal, leftVal]
JumpIfFalse -> evalRight       ; pops one; falsy -> evalRight; truthy -> fall through
Jump -> end                    ; (truthy path) keep leftVal, skip right entirely
evalRight:
Pop                             ; (falsy path) discard leftVal
emit right                      ; [rightVal]
end:
```

For `BinOp::And` (`left && right`): if `left` is falsy, short-circuit
immediately, the result is `left`'s own value, `right` is never
evaluated. If `left` is truthy, discard it, evaluate `right`, the
result is `right`'s own value.
```
emit left                     ; [leftVal]
Dup                            ; [leftVal, leftVal]
JumpIfFalse -> end             ; pops one; falsy -> end (short circuit, leftVal remains); truthy -> fall through
Pop                             ; (truthy path) discard leftVal
emit right                      ; [rightVal]
end:
```
`And` needs only one jump target, simpler than `Or`, because
`JumpIfFalse`'s own false-branch already is the short-circuit path.

The result pushed is the deciding operand's raw value, not forced to
a strict 0/1, consistent with how the rest of this VM already treats
truthiness generically: `isTruthy()` is called wherever a condition is
consumed (`JumpIfFalse`), values are never normalized to a dedicated
boolean type anywhere else either. This also means `||`/`&&` compose
correctly if ever nested or chained without any extra normalization
step, and it is what makes the target line's three-way `||` chain
work correctly through repeated application of the same two-operand
rule above.

`emitExpr()` also gains a `UnaryExpr` branch. Unlike `Or`/`And`, unary
`!` always evaluates its one operand unconditionally, there is nothing
to short-circuit, so this is a plain new opcode, the same shape as
`Eq`/`Neq`:
```cpp
if (auto* un = dynamic_cast<const UnaryExpr*>(&expr)) {
    emitExpr(*un->operand);
    out_->code.push_back(Instruction{OpCode::Not, 0, 0});
    return;
}
```

## Bytecode/VM changes (Bytecode.hpp, VM.cpp)

- `Bytecode.hpp`: add one new opcode, `Not`, next to the existing
  comparison opcodes. `Or`/`And` need no new opcode at all, they are
  built entirely out of `Dup`/`JumpIfFalse`/`Jump`/`Pop`, all already
  declared (and, after this slice, all already implemented).

- `VM.cpp` execution switch:
  - `case OpCode::Dup`: currently missing, falls to `default`. Add it:
    if `localStack` is empty, throw `LpcRuntimeError("Dup: stack
    underflow")`, same guard style already used by `Add`/`Eq`/etc.;
    otherwise push a copy of `localStack.back()`.
  - `case OpCode::Not`: pop one value (stack-underflow guard, same
    style), push `Value(static_cast<int64_t>(isTruthy(v) ? 0 : 1))`,
    reusing the existing `isTruthy()` helper directly. No new
    truthiness logic needed.
  - No new case for `Or`/`And` themselves: correctness comes entirely
    from `CodeGen`'s jump-based emission plus the already-correct
    `Jump`/`JumpIfFalse`/`Pop`/`Dup` (new) cases underneath it.

## Test plan (mirrors the two prior slices)

Unit tests in `test_lexer.cpp`:
- Lex `"||"` and `"&&"` each as a single two-character `Symbol` token
  (not two separate `|`/`&` tokens), and confirm a lone `|`/`&` still
  lexes as its own one-character `Symbol` without throwing.
- Parse an expression shaped like the target line
  (`!lines[i] || lines[i] == "" || lines[i][0] == '#'`-equivalent
  using this driver's existing var/index/string support) and assert
  the AST groups as expected: outermost `BinaryExpr` op is `Or`, its
  left operand is a `UnaryExpr` (op `Not`) wrapping a `VarRefExpr` or
  `IndexExpr`, matching the precedence chain above.
- Codegen that same snippet and assert the instruction stream
  contains both a `Dup` and a `Not` opcode, plus at least one
  `JumpIfFalse` whose patched target skips forward past other
  instructions, structurally confirming the short-circuit shape, not
  just opcode presence.
- A VM-level execution test proving short-circuiting actually happens
  at runtime, not just that the opcodes exist: construct a small
  program equivalent to `x || y` where evaluating `y` would be
  observable (for example, `y` is itself an indexed read that would
  throw an `LpcRuntimeError` if actually evaluated, similar to the
  real crash risk this slice exists to prevent) and `x` is truthy;
  assert the program returns successfully with `x`'s value and does
  not throw, proving `y` was never evaluated. A mirrored case for
  `&&` with `x` falsy.

`mudlib_stub` test object: extend the existing test-object pattern
(same family as `array_check.c`) with a small function reproducing the
exact blocking shape from `secure/daemon/master.c`, `!x || x == "" ||
x[0] == '#'`-equivalent, called once with a null/empty value and once
with a normal value, with `write()` output in each branch proving both
which branch ran and that no crash occurred on the empty/null case,
which is the concrete scenario this slice exists to fix. Followed by
the same live-boot verification style as before: temporarily wire the
new test object into `master.c`'s `create()`, boot against
`mudlib_stub`, capture the transcript, then revert `master.c`.

## Sequencing after this slice

Still true, unchanged from the arrays/mappings slice's note: this
slice does not touch `sscanf()`. After `||`/`&&`/`!` land,
`secure/daemon/master.c`'s `load_access()` will still stop at its two
`sscanf()` calls two lines later, out of scope here, same
as already noted in `arrays-mappings-slice-plan.md`.
