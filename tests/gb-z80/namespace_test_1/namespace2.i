
namespace1.MacroNamespace1

.section "SectionNamespace2" keep
labelNamespace2: nop
.ends

.section "fooSection" free
someLabel:
        namespace1.someMacro 123
.ends
