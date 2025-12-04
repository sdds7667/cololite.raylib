# game_actors library
add_library(game_actors SHARED)

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS *.c *.cc)
file(GLOB HEADER_FILES CONFIGURE_DEPENDS headers/*.h headers/*.hh)
target_sources(game_actors PRIVATE ${SOURCE_FILES} ${HEADER_FILES})

# Export symbols for shared library
set_target_properties(game_actors PROPERTIES
        CXX_VISIBILITY_PRESET default
        VISIBILITY_INLINES_HIDDEN OFF
)

target_include_directories(game_actors PUBLIC headers)
target_include_directories(game_actors PRIVATE private_headers)
target_link_libraries(game_actors PUBLIC game engine)