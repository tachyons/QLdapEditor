cmake_minimum_required(VERSION 3.16)
project(LdapEditorCore)

# Locate OpenLDAP headers.
# First pass: only search explicit hints (NO_DEFAULT_PATH prevents macOS from
# resolving ldap.h to the system LDAP.framework, which is API-incompatible).
find_path(LDAP_INCLUDE_DIR ldap.h
    HINTS /opt/homebrew/opt/openldap/include
    NO_DEFAULT_PATH
)
# Second pass: fall back to system paths (Linux, Windows/vcpkg).
if(NOT LDAP_INCLUDE_DIR)
    find_path(LDAP_INCLUDE_DIR ldap.h)
endif()

# Use CMake's portable OpenSSL finder instead of raw FIND_LIBRARY
if(APPLE)
    # Prefer Homebrew's OpenSSL over the deprecated macOS system copy
    set(OPENSSL_ROOT_DIR /opt/homebrew/opt/openssl)
endif()
find_package(OpenSSL REQUIRED)

find_library(LDAP_LIBRARY NAMES ldap libldap
    HINTS /opt/homebrew/opt/openldap/lib /usr/lib64
)
find_library(LBER_LIBRARY NAMES lber liblber
    HINTS /opt/homebrew/opt/openldap/lib /usr/lib64
)

set(LdapCore_SRCS
    CLdapException.cpp
    CLdapSchema.cpp
    cldapserver.cpp
    CLdapSchemaValidator.cpp
    CLdapEntry.cpp
    CLdapData.cpp
    CLdapAttribute.cpp
    ../common/common.cpp
)

if(APPLE)
    list(APPEND LdapCore_SRCS sasl_decode64.cpp)
endif()

file(GLOB OpenLDAPCpp_SRCS
    "../3rd_party/openldap/contrib/ldapcpp/src/*.cpp"
    "../3rd_party/openldap/contrib/ldapcpp/src/*.h"
)

qt_add_library(${PROJECT_NAME} SHARED ${LdapCore_SRCS} ${OpenLDAPCpp_SRCS})

target_compile_definitions(${PROJECT_NAME} PRIVATE
    LDAPEDITORCORE_SO
    LDAPEDITORCORE_SO_EXPORTS
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/3rd_party/openldap/contrib/ldapcpp/src
    ${LDAP_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    ${LDAP_LIBRARY}
    ${LBER_LIBRARY}
    OpenSSL::SSL
    OpenSSL::Crypto
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
)
