        .function NS_FUNC() 5
        .define NS_ALIAS(value) value + 2

        .if !definedfunction(NS_FUNC)
          .fail "definedfunction(NS_FUNC) failed inside a non-isolated namespaced include"
        .endif

        .if !definedfunction(NS_ALIAS)
          .fail "definedfunction(NS_ALIAS) failed inside a non-isolated namespaced include"
        .endif

        .db NS_FUNC(), NS_ALIAS(5)