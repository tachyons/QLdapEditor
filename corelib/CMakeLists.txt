cmake_minimum_required(VERSION 3.5.0)

project(LdapEditorCore)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include_directories(/usr/include)
include_directories(../common)
include_directories(../3rd_party/openldap/contrib/ldapcpp/src)

if (APPLE)
    include_directories(/opt/homebrew/opt/openldap/include)
    include_directories(/opt/homebrew/opt/openssl/include)
    link_directories(/opt/homebrew/opt/openldap/lib)
    link_directories(/opt/homebrew/opt/openssl/lib)
endif()

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


FIND_LIBRARY(LDAP_LIBRARY NAMES ldap PATHS /opt/homebrew/opt/openldap/lib /usr/lib64)
FIND_LIBRARY(LBER_LIBRARY NAMES lber PATHS /opt/homebrew/opt/openldap/lib /usr/lib64)
FIND_LIBRARY(SSL_LIBRARY NAMES ssl PATHS /opt/homebrew/opt/openssl/lib /usr/lib64)
FIND_LIBRARY(CRYPTO_LIBRARY NAMES crypto PATHS /opt/homebrew/opt/openssl/lib /usr/lib64)


add_library(${PROJECT_NAME} SHARED ${LdapCore_SRCS} ${OpenLDAPCpp_SRCS})

target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Core5Compat ${LDAP_LIBRARY} ${LBER_LIBRARY} ${SSL_LIBRARY} ${CRYPTO_LIBRARY})


INSTALL(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
)
