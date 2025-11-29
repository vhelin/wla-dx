.include "memorylayout.s"

.SECTION "ROM Bank $000 II" BANK 0 SLOT "MySlot" org 0 OVERWRITE
.dw MyLabel
MyLabel:
.ENDS

        .define HELLO 123 EXPORT
