# Ternary conditional operator feature slice: plan

Status: planning only, no implementation yet.

Scope: add the ternary conditional operator, `cond ? a : b`, as a
genuine expression construct. Confirmed as the blocker identified by
the tenth boot attempt:

```
[object] compile error in .../secure/daemon/master.c: lexer: unrecognized character '?' at line 464
```

Raw line 433, preprocessed line 464 (same mapping convention as the
range-indexing slice: this driver preprocesses master.c with `cpp`
before lexing, so boot-log line numbers are from that expanded unit,
not the editor's raw line numbers; this document uses raw numbers
throughout except where a preprocessed number is needed to match a
boot log verbatim). All three real sites were already found and listed
by the range-indexing slice's own Step 4 investigation, which scanned
ahead for the next gap; this slice re-confirms and scopes them
properly rather than taking that list on faith.

## Step 1: re-confirmed shapes of all three real sites

```
raw 433: tell_object(this_player(1), sprintf("%sTrace written to /log/%s\n", mp["error"], (caught ? "catch" : "runtime")));
raw 469: string objfn = obj ? file_name(obj) : "<none>";
raw 519:       return (__PlayerName ? __PlayerName : "Mudlib");
```

None involve nesting: no site's `a` or `b` branch is itself another
ternary. All three branches on both sides are simple (a string
literal, a variable reference, or a single call expression), never a
compound or nested conditional. So right-associativity in the parser
design below is a correctness property being built defensively, not
something any real site in this file currently exercises. It is kept
in the design anyway (see the Parser section) for the same reason the
char-literals slice built a full escape table despite master.c using
none of its escapes: it is the direct, no-extra-cost consequence of
how the recursive-descent grammar has to be shaped for correctness at
all, not a speculative extra feature bolted on top.

Each site's surrounding context is worth noting since it affects how
little new parser surface this slice actually needs, confirmed by
reading each call site, not assumed:
- raw 433's ternary is the last argument to `sprintf(...)`, itself
  inside an extra layer of parentheses, and that whole `sprintf(...)`
  call is itself an argument to `tell_object(...)`.
- raw 469's ternary is a local variable declaration's initializer,
  `string objfn = ...;`, no surrounding parentheses.
- raw 519's ternary is wrapped directly in parentheses and used as a
  `return` expression.

Every one of these surrounding contexts (a call argument, a variable
declaration initializer, a parenthesized group, a return expression)
already funnels through `parseExpr()` today. Confirmed by reading
`Parser.cpp` directly: `parseArgList()` calls `parseExpr()` per
argument, `parseVarDeclStatement()` calls `parseExpr()` for its
initializer, `parsePrimary()`'s `(` handling calls `parseExpr()` for
the grouped expression, and `parseReturnStatement()` calls `parseExpr()`
for its value. So once ternary sits at the top of `parseExpr()`'s
precedence chain (see the Parser section below), all three real sites
compose correctly with zero additional changes anywhere else in the
parser. This is worth stating plainly rather than assumed, since it is
the reason this slice's parser change is genuinely small: one new
function, one one-line change to what `parseExpr()` calls.

## Step 2: re-scan for other `?` occurrences

Re-ran the search rather than trusting the range-indexing slice's list
unchanged. Same result:

```
148:    if(err=catch(call_other(str, "???")))
163:        catch(call_other(items[i], "???"));
433:        tell_object(this_player(1), sprintf("%sTrace written to /log/%s\n", mp["error"], (caught ? "catch" : "runtime")));
469:    string objfn = obj ? file_name(obj) : "<none>";
519:      return (__PlayerName ? __PlayerName : "Mudlib");
```

Lines 148 and 163 are false positives, the literal three-character
string `"???"` passed as a (deliberately nonexistent) function name to
`call_other()`, fully consumed by `lexString()` before reaching the
main dispatch, the same category of false positive already documented
for `"..."` ellipsis text in the range-indexing slice. `master.h` has
zero `?` occurrences (re-confirmed directly; it is a pure prototype
list, no executable code). So the real count is unchanged at three,
matching what the range-indexing slice already reported.

## Lexer changes (Lexer.cpp)

