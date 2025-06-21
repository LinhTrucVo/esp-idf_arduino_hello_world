# Recursively find all .c and .cpp files
file(GLOB_RECURSE LIB_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp"
)

# Recursively find all directories containing .h files
file(GLOB_RECURSE HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.h"
)

# Extract unique directories containing header files
set(INCLUDE_DIRS "")
foreach(_header ${HEADER_FILES})
    get_filename_component(_dir ${_header} DIRECTORY)
    list(APPEND INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)

# Register the component
idf_component_register(
    SRCS ${LIB_SRCS}
    INCLUDE_DIRS ${INCLUDE_DIRS}
    REQUIRES arduino
)