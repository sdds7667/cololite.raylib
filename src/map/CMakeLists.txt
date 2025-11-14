# Map library
add_library(map SHARED)

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS *.c *.cc)
file(GLOB HEADER_FILES CONFIGURE_DEPENDS headers/*.h headers/*.hh)
target_sources(map PRIVATE ${SOURCE_FILES} ${HEADER_FILES})

# Export symbols for shared library
set_target_properties(map PROPERTIES
        CXX_VISIBILITY_PRESET default
        VISIBILITY_INLINES_HIDDEN OFF
)

target_include_directories(map PUBLIC headers)
target_link_libraries(map PRIVATE utils)


## Map Tests
add_subdirectory(tests)
