include(FetchContent)

FetchContent_Declare(
    plh
    GIT_REPOSITORY https://github.com/stevemk14ebr/PolyHook_2_0.git
    GIT_TAG e8d33f54f9d11943f86fc326dae31dcc8ea7c389
)
FetchContent_GetProperties(plh)
if(NOT plh_POPULATED)
    FetchContent_Populate(plh)
    set(POLYHOOK_BUILD_STATIC_RUNTIME OFF)
    add_subdirectory(${plh_SOURCE_DIR} ${plh_BINARY_DIR})
endif()

FetchContent_Declare(
    gsl
    GIT_REPOSITORY https://github.com/microsoft/GSL.git
    GIT_TAG main
)
FetchContent_GetProperties(gsl)
if(NOT gsl_POPULATED)
    FetchContent_Populate(gsl)
    add_subdirectory(${gsl_SOURCE_DIR} ${gsl_BINARY_DIR})
endif()

FetchContent_Declare(
    kiero
    GIT_REPOSITORY https://github.com/DisabledMallis/kiero.git
    GIT_TAG master
)
FetchContent_GetProperties(kiero)
if(NOT kiero_POPULATED)
    FetchContent_Populate(kiero)
    set(KIERO_INCLUDE_D3D11 ON)
    set(KIERO_INCLUDE_D3D12 ON)
    set(KIERO_USE_POLYHOOK ON)
    add_subdirectory(${kiero_SOURCE_DIR} ${kiero_BINARY_DIR})
endif()

FetchContent_Declare(
    menum
    GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
    GIT_TAG master
)
FetchContent_GetProperties(menum)
if(NOT menum_POPULATED)
    FetchContent_Populate(menum)
    add_subdirectory(${menum_SOURCE_DIR} ${menum_BINARY_DIR})
endif()

FetchContent_Declare(
    libhat
    GIT_REPOSITORY https://github.com/BasedInc/libhat.git
    GIT_TAG 224fd84f37bb5ebcfb65d45db9af438ee68bac59
)
FetchContent_GetProperties(libhat)
if(NOT libhat_POPULATED)
    FetchContent_Populate(libhat)
    message("${libhat_SOURCE_DIR}")
    message("${libhat_BINARY_DIR}")
    add_subdirectory(${libhat_SOURCE_DIR} ${libhat_BINARY_DIR})
endif()

#FetchContent_Declare(
#    libtbb
#    GIT_REPOSITORY https://github.com/oneapi-src/oneTBB.git
#    GIT_TAG onetbb_2021
#)
#FetchContent_GetProperties(libtbb)
#if(NOT libtbb_POPULATED)
#    FetchContent_Populate(libtbb)
#    set(BUILD_SHARED_LIBS OFF)
#    set(TBB_TEST OFF CACHE BOOL "" FORCE)
#    add_subdirectory(${libtbb_SOURCE_DIR} ${libtbb_BINARY_DIR})
#endif()

FetchContent_Declare(
        dxtk
        GIT_REPOSITORY https://github.com/microsoft/DirectXTK.git
        GIT_TAG main
)
FetchContent_GetProperties(dxtk)
if(NOT dxtk_POPULATED)
    FetchContent_Populate(dxtk)
    add_subdirectory(${dxtk_SOURCE_DIR} ${dxtk_BINARY_DIR})
endif()

# Build HLSL shaders
add_custom_target(NuvolaShaders ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/NuvolaShaders)


set(SHADER_DIR ${CMAKE_CURRENT_LIST_DIR}/Client/Rendering/Effects)
set(HLSL_FILES_VS ${SHADER_DIR}/BlockInstanceVS.hlsl)
set(HLSL_FILES_PS
    ${SHADER_DIR}/DefaultPS.hlsl
    #${SHADER_DIR}/OutlinePS.hlsl
)

foreach(FILE ${HLSL_FILES_VS})
    get_filename_component(FILE_WE ${FILE} NAME_WE)
    add_custom_command(
            TARGET NuvolaShaders
            COMMAND fxc.exe /nologo /Evertex_main /T vs_5_0 $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fh ${CMAKE_BINARY_DIR}/NuvolaShaders/${FILE_WE}.inc /Fd ${CMAKE_BINARY_DIR}/${FILE_WE}.pdb ${FILE}
            MAIN_DEPENDENCY ${FILE}
            COMMENT "HLSL ${FILE}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM)
endforeach(FILE)

foreach(FILE ${HLSL_FILES_PS})
    get_filename_component(FILE_WE ${FILE} NAME_WE)
    add_custom_command(
            TARGET NuvolaShaders
            COMMAND fxc.exe /nologo /Epixel_main /T ps_5_0 $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fh ${CMAKE_BINARY_DIR}/NuvolaShaders/${FILE_WE}.inc /Fd ${CMAKE_BINARY_DIR}/${FILE_WE}.pdb ${FILE}
            MAIN_DEPENDENCY ${FILE}
            COMMENT "HLSL ${FILE}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM)
endforeach(FILE)

# Add the built shaders to the project's include directories
include_directories(${CMAKE_BINARY_DIR}/NuvolaShaders)

# Add Nuvola target AFTER shaders
add_library(Nuvola SHARED)
add_dependencies(Nuvola NuvolaShaders)

# Add PCH
target_precompile_headers(Nuvola PUBLIC
    #EnTT headers
    <entt/core/type_info.hpp>

    #AsmJit headers
    <asmjit/asmjit.h>

    #Zydis headers
    <Zydis/Zydis.h>
    <Zycore/Status.h>

    #PolyHook2 headers
    <polyhook2/Detour/x64Detour.hpp>

    #Standard Library headers
    <algorithm>
    <any>
    <cassert>
    <functional>
    <list>
    <memory>
    <ranges>
    <set>
    <string>
    <unordered_map>
    <vector>
)

# Add hot-reload support link options
target_link_options(Nuvola PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)
target_compile_options(Nuvola PRIVATE $<$<CONFIG:Debug>:/ZI>)

target_sources(Nuvola PRIVATE 
	"main.cpp"
)

target_link_libraries(Nuvola PRIVATE Shared Kiero GSL PolyHook_2 version magic_enum DirectXTK libhat)

#string(REPLACE "/O2" "/Od" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
#string(REPLACE "/O2" "/Od" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")

target_precompile_headers(Nuvola PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:<${CMAKE_CURRENT_LIST_DIR}/pch.cpp$<ANGLE-R>>")

add_subdirectory(Client)
add_subdirectory(Mem)
add_subdirectory(Utils)

message("${CMAKE_CURRENT_LIST_DIR}")
