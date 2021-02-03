function(prepend VAR PREFIX)
    set(listVar "")
    foreach(f ${${VAR}})
        list(APPEND listVar "${PREFIX}${f}")
    endforeach(f)
    set(${VAR} "${listVar}" PARENT_SCOPE)
endfunction()