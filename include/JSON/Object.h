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

#ifndef JSON__OBJECT_H_
#define JSON__OBJECT_H_

#include <cstdint>
#include <cmath>
#include <map>
#include <iostream>
#include <string>

#include "Array.h"
#include "JSON.h"

namespace JSON
{

    class Array;

    class Object : public JSON
    {
        friend class Array;

        /**
         * Used to construct a Null JSON object ("")
         */
        Object(JSON::Type type, bool trace = false);

    public:
        /**
         * A Null JSON Object
         */
        static Object Null;

        /**
         * Construct an empty JSON object ("{}")
         */
        Object(bool trace = false);

        /**
         * Construct a JSON object from jsonstr
         *
         * @param jsonstr UTF-8 string representing object
         */
        Object(const std::string& jsonstr, bool trace = false);

        /**
         * Construct a JSON object from jsonstr
         *
         * @param jsonstr UTF-8 string representing object
         */
        Object(const char* jsonstr, bool trace = false);

        typedef std::map<std::string,std::string> Kvps;

        /**
         * Construct a JSON object from key value pairs
         *
         * @param kvps map of key value pairs, the values are inserted to the JSON object as strings
         */
        Object(const Kvps& kvps, bool trace = false);

        /**
         * Construct a JSON object from the contents of a file
         *
         * @param is open input stream of UTF-8 characters
         */
        Object(std::istream& is, bool trace = false);

        /**
         * Destroy a JSON object freeing the
         * memory associated with the parser if
         * and if the parser is owned by this object
         */
        virtual ~Object();

        /**
         * @see JSON::getType(const std::string&)
         */
        Type getType(const std::string& key) const;

        /**
         * @see JSON::getBoolean(const std::string&)
         */
        bool getBoolean(const std::string& key) const;

        /**
         * @see JSON::getDouble(const std::string&)
         */
        double getDouble(const std::string& key) const;

        /**
         * @see JSON::getInt<INTTYPE>(const std::string&)
         */
        template<typename INTTYPE>
        INTTYPE getInt(const std::string& key) const
        {
            return JSON::getInt<INTTYPE>(key);
        }

        /**
         * @see JSON::getLong(const std::string&)
         */
        long getLong(const std::string& key) const;

        /**
         * @see JSON::getString(const std::string&)
         */
        std::string getString(const std::string& key) const;

        /**
         * Delegates to JSON.
         * @see JSON::getJSONArray(const std::string&, JSON&, bool)
         * @return reference to value for chaining method calls
         */
        Array& getJSONArray(const std::string& key, Array& value, bool copy = false) const;

        /**
         * Delegates to JSON.
         * @see JSON::getJSONObject(const std::string&, JSON&, bool)
         * @return reference to value for chaining method calls
         */
        Object& getJSONObject(const std::string& key, Object& value, bool copy = false) const;

        /**
         * @see JSON::has(const std::string&)
         */
        bool has(const std::string& key) const;

        /**
         * @see JSON::length()
         */
        std::size_t length() const;

        /**
         * @see JSON::isNull(const std::string&)
         */
        bool isNull(const std::string& key) const;

        /**
         * @see JSON::getString(const std::string&)
         */
        bool optBoolean(const std::string& key, bool defaultValue = false) const;

        /**
         * @see JSON::getString(const std::string&)
         */
        double optDouble(const std::string& key, double defaultValue = std::nan("json")) const;

        /**
         * @see JSON::getString(const std::string&)
         */
        template<typename INTTYPE>
        INTTYPE optInt(const std::string& key, INTTYPE defaultValue = 0) const
        {

            return JSON::optInt<INTTYPE>(key, defaultValue);
        }

        /**
         * @see JSON::getString(const std::string&)
         */
        long optLong(const std::string& key, long defaultValue = 0L) const;

        /**
         * @see JSON::getString(const std::string&)
         */
        std::string optString(const std::string& key, const std::string& defaultValue = "") const;

        /**
         * Delegates to JSON.
         * @see JSON::optJSONArray(const std::string&, JSON&, bool)
         * @return reference to value for chaining method calls, or Array::Null if key not present
         */
        Array& optJSONArray(const std::string& key, Array& value, bool copy = false) const;

        /**
         * Returns a Array by value rather than reference
         * @see JSON::optJSONArray(const std::string&, JSON&, bool)
         * @return value for inline method calls, or Array::Null if key not present
         */
        const Array optJSONArray(const std::string& key, bool copy = false) const
        {
            Array val;
            return optJSONArray(key, val, copy);
        }

