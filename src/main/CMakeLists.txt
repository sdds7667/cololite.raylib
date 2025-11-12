file(GLOB SOURCE_FILES CONFIGURE_DEPENDS 
    *.c
    *.cc
)
file(GLOB HEADER_FILES CONFIGURE_DEPENDS *.h *.hh)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE map engine)
