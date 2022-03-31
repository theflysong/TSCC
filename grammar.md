# grammar
```
Number -> [0-9]+
Identifier -> [a-z][A-Z]_([a-z][A-Z]_)*
Variables -> variables
TypeByte -> byte
TypeWord -> word
TypeDword -> dword
TypeQword -> qword
TypeFloat -> float
TypeDouble -> double
TypePointer -> pointer
Assign -> assign
Call -> call
Ret -> ret
Br -> br
Goto -> goto
Nop -> nop
Select -> select
Add -> add
Sub -> sub
Mul -> mul
SMul -> smul
Div -> div
SDiv -> sdiv
Rem -> rem
SRem -> srem
And -> and
Or -> or
Xor -> xor
Shl -> shl
AShr -> ashr
LShr -> lshr
Not -> not
Neg -> neg
Lea -> lea
Inc -> inc
Dec -> dec
With -> with
Cz -> cz
Cnz -> cnz
Ceq -> ceq
Cne -> cne
Cgt -> cgt
Cgts -> cgts
Cge -> cge
Cges -> cges
Clt -> clt
Clts -> clts
Cle -> cle
Cles -> cles
Label -> label
Define -> define
String -> "([-"]|\.)*"
Comma -> ,
Colon -> :
LeftBracket -> [
RightBracket -> ]
Dot -> .
Comment -> #(.|' '|'\t')* -> skip

S -> variables code | code
type -> TypeByte | TypeWord | TypeDword | TypeQword | TypeFloat | TypeDouble | TypePointer
varDef -> LeftBracket type Comma Identifier RightBracket
_varList -> Comma varList | ε
varList -> varDef _varList
variables -> Variables Colon varList
code -> instruction code | ε
assignInstruction -> Assign Identifier Comma clauseInstruction
callInstruction -> Call Identifier argList
argList -> Comma factor argList | ε
factor -> Number | Identifier | String | clauseInstruction
binOpInstruction -> binOp factor Comma factor
binOp -> And | Sub | Mul | SMul | Div | SDiv | Rem | SRem | And | Or | Xor | Shl | AShr | LShr
unOpInstruction -> unOp factor
unOp -> Not | Neg | Lea | Inc | Dec | With
selectInstruction -> Select factor Comma factor Comma factor
nopInstruction -> Nop
brInstruction -> Br factor Comma Identifier Comma Identifier
gotoInstruction -> Goto Identifier
fakeLabelInstruction -> Label Identifier
fakeDefineInstruction -> Define Identifier factor
_fakeInstruction -> fakeLabelInstruction | fakeDefineInstruction
fakeInstruction -> Dot _fakeInstruction
_clauseInstruction -> assignInstruction | callInstruction | binOpInstruction | unOpInstruction
clauseInstruction -> LeftBracket _clauseInstruction RightBracket
instruction -> assignInstruction | callInstruction | nopInstruction | brInstruction | gotoInstruction | fakeInstruction
```

# FIRST&FOLLOW

