#include(FetchContent)

add_executable(Injector)

# Add sources
target_sources(Injector PRIVATE
	"main.cpp"
 )
add_subdirectory(Injection)
add_subdirectory(Pages)
add_subdirectory(Process)
add_subdirectory(Versions)
#add_subdirectory(Discord)

# Add hot-reload support link options
target_link_options(Injector PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)
target_compile_options(Injector PRIVATE $<$<CONFIG:Debug>:/ZI>)

target_link_libraries(Injector PRIVATE Shared CxxUrl)

# discord rpc
#FetchContent_Declare(
#    discordrpc
#    GIT_REPOSITORY https://github.com/discord/discord-rpc.git
#    GIT_TAG master
#)
#FetchContent_GetProperties(discordrpc)
#if(NOT discordrpc_POPULATED)
#    FetchContent_Populate(discordrpc)
#    add_subdirectory(${discordrpc_SOURCE_DIR} ${discordrpc_BINARY_DIR})
#endif()

target_link_libraries(Injector PRIVATE)# discord-rpc)