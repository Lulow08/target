cmake_minimum_required(VERSION 3.20)
project(target VERSION 0.1 LANGUAGES CXX)

# --- C++ Standard Configuration ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Standard CMake Modules ---
include(GNUInstallDirs)
include(FetchContent)

# --- Find System Dependencies ---
find_package(Qt6 6.10 REQUIRED COMPONENTS Quick Concurrent)
find_package(LayerShellQt REQUIRED)
find_package(Lua REQUIRED)

# --- External Libraries via FetchContent ---

# Sol2
FetchContent_Declare(
    sol2
    GIT_REPOSITORY https://github.com/ThePhD/sol2.git
    GIT_TAG develop
)
FetchContent_MakeAvailable(sol2)

# nlohmann_json
find_package(nlohmann_json 3.11 QUIET)
if(NOT nlohmann_json_FOUND)
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()

# --- Qt Project Setup ---
qt_standard_project_setup(REQUIRES 6.10)
set_source_files_properties(ui/qml/theme/Theme.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

# --- Define Executable ---
qt_add_executable(${PROJECT_NAME}
    src/main.cpp

    # --- Setup ---
    src/setup/SystemSetup.cpp
    src/setup/StyleSetup.cpp

    # Internals
    src/setup/internals/LayerShellConfigurator.cpp
)

# --- Qt Quick / QML Module Setup ---
qt_add_qml_module(${PROJECT_NAME}
    URI target
    VERSION 1.0
    QML_FILES
        ui/qml/Main.qml

        # --- Theme ---
        ui/qml/theme/Theme.qml

    RESOURCES
        # --- Fonts ---
        ui/assets/fonts/BDOGrotesk-Regular.ttf

        # --- Icons ---
        ui/assets/icons/ic-target.svg
        ui/assets/icons/ic-app-icon-fallback.svg

    SOURCES
        # --- Setup ---
        src/setup/SystemSetup.hpp
        src/setup/StyleSetup.hpp

        # Internals
        src/setup/internals/LayerShellConfigurator.hpp

        # --- Adapter ---
        src/adapter/ActionDispatcher.hpp
        src/adapter/QueryController.hpp
        src/adapter/Renderer.hpp
        src/adapter/RenderNode.hpp

        # --- Core ---
        # Execution
        src/core/execution/ExecutionEngine.hpp
        src/core/execution/ActionRegistry.hpp
        src/core/execution/CommandResolver.hpp
        src/core/execution/CommandInstance.hpp
        src/core/execution/Action.hpp

        # Extensibility
        src/core/extensibility/ExtensionRegistry.hpp
        src/core/extensibility/ExtensionDiscovery.hpp
        src/core/extensibility/ManifestLoader.hpp
        src/core/extensibility/ExtensionDescriptor.hpp
        src/core/extensibility/CommandDescriptor.hpp
        src/core/extensibility/ExtensionData.hpp
        src/core/extensibility/CommandData.hpp

        # Lua
        src/core/lua/LuaRunner.hpp
        src/core/lua/LuaContextFactory.hpp

        # VUI
        src/core/vui/VUIComposer.hpp
        src/core/vui/VUITree.hpp
        src/core/vui/VUINode.hpp

        # Navigation
        src/core/navigation/NavigationStack.hpp

        # Loaders
        src/core/loaders/CommandLoader.hpp

        # Types
        src/core/types/CommandID.hpp
        src/core/types/ActionID.hpp

        # --- Services ---
        src/services/CommandService.hpp
        src/services/IService.hpp
        SOURCES src/core/execution/ActionContext.hpp
        SOURCES src/core/types/Result.hpp
        SOURCES src/core/vui/VUIComposer.cpp
        SOURCES src/core/navigation/NavigationStack.cpp
        SOURCES src/core/loaders/CommandLoader.cpp
        SOURCES src/core/lua/LuaContextFactory.cpp
        SOURCES src/core/lua/LuaRunner.cpp
        SOURCES src/core/lua/internal/CompiledChunk.hpp
        SOURCES src/services/CommandService.cpp
        SOURCES src/core/lua/internal/LuaProxies.hpp
        SOURCES src/core/lua/internal/LuaVUIMapper.hpp src/core/lua/internal/LuaVUIMapper.cpp
        SOURCES src/core/execution/ActionRegistry.cpp
        SOURCES src/core/execution/CommandResolver.cpp
        SOURCES src/core/execution/ExecutionEngine.cpp
        SOURCES src/core/execution/CommandResult.hpp
        SOURCES src/adapter/Renderer.cpp
        SOURCES src/adapter/QueryController.cpp
        SOURCES src/adapter/ActionDispatcher.cpp
        SOURCES src/AppLoader.hpp src/AppLoader.cpp
        SOURCES src/core/extensibility/ExtensionRegistry.cpp
        SOURCES src/core/extensibility/ExtensionDiscovery.cpp
        SOURCES src/core/extensibility/ManifestLoader.cpp
        QML_FILES ui/qml/views/RootView.qml
        QML_FILES ui/qml/components/SearchBar.qml
        QML_FILES ui/qml/components/List.qml
        QML_FILES ui/qml/components/ListItem.qml
        QML_FILES ui/qml/components/Section.qml
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${LUA_INCLUDE_DIRS}
)

# inject source dir so a path resolver can find modules/ in dev builds
#target_compile_definitions(${PROJECT_NAME} PRIVATE
#    TARGET_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
#)

# --- Link Libraries ---
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Quick
        Qt6::Concurrent
        LayerShellQt::Interface
        sol2::sol2
        nlohmann_json::nlohmann_json
        ${LUA_LIBRARIES}
)

# --- Build Output Properties ---
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# --- Installation Rules ---

# Install the binary
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install built-in modules/plugins (from source 'modules' to 'share/target/modules')
install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/modules/
    DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/modules
)