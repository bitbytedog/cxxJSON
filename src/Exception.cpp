/*
Copyright (c) 2016, Steve Williams (bitbytedog)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <JSON/Exception.h>

namespace JSON
{

    using namespace std;

    Exception&
    Exception::operator= (const Exception& rhs)
    {
        return *this;
    }

    Exception::Exception(const string& name, const string& file, int line, const string& method, const string& message) throw()
        : runtime_error(((ostringstream&)(m_oss << name << " thrown by: " << method << " (" << file << ":" << line << ") " << message << endl)).str())
        , m_name(name)
        , m_file(file)
        , m_line(line)
        , m_method(method)
        , m_message(message)
    {
    }

    Exception::Exception(const Exception& rhs)
        : runtime_error(rhs)
        , m_name(rhs.m_name)
        , m_file(rhs.m_file)
        , m_line(rhs.m_line)
        , m_method(rhs.m_method)
        , m_message(rhs.m_message)
    {
    }

    Exception::~Exception() throw()
    {
    }

    const string&
    Exception::getName() const
    {
        return m_name;
    }

    const string&
    Exception::getfile() const
    {
        return m_file;
    }

    int
    Exception::getLine() const
    {
        return m_line;
    }

    const string&
    Exception::getMethod() const
    {
        return m_method;
    }

    const string&
    Exception::getMessage() const
    {
        return m_message;
    }

    string
    Exception::toString() const
    {
        ostringstream ss;
        ss << m_name << " thrown by: " << m_method << " (" << m_file << ":" << m_line << ") ";

        if(m_message.length() > 0)
            ss << "message: " << m_message;

        return ss.str();
    }

}

