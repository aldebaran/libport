# Custom extension to the cmake 'string' built-in command.

if(NOT STRING_CMAKE_GUARD)
  set(STRING_CMAKE_GUARD TRUE)

  # Put in _index_ the index of the regexp in _regexp_list_ that matches _str_.
  function(string_list_match regexp_list str index)

    list(LENGTH regexp_list regexp_count)

    if(NOT regexp_count EQUAL 0)

      math(EXPR regexp_count "${regexp_count} - 1")
      foreach(i RANGE 0 ${regexp_count})

	list(GET regexp_list ${i} rx)

	if(str MATCHES ${rx})
	  set(index ${i} PARENT_SCOPE)
	  return()
	endif(str MATCHES ${rx})

      endforeach(i)

    endif(NOT regexp_count EQUAL 0)

    set(index -1 PARENT_SCOPE)

  endfunction(string_list_match)

endif(NOT STRING_CMAKE_GUARD)

