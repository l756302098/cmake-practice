CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT(RtspClient)

SET(CMAKE_DEBUG_POSTFIX "d")
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR};${CMAKE_MODULE_PATH}")

MACRO(NEW_EXECUTABLE EXECUTABLE_NAME)

    ADD_EXECUTABLE(${EXECUTABLE_NAME} ${EXECUTABLE_FILES})

    SET_TARGET_PROPERTIES(${EXECUTABLE_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
    SET_TARGET_PROPERTIES(${EXECUTABLE_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/lib"
                                                        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/lib"
                                                        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/lib"
                                                        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/lib"
                                                        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/bin"
                                                        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/bin")

    INSTALL(TARGETS ${EXECUTABLE_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
                                       LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
                                       ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

    TARGET_INCLUDE_DIRECTORIES(${EXECUTABLE_NAME} PUBLIC ${EXTERNAL_INCLUDES})
    TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME} ${EXTERNAL_LIBRARIES})

ENDMACRO(NEW_EXECUTABLE)

MACRO(NEW_LIBRARY LIBRARY_NAME LIBRARY_TYPE)

    ADD_LIBRARY(${LIBRARY_NAME} ${LIBRARY_TYPE} ${LIBRARY_FILES})

    SET_TARGET_PROPERTIES(${LIBRARY_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
    SET_TARGET_PROPERTIES(${LIBRARY_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/lib"
                                                     ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/lib"
                                                     LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/lib"
                                                     LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/lib"
                                                     RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/bin"
                                                     RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/bin")

    INSTALL(TARGETS ${LIBRARY_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
                                    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
                                    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

    TARGET_INCLUDE_DIRECTORIES(${LIBRARY_NAME} PUBLIC ${EXTERNAL_INCLUDES})
    TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${EXTERNAL_LIBRARIES})

ENDMACRO(NEW_LIBRARY)

MACRO(USE_STATIC_RUNTIME)

    IF(MSVC)
        FOREACH(FLAG_VAR
                CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
                CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
            IF(${FLAG_VAR} MATCHES "/MD")
                STRING(REGEX REPLACE "/MD" "/MT" ${FLAG_VAR} "${${FLAG_VAR}}")
            ENDIF(${FLAG_VAR} MATCHES "/MD")
        ENDFOREACH(FLAG_VAR)
    ENDIF(MSVC)

ENDMACRO(USE_STATIC_RUNTIME)

IF(NOT WIN32)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -std=c++11")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-register")
ENDIF(NOT WIN32)

IF(MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /bigobj")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
ENDIF(MSVC)

###
SET(EXE_NAME rtsp_client)
SET(EXECUTABLE_FILES
    rtsp_client.cpp
)

ADD_DEFINITIONS(-DASIO_STANDALONE)
IF(WIN32)
    ADD_DEFINITIONS(-D_WEBSOCKETPP_CPP11_INTERNAL_)
ELSE(WIN32)
    ADD_DEFINITIONS(-DHAS_SOCKLEN_T)
ENDIF(WIN32)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)
NEW_EXECUTABLE(${EXE_NAME} SHARED)

IF(WIN32)
	TARGET_LINK_OPTIONS(${EXE_NAME} PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/FORCE:MULTIPLE>)
    TARGET_LINK_LIBRARIES(${EXE_NAME} winmm)
ELSE(WIN32)
	TARGET_LINK_LIBRARIES(${EXE_NAME} pthread dw)
ENDIF(WIN32)
TARGET_COMPILE_OPTIONS(${EXE_NAME} PUBLIC -D_SCL_SECURE_NO_WARNINGS)

