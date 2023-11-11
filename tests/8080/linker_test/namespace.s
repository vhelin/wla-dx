
        ; @BT linked.rom
        
.macro NAMESPACEMACRO
	.db \1
.endm

.macro INCREMENTMACRO
.redefine _out \1+1
.endm

.section "NamespaceTest1" keep
NamespaceMain:
	nop
	call NamespaceMain
	call NamespaceMain+1
	.ends

.section "NamespaceTest2" free keep
NamespaceBonus:
	nop
	call NamespaceBonus
	call NamespaceBonus+1

do:
@loop:	call @loop

	.db "NS>"               ; @BT TEST-NS NS START
	NAMESPACEMACRO $aa      ; @BT AA
	.dbm INCREMENTMACRO 2   ; @BT 03
	.incbin "binaryfile" FILTER INCREMENTMACRO ; @BT 49 46 4D 4D 50
	.db "<NS"                                  ; @BT END
.ends

.section "Main" FREE KEEP
MainLoop:
	.db "N1>"               ; @BT TEST-N1 N1 START
	.db 1, 0                ; @BT 01 00
	.db "<N1"               ; @BT END
.ends

.section "Bonus" APPENDTO "Main"
	.db "Bonus", 0
.ends

.section "MoreNops" APPENDTO "*:DUMMY3"
	nop
	nop
	nop
.ends
