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

    Object Object::Null(JSON::JNONE);

    Object::Object(bool trace)
        : JSON(JSON::JOBJECT, trace)
    {
    }

    Object::Object(JSON::Type type, bool trace)
        : JSON(type, trace)
    {
    }

    Object::Object(const std::string& jsonstr, bool trace)
        : JSON(jsonstr.empty() ? "{}" : jsonstr, trace)
    {
        if(JSON::getType() != JSON::JOBJECT)
            THROW_MSG(Exception, "parse error: type mismatch");
    }

    Object::Object(const char* jsonstr, bool trace)
        : JSON(jsonstr == NULL || *jsonstr == '\0' ? "{}" : jsonstr, trace)
    {
        if(JSON::getType() != JSON::JOBJECT)
            THROW_MSG(Exception, "parse error: type mismatch");
    }

    Object::Object(const Kvps& kvps, bool trace)
        : JSON(JSON::JOBJECT, trace)
    {
        /*
         * no need to check for duplicates, just insert all keys in map<>
         */
        for(Kvps::const_iterator i = kvps.begin(); i != kvps.end(); ++i)
            addItem(i->first, i->second);
    }

    Object::Object(istream& is, bool trace)
        : JSON(is, trace)
    {
        if(JSON::getType() != JSON::JOBJECT)
            THROW_MSG(Exception, "parse error: type mismatch");
    }

    Object::~Object()
    {
    }

    JSON::Type
    Object::getType(const std::string& key) const
    {
        return JSON::getType(key);
    }

    bool
    Object::getBoolean(const string& key) const
    {
        return JSON::getBoolean(key);
    }

    double
    Object::getDouble(const string& key) const
    {
        return JSON::getDouble(key);
    }

    long
    Object::getLong(const string& key) const
    {
        return JSON::getLong(key);
    }

    string
    Object::getString(const string& key) const
    {
        return JSON::getString(key);
    }

    Array&
    Object::getJSONArray(const std::string& key, Array& value, bool copy) const
    {
        JSON::getJSONArray(key, value, copy);
        return value;
    }

    Object&
    Object::getJSONObject(const std::string& key, Object& value, bool copy) const
    {
        JSON::getJSONObject(key, value, copy);
        return value;
    }

    bool
    Object::has(const std::string& key) const
    {

        return JSON::has(key);
    }

    size_t
    Object::length() const
    {
        return JSON::length();
    }

    bool
    Object::isNull(const std::string& key) const
    {

        return JSON::isNull(key);
    }

    bool
    Object::optBoolean(const std::string& key, bool defaultValue) const
    {

        return JSON::optBoolean(key, defaultValue);
    }

    double
    Object::optDouble(const std::string& key, double defaultValue) const
    {

        return JSON::optDouble(key, defaultValue);
    }

    long
    Object::optLong(const std::string& key, long defaultValue) const
    {

        return JSON::optLong(key, defaultValue);
    }

    std::string
    Object::optString(const std::string& key, const std::string& defaultValue) const
    {

        return JSON::optString(key, defaultValue);
    }

    Array&
    Object::optJSONArray(const std::string& key, Array& value, bool copy) const
    {
        if(JSON::optJSONArray(key, value, copy))
            return value;
        else
            return Array::Null;
    }

    Object&
    Object::optJSONObject(const std::string& key, Object& value, bool copy) const
    {
        if(JSON::optJSONObject(key, value, copy))
            return value;
        else
            return Object::Null;
    }

    Object&
    Object::put(const std::string& key, bool value)
    {

        if(key.empty())
            THROW_MSG(Exception, "no key provided for boolean value");

        if(JSON::has(key))
            replaceItem(key, value);
        else
            addItem(key, value);

        return *this;
    }

    Object&
    Object::put(const std::string& key, double value)
    {

        if(key.empty())
            THROW_MSG(Exception, "no key provided for double number value");

        if(JSON::has(key))
            replaceItem(key, value);
        else
            addItem(key, value);

        return *this;
    }

    Object&
    Object::put(const std::string& key, long value)
    {

        if(key.empty())
            THROW_MSG(Exception, "no key provided for long number value");

        if(JSON::has(key))
            replaceItem(key, value);
        else
            addItem(key, value);

        return *this;
    }

    Object&
    Object::put(const std::string& key, const std::string& value)
    {

        if(key.empty())
            THROW_MSG(Exception, "no key provided for string value");

        if(JSON::has(key))
            replaceItem(key, value);
        else
            addItem(key, value);

        return *this;
    }

    Object&
    Object::put(const std::string& key, const char* value)
    {

        if(0 == value)
            return put(key, std::string());
        else
            return put(key, std::string(value));
    }

    Object&
    Object::put(const std::string& key, const Array& value)
    {

        if(key.empty())
            THROW_MSG(Exception, "no key provided for JSON array");

        if(JSON::has(key))
            replaceItem(key, dynamic_cast<const JSON&>(value));
        else
            addItem(key, dynamic_cast<const JSON&>(value));

        return *this;
    }

    Object&
    Object::put(const std::string& key, const Object& value)
    {

        if(key.empty())
            THROW_MSG(Exception, "no key provided for JSON object");

        if(JSON::has(key))
            replaceItem(key, dynamic_cast<const JSON&>(value));
        else
            addItem(key, dynamic_cast<const JSON&>(value));

        return *this;
    }

    Object&
    Object::put(const std::string& key)
    {

        if(key.empty())
            THROW_MSG(Exception, "no key provided for null value");

        if(JSON::has(key))
            replaceNullItem(key);
        else
            addNullItem(key);

        return *this;
    }

    Object&
    Object::append(const std::string& key, bool value)
    {

        if(key.empty())
            THROW_MSG(Exception, "no key provided for JSON array");

        Array array;
        if(!JSON::optJSONArray(key, array))
            JSON::addItem(key, dynamic_cast<const JSON&>(array));

        array.append(value);
        return *this;
    }

    Object&
    Object::append(const std::string& key, double value)
    {

        if(key.empty())
            THROW_MSG(Exception, "no key provided for JSON array");

        Array array;
        if(!JSON::optJSONArray(key, array))
            JSON::addItem(key, dynamic_cast<const JSON&>(array));

        array.append(value);
        return *this;
    }

    Object&
    Object::append(const std::string& key, long value)
    {

        if(key.empty())
            THROW_MSG(Exception, "no key provided for JSON array");

        Array array;
        if(!JSON::optJSONArray(key, array))
            JSON::addItem(key, dynamic_cast<const JSON&>(array));

        array.append(value);
        return *this;
    }

    Object&
    Object::append(const std::string& key, const std::string& value)
    {

        if(key.empty())
            THROW_MSG(Exception, "no key provided for JSON array");

        Array array;
        if(!JSON::optJSONArray(key, array))
            JSON::addItem(key, dynamic_cast<const JSON&>(array));

        array.append(value);
        return *this;
    }

    Object&
    Object::append(const std::string& key, const char* value)
    {

        if(0 == value)
            return append(key, std::string());
        else
            return append(key, std::string(value));
    }


    Object&
    Object::append(const std::string& key, const Array& value)
    {

        if(key.empty())
            THROW_MSG(Exception, "no key provided for JSON array");

        Array array;
        if(!JSON::optJSONArray(key, array))
            JSON::addItem(key, dynamic_cast<const JSON&>(array));

        array.append(value);
        return *this;
    }

    Object&
    Object::append(const std::string& key, const Object& value)
    {

        if(key.empty())
            THROW_MSG(Exception, "no key provided for JSON array");

        Array array;
        if(!JSON::optJSONArray(key, array))
            JSON::addItem(key, dynamic_cast<const JSON&>(array));

        array.append(value);
        return *this;
    }

    Object&
    Object::append(const std::string& key)
    {

        if(key.empty())
            THROW_MSG(Exception, "no key provided for JSON array");

        Array array;
        if(!JSON::optJSONArray(key, array))
            JSON::addItem(key, dynamic_cast<const JSON&>(array));

        array.append();
        return *this;
    }

    Object&
    Object::remove(const std::string& key)
    {

        if(key.empty())
            THROW_MSG(Exception, "no key provided for JSON array");

        removeItem(key);

        return *this;
    }

    bool
    Object::operator==(const Object& other) const
    {

        /*
         * compare compressed JSON
         */
        return toString() == other.toString();
    }

    bool
    Object::operator!=(const Object& other) const
    {

        return !(*this == other);
    }

} /* namespace JSON */
