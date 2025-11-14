# Utils library
add_library(utils SHARED)

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS *.c *.cc)
file(GLOB HEADER_FILES CONFIGURE_DEPENDS headers/*.h headers/*.hh)
target_sources(utils PRIVATE ${SOURCE_FILES} ${HEADER_FILES})

# Export symbols for shared library
set_target_properties(utils PROPERTIES
        CXX_VISIBILITY_PRESET default
        VISIBILITY_INLINES_HIDDEN OFF
)

target_include_directories(utils PUBLIC headers)