        /**
         * Delegates to JSON.
         * @see JSON::optJSONObject(const std::string&, JSON&, bool)
         * @return reference to value for chaining method calls, or Object::Null if key not present
         */
        Object& optJSONObject(const std::string& key, Object& value, bool copy = false) const;

        /**
         * Returns a Object by value rather than reference
         * @see JSON::optJSONObject(const std::string&, JSON&, bool)
         * @return value for inline method calls, or Object::Null if key not present
         */
        const Object optJSONObject(const std::string& key, bool copy = false) const
        {
            Object val;
            return optJSONObject(key, val, copy);
        }

        /**
         * @brief add boolean @a value as parameter @a key to the JSON object
         *
         * This method adds a boolean @a value to this object with @a key as the parameter name, if @a key is empty
         * this method throws an JSONException.
         *
         * @param key name of parameter
         * @param value of boolean parameter
         * @return a reference to this JSON object
         */
        Object& put(const std::string& key, bool value);

        /**
         * @brief add double number @a value as parameter @a key to the JSON object
         *
         * This method adds a double number @a value to this object with @a key as the parameter name, if @a key is empty
         * this method throws an JSONException.
         *
         * @param key name of parameter
         * @param value of double number parameter
         * @return a reference to this JSON object
         */
        Object& put(const std::string& key, double value);

        /**
         * @brief add integer number @a value as parameter @a key to the JSON object
         *
         * This method adds a integer number @a value to this object with @a key as the parameter name, if @a key is empty
         * this method throws an JSONException.
         *
         * @param key name of parameter
         * @param value of integer number parameter
         * @return a reference to this JSON object
         */
        template<typename INTTYPE>
        Object& put(const std::string& key, INTTYPE value)
        {

            if(key.empty())
                THROW_MSG(JSONException, "no key provided for integer number value");

            if(JSON::has(key))
                replaceItem<INTTYPE>(key, value);
            else
                addItem<INTTYPE>(key, value);

            return *this;
        }

        /**
         * @brief add JSON array @a value as parameter @a key to the JSON object
         *
         * This method adds a JSON array @a value to this object with @a key as the parameter name, if @a key is empty
         * this method throws an JSONException.
         *
         * @param key name of parameter
         * @param value of array parameter
         * @return a reference to this JSON object
         */
        Object& put(const std::string& key, const Array& value);

        /**
         * @brief add JSON object @a value as parameter @a key to the JSON object
         *
         * This method adds a JSON object @a value to this object with @a key as the parameter name, if @a key is empty
         * this method throws an JSONException.
         *
         * @param key name of parameter
         * @param value of object parameter
         * @return a reference to this JSON object
         */
        Object& put(const std::string& key, const Object& value);

        /**
         * @brief add long number @a value as parameter @a key to the JSON object
         *
         * This method adds a long number @a value to this object with @a key as the parameter name, if @a key is empty
         * this method throws an JSONException.
         *
         * @param key name of parameter
         * @param value of long number parameter
         * @return a reference to this JSON object
         */
        Object& put(const std::string& key, long value);

        /**
         * @brief add string @a value as parameter @a key to the JSON object
         *
         * This method adds a string @a value to this object with @a key as the parameter name, if @a key is empty
         * this method throws an JSONException.
         *
         * @param key name of parameter
         * @param value of string parameter
         * @return a reference to this JSON object
         */
        Object& put(const std::string& key, const std::string& value);

        /**
         * @brief convert const char* to string before insertion
         *
         * @param key name of parameter
         * @param value of string parameter
         * @return a reference to this JSON object
         */
        Object& put(const std::string& key, const char* value);

        /**
         * @brief add a null parameter @a key to the JSON object
         *
         * This method adds a null value to this object with @a key as the parameter name, if @a key is empty
         * this method throws an JSONException. A null parameter is a named parameter with no value. Information
         * is simply deduced from its presence in the object.
         *
         * @param key name of parameter
         * @return a reference to this JSON object
         */
        Object& put(const std::string& key);

        /**
         * @brief Append @a value to the JSON array associated with @a key
         *
         * If @key does not exist in the object, a new key with a type of
         * JSON array is created, then the @a value is appended. This method
         * throws a JSONException if key is empty or the key exists and
         * the currently associated type is not a JSON array.
         *
         * @param key name of array parameter
         * @param value of parameter to be appended to the array as a boolean value
         * @return a reference to this object
         */
        Object& append(const std::string& key, bool value);

