
if (APPLE)
# Engine library
add_library(macosspecific SHARED)

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS *.c *.cc *.mm)
file(GLOB HEADER_FILES CONFIGURE_DEPENDS headers/*.h headers/*.hh)
target_sources(macosspecific PRIVATE ${SOURCE_FILES} ${HEADER_FILES})

# Export symbols for shared library
set_target_properties(macosspecific PROPERTIES
    CXX_VISIBILITY_PRESET default
    VISIBILITY_INLINES_HIDDEN OFF
)

target_include_directories(macosspecific PUBLIC headers)
target_link_libraries(macosspecific PRIVATE raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Cocoa")
endif()
