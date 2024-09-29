# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/tema8_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/tema8_autogen.dir/ParseCache.txt"
  "tema8_autogen"
  )
endif()
