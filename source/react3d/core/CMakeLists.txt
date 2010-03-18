set(LIB_NAME react3d_core)

file(GLOB REACT3D_CORE_SOURCE_FILES
     ${SOURCE_PATH}/react3d/core/*.cpp
     ${SOURCE_PATH}/react3d/core/kernel/*.cpp
     ${SOURCE_PATH}/react3d/core/math/*.cpp
)

file(GLOB REACT3D_CORE_HEADER_FILES
     ${HEADER_PATH}/react3d/core/*.h*
     ${HEADER_PATH}/react3d/core/kernel/*.h*
     ${HEADER_PATH}/react3d/core/math/*.h*
)

# source files for static lib
add_library(${LIB_NAME} ${LIBMODE}
            ${REACT3D_CORE_SOURCE_FILES}
            ${REACT3D_CORE_HEADER_FILES}
)

INCLUDE_DIRECTORIES(${HEADER_PATH}
)

# libraries to link into the executable, both Release and Debug
TARGET_LINK_LIBRARIES(${LIB_NAME}
)

if (USE_DYNAMIC_LINK)
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES DEFINE_SYMBOL REACT3D_CORE_EXPORTS)
else (USE_DYNAMIC_LINK)
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES DEFINE_SYMBOL USE_STATIC_LINKING)
endif (USE_DYNAMIC_LINK)

# little hack to get the .dll into /bin and .lib into /lib
if (MSVC)
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES PREFIX "../../bin/")
  SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES IMPORT_PREFIX "../")
endif (MSVC)
