
.macro NAMESPACEMACRO
	.db \1
.endm

.macro INCREMENTMACRO
.redefine _out \1+1
.endm

.section "!NamespaceTest1" namespace "hello"
NamespaceMain:
	nop
	call NamespaceMain
	call NamespaceMain+1
	.ends

.section "!NamespaceTest2" free
NamespaceBonus:
	nop
	call NamespaceBonus
	call NamespaceBonus+1

do:
@loop:	call @loop

	.db "NS>"
	NAMESPACEMACRO $aa
	.dbm INCREMENTMACRO 2
	.incbin "binaryfile" FILTER INCREMENTMACRO
	.db "<NS"
.ends
