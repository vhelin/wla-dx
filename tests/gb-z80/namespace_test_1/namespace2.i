
namespace1.MacroNamespace1

.section "SectionNamespace2" keep
labelNamespace2: nop
.ends

.section "fooSection" free
someLabel:
        namespace1.someMacro 123
.ends


.section "some section" free
label:
    namespace1.myMacro 1
    namespace1.myMacro 2
.ends

    namespace1.myMacro 1
    namespace1.myMacro 2
    namespace1.myMacro 3

    namespace1.myMacro2 2