1. Add `c == '?'` to the character whitelist in `tokenize()`'s
   dispatch, routed to `lexSymbol()` the same way `.` was added by the
   range-indexing slice:
   ```cpp
   } else if (c == '(' || c == ')' || c == '{' || c == '}' ||
              c == '[' || c == ']' || c == ':' ||
              c == ';' || c == ',' || c == '-' || c == '=' ||
              c == '!' || c == '<' || c == '>' || c == '*' || c == '+' ||
              c == '|' || c == '&' || c == '/' || c == '%' || c == '.' ||
              c == '?') {
       tokens.push_back(lexSymbol());
   }
   ```
   `?` has no two-character form in this language (no `?:`-as-one-token,
   no `??`), so it needs no lookahead branch in `lexSymbol()`; it falls
   through to the existing final line and becomes a plain
   `Symbol("?")`, the same way `*` or `+` already do.

2. `:` needs no lexer change: confirmed by reading the current
   whitelist directly, it is already there (added by the
   arrays/mappings slice for mapping-literal `key:value` syntax,
   `([ "sword" : 10 ])`). Checked before assuming it needed adding,
   per the task's instruction. Re-confirmed there is no ambiguity with
   its new ternary use: grepping `Parser.cpp` for `":"` shows exactly
   one consumer today, `expectText(":", "mapping literal entry")`
   inside the mapping-literal parsing path (reached only from
   `parsePrimary()`'s `([` handling). Ternary's `:` is consumed by a
   new, separate parser function (below), reached from a completely
   different call path (the top of `parseExpr()`'s precedence chain).
   Both are real uses of the same token text in different grammar
   positions, exactly the way `-` already means both subtraction and
   `->`'s first character depending on lookahead, or the way a bare
   `Symbol` token's meaning already depends on which parser function is
   looking for it, not on anything the lexer needs to disambiguate.

3. No `Lexer.hpp` changes: no new method, no new `TokenType`.

## AST changes: a new `TernaryExpr` node

Unlike the range-indexing slice's `IndexExpr::rangeEnd` (an optional
field bolted onto an existing node, justified there because the range
and single-index forms shared their entire parse-time shape), ternary
does not fit any existing node. It needs three children (`condition`,
`thenBranch`, `elseBranch`), where `BinaryExpr` has two and `UnaryExpr`
has one, and its runtime behavior, conditionally evaluating exactly one
of two branches, is not a variant of anything `BinaryExpr` or
`UnaryExpr` already does. This is the same reasoning already used
consistently across every prior slice: reuse an existing node only
when the parse-time shape and runtime behavior both genuinely match
(`IntLiteral` for char literals, `BinaryExpr` for `Sub`/`Mul`/`Div`/
`Mod`/`Or`/`And`, `IndexExpr` extended for range indexing), and add a
new node when they do not. Ternary clearly falls in the second
category.

```cpp
struct TernaryExpr : AstNode {
    AstPtr condition;
    AstPtr thenBranch;
    AstPtr elseBranch;
};
```

## Parser changes: a new top-of-chain function, not an assignment-adjacent one

Standard C-family grammars place the ternary conditional just above
assignment and below logical-or, because assignment is itself an
expression in C (`x = y` has a value) and the grammar needs the
ternary's whole three-part shape to bind more tightly than `=` but
looser than `||`. This grammar has no assignment-expression level at
all: confirmed by reading the current precedence chain and
`parseStatement()` directly, `x = y;` is handled entirely as
`AssignStmt` at the statement level (`parseAssignStatement()`), never
inside `parseExpr()`'s chain
(`parseLogicalOr -> parseLogicalAnd -> parseEquality -> parseComparison
-> parseAdditive -> parseMultiplicative -> parseUnary -> parsePostfix
-> parsePrimary`). So "just above assignment" has nothing to sit above
here; the correct placement collapses to simply the new top of
`parseExpr()`'s chain, above `parseLogicalOr`.

```cpp
AstPtr Parser::parseTernary() {
    AstPtr condition = parseLogicalOr();

    if (checkText("?")) {
        advance();
        AstPtr thenBranch = parseTernary();
        expectText(":", "ternary expression");
        AstPtr elseBranch = parseTernary();

        auto tern = std::make_unique<TernaryExpr>();
        tern->condition = std::move(condition);
        tern->thenBranch = std::move(thenBranch);
        tern->elseBranch = std::move(elseBranch);
        return tern;
    }

    return condition;
}

AstPtr Parser::parseExpr() {
    return parseTernary();
}
```

Right-associativity comes from the `elseBranch` recursing into
`parseTernary()` itself rather than stopping at `parseLogicalOr()`, so
`a ? b : c ? d : e` parses as `a ? b : (c ? d : e)`, matching standard
C-family behavior. The `thenBranch` also recurses into `parseTernary()`
(equivalent to calling `parseExpr()` there, since that is now exactly
what `parseExpr()` does); this allows a nested ternary in the
then-branch too, `a ? (b ? c : d) : e`, with no parentheses required
around it either, since it is unambiguously bounded by the mandatory
`:` that follows.

No `Parser.hpp` change beyond declaring the one new `parseTernary`
method alongside the existing precedence-chain declarations.

## CodeGen/VM changes: reuse `Jump`/`JumpIfFalse`, no new opcode

A ternary is structurally an expression-position `if`/`else`: evaluate
the condition, branch, and (unlike `if`/`else`) always leave exactly
one value on the stack as the expression's result. `CodeGen.cpp`
already has this exact shape twice: `emitIfStmt()` (condition, then
`JumpIfFalse`, then-branch, `Jump` to end, else-branch) and
`emitLogicalExpr()` (the `||`/`&&` short-circuit case, which
additionally needs `Dup`/`Pop` because it sometimes keeps the left
operand's own value unchanged). Ternary needs the `Jump`/`JumpIfFalse`
backpatching shape from the first, but not the `Dup`/`Pop` complexity
of the second: it always evaluates exactly one of its two branches and
always produces that branch's freshly-computed value, never a
conditionally-kept earlier value the way `||`/`&&` do. So the design
below reuses the existing `emitJumpPlaceholder()`/`patchJumpToHere()`
helpers directly, the same ones `emitIfStmt()` and `emitLogicalExpr()`
already call, and needs no new `OpCode`.

```cpp
void CodeGen::emitTernaryExpr(const TernaryExpr& tern) {
    emitExpr(*tern.condition);
    size_t jumpIfFalseIdx = emitJumpPlaceholder(OpCode::JumpIfFalse);

    emitExpr(*tern.thenBranch);
    size_t jumpToEndIdx = emitJumpPlaceholder(OpCode::Jump);

    patchJumpToHere(jumpIfFalseIdx);
    emitExpr(*tern.elseBranch);

    patchJumpToHere(jumpToEndIdx);
}
```

`CodeGen.hpp` declares `emitTernaryExpr` alongside the existing
`emitLogicalExpr`/`emitIfStmt` declarations. `emitExpr()`'s existing
dispatch chain gains one new branch:

```cpp
if (auto* tern = dynamic_cast<const TernaryExpr*>(&expr)) {
    emitTernaryExpr(*tern);
    return;
}
```

No `VM.cpp` change at all: `Jump` and `JumpIfFalse` are already fully
implemented (used by `if`, `while`, and `||`/`&&` today), and the
result is simply whatever value is on top of the stack when execution
reaches the patched `Jump`'s target, exactly the same "the stack
already holds the right answer" property `if`/`else`-as-statement and
`||`/`&&` both already rely on.

## Ternary as an assignment target: confirmed out of scope

Not valid LPC or C in the first place (a ternary expression yields a
value, not something assignable to), so this was never a real
candidate for support, but confirmed rather than assumed: none of the
three real sites remotely resembles `(cond ? a : b) = x;`, and this is
structurally unreachable regardless, not merely unobserved.
`parseAssignStatement()` only ever begins by consuming a leading
`Ident` token before `=` (`Token nameTok = expect(TokenType::Ident,
"assignment target");`), never a parenthesized or conditional
expression, so this shape could not parse as an assignment even if a
real site existed that looked like it.

## Step 4: does resolving all three ternary sites clear the rest of the file?

Split into two separate questions, lexer and parser, since compilation
here is two full passes over the whole file (`tokenize()` completely
before `parseProgram()` ever starts), and each pass can only reveal its
own kind of gap.

**Lexer: yes, fully clear.** Re-ran the same whitelist-scan methodology
the range-indexing slice used, over the current preprocessed-and-
stripped unit, this time also allowing `?` (and `.`/`..`, already
landed): zero unrecognized characters found anywhere in the file, not
just before line 433. This is the first time across all ten boot
attempts that the lexer whitelist scan has come back completely clean,
so this slice is confirmed to be the last purely lexer-level gap.

**Parser: no, and the next gap is earlier and more fundamental than
expected.** Lexing succeeding for the whole file for the first time
means `parseProgram()` will actually run start to finish for the first
time in this project's boot-attempt history, since every prior attempt
never got past `tokenize()`. `parseProgram()`'s loop calls
`parseFunction()` unconditionally for every top-level construct, with
no branch for anything else. `master.c` `#include`s `master.h` at raw
line 15, which cpp inlines textually before any of master.c's own
top-level content; `master.h` is a 52-line pure prototype list (every
line matches `[modifiers] type [*] name(params);`), and every shape in
it is already covered by existing unit tests taken from these exact
real lines (`testTwoModifiersBeforeReturnTypeParseAsPrototype`'s
`"static private void load_access(string cfg, mapping ref);"`,
`testSingleModifierBeforeReturnTypeParsesAsPrototype`'s `"private void
flag(string str);"`, `testFunctionTypeParameterParsesAsPrototype`'s
`"mixed apply_unguarded(function f);"`, and
`testAsteriskParameterTypeParsesWithIsArrayTrue`'s `"int
valid_socket(mixed *info);"` are all literal master.h lines). So the
entire prototype block parses cleanly, confirmed rather than assumed.

Immediately after that block, `master.c` itself declares four
top-level (global/object) variables, raw lines 18 through 21:

```
static private object __Unguarded;
static private string __PlayerName;
static private object __NewPlayer;
static private mapping __Groups, __ReadAccess, __WriteAccess;
```

`parseFunction()` (the only thing `parseProgram()` ever calls) consumes
leading modifiers, a type, an optional `*`, and a name, then
unconditionally calls `expectText("(", "function declaration
parameters")`, assuming every top-level declaration is a function.
For `static private object __Unguarded;`, the token after the name is
`;`, not `(`, so this throws immediately:
```
parse error: expected "(" in function declaration parameters at line 18 (got ";")
```
(preprocessed line 66, confirmed by locating this exact statement in
the preprocessed-and-stripped unit). This is not reachable today
because every prior boot attempt failed at the lexer stage first, so
this gap has been sitting behind all nine of them, undetected until
this slice's Step 4 scan actually let `tokenize()` finish. This means
resolving the ternary operator does not get this file meaningfully
closer to compiling: the very next failure is not deep in the file
near the ternary sites, it is four lines below `master.h`'s own
prototype block, essentially at the top of master.c's real content,
before `create()` itself is ever parsed.

This is a materially different, and larger, gap than a missing
operator: top-level (global/object) variable declarations are not
handled by `parseProgram()`/`parseFunction()` at all today, only
function declarations are. It also compounds with a second, separate
gap: even `parseVarDeclStatement()` (the existing LOCAL variable
declaration parser, used inside function bodies) only handles one
`type name [= init];` per statement; it has no comma-separated
multi-name support, confirmed by reading it directly. Line 21's
`mapping __Groups, __ReadAccess, __WriteAccess;` needs that too, and
needs it whether the eventual fix threads it through the same code
path as local declarations or a separate global-declaration path
introduces its own comma handling. This is a real, separate design
question the ternary slice does not have to answer, but it is
recording it here for whoever scopes that slice.

Beyond that: even once global-variable declarations are supported and
`create()` and everything after it can finally be parsed, the `for`
loop is still completely unimplemented (`for` is a `Keyword` token
already, but `parseStatement()` has no branch for it and
`parseFunction()`/`parseProgram()` never call anything named
`parseFor`), and appears four times, raw lines 64, 94, 161, and 459
(`for(i=0; i<maxi; i++) { ... }` and its siblings), every one of them
using postfix increment (`i++`) in its own increment clause, which is
the same `++`/`--` gap the arithmetic-operators slice already flagged
as deferred, now confirmed to have a second, concrete real site beyond
the four `while(i--)` sites that slice already found. `switch`, `do`,
`break` are not used anywhere in master.c (checked directly, zero
matches), so they do not need to be part of that future slice's scope
regardless of whether they are implemented elsewhere in this driver.

So the real chain ahead, in file order, is: ternary (this slice) ->
top-level variable declarations, including the comma-separated case
(not yet scoped) -> `for` loops plus `++`/`--` (not yet scoped,
overlaps with the arithmetic-operators slice's already-known deferred
item) -> the `Add` string+int gap at line 152 (already known,
confirmed still unresolved by directly reading `VM.cpp`'s `Add` case,
unchanged since the range-indexing slice's report). Each of these is a
real, separate gap, not a variant of this slice's ternary work, the
same way the range-indexing slice's own findings were separate from
what it implemented. None of them are addressed by this slice; they
are named here so the next scoping pass does not have to rediscover
them from scratch.

## Test plan (mirrors the prior slices)

Unit tests in `test_lexer.cpp`:
- Lex `?` and assert it produces a single `Symbol("?")` token.
- Lex the real shape, `caught ? "catch" : "runtime"`, and assert the
  token stream is `Ident, ?, String, :, Ident`, confirming `?` and the
  already-whitelisted `:` both come through correctly in sequence.
- Parse `a ? b : c` and assert the result is a `TernaryExpr` whose
  `condition`, `thenBranch`, and `elseBranch` are each the expected
  `VarRefExpr`.
- Parse the real shape with parentheses, `(caught ? "catch" :
  "runtime")`, confirming the existing `(`-grouping path in
  `parsePrimary()` correctly returns the inner `TernaryExpr` unchanged.
- Parse a right-associativity case, `a ? b : c ? d : e`, and assert the
  outer `TernaryExpr`'s `elseBranch` is itself a `TernaryExpr`, not a
  parse error and not a left-associative (incorrect) grouping.
- Parse a then-branch nesting case, `a ? (b ? c : d) : e`, and assert
  the outer `TernaryExpr`'s `thenBranch` is itself a `TernaryExpr`.
- Codegen `a ? b : c` and assert the instruction stream contains
  exactly one `JumpIfFalse` and one `Jump`, and that the `JumpIfFalse`
  target lands after the then-branch's code (at the start of the
  else-branch), matching `emitIfStmt`'s own jump-target convention.
- VM-level execution tests: `1 ? 10 : 20` returns `10`; `0 ? 10 : 20`
  returns `20`; a ternary whose condition is a comparison,
  `(5 == 5) ? "yes" : "no"`, returns `"yes"`; confirm only one branch
  actually executes by using a marker efun in the untaken branch the
  same way the logical-operators slice's short-circuit tests do
  (`1 ? 1 : nonexistent_marker_efun()` must return `1` without
  throwing "undefined efun").
- A VM-level test reproducing one real blocking site directly:
  `string objfn; object obj; obj = 0; objfn = obj ? "x" : "<none>";
  return objfn;` returns `"<none>"`.

`mudlib_stub` live-verification object: same pattern as the prior
slices, reproducing one of the three real shapes (most simply, raw
line 469's `obj ? file_name(obj) : "<none>"` pattern, since it needs
no extra scaffolding beyond what already exists) against both a null
and non-null target, asserted the way `range_index_check.c` asserts its
own expected path string. Not written or run as part of this planning
pass, per this repository's standing verification boundary; this
document only proposes what that step should cover once the user asks
for implementation.

## Sequencing after this slice

Confirmed still out of scope, unchanged from prior slices' notes:
`sscanf()`, range/array-indexing as an assignment target, open-ended
ranges, negative range bounds. Newly confirmed by this slice's own
Step 4 scan and not yet scoped by any slice: top-level (global/object)
variable declarations, including the comma-separated multi-name case
(raw lines 18 through 21, blocking immediately after this slice
lands), and `for` loops together with postfix `++`/prefix or postfix
`--` (raw lines 64, 94, 161, 459, plus the four `while(i--)` sites the
arithmetic-operators slice already found). The `Add` string+int gap at
line 152 remains open behind all of these, confirmed unchanged by
directly re-reading `VM.cpp`'s current `Add` case. Given the
whole-file compile model, top-level variable declarations are the
strongest candidate for the eleventh boot attempt's blocker once this
slice lands, not `for`/`++`, since line 18 comes well before any of the
`for` sites in file order and parsing is sequential top to bottom. Not
confirmed until an actual boot attempt runs.
