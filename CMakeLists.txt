CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
PROJECT(QLdapEditor)

SET(PRODUCT "LDAP Editor")
SET(PACKAGENAME "ldapeditor")
SET(COMPANY "Very Big Company Inc.")

SET(CMAKE_VERSION_MAJOR 0)
SET(CMAKE_VERSION_MINOR 5)
SET(CMAKE_VERSION_PATCH 57)


SET(VERSION "${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}")

FILE(WRITE ${CMAKE_BINARY_DIR}/version.h.in
"/*CMAKE Generated file, do not edit*/\n
\#define VERSION \"@VERSION@\"\n\
\#define PRODUCT \"@PRODUCT@\"\n\
\#define COMPANY \"@COMPANY@\"\n\
"
)
configure_file(${CMAKE_BINARY_DIR}/version.h.in ${CMAKE_BINARY_DIR}/version.h)



SET(APP_DEFAULT_LOCATION_DIR "/usr/local/bin/${PACKAGE_NAME}" )
SET(SO_DEFAULT_LOCATION_DIR "/usr/local/lib/${PACKAGE_NAME}" )
SET(PACKAGEGROUP "Utilities")

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX /usr/local CACHE PATH "Install path prefix, prepended onto install directories."  FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall)
add_compile_options(-Wextra)

#include_directories(common)
add_subdirectory(corelib)
add_subdirectory(editor)


#INSTALL (DIRECTORY 3rd_party/openldap/lib/ DESTINATION lib)

# ── CPack — common settings ───────────────────────────────────────────────────
SET(CPACK_PACKAGE_NAME        ${PACKAGENAME})
SET(CPACK_PACKAGE_VERSION     ${VERSION})
SET(CPACK_PACKAGE_RELEASE     1)
SET(CPACK_PACKAGE_VENDOR      ${COMPANY})
SET(CPACK_PACKAGE_CONTACT     "Dmitry Markevich <d_markevich@bte.ae>")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PRODUCT} tool for view/edit LDAP directories.")
SET(CPACK_PACKAGE_EXECUTABLES LdapEditorApp "${PRODUCT}")

# ── CPack — macOS (DMG drag-and-drop) ────────────────────────────────────────
if(APPLE)
    SET(CPACK_GENERATOR           "DragNDrop")
    SET(CPACK_DMG_VOLUME_NAME     "${PRODUCT}")
    SET(CPACK_DMG_FORMAT          "UDZO")
    SET(CPACK_PACKAGE_FILE_NAME   "${PACKAGENAME}-${VERSION}-macos-${CMAKE_SYSTEM_PROCESSOR}")
    SET(CPACK_PACKAGE_ICON        "${CMAKE_CURRENT_SOURCE_DIR}/editor/images/LdapEditorApp.png")

# ── CPack — Windows (NSIS installer) ─────────────────────────────────────────
elseif(WIN32)
    SET(CPACK_GENERATOR           "NSIS")
    SET(CPACK_PACKAGE_FILE_NAME   "${PACKAGENAME}-${VERSION}-windows-x64")
    SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${PRODUCT}")
    SET(CPACK_NSIS_PACKAGE_NAME   "${PRODUCT}")
    SET(CPACK_NSIS_DISPLAY_NAME   "${PRODUCT}")
    SET(CPACK_NSIS_CONTACT        "${CPACK_PACKAGE_CONTACT}")
    SET(CPACK_NSIS_HELP_LINK      "https://github.com/Shcherbich/QLdapEditor")
    SET(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/Shcherbich/QLdapEditor")
    SET(CPACK_NSIS_MODIFY_PATH    ON)
    SET(CPACK_NSIS_EXECUTABLES_DIRECTORY "bin")
    SET(CPACK_NSIS_MUI_ICON       "${CMAKE_CURRENT_SOURCE_DIR}/editor/install/LdapEditorApp.ico")
    SET(CPACK_NSIS_MUI_UNIICON    "${CMAKE_CURRENT_SOURCE_DIR}/editor/install/LdapEditorApp.ico")
    SET(CPACK_NSIS_CREATE_ICONS_EXTRA
        "CreateShortcut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PRODUCT}.lnk' '$INSTDIR\\\\bin\\\\LdapEditorApp.exe'"
    )
    SET(CPACK_NSIS_DELETE_ICONS_EXTRA
        "Delete '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PRODUCT}.lnk'"
    )

# ── CPack — Linux (DEB + RPM) ─────────────────────────────────────────────────
else()
    SET(CPACK_GENERATOR           "DEB;RPM")
    SET(CPACK_PACKAGE_FILE_NAME   "${PACKAGENAME}-${VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
    SET(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
    SET(CPACK_PACKAGE_DEFAULT_LOCATION "bin")

    # DEB
    SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")
    SET(CPACK_DEBIAN_PACKAGE_SECTION    "utils")
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS    "libldap-2.5-0 | libldap-2.4-2, libssl3 | libssl1.1")

    # RPM
    SET(CPACK_RPM_PACKAGE_LICENSE "GPLv2")
    SET(CPACK_RPM_PACKAGE_GROUP   ${PACKAGEGROUP})
    LIST(APPEND CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST
        "/usr" "/usr/local" "/usr/local/bin" "/usr/local/lib"
        "/usr/share" "/usr/share/applications" "/usr/share/desktop-directories"
        "/usr/share/icons" "/usr/share/icons/hicolor" "/usr/share/icons/hicolor/32x32"
        "/usr/share/icons/hicolor/32x32/apps"
        "/usr/share/xdg" "/usr/share/xdg/menus" "/usr/share/xdg/menus/applications-merged"
        "/opt"
    )
endif()

INCLUDE(CPack)
