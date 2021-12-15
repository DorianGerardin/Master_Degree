set(GLM_VERSION "0.9.9")
set(GLM_INCLUDE_DIRS "/home/dorian/Master/M1/s7/HAI719I_prog3D/TP5/External/glm")

if (NOT CMAKE_VERSION VERSION_LESS "3.0")
    include("${CMAKE_CURRENT_LIST_DIR}/glmTargets.cmake")
endif()
