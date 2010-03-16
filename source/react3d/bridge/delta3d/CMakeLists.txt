set(LIB_NAME react3d_bridge_delta3d)

file(GLOB REACT3D_BRIDGE_DELTA3D_SOURCE_FILES
     ${SOURCE_PATH}/react3d/bridge/delta3d/*.cpp
)

file(GLOB REACT3D_BRIDGE_DELTA3D_HEADER_FILES
     ${HEADER_PATH}/react3d/bridge/delta3d/*.h*
)

# source files for static lib
add_library(${LIB_NAME} #SHARED
            ${REACT3D_BRIDGE_DELTA3D_SOURCE_FILES}
            ${REACT3D_BRIDGE_DELTA3D_HEADER_FILES}
)

INCLUDE_DIRECTORIES(${HEADER_PATH}
                    ${DELTA3D_INCLUDE_DIR}
)

# little hack to get the .dll into /bin and .lib into /lib
IF (MSVC)
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES PREFIX "../../bin/")
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES IMPORT_PREFIX "../")
ENDIF (MSVC)

#libraries to link into the executable, both Release and Debug
TARGET_LINK_LIBRARIES(${LIB_NAME}
                      optimized ${DTCORE_LIBRARY}      debug ${DTCORE_DEBUG_LIBRARY}
                      optimized ${DTUTIL_LIBRARY}      debug ${DTUTIL_DEBUG_LIBRARY}
                      optimized ${DTANIM_LIBRARY}      debug ${DTANIM_DEBUG_LIBRARY}
                      optimized ${DTGAME_LIBRARY}      debug ${DTGAME_DEBUG_LIBRARY}
)

SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES DEFINE_SYMBOL REACT3D_BRIDGE_DELTA3D_EXPORTS)