| Symbol                | FIRST                                                                  | FOLLOW                                                    |
| --------------------- | ---------------------------------------------------------------------- | --------------------------------------------------------- |
| S                     | Variables FIRST(instruction) ε                                         | $                                                         |
| type                  | TypeByte TypeWord TypeDword TypeQword TypeFloat TypeDouble TypePointer | Comma                                                     |
| varDef                | LeftBracket                                                            | Comma FOLLOW(variables)                                   |
| _varList              | Comma ε                                                                | FOLLOW(variables)                                         |
| varList               | LeftBracket                                                            | FOLLOW(variables)                                         |
| variables             | Variables                                                              | FIRST(code) $                                             |
| code                  | FIRST(instruction) ε                                                   | $                                                         |
| assignInstruction     | Assign                                                                 | FOLLOW(instruction) RightBracket                          |
| argList               | Comma ε                                                                | FOLLOW(instruction) RightBracket                          |
| factor                | Number Identifier String LeftBracket                                   | Comma FIRST(instruction) RightBracket                     |
| callInstruction       | Call                                                                   | FOLLOW(instruction) RightBracket                          |
| binOpInstruction      | FIRST(binOp)                                                           | Right Bracket                                             |
| binOp                 | And Sub Mul SMul Div SDiv Rem SRem And Or Xor Shl AShr LShr            | FIRST(factor)                                             |
| unOpInstruction       | FIRST(unOp)                                                            | Right Bracket                                             |
| unOp                  | Not Neg Lea Inc Dec With                                               | FIRST(factor)                                             |
| selectInstrucion      | Select                                                                 | RightBracket                                              |
| nopInstruction        | Nop                                                                    | FOLLOW(instruction)                                       |
| brInstruction         | Br                                                                     | FOLLOW(instruction)                                       |
| gotoInstruction       | Goto                                                                   | FOLLOW(instruction)                                       |
| fakeLabelInstruction  | Label                                                                  | FOLLOW(instruction)                                       |
| fakeDefineInstruction | Define                                                                 | FOLLOW(instruction)                                       |
| _fakeInstruction      | Label Define                                                           | FOLLOW(instruction)                                       |
| fakeInstruction       | Dot                                                                    | FOLLOW(instruction)                                       |
| _clauseInstruction    | Assign Call FIRST(binOp) FIRST(unOp)                                   | RightBracket                                              |
| clauseInstruction     | LeftBracket                                                            | Comma FIRST(instruction) RightBracket FOLLOW(instruction) |
| instruction           | Assign Call Nop Br Goto Dot                                            | FIRST(instruction) $                                      |

