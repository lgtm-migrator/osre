find_package(Python COMPONENTS Interpreter Development)

INCLUDE_DIRECTORIES(
    ./
    ./src
    ${Python_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}
)

link_directories(
    ${Python_LIBRARY_DIRS}
)

SET( inspector_module_src 
    src/Modules/InspectorModule/InspectorModule.cpp
    src/Modules/InspectorModule/InspectorModule.h
)

SET( log_module_src
    src/Modules/LogModule/LogModule.h
    src/Modules/LogModule/LogModule.cpp
)

SET( editor_module_src
    src/Modules/EditorModule/EditorModule.h
    src/Modules/EditorModule/EditorModule.cpp
)

SET( modules_src 
    src/Modules/ModuleRegistry.h
    src/Modules/ModuleRegistry.cpp
)

SET( gui_src
    src/Gui/UIElementsWin32.h
    src/Gui/UIElementsWin32.cpp
    src/Gui/UIElements.cpp
    src/Gui/UIElements.h
)

SET( scripting_src 
    src/Scripting/PythonInterface.cpp
    src/Scripting/PythonInterface.h
)

set(renderview_src
    src/RenderView/MainRenderView.cpp
    src/RenderView/MainRenderView.h    
)

set(actions_src
    src/Actions/ActionBase.h
    src/Actions/ImportAction.cpp
    src/Actions/ImportAction.h
)

SET (main_src
    src/SceneData.h
    src/ProgressReporter.cpp
    src/ProgressReporter.h
    src/OsreEdApp.cpp
    src/OsreEdApp.h
    src/main.cpp

)
SOURCE_GROUP(Gui                      FILES ${gui_src})
SOURCE_GROUP(RenderView               FILES ${renderview_src})
SOURCE_GROUP(Actions                  FILES ${actions_src})
SOURCE_GROUP(Modules\\InspectorModule FILES ${inspector_module_src} )
SOURCE_GROUP(Modules\\LogModule       FILES ${log_module_src} )
SOURCE_GROUP(Modules\\EditorModule    FILES ${editor_module_src} )

SOURCE_GROUP(Modules\\                FILES ${modules_src} )
SOURCE_GROUP(Scripting\\              FILES ${scripting_src} )
SOURCE_GROUP(Main\\                   FILES ${main_src})

ADD_EXECUTABLE( osre_ed
    OSRE_Ed.rc    
    resource.h
    ${gui_src}
    ${actions_src}
    ${inspector_module_src}
    ${log_module_src}
    ${editor_module_src}
    ${modules_src}
    ${scripting_src}
    ${renderview_src}
    ${main_src}
)

target_link_libraries ( osre_ed osre comctl32.lib winmm.lib )
