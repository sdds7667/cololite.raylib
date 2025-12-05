# Engine library
add_library(game_actors SHARED)

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS *.c *.cc)
file(GLOB HEADER_FILES CONFIGURE_DEPENDS headers/*.h headers/*.hh)
file(GLOB MAP_HEADER_FILES CONFIGURE_DEPENDS map/headers/*.h map/headers/*.hh)
file(GLOB MAP_SOURCE_FILES CONFIGURE_DEPENDS map/*.c map/*.cc)
target_sources(game_actors PRIVATE ${SOURCE_FILES} ${HEADER_FILES} ${MAP_HEADER_FILES} ${MAP_SOURCE_FILES})

# Export symbols for shared library
set_target_properties(game_actors PROPERTIES
        CXX_VISIBILITY_PRESET default
        VISIBILITY_INLINES_HIDDEN OFF
)

target_include_directories(game_actors PUBLIC headers)
target_include_directories(game_actors PUBLIC map/headers)
target_link_libraries(game_actors PUBLIC engine)
target_link_libraries(${PROJECT_NAME} PRIVATE game_actors)

