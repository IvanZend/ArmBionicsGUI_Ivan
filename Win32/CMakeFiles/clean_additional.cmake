# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "ArmBionicsGUIWin_autogen"
  "CMakeFiles\\ArmBionicsGUIWin_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\ArmBionicsGUIWin_autogen.dir\\ParseCache.txt"
  "QCustomPlot-library\\lib\\CMakeFiles\\qcustomplot_autogen.dir\\AutogenUsed.txt"
  "QCustomPlot-library\\lib\\CMakeFiles\\qcustomplot_autogen.dir\\ParseCache.txt"
  "QCustomPlot-library\\lib\\qcustomplot_autogen"
  )
endif()
