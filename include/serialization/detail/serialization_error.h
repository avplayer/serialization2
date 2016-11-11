#pragma once

#include <exception>
#include <iostream>

namespace serialization
{

class serialization_error: public std::exception
{

public:
    serialization_error(const char *what, const char *where)
        : m_what(what)
        , m_where(where)
    {
    }

    const char *what() const throw()
    {
        return m_what;
    }

    const char *where() const
    {
        return m_where;
    }

private:
    const char *m_what;
    const char *m_where;
};
	
void throw_serialization_error(const char * where, const char * what)
{
    throw serialization_error(what, where);
}

} // namespace serialization
