include_directories(
    ${ANDROID_INCLUDE}
    ${PUBLIC_INCLUDE}
    ${COMMON_INCLUDE}
    ${PLATFORM_INCLUDE}
    ${FRAMEWORK_INCLUDE}
    ${APPS_INCLUDE}
    )

aux_source_directory(. DSP_SRCS)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGUN_NEED_GL")
#set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -DAEC_MIC_NUM")

if(CMAKE_BUILD_TARGET STREQUAL ndk)
    string(REPLACE "-DANDROID" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

# get static library name
string(REPLACE "${DSPSOURCES}/src" "" MODULE_FOLDER  ${CMAKE_CURRENT_SOURCE_DIR} )
string(MAKE_C_IDENTIFIER ${MODULE_FOLDER} MODULE_FOLDER)
set(TARGET_NAME "dsp${MODULE_FOLDER}")

add_library(${TARGET_NAME} STATIC ${DSP_SRCS})
target_link_libraries(${TARGET_NAME})

install(TARGETS ${TARGET_NAME} ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}") 