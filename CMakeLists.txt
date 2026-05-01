cmake_minimum_required(VERSION 3.16)
project(QLdapEditor VERSION 0.5.57 LANGUAGES CXX)

set(PRODUCT     "LDAP Editor")
set(PACKAGENAME "ldapeditor")
set(COMPANY     "Very Big Company Inc.")
set(PACKAGEGROUP "Utilities")

file(WRITE ${CMAKE_BINARY_DIR}/version.h.in [=[
/* CMAKE Generated file, do not edit */
#define VERSION "@PROJECT_VERSION@"
#define PRODUCT "@PRODUCT@"
#define COMPANY "@COMPANY@"
]=])
configure_file(${CMAKE_BINARY_DIR}/version.h.in ${CMAKE_BINARY_DIR}/version.h @ONLY)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr/local CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()
set(CMAKE_AUTORCC ON)

add_subdirectory(corelib)
add_subdirectory(editor)

# ── CPack — common settings ───────────────────────────────────────────────────
set(CPACK_PACKAGE_NAME        ${PACKAGENAME})
set(CPACK_PACKAGE_VERSION     ${PROJECT_VERSION})
set(CPACK_PACKAGE_RELEASE     1)
set(CPACK_PACKAGE_VENDOR      ${COMPANY})
set(CPACK_PACKAGE_CONTACT     "Dmitry Markevich <d_markevich@bte.ae>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PRODUCT} tool for view/edit LDAP directories.")
set(CPACK_PACKAGE_EXECUTABLES LdapEditorApp "${PRODUCT}")

# ── CPack — macOS (DMG drag-and-drop) ────────────────────────────────────────
if(APPLE)
    set(CPACK_GENERATOR         "DragNDrop")
    set(CPACK_DMG_VOLUME_NAME   "${PRODUCT}")
    set(CPACK_DMG_FORMAT        "UDZO")
    set(CPACK_PACKAGE_FILE_NAME "${PACKAGENAME}-${PROJECT_VERSION}-macos-${CMAKE_SYSTEM_PROCESSOR}")
    set(CPACK_PACKAGE_ICON      "${CMAKE_CURRENT_SOURCE_DIR}/editor/images/LdapEditorApp.png")

# ── CPack — Linux (DEB + RPM) ─────────────────────────────────────────────────
else()
    set(CPACK_GENERATOR                 "DEB;RPM")
    set(CPACK_PACKAGE_FILE_NAME         "${PACKAGENAME}-${PROJECT_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
    set(CPACK_PACKAGING_INSTALL_PREFIX  ${CMAKE_INSTALL_PREFIX})
    set(CPACK_PACKAGE_DEFAULT_LOCATION  "bin")

    # DEB
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")
    set(CPACK_DEBIAN_PACKAGE_SECTION    "utils")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS    "libldap-2.5-0 | libldap-2.4-2, libssl3 | libssl1.1")

    # RPM
    set(CPACK_RPM_PACKAGE_LICENSE "GPLv2")
    set(CPACK_RPM_PACKAGE_GROUP   ${PACKAGEGROUP})
    list(APPEND CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST
        "/usr" "/usr/local" "/usr/local/bin" "/usr/local/lib"
        "/usr/share" "/usr/share/applications" "/usr/share/desktop-directories"
        "/usr/share/icons" "/usr/share/icons/hicolor" "/usr/share/icons/hicolor/32x32"
        "/usr/share/icons/hicolor/32x32/apps"
        "/usr/share/xdg" "/usr/share/xdg/menus" "/usr/share/xdg/menus/applications-merged"
        "/opt"
    )
endif()

include(CPack)
