include(FetchContent)
set(FETCHCONTENT_QUIET off)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

include(GoogleTest)

add_definitions(-DUSE_UTILS_ASSERT)

set(cpps
    edge
    chain
    graph
    paths
    vertex_merger_core
    vertex_merger
    vertex_split
    edge_creation
    edge_deletion
    component_creation_deletion
    vertices
    pulling_1
    pulling_2
    pulling_3
)

foreach(src ${cpps})

    set(${src} unittest_${src})
    add_executable(${src} ${src}.cpp)

    include_directories(${src} PUBLIC
                        ../include
                        ../external)

    #target_link_libraries(unittests PRIVATE gmock)
    target_link_libraries(${src} PRIVATE gtest_main)

    gtest_discover_tests(${src})

    set(bindir ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    add_custom_command(
        TARGET ${src}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${src}>  ${bindir}
    )

    unset(${src})

endforeach()
