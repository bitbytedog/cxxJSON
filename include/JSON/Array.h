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

#ifndef JSON__ARRAY_H_
#define JSON__ARRAY_H_

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include "JSON.h"

namespace JSON
{

    class Object;

    class Array: public JSON
    {
        friend class Object;

        /**
         * Used to construct a Null JSON object ("")
         */
        Array(JSON::Type type, bool trace = false);


    public:
        /**
         * A Null JSON Array
         */
        static Array Null;

        /**
         * Construct an empty JSON array ("[]")
         */
        Array(bool trace = false);

        /**
         * Construct a JSON array from jsonstr
         *
         * @param jsonstr UTF-8 string representing array
         */
        Array(const std::string& jsonstr, bool trace = false);

        /**
         * Construct a JSON array from jsonstr
         *
         * @param jsonstr UTF-8 string representing array
         */
        Array(const char* jsonstr, bool trace = false);

        /**
         * Construct a JSON array from the contents of a file
         *
         * @param is open input stream of UTF-8 characters
         */
        Array(std::istream& is, bool trace = false);

        /**
         * Destroy a JSON array freeing the
         * memory associated with the parser if
         * and only if the parser is owned by this object
         */
        virtual ~Array();

        /**
         * @see JSON::getType(int)
         */
        Type getType(int index) const;

        /**
         * @see JSON::getBoolean(int)
         */
        bool getBoolean(int index) const;

        /**
         * @see JSON::getDouble(int)
         */
        double getDouble(int index) const;

        /**
         * @see JSON::getInt(int)
         */
        template<typename INTTYPE>
        INTTYPE getInt(int index) const
        {
            return JSON::getInt<INTTYPE>(index);
        }

        /**
         * @see JSON::getLong(int)
         */
        long getLong(int index) const;

        /**
         * @see JSON::getString(int)
         */
        std::string getString(int index) const;

        /**
         * Delegates to JSON.
         * @see JSON::getJSONArray(int, JSON&, bool)
         * @return reference to value for chaining method calls
         */
        Array& getJSONArray(int index, Array& value, bool copy = false) const;

        /**
         * Delegates to JSON.
         * @see JSON::getJSONObject(int, JSON&, bool)
         * @return reference to value for chaining method calls
         */
        Object& getJSONObject(int index, Object& value, bool copy = false) const;

        /**
         * @see JSON::length()
         */
        std::size_t length() const;

        /**
         * @see JSON::isNull(int)
         */
        bool isNull(int index) const;

        /**
         * @see JSON::optBoolean(int)
         */
        bool optBoolean(int index, bool defaultValue = false) const;

        /**
         * @see JSON::optDouble(int)
         */
        double optDouble(int index, double defaultValue = std::nan("json")) const;

        /**
         * @see JSON::optInt(int)
         */
        template<typename INTTYPE>
        INTTYPE optInt(int index, INTTYPE defaultValue = 0) const
        {
            return JSON::optInt<INTTYPE>(index, defaultValue);
        }

        /**
         * @see JSON::optLong(int)
         */
        long optLong(int index, long defaultValue = 0L) const;

        /**
         * @see JSON::optString(int)
         */
        std::string optString(int index, const std::string& defaultValue = "") const;

        /**
         * Delegates to JSON.
         * @see JSON::optJSONArray(int, JSON&, bool)
         * @return reference to value for chaining method calls, or Array::Null if key not present
         */
        Array& optJSONArray(int index, Array& value, bool copy = false) const;

        /**
         * Delegates to JSON.
         * @see JSON::optJSONObject(int, JSON&, bool)
         * @return reference to value for chaining method calls, or Object::Null if key not present
         */
        Object& optJSONObject(int index, Object& value, bool copy = false) const;

        /*
         * @brief add boolean @a value at @a index in the JSON array
         *
         * This method replaces a boolean @a value in this array at @a index, if @a index is greater than or equal to length
         * this method throws an JSONException.
         *
         * @param index of parameter
         * @param value of boolean parameter
         * @return a reference to this JSON array
         */
        Array& put(int index, bool value);

