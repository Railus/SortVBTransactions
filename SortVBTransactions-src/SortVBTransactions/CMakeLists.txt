cmake_minimum_required(VERSION "3.14.0")

# Project name and setting language for cpp linker
project(SortVBTransactions LANGUAGES CXX)

set (CMAKE_PREFIX_PATH "C:\\_DevData\\Qt\\Qt5.12.8\\5.12.8\\msvc2017_64\\")

set(CMAKE_INCLUDE_CURRENT_DIR on)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)

# run Qt's MOC when needed
set(CMAKE_AUTOMOC ON)

set(CMAKE_CONFIGURATION_TYPES Debug;Release)

set(project_ui
	mainwindow.ui)

set(moc_headers
    main.h
	mainwindow.h)

set(project_headers
    Transaction.h
    TransactionGroups.h
    HelperFunctions.h
)

set(project_sources 
	main.cpp
    mainwindow.cpp
    Transaction.cpp
    TransactionGroups.cpp
	HelperFunctions.cpp)
	
qt5_wrap_ui(project_headers_wrapped ${project_ui})	
qt5_wrap_cpp(project_sources_moc ${moc_headers})

add_executable(${PROJECT_NAME} ${moc_headers} ${project_sources} 
    ${project_headers_wrapped} ${project_sources_moc} 
    ${project_headers})

target_link_libraries(${PROJECT_NAME}
	Qt5::Core
	Qt5::Widgets
    Qt5::Gui)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

#Copy Qt dlls into build folder debug/release
if(WIN32)
    if($<CONFIG:Debug>)
        get_target_property(WIDGETDLL Qt5::Widgets IMPORTED_LOCATION_DEBUG)
    else($<CONFIG:Debug>)
        get_target_property(WIDGETDLL Qt5::Widgets IMPORTED_LOCATION_RELEASE)
    endif($<CONFIG:Debug>)
    add_custom_command(
    TARGET SortVBTransactions POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt5::Core>
        $<TARGET_FILE_DIR:SortVBTransactions>)
		
    add_custom_command(
    TARGET SortVBTransactions POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt5::Widgets>
        $<TARGET_FILE_DIR:SortVBTransactions>)
		
    add_custom_command(
    TARGET SortVBTransactions POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt5::Gui>
        $<TARGET_FILE_DIR:SortVBTransactions>)
endif(WIN32)