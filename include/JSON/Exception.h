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

#ifndef JSON__EXCEPTION_H_
#define JSON__EXCEPTION_H_

#include <exception>
#include <sstream>
#include <string>

namespace JSON
{

#define THROW(exception) throw exception(#exception, __FILE__, __LINE__, __PRETTY_FUNCTION__)
#define THROW_MSG(exception, message) \
do { \
std::ostringstream oss; \
oss << message; \
throw exception(#exception, __FILE__, __LINE__, __PRETTY_FUNCTION__, oss.str()); \
}while(0)

    class Exception : public std::runtime_error
    {
        /**
         * Assignment not allowed
         */
        Exception& operator= (const Exception& rhs);

    protected:
        std::ostringstream m_oss;

    public:

        Exception(const std::string& name, const std::string& file, int line, const std::string& method, const std::string& message = "") throw();

        Exception(const Exception& rhs);

        virtual ~Exception() throw();

        const std::string& getName() const;

        const std::string& getfile() const;

        int getLine() const;

        const std::string& getMethod() const;

        const std::string& getMessage() const;

        std::string toString() const;

    private:
        const std::string m_name;
        const std::string m_file;
        int m_line;
        const std::string m_method;
        const std::string m_message;
    };

}
#endif /* JSON__EXCEPTION_H_ */
