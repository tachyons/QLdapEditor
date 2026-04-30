#include "CLdapException.h"

namespace ldapcore
{

CLdapNameMissedException::CLdapNameMissedException(char const* lpText)
	: m_what(lpText)
{
}

char const* CLdapNameMissedException::what() const noexcept
{
	return m_what.c_str();
}


CLdapMatchRuleException::CLdapMatchRuleException(char const* lpText)
	: m_what(lpText)
{
}

char const* CLdapMatchRuleException::what() const noexcept
{
	return m_what.c_str();
}


CLdapServerException::CLdapServerException(char const* lpText)
    : m_what(lpText)
{
}

char const* CLdapServerException::what() const noexcept
{
    return m_what.c_str();
}

}
