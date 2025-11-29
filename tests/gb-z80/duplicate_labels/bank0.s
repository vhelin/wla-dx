.include "memorylayout.s"

.SECTION "ROM Bank $000" BANK 0 SLOT "MySlot" org 0 FORCE
.dw MyLabel
MyLabel:
.ENDS
        
        .define HELLO 123 EXPORT
        