        /**
         * @brief add double @a value at @a index in the JSON array
         *
         * This method replaces a double @a value in this array at @a index, if @a index is greater than or equal to length
         * this method throws an JSONException.
         *
         * @param index of parameter
         * @param value of double parameter
         * @return a reference to this JSON array
         */
        Array& put(int index, double value);

        /**
         * @brief add integer @a value at @a index in the JSON array
         *
         * This method replaces a boolean @a value in this array at @a index, if @a index is greater than or equal to length
         * this method throws an JSONException.
         *
         * @param index of parameter
         * @param value of integer parameter
         * @return a reference to this JSON array
         */
        template<typename INTTYPE>
        Array& put(int index, INTTYPE value)
        {

            if(index < 0 || index >= (int)JSON::length())
                THROW_MSG(JSONException, "index out of bounds for JSON array: [" << index << "]");

            replaceItem<INTTYPE>(index, value);

            return *this;
        }

        /**
         * @brief add JSON array @a value at @a index in the JSON array
         *
         * This method replaces a boolean @a value in this array at @a index, if @a index is greater than or equal to length
         * this method throws an JSONException.
         *
         * @param index of parameter
         * @param value of JSON array parameter
         * @return a reference to this JSON array
         */
        Array& put(int index, const Array& value);

        /**
         * @brief add JSON object @a value at @a index in the JSON array
         *
         * This method replaces a boolean @a value in this array at @a index, if @a index is greater than or equal to length
         * this method throws an JSONException.
         *
         * @param index of parameter
         * @param value of JSON object parameter
         * @return a reference to this JSON array
         */
        Array& put(int index, const Object& value);

        /**
         * @brief add long @a value at @a index in the JSON array
         *
         * This method replaces a boolean @a value in this array at @a index, if @a index is greater than or equal to length
         * this method throws an JSONException.
         *
         * @param index of parameter
         * @param value of long parameter
         * @return a reference to this JSON array
         */
        Array& put(int index, long value);

        /**
         * @brief add string @a value at @a index in the JSON array
         *
         * This method replaces a boolean @a value in this array at @a index, if @a index is greater than or equal to length
         * this method throws an JSONException.
         *
         * @param index of parameter
         * @param value of string parameter
         * @return a reference to this JSON array
         */
        Array& put(int index, const std::string& value);

        /**
         * @brief convert const char* to string before insertion
         *
         * @param index of parameter
         * @param value of string parameter
         * @return a reference to this JSON array
         */
        Array& put(int index, const char* value);

        /**
         * @brief add a null parameter at @a index in the JSON array
         *
         * This method adds a null value in this array at @a index, if @a index is greater than or equal to length
         * this method throws an JSONException. A null parameter is a parameter with no value. Information
         * is simply deduced from its presence in the object.
         *
         * @param index of parameter
         * @return a reference to this JSON object
         */
        Array& put(int index);

        /*
         * @brief put boolean @a value at end of the JSON array
         *
         * This method appends a boolean @a value to this array
         *
         * This method delegates to the equivalent append method
         *
         * @param value of boolean parameter
         * @return a reference to this JSON array
         */
        Array& put(bool value) {
            return append(value);
        }

        /**
         * @brief put double @a value at at the end of the JSON array
         *
         * This method appends a double @a value to this array
         *
         * This method delegates to the equivalent append method
         *
         * @param value of double parameter
         * @return a reference to this JSON array
         */
        Array& put(double value) {
            return append(value);
        }

        /**
         * @brief put integer @a value at at the end of the JSON array
         *
         * This method appends a boolean @a value to this array
         *
         * This method delegates to the equivalent append method
         *
         * @param value of integer parameter
         * @return a reference to this JSON array
         */
        template<typename INTTYPE>
        Array& put(INTTYPE value) {
            return append<INTTYPE>(value);
        }

        /**
         * @brief put JSON array @a value at at the end of the JSON array
         *
         * This method appends a boolean @a value to this array
         *
         * This method delegates to the equivalent append method
         *
         * @param value of JSON array parameter
         * @return a reference to this JSON array
         */
        Array& put(const Array& value) {
            return append(value);
        }

