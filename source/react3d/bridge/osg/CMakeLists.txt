set(LIB_NAME react3d_bridge_osg)

file(GLOB REACT3D_BRIDGE_OSG_SOURCE_FILES
     ${SOURCE_PATH}/react3d/bridge/osg/*.cpp
)

file(GLOB REACT3D_BRIDGE_OSG_HEADER_FILES
     ${HEADER_PATH}/react3d/bridge/osg/*.h*
)

# source files for static lib
add_library(${LIB_NAME} #SHARED
            ${REACT3D_BRIDGE_OSG_SOURCE_FILES}
            ${REACT3D_BRIDGE_OSG_HEADER_FILES}
)

INCLUDE_DIRECTORIES(${HEADER_PATH}
                    ${OSG_INCLUDE_DIR}
)

# little hack to get the .dll into /bin and .lib into /lib
IF (MSVC)
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES PREFIX "../../bin/")
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES IMPORT_PREFIX "../")
ENDIF (MSVC)

# libraries to link into the executable, both Release and Debug
TARGET_LINK_LIBRARIES(${LIB_NAME}
                      optimized ${OSG_LIBRARY}         debug ${OSG_LIBRARY_DEBUG}
)

SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES DEFINE_SYMBOL REACT3D_BRIDGE_OSG_EXPORTS)
