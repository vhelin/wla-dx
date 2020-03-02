
.DEFINE TESB 1
.EXPORT TESB

.SECTION "test"
test_00:	NOP
.ENDS

// .PRINTT "what's the name of this program? "
// .INPUT INFO
.DEFINE INFO = "HelloMoto"	

.SECTION "information"
.DB INFO
.ENDS