        /**
         * @brief Append @a value to the JSON array associated with @a key
         *
         * If @key does not exist in the object, a new key with a type of
         * JSON array is created, then the @a value is appended. This method
         * throws a JSONException if key is empty or the key exists and
         * the currently associated type is not a JSON array.
         *
         * @param key name of array parameter
         * @param value of parameter to be appended to the array as a double number value
         * @return a reference to this object
         */
        Object& append(const std::string& key, double value);

        /**
         * @brief Append @a value to the JSON array associated with @a key
         *
         * If @key does not exist in the object, a new key with a type of
         * JSON array is created, then the @a value is appended. This method
         * throws a JSONException if key is empty or the key exists and
         * the currently associated type is not a JSON array.
         *
         * @param key name of array parameter
         * @param value of parameter to be appended to the array as an integer number value
         * @return a reference to this object
         */
        template<typename INTTYPE>
        Object& append(const std::string& key, INTTYPE value)
        {

            if(key.empty())
                THROW_MSG(JSONException, "no key provided for JSON array");

            Array array;
            if(!JSON::optJSONArray(key, array))
                JSON::addItem(key, dynamic_cast<const JSON&>(array));

            array.append(value);
            return *this;
        }

        /**
         * @brief Append @a value to the JSON array associated with @a key
         *
         * If @key does not exist in the object, a new key with a type of
         * JSON array is created, then the @a value is appended. This method
         * throws a JSONException if key is empty or the key exists and
         * the currently associated type is not a JSON array.
         *
         * @param key name of array parameter
         * @param value of parameter to be appended to the array as a JSON array value
         * @return a reference to this object
         */
        Object& append(const std::string& key, const Array& value);

        /**
         * @brief Append @a value to the JSON array associated with @a key
         *
         * If @key does not exist in the object, a new key with a type of
         * JSON array is created, then the @a value is appended. This method
         * throws a JSONException if key is empty or the key exists and
         * the currently associated type is not a JSON array.
         *
         * @param key name of array parameter
         * @param value of parameter to be appended to the array as a JSON object value
         * @return a reference to this object
         */
        Object& append(const std::string& key, const Object& value);

        /**
         * @brief Append @a value to the JSON array associated with @a key
         *
         * If @key does not exist in the object, a new key with a type of
         * JSON array is created, then the @a value is appended. This method
         * throws a JSONException if key is empty or the key exists and
         * the currently associated type is not a JSON array.
         *
         * @param key name of array parameter
         * @param value of parameter to be appended to the array as a long number value
         * @return a reference to this object
         */
        Object& append(const std::string& key, long value);

        /**
         * @brief Append @a value to the JSON array associated with @a key
         *
         * If @key does not exist in the object, a new key with a type of
         * JSON array is created, then the @a value is appended. This method
         * throws a JSONException if key is empty or the key exists and
         * the currently associated type is not a JSON array.
         *
         * @param key name of array parameter
         * @param value of parameter to be appended to the array as a string value
         * @return a reference to this object
         */
        Object& append(const std::string& key, const std::string& value);

        /**
         * @brief convert const char* to string before insertion
         *
         * @param key name of parameter
         * @param value of string parameter
         * @return a reference to this JSON object
         */
        Object& append(const std::string& key, const char* value);

        /**
         * @brief Append @a value to the JSON array associated with @a key
         *
         * If @key does not exist in the object, a new key with a type of
         * JSON array is created, then the @a value is appended. This method
         * throws a JSONException if key is empty or the key exists and
         * the currently associated type is not a JSON array.
         *
         * @param key name of array parameter
         * @param value of parameter to be appended to the array as an empty (null) value
         * @return a reference to this object
         */
        Object& append(const std::string& key);

        /**
         * @brief remove named item from the JSON object
         *
         * This method removes an item from the object.
         *
         * @return a reference to this JSON object
         */
        Object& remove(const std::string& key);

        /**
         * @brief return true if objects are equal
         *
         * The objects are equal if the contained JSON is equal. A special
         * case is added for null to speed up the comparison
         *
         * @return true if @a other object equals this object
         */
        bool operator==(const Object& other) const;

        /**
         * @brief return true if objects are not equal
         *
         * The objects are not equal if the contained JSON is not equal. A special
         * case is added for null to speed up the comparison
         *
         * @param other
         * @return
         */
        bool operator!=(const Object& other) const;

    };

} /* namespace JSON */
#endif /* JSON__OBJECT_H_ */
