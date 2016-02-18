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
#include <JSON/Array.h>
#include <JSON/Object.h>

namespace JSON
{
    using namespace std;

    Array Array::Null(JSON::JNONE);

    Array::Array(bool trace)
        : JSON(JSON::JARRAY, trace)
    {
    }

    Array::Array(JSON::Type type, bool trace)
        : JSON(type, trace)
    {
    }


    Array::Array(const std::string& jsonstr, bool trace)
        : JSON(jsonstr.empty() ? "[]" : jsonstr, trace)
    {
        if(JSON::getType() != JSON::JARRAY)
            THROW_MSG(Exception, "parse error: type mismatch");
    }

    Array::Array(const char* jsonstr, bool trace)
        : JSON(jsonstr == NULL || *jsonstr == '\0' ? "[]" : jsonstr, trace)
    {
        if(JSON::getType() != JSON::JARRAY)
            THROW_MSG(Exception, "parse error: type mismatch");
    }

    Array::Array(istream& is, bool trace)
        : JSON(is, trace)
    {
        if(JSON::getType() != JSON::JARRAY)
            THROW_MSG(Exception, "parse error: type mismatch");
    }

    Array::~Array()
    {
    }

    JSON::Type
    Array::getType(int index) const
    {
        return JSON::getType(index);
    }

    bool
    Array::getBoolean(int index) const
    {
        return JSON::getBoolean(index);
    }

    double
    Array::getDouble(int index) const
    {
        return JSON::getDouble(index);
    }

    long
    Array::getLong(int index) const
    {
        return JSON::getLong(index);
    }

    string
    Array::getString(int index) const
    {
        return JSON::getString(index);
    }

    Array&
    Array::getJSONArray(int index, Array& value, bool copy) const
    {
        JSON::getJSONArray(index, value, copy);
        return value;
    }

    Object&
    Array::getJSONObject(int index, Object& value, bool copy) const
    {
        JSON::getJSONObject(index, value, copy);
        return value;
    }

    size_t
    Array::length() const
    {
        return JSON::length();
    }

    bool
    Array::isNull(int index) const
    {
        return JSON::isNull(index);
    }

    bool
    Array::optBoolean(int index, bool defaultValue) const
    {
        return JSON::optBoolean(index, defaultValue);
    }

    double
    Array::optDouble(int index, double defaultValue) const
    {
        return JSON::optDouble(index, defaultValue);
    }

    long
    Array::optLong(int index, long defaultValue) const
    {
        return JSON::optLong(index, defaultValue);
    }

    std::string
    Array::optString(int index, const std::string& defaultValue) const
    {
        return JSON::optString(index, defaultValue);
    }

    Array&
    Array::optJSONArray(int index, Array& value, bool copy) const
    {
        if(JSON::optJSONArray(index, value, copy))
            return value;
        else
            return Array::Null;
    }

    Object&
    Array::optJSONObject(int index, Object& value, bool copy) const
    {
        if(JSON::optJSONObject(index, value, copy))
            return value;
        else
            return Object::Null;
    }

    Array&
    Array::put(int index, bool value)
    {

        if(index < 0 || index >= (int)JSON::length())
            THROW_MSG(Exception, "index out of bounds for JSON array: [" << index << "]");

        replaceItem(index, value);

        return *this;
    }

    Array&
    Array::put(int index, double value)
    {

        if(index < 0 || index >= (int)JSON::length())
            THROW_MSG(Exception, "index out of bounds for JSON array: [" << index << "]");

        replaceItem(index, value);

        return *this;
    }

    Array&
    Array::put(int index, long value)
    {

        if(index < 0 || index >= (int)JSON::length())
            THROW_MSG(Exception, "index out of bounds for JSON array: [" << index << "]");

        replaceItem(index, value);

        return *this;
    }

    Array&
    Array::put(int index, const std::string& value)
    {

        if(index < 0 || index >= (int)JSON::length())
            THROW_MSG(Exception, "index out of bounds for JSON array: [" << index << "]");

        replaceItem(index, value);

        return *this;
    }

    Array&
    Array::put(int index, const char* value)
    {

        if(0 == value)
            return put(index, std::string());
        else
            return put(index, std::string(value));
    }


    Array&
    Array::put(int index, const Array& value)
    {

        if(index < 0 || index >= (int)JSON::length())
            THROW_MSG(Exception, "index out of bounds for JSON array: [" << index << "]");

        replaceItem(index, dynamic_cast<const JSON&>(value));

        return *this;
    }

    Array&
    Array::put(int index, const Object& value)
    {

        if(index < 0 || index >= (int)JSON::length())
            THROW_MSG(Exception, "index out of bounds for JSON array: [" << index << "]");

        replaceItem(index, dynamic_cast<const JSON&>(value));

        return *this;
    }

    Array&
    Array::put(int index)
    {

        if(index < 0 || index >= (int)JSON::length())
            THROW_MSG(Exception, "index out of bounds for JSON array: [" << index << "]");

        replaceItem(index);

        return *this;
    }

    Array&
    Array::append(bool value)
    {

        addItem(value);

        return *this;
    }

    Array&
    Array::append(double value)
    {

        addItem(value);

        return *this;
    }

    Array&
    Array::append(long value)
    {

        addItem(value);

        return *this;
    }

    Array&
    Array::append(const std::string& value)
    {

        addItem(value);

        return *this;
    }

    Array&
    Array::append(const char* value)
    {

        if(0 == value)
            return append(std::string());
        else
            return append(std::string(value));
    }


    Array&
    Array::append(const Array& value)
    {

        addItem(dynamic_cast<const JSON&>(value));

        return *this;
    }

    Array&
    Array::append(const Object& value)
    {

        addItem(dynamic_cast<const JSON&>(value));

        return *this;
    }

    Array&
    Array::append()
    {

        addItem();

        return *this;
    }

    Array&
    Array::remove(int index)
    {

        removeItem(index);

        return *this;
    }

    bool
    Array::operator==(const Array& other) const
    {

        /*
         * compare compressed JSON
         */
        return toString() == other.toString();
    }

    bool
    Array::operator!=(const Array& other) const
    {

        return !(*this == other);
    }


} /* namespace JSON */
