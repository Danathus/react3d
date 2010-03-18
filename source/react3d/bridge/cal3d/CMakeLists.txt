set(LIB_NAME react3d_bridge_cal3d)

file(GLOB REACT3D_BRIDGE_CAL3D_SOURCE_FILES
     ${SOURCE_PATH}/react3d/bridge/cal3d/*.cpp
)

file(GLOB REACT3D_BRIDGE_CAL3D_HEADER_FILES
     ${HEADER_PATH}/react3d/bridge/cal3d/*.h*
)

# source files for static lib
add_library(${LIB_NAME} ${LIBMODE}
            ${REACT3D_BRIDGE_CAL3D_SOURCE_FILES}
            ${REACT3D_BRIDGE_CAL3D_HEADER_FILES}
)

INCLUDE_DIRECTORIES(${HEADER_PATH}
                    ${CAL3D_INCLUDE_DIR}
)

# little hack to get the .dll into /bin and .lib into /lib
IF (MSVC)
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES PREFIX "../../bin/")
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES IMPORT_PREFIX "../")
ENDIF (MSVC)

#libraries to link into the executable, both Release and Debug
TARGET_LINK_LIBRARIES(${LIB_NAME}
                      react3d_core
                      optimized ${CAL3D_LIBRARY}       debug ${CAL3D_LIBRARY_DEBUG}
)

if (USE_DYNAMIC_LINK)
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES DEFINE_SYMBOL REACT3D_BRIDGE_CAL3D_EXPORTS)
else (USE_DYNAMIC_LINK)
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES DEFINE_SYMBOL USE_STATIC_LINKING)
endif (USE_DYNAMIC_LINK)
