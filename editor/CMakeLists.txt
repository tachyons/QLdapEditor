CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
PROJECT(LdapEditorApp)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)

FIND_PACKAGE(Qt6Widgets CONFIG REQUIRED)
FIND_PACKAGE(Qt6Core5Compat CONFIG REQUIRED)

ADD_DEFINITIONS(${QT_DEFINITIONS})

include_directories(../common)
include_directories(../corelib)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})

SET(ldapeditor_SOURCES
    utilities.cpp
    changepassworddialog.cpp
    changepassworddialog.ui
    addusertogroupdialog.cpp
    addusertogroupdialog.ui
    resources.qrc
    ldapnewattributedialog.cpp
    ldapnewattributedialog.ui
    ldapdataeditdelegate.cpp
    attributemodelhelper.cpp
    ldapsettings.cpp
    simplecrypt.cpp
    ldaptreemodel.cpp
    ldapattributesmodel.cpp
    ldapsearchdialog.cpp
    ldapsearchdialog.ui
    connectiondialog.cpp
    connectiondialog.ui
    ldaptableview.cpp
    ldaptreeview.cpp
    ldapnewentrydialog.cpp
    ldapnewentrydialog.ui
    mainwindow.cpp
    main.cpp
    ../common/common.cpp
)

SET(ldapeditor_HEADERS
    utilities.h
    changepassworddialog.h
    addusertogroupdialog.h
    ldapnewattributedialog.h
    ldapdataeditdelegate.h
    attributemodelhelper.h
    ldapsettings.h
    simplecrypt.h
    ldaptreemodel.h
    ldapattributesmodel.h
    ldapsearchdialog.h
    connectiondialog.h
    ldaptableview.h
    ldaptreeview.h
    ldapnewentrydialog.h
    mainwindow.h
    ldapeditordefines.h
    ../common/common.h
)

# ── macOS: generate ICNS icon and configure bundle ───────────────────────────

if(APPLE)
    set(ICON_PNG "${CMAKE_CURRENT_SOURCE_DIR}/images/LdapEditorApp.png")
    set(ICONSET_DIR "${CMAKE_CURRENT_BINARY_DIR}/LdapEditorApp.iconset")
    set(ICON_ICNS "${CMAKE_CURRENT_BINARY_DIR}/LdapEditorApp.icns")

    add_custom_command(
        OUTPUT "${ICON_ICNS}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${ICONSET_DIR}"
        COMMAND sips -z 16  16  "${ICON_PNG}" --out "${ICONSET_DIR}/icon_16x16.png"
        COMMAND sips -z 32  32  "${ICON_PNG}" --out "${ICONSET_DIR}/icon_16x16@2x.png"
        COMMAND sips -z 32  32  "${ICON_PNG}" --out "${ICONSET_DIR}/icon_32x32.png"
        COMMAND sips -z 64  64  "${ICON_PNG}" --out "${ICONSET_DIR}/icon_32x32@2x.png"
        COMMAND sips -z 128 128 "${ICON_PNG}" --out "${ICONSET_DIR}/icon_128x128.png"
        COMMAND sips -z 256 256 "${ICON_PNG}" --out "${ICONSET_DIR}/icon_128x128@2x.png"
        COMMAND sips -z 256 256 "${ICON_PNG}" --out "${ICONSET_DIR}/icon_256x256.png"
        COMMAND sips -z 512 512 "${ICON_PNG}" --out "${ICONSET_DIR}/icon_256x256@2x.png"
        COMMAND sips -z 512 512 "${ICON_PNG}" --out "${ICONSET_DIR}/icon_512x512.png"
        COMMAND iconutil -c icns "${ICONSET_DIR}" -o "${ICON_ICNS}"
        DEPENDS "${ICON_PNG}"
        COMMENT "Generating macOS application icon (ICNS)"
    )
    set_source_files_properties("${ICON_ICNS}" PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
        GENERATED TRUE
    )
    list(APPEND ldapeditor_SOURCES "${ICON_ICNS}")

# ── Linux: include XDG desktop files as project sources (IDE visibility) ─────

else()
    list(APPEND ldapeditor_SOURCES
        install/LdapEditorApp.desktop
        install/LdapEditorApp.directory
        install/LdapEditorApp.menu
    )
endif()

# ── Executable ───────────────────────────────────────────────────────────────

ADD_EXECUTABLE(${PROJECT_NAME}
    MACOSX_BUNDLE
    ${ldapeditor_SOURCES}
    ${ldapeditor_HEADERS}
)

# ── Link libraries ───────────────────────────────────────────────────────────

target_link_libraries(${PROJECT_NAME}
    LdapEditorCore
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Core5Compat
)

# ── macOS bundle properties and deployment ───────────────────────────────────

if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE                TRUE
        MACOSX_BUNDLE_INFO_PLIST     "${CMAKE_CURRENT_SOURCE_DIR}/install/Info.plist.in"
        MACOSX_BUNDLE_ICON_FILE      "LdapEditorApp.icns"
        MACOSX_BUNDLE_BUNDLE_NAME    "${PRODUCT}"
        MACOSX_BUNDLE_BUNDLE_VERSION "${VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION}"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.ldapeditor.app"
        MACOSX_BUNDLE_COPYRIGHT      "Copyright 2024 ${COMPANY}"
        INSTALL_RPATH                "@executable_path/../Frameworks"
        INSTALL_RPATH_USE_LINK_PATH  TRUE
    )

    # Copy LdapEditorCore dylib into the bundle Frameworks directory
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>/Contents/Frameworks"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:LdapEditorCore>"
            "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>/Contents/Frameworks/"
        COMMENT "Bundling LdapEditorCore"
    )

    # Run macdeployqt to embed Qt frameworks
    find_program(MACDEPLOYQT_EXEC macdeployqt
        HINTS "${Qt6_DIR}/../../../bin"
    )
    if(MACDEPLOYQT_EXEC)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${MACDEPLOYQT_EXEC}"
                "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>"
                -always-overwrite
            COMMENT "Running macdeployqt"
        )
    else()
        message(WARNING "macdeployqt not found — Qt frameworks will not be bundled")
    endif()

    install(TARGETS ${PROJECT_NAME}
        BUNDLE  DESTINATION .
        RUNTIME DESTINATION bin
    )

# ── Linux: install and XDG desktop integration ───────────────────────────────

else()
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INSTALL_RPATH             "${CMAKE_INSTALL_PREFIX}/lib"
        INSTALL_RPATH_USE_LINK_PATH TRUE
    )

    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
    )

    install(FILES install/LdapEditorApp.png
        DESTINATION "share/icons/hicolor/32x32/apps")
    install(FILES install/LdapEditorApp.desktop
        DESTINATION "share/applications")
    install(FILES install/LdapEditorApp.menu
        DESTINATION "share/xdg/menus/applications-merged")
    install(FILES install/LdapEditorApp.directory
        DESTINATION "share/desktop-directories")
endif()
