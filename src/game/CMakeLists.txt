add_library(game SHARED)

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS *.c *.cc *.mm)
file(GLOB HEADER_FILES CONFIGURE_DEPENDS headers/*.h headers/*.hh)
target_sources(game PRIVATE ${SOURCE_FILES} ${HEADER_FILES})

# Export symbols for shared library
set_target_properties(game PROPERTIES
        CXX_VISIBILITY_PRESET default
        VISIBILITY_INLINES_HIDDEN OFF
)

target_include_directories(game PUBLIC headers)
