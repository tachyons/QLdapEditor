cmake_minimum_required(VERSION 3.16)
project(LdapEditorApp)

# ── macOS: generate ICNS icon ────────────────────────────────────────────────

if(APPLE)
    set(ICON_PNG    "${CMAKE_CURRENT_SOURCE_DIR}/images/LdapEditorApp.png")
    set(ICONSET_DIR "${CMAKE_CURRENT_BINARY_DIR}/LdapEditorApp.iconset")
    set(ICON_ICNS   "${CMAKE_CURRENT_BINARY_DIR}/LdapEditorApp.icns")

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
endif()

# ── Executable ───────────────────────────────────────────────────────────────

qt_add_executable(${PROJECT_NAME}
    MACOSX_BUNDLE
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
    ${CMAKE_SOURCE_DIR}/common/common.cpp
)

if(APPLE)
    target_sources(${PROJECT_NAME} PRIVATE "${ICON_ICNS}")
else()
    # XDG desktop files — added for IDE visibility only, not compiled
    target_sources(${PROJECT_NAME} PRIVATE
        install/LdapEditorApp.desktop
        install/LdapEditorApp.directory
        install/LdapEditorApp.menu
    )
endif()

# ── Include directories ───────────────────────────────────────────────────────

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/corelib
    ${CMAKE_BINARY_DIR}
)

# ── Link libraries ───────────────────────────────────────────────────────────

target_link_libraries(${PROJECT_NAME} PRIVATE
    LdapEditorCore
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# ── macOS bundle properties and deployment ───────────────────────────────────

if(APPLE)
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt
        HINTS "${Qt6Core_DIR}/../../../bin"
        REQUIRED
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE                      TRUE
        MACOSX_BUNDLE_INFO_PLIST           "${CMAKE_CURRENT_SOURCE_DIR}/install/Info.plist.in"
        MACOSX_BUNDLE_ICON_FILE            "LdapEditorApp.icns"
        MACOSX_BUNDLE_BUNDLE_NAME          "${PRODUCT}"
        MACOSX_BUNDLE_BUNDLE_VERSION       "${CMAKE_PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${CMAKE_PROJECT_VERSION}"
        MACOSX_BUNDLE_GUI_IDENTIFIER       "com.ldapeditor.app"
        MACOSX_BUNDLE_COPYRIGHT            "Copyright 2024 ${COMPANY}"
        INSTALL_RPATH                      "@executable_path/../Frameworks"
        BUILD_WITH_INSTALL_RPATH           TRUE
        INSTALL_RPATH_USE_LINK_PATH        FALSE
    )

    # Deploy Qt frameworks, platform plugins, and all linked dependencies
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${MACDEPLOYQT_EXECUTABLE}
            "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>"
            -always-overwrite
        COMMENT "Running macdeployqt"
    )

    # Re-sign after macdeployqt invalidates the previous signature
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND codesign --deep --force --sign -
            "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>"
        COMMENT "Ad-hoc signing app bundle"
    )

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
