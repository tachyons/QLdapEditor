cmake_minimum_required(VERSION 3.16)

project(LdapEditorCore)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include_directories(../common)
include_directories(../3rd_party/openldap/contrib/ldapcpp/src)

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
if(LDAP_INCLUDE_DIR)
    include_directories(BEFORE ${LDAP_INCLUDE_DIR})
endif()

# Use CMake's portable OpenSSL finder instead of raw FIND_LIBRARY
if(APPLE)
    # Prefer Homebrew's OpenSSL over the deprecated macOS system copy
    set(OPENSSL_ROOT_DIR /opt/homebrew/opt/openssl)
endif()
find_package(OpenSSL REQUIRED)

add_compile_options(-DLDAPEDITORCORE_SO)
add_compile_options(-DLDAPEDITORCORE_SO_EXPORTS)


FIND_PACKAGE(Qt6Core CONFIG REQUIRED)
FIND_PACKAGE(Qt6Widgets CONFIG REQUIRED)
FIND_PACKAGE(Qt6Core5Compat CONFIG REQUIRED)

set(LdapCore_SRCS
    datatypes.h
    datatypesinternal.h 
    CLdapException.h
    CLdapException.cpp
    CLdapSchema.h
    CLdapSchema.cpp
    cldapserver.h
    cldapserver.cpp
    CLdapSchemaValidator.cpp
    qfunctional.h
    CLdapEntry.h
    CLdapEntry.cpp
    CLdapData.h
    CLdapData.cpp
    CLdapAttribute.h
    CLdapAttribute.cpp    
    ../common/common.cpp
    ../common/common.h
)

if (APPLE)
    set(LdapCore_SRCS ${LdapCore_SRCS}  sasl_decode64.cpp)
endif(APPLE)

file(GLOB OpenLDAPCpp_SRCS
    "../3rd_party/openldap/contrib/ldapcpp/src/*.cpp"
    "../3rd_party/openldap/contrib/ldapcpp/src/*.h"
)


FIND_LIBRARY(LDAP_LIBRARY NAMES ldap libldap
    HINTS /opt/homebrew/opt/openldap/lib /usr/lib64
)
FIND_LIBRARY(LBER_LIBRARY NAMES lber liblber
    HINTS /opt/homebrew/opt/openldap/lib /usr/lib64
)


add_library(${PROJECT_NAME} SHARED ${LdapCore_SRCS} ${OpenLDAPCpp_SRCS})

target_link_libraries(${PROJECT_NAME}
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Core5Compat
    ${LDAP_LIBRARY} ${LBER_LIBRARY}
    OpenSSL::SSL OpenSSL::Crypto
)


INSTALL(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
)
