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
Comment -> #(.|' '|'\t')*

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
fakeLabelInstruction -> Dot Label Identifier
fakeDefineInstruction -> Dot Define Identifier factor
fakeInstruction -> fakeLabelInstruction | fakeDefineInstruction
clauseInstruction -> LeftBracket (assignInstruction | callInstruction | binOpInstruction | unOpInstruction) RightBracket
instruction -> assignInstruction | callInstruction | nopInstruction | brInstruction | gotoInstruction | fakeInstruction
```

# FIRST
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
| fakeLabelInstruction  | Dot                                                                    | FOLLOW(instruction)                                       |
| fakeDefineInstruction | Dot                                                                    | FOLLOW(instruction)                                       |
| fakeInstruction       | Dot                                                                    | FOLLOW(instruction)                                       |
| clauseInstruction     | LeftBracket                                                            | Comma FIRST(instruction) RightBracket FOLLOW(instruction) |
| instruction           | Assign Call Nop Br Goto Dot                                            | FIRST(code) $                                             |