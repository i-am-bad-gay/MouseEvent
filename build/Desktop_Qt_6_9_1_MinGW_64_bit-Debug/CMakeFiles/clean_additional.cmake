# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\mouseevent_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\mouseevent_autogen.dir\\ParseCache.txt"
  "mouseevent_autogen"
  )
endif()
