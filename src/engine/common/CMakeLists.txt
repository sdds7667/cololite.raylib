# Engine library
add_library(engine SHARED)

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS *.c *.cc)
file(GLOB HEADER_FILES CONFIGURE_DEPENDS headers/*.h headers/*.hh)
target_sources(engine PRIVATE ${SOURCE_FILES} ${HEADER_FILES})

# Export symbols for shared library
set_target_properties(engine PROPERTIES
        CXX_VISIBILITY_PRESET default
        VISIBILITY_INLINES_HIDDEN OFF
)

target_include_directories(engine PUBLIC headers)
target_link_libraries(engine PUBLIC game raylib utils)

if (APPLE)
    target_link_libraries(engine PUBLIC macosspecific)
endif ()

## Map Tests
add_subdirectory(tests)