# TABLE
```
0: S -> code
1: S -> variable code
2: type -> FIRST(type)
3: varDef-> [ type , Identifier ]
4: _varList -> , varDef
5: _varList -> ε
6: varList -> varDef _varList
7: variables -> Variables : varList
8: code -> instruction code
9: code -> ε
10: assignInstruction -> Assign Identifier, clauseIntruction
11: argList -> , factor
12: argList -> ε
13: factor -> Number
14: factor -> Identifier
15: factor -> String
16: factor -> clauseInstruction
17: callInstruction -> Call Identifier argList
18: binOpInstruction -> binOp factor , factor
19: binOp -> FIRST(binOp)
20: unOpInstruction -> unOp factor
21: unOp -> FIRST(unOp)
22: selectInstruction -> Select factor , factor , factor
23: nopInstruction -> Nop
24: brInstruction -> Br factor , Identifier , Identifier
25: gotoInstruction -> Goto Identifier
26: fakeInstruction -> Dot _fakeInstruction
27: _clauseInstruction -> assignInstruction
28: _clauseInstruction -> callInstruction
29: _clauseInstruction -> binOpInstruction
30: _clauseInstruction -> unOpInstruction
31: clauseInstruction -> [ _clauseInstruction ]
32: instruction -> assignInstruction
33: instruction -> callInstruction
34: instruction -> nopInstruction
35: instruction -> brInstruction
36: instruction -> gotoInstruction
37: instruction -> fakeInstruction
```
|       M\[N,T\]      |Number|Identifier|String| ,| :| [| ]| .|Assign|Call|FIRST(binOp)|FIRST(unOp)|Select|Nop|Br|Goto|Variables|FIRST(type)| $|
|---------------------|------|----------|------|--|--|--|--|--|------|----|------------|-----------|------|---|--|----|---------|-----------|--|
|          S          |   /  |     /    |   /  | /| /| /| /| /|   0  |  0 |      /     |     /     |   /  | 0 | 0|  0 |    1    |     /     | 1|
|         type        |   /  |     /    |   /  | /| /| /| /| /|   /  |  / |      /     |     /     |   /  | / | /|  / |    /    |     2     | /|
|        varDef       |   /  |     /    |   /  | /| /| 3| /| /|   /  |  / |      /     |     /     |   /  | / | /|  / |    /    |     /     | /|
|       _varList      |   /  |     /    |   /  | 4| /| /| /| /|   5  |  5 |      /     |     /     |   /  | 5 | 5|  5 |    /    |     /     | 5|
|       varList       |   /  |     /    |   /  | /| /| 6| /| /|   /  |  / |      /     |     /     |   /  | / | /|  / |    /    |     /     | /|
|      variables      |   /  |     /    |   /  | /| /| /| /| /|   /  |  / |      /     |     /     |   /  | / | /|  / |    7    |     /     | /|
|         code        |   /  |     /    |   /  | /| /| /| /| /|   8  |  8 |      /     |     /     |   /  | 8 | 8|  8 |    /    |     /     | 9|
|  assignInstruction  |   /  |     /    |   /  | /| /| /| /| /|  10  |  / |      /     |     /     |   /  | / | /|  / |    /    |     /     | /|
|       argList       |   /  |     /    |   /  | /| /| /|11| /|  12  | 12 |      /     |     /     |   /  | 12|12| 12 |    /    |     /     |12|
|        factor       |  13  |    14    |  15  | /| /|16| /| /|   /  |  / |      /     |     /     |   /  | / | /|  / |    /    |     /     | /|
|   callInstruction   |   /  |     /    |   /  | /| /| /| /| /|   /  | 17 |      /     |     /     |   /  | / | /|  / |    /    |     /     | /|
|   binOpInstruction  |   /  |     /    |   /  | /| /| /| /| /|   /  |  / |     18     |     /     |   /  | / | /|  / |    /    |     /     | /|
|        binOp        |   /  |     /    |   /  | /| /| /| /| /|   /  |  / |     19     |     /     |   /  | / | /|  / |    /    |     /     | /|
|    unOpInstruction  |   /  |     /    |   /  | /| /| /| /| /|   /  |  / |      /     |     20    |   /  | / | /|  / |    /    |     /     | /|
|         unOp        |   /  |     /    |   /  | /| /| /| /| /|   /  |  / |      /     |     21    |   /  | / | /|  / |    /    |     /     | /|
|  selectInstruction  |   /  |     /    |   /  | /| /| /| /| /|   /  |  / |      /     |     /     |  22  | / | /|  / |    /    |     /     | /|
|    nopInstruction   |   /  |     /    |   /  | /| /| /| /| /|   /  |  / |      /     |     /     |   /  | 23| /|  / |    /    |     /     | /|
|    brInstruction    |   /  |     /    |   /  | /| /| /| /| /|   /  |  / |      /     |     /     |   /  | / |24|  / |    /    |     /     | /|
|   gotoInstruction   |   /  |     /    |   /  | /| /| /| /| /|   /  |  / |      /     |     /     |   /  | / | /| 25 |    /    |     /     | /|
| fakeLabelInstruction|   /  |     /    |   /  | /| /| /| /| /|   /  |  / |      /     |     /     |   /  | / | /|  / |    /    |     /     | /|
|fakeDefineInstruction|   /  |     /    |   /  | /| /| /| /| /|   /  |  / |      /     |     /     |   /  | / | /|  / |    /    |     /     | /|
|   _fakeInstruction  |   /  |     /    |   /  | /| /| /| /| /|   /  |  / |      /     |     /     |   /  | / | /|  / |    /    |     /     | /|
|   fakeInstruction   |   /  |     /    |   /  | /| /| /| /|26|   /  |  / |      /     |     /     |   /  | / | /|  / |    /    |     /     | /|
|  _clauseInstruction |   /  |     /    |   /  | /| /| /| /| /|  27  | 28 |     29     |     30    |   /  | / | /|  / |    /    |     /     | /|
|  clauseInstruction  |   /  |     /    |   /  | /| /|31| /| /|   /  |  / |      /     |     /     |   /  | / | /|  / |    /    |     /     | /|
|     instruction     |   /  |     /    |   /  | /| /| /| /|37|  32  | 33 |      /     |     /     |   /  | 34|35| 36 |    /    |     /     | /|

```
0: fakeLabelInstruction -> Label Identifier
1: fakeDefineInstruction -> Define Identifier , factor
2: _fakeInstruction -> fakeLabelInstruction
3: _fakeInstruction -> fakeDefineInstruction
```

|       M\[N,T\]      |Label|Define|
|---------------------|-----|------|
| fakeLabelInstruction|  0  |   /  |
|fakeDefineInstruction|  /  |   1  |
|   _fakeInstruction  |  2  |   3  |

**unOp&binOp表略**