FIND_PROGRAM(GIT_EXECUTABLE NAMES git git.exe DOC "git command line client")

get_filename_component(SOURCE_DIR ${SRC} PATH)

EXECUTE_PROCESS(
     COMMAND ${GIT_EXECUTABLE} svn info
     COMMAND grep "Revision"
     WORKING_DIRECTORY "${SOURCE_DIR}"
     OUTPUT_VARIABLE VERSION
     OUTPUT_STRIP_TRAILING_WHITESPACE
)

if (NOT DEFINED ${NAME})
   set( ${NAME} "unknown" )
endif()

if (NOT VERSION)
   FIND_PACKAGE(Subversion)

   if (Subversion_FOUND)

   EXECUTE_PROCESS(
      COMMAND svnversion
      WORKING_DIRECTORY "${SOURCE_DIR}"
      OUTPUT_VARIABLE VERSION
      OUTPUT_STRIP_TRAILING_WHITESPACE
   )
   endif(Subversion_FOUND)

   if (VERSION AND (NOT ${VERSION} MATCHES "^exported"))
      if (STRIP_M) 
         string(REGEX REPLACE "M$" "" VERSION ${VERSION})
      endif()
      set( ${NAME} ${VERSION} )
   endif()
else()
   string(REGEX REPLACE "Revision: " "" ${NAME} ${VERSION})
endif(NOT VERSION)

set(NAVIT_VARIANT "-")

message (STATUS "SVN-version ${${NAME}}")
CONFIGURE_FILE(${SRC} ${DST} @ONLY)