        /**
         * @brief aput JSON object @a value at at the end of the JSON array
         *
         * This method appends a boolean @a value to this array
         *
         * This method delegates to the equivalent append method
         *
         * @param value of JSON object parameter
         * @return a reference to this JSON array
         */
        Array& put(const Object& value) {
            return append(value);
        }

        /**
         * @brief put long @a value at at the end of the JSON array
         *
         * This method appends a boolean @a value to this array
         *
         * This method delegates to the equivalent append method
         *
         * @param value of long parameter
         * @return a reference to this JSON array
         */
        Array& put(long value) {
            return append(value);
        }

        /**
         * @brief put string @a value at at the end of the JSON array
         *
         * This method appends a boolean @a value to this array
         *
         * This method delegates to the equivalent append method
         *
         * @param value of string parameter
         * @return a reference to this JSON array
         */
        Array& put(const std::string& value) {
            return append(value);
        }

        /**
         * @brief convert const char* to string before put at end
         *
         * @param value of string parameter
         * @return a reference to this JSON array
         */
        Array& put(const char* value) {
            return append(value);
        }

        /**
         * @brief put a null parameter at the end of the JSON array
         *
         * This method appends a null value to this array
         * A null parameter is a parameter with no value. Information
         * is simply deduced from its presence in the object.
         *
         * This method delegates to the equivalent append method
         *
         * @return a reference to this JSON object
         */
        Array& put() {
            return append();
        }

        /**
         * @brief append boolean @a value at end of the JSON array
         *
         * This method adds a boolean @a value at the end of this array
         *
         * @param value of boolean parameter
         * @return a reference to this JSON array
         */
        Array& append(bool value);

        /**
         * @brief append double @a value at end of the JSON array
         *
         * This method adds a number @a value at the end of this array
         *
         * @param value of double parameter
         * @return a reference to this JSON array
         */
        Array& append(double value);

        /**
         * @brief append integer @a value at end of the JSON array
         *
         * This method adds a number @a value at the end of this array
         *
         * @param value of integer parameter
         * @return a reference to this JSON array
         */
        template<typename INTTYPE>
        Array& append(INTTYPE value)
        {

            addItem<INTTYPE>(value);

            return *this;
        }

        /**
         * @brief append an array @a value at end of the JSON array
         *
         * This method adds an array @a value at the end of this array
         *
         * @param value of array parameter
         * @return a reference to this JSON array
         */
        Array& append(const Array& value);

        /**
         * @brief append an object @a value at end of the JSON array
         *
         * This method adds an object @a value at the end of this array
         *
         * @param value of object parameter
         * @return a reference to this JSON array
         */
        Array& append(const Object& value);

        /**
         * @brief append long @a value at end of the JSON array
         *
         * This method adds a number @a value at the end of this array
         *
         * @param value of long parameter
         * @return a reference to this JSON array
         */
        Array& append(long value);

        /**
         * @brief append string @a value at end of the JSON array
         *
         * This method adds a string @a value at the end of this array
         *
         * @param value of string parameter
         * @return a reference to this JSON array
         */
        Array& append(const std::string& value);

        /**
         * @brief convert const char* to string before appending
         *
         * @param value of string parameter
         * @return a reference to this JSON array
         */
        Array& append(const char* value);

        /**
         * @brief append a null at end of the JSON array
         *
         * This method adds a null value to the end of the array. A null parameter is a parameter with no value. Information
         * is simply deduced from its presence in the object.
         *
         * @return a reference to this JSON array
         */
        Array& append();

        /**
         * @brief remove item at index in the JSON array
         *
         * This method removes an item from the array, reducing the
         * length by one.
         *
         * @return a reference to this JSON array
         */
        Array& remove(int index);

        /**
         * @brief return true if objects are equal
         *
         * The objects are equal if the contained JSON is equal. A special
         * case is added for null to speed up the comparison
         *
         * @return true if @a other object equals this object
         */
        bool operator==(const Array& other) const;

        /**
         * @brief return true if objects are not equal
         *
         * The objects are not equal if the contained JSON is not equal. A special
         * case is added for null to speed up the comparison
         *
         * @param other
         * @return
         */
        bool operator!=(const Array& other) const;

    };

} /* namespace JSON */
#endif /* JSON__ARRAY_H_ */
