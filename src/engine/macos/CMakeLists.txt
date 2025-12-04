
if (APPLE)
    # Engine library
    add_library(macos SHARED)

    file(GLOB SOURCE_FILES CONFIGURE_DEPENDS *.c *.cc *.mm)
    file(GLOB HEADER_FILES CONFIGURE_DEPENDS headers/*.h headers/*.hh)
    target_sources(macos PRIVATE ${SOURCE_FILES} ${HEADER_FILES})

    # Export symbols for shared library
    set_target_properties(macos PROPERTIES
            CXX_VISIBILITY_PRESET default
            VISIBILITY_INLINES_HIDDEN OFF
    )

    target_include_directories(macos PUBLIC headers)
    target_link_libraries(macos PRIVATE raylib)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Cocoa")
endif ()
