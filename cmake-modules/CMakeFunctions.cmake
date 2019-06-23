############################################################
# Returns the definitions and includes from a cflags output
# 
# _list cflags var
# _includes return var with the Includes
# _definitions return var with the Definitions
function(PARSE_CFLAGS _list _includes _definitions)

   separate_arguments(${_list})
   foreach(_item ${${_list}})   
      if("${_item}" MATCHES "^-I")
         string(SUBSTRING ${_item} 2 -1 _item_t)
         set(_includes_t ${_includes_t} ${_item_t})
      else("${_item}" MATCHES "^-I")
         set(_definitions_t ${_definitions_t} ${_item})
      endif("${_item}" MATCHES "^-I")
   endforeach(_item)
   
   string (REPLACE ";" " " _definitions_k "${_definitions_t}")
   
   set(${_includes} ${_includes_t} PARENT_SCOPE )
   set(${_definitions} ${_definitions_k} PARENT_SCOPE)

   #message(STATUS "Calling Macro ${${_list}} and returning ${_includes_k} and ${_definitions_k}")
endfunction(PARSE_CFLAGS)
###############################################################################


###############################################################################
# Retrieve information from a ldflags output
# 
# _list ldflags var
# _libraries return var with the libraries
# _ldflags return var with the ldflags value
# _libdir return var with the libdir value
function(PARSE_LDFLAGS _list _libraries _ldflags _libdir)

   separate_arguments(${_list})
   foreach(_item ${${_list}})  
      if("${_item}" MATCHES "^-l")
         string(SUBSTRING ${_item} 2 -1 _item_t)
         set(_libraries_t ${_libraries_t} ${_item_t})
      elseif("${_item}" MATCHES "^-L")
         set(_libdir_t ${_libdir_t} ${_item})
      else("${_item}" MATCHES "^-L") 
         set(_ldflags_t ${_ldflags_t} ${_item})
      endif("${_item}" MATCHES "^-l")
   endforeach(_item ${${_list}})

   string (REPLACE ";" " " _ldflags_k "${_ldflags_t}")
   string (REPLACE ";" " " _libdir_k "${_libdir_t}")  

   set(${_libraries} ${_libraries_t} PARENT_SCOPE )
   set(${_ldflags} ${_ldflags_k} PARENT_SCOPE)
   set(${_libdir} ${_libdir_k} PARENT_SCOPE)

   #message(STATUS "Calling Macro ${${_list}} and returning ${_libraries_k} ${_ldflags_k} ${_libdir_k}")
endfunction(PARSE_LDFLAGS)
