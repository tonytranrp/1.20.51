include(FetchContent)

add_library(Shared STATIC)

FetchContent_Declare(
    cxxurl
    GIT_REPOSITORY https://github.com/chmike/CxxUrl.git
    GIT_TAG master
)
FetchContent_GetProperties(cxxurl)
if(NOT cxxurl_POPULATED)
    FetchContent_Populate(cxxurl)

    add_library(CxxUrl STATIC)
    target_sources(CxxUrl PRIVATE
        "${cxxurl_SOURCE_DIR}/url.cpp"
    )
    target_include_directories(CxxUrl INTERFACE ${cxxurl_SOURCE_DIR})
endif()

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG master
)
FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR})
endif()

FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG master
)
FetchContent_GetProperties(entt)
if(NOT entt_POPULATED)
    FetchContent_Populate(entt)
    add_subdirectory(${entt_SOURCE_DIR} ${entt_BINARY_DIR})
endif()

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)
FetchContent_GetProperties(stb)
if(NOT stb_POPULATED)
    FetchContent_Populate(stb)
    add_library(stb INTERFACE)
    target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})
endif()

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
)
FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
    add_library(glm INTERFACE)
    target_include_directories(glm INTERFACE ${glm_SOURCE_DIR})
endif()

FetchContent_Declare(
    sbs
    GIT_REPOSITORY https://github.com/DisabledMallis/ShouldBeStandard.git
    GIT_TAG main
)
FetchContent_GetProperties(sbs)
if(NOT sbs_POPULATED)
    FetchContent_Populate(sbs)
    add_subdirectory(${sbs_SOURCE_DIR} ${sbs_BINARY_DIR})
endif()

add_subdirectory(Auth)
add_subdirectory(Data)
add_subdirectory(Files)
add_subdirectory(Fonts)
add_subdirectory(Graphics)
add_subdirectory(ImGui)
add_subdirectory(Menu)
add_subdirectory(Registry)
add_subdirectory(Utils)
add_subdirectory(Web)

# Add hot-reload support link options
target_link_options(Shared PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)
target_compile_options(Shared PRIVATE $<$<CONFIG:Debug>:/ZI>)

target_include_directories(Shared PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(Shared PUBLIC d3d11 d3d12 d2d1 dxguid nlohmann_json EnTT stb glm SBS PRIVATE Dbghelp WinHTTP)