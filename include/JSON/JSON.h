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

#ifndef JSON__JSON_H_
#define JSON__JSON_H_

#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

#include <JSON/Exception.h>
#include <JSON/cJSON/cJSON.h>
#include <JSON/Enumeration.h>

struct cJSON;

namespace JSON
{

#if 0
#define JTRACE JTrace __tracer(this, __FUNCTION__)
#define JLOG(msg) log(__FUNCTION__, msg)
#else
#define JTRACE
#define JLOG(msg)
#endif

    class JSON
    {
        class JTrace
        {
            const JSON* m_self;
            const char* m_func;
        public:
            JTrace(const JSON* self, const char* func) : m_self(self), m_func(func) {
                m_self->log(m_func, "onEntry");
            }
            ~JTrace() {
                m_self->log(m_func, "onExit");
            }
        };

    public:
        enum Type
        {
            JNONE = 0,
            JARRAY,
            JOBJECT,
            JBOOLEAN,
            JSTRING,
            JNUMBER,
            JNULL
        };

    private:
        /**
         * trace memory allocation and chain management to cout
         *
         * If an object is created by copy or assigned
         * the trace attribute is inherited.
         */
        bool m_trace;

        /**
         * print a trace message
         */
        inline void log(const char* func, const char* msg) const
        {
            if(m_trace)
                std::cout << "JSON TRACE self: " << (void*)m_self << " next: " << m_next << " prev: " << m_prev << " node: " << m_node << " root: " << m_root << "[(" << func << "): " << msg << "]" << std::endl;
        }

        /**
         * Object chain to link in all objects that have a reference
         * to a cJSON parse tree. If the node owning the parse tree
         * is deleted, the parse tree ownership will be passed to the
         * next object in the chain.
         */
        JSON* m_next;

        /**
         * Doubly linked list to ease removal of a node
         */
        JSON* m_prev;

        /**
         * Cheat non const pointer used during copy construction
         * to get a not const reference to this object.
         *
         * Every large body of code has a sin and this is it,
         * lose the constness of the node so it can be
         * modified.
         */
        JSON* m_self;

        /**
         * Add this object to the chain after node
         *
         * @param node after which this node is added
         */
        void addSelf(const JSON* node) const;

        /**
         * Remove self from chain, called when the referenced parse tree
         * changes.
         *
         * If this object just references a parse tree, it removes itself
         * from the chain. If it owns the parse tree and there are other nodes
         * in the chain, it transfers the tree to the next node in the chain
         * before removing itself.
         */
        void removeSelf();

        /**
         * The underlying cJSON parser, NULL, if this is not
         * the owning node
         */
        cJSON* m_root;

        /**
         * The node in the parser tree this object references
         */
        cJSON* m_node;

        /**
         * Get the named item
         * <p>
         * A shallow copy should be used for reading or writing the JSON while the parent remains valid,
         * it is quickest. A deep copy should be used if the JSON element is required after the parent
         * object is destroyed.
         *
         * @param key name of item
         * @param optional if false and key is not present throw Exception
         * @param copy perform a deep copy of the JSON node - this node will own the referenced parse tree
         * @return item pointer (maybe 0 if key is not present and optional is true)
         */
        cJSON* getItem(const std::string& key, bool optional = false, bool copy = false) const;

        /**
         * Get the indexed item
         * <p>
         * A shallow copy should be used for reading or writing the JSON while the parent remains valid,
         * it is quickest. A deep copy should be used if the JSON element is required after the parent
         * object is destroyed.
         *
         * @param index of item
         * @param optional if false and key is not present throw Exception
         * @param copy perform a deep copy of the JSON node - this node will own the referenced parse tree
         * @return item pointer (maybe 0 if key is not present and optional is true)
         */
        cJSON* getItem(int index, bool optional = false, bool copy = false) const;

    protected:

        /**
         * Create an empty JSON parse tree of specified type. This can be used
         * to hold key value pairs and generate a JSON UTF8 string.
         * @param type ARRAY or OBJECT
         */
        JSON(Type type, bool trace=false);

        /**
         * Construct a JSON parse tree from the contents of a file
         *
         * @param is open input stream of UTF-8 characters
         */
        JSON(std::istream& is, bool trace = false);

        JSON(const JSON& other);

        virtual ~JSON();

        /**
         * @brief parse a JSON string to create a new parse tree
         *
         * The type of the string will be checked against the type
         * of this object. If they mismatch a Exception will
         * be thrown.
         *
         * @param json a JSON formatted null terminated 'C' string
         */
        void parse(const char* json);

        /**
         * Get the JSON type encapsulated
         * by this object
         * @return type
         */
        Type getType() const;

        /**
         * Get the JSON type of parameter with name in this object
         *
         * @param key name of item
         * @return type of parameter
         */
        virtual Type getType(const std::string& key) const;

        /**
         * Get the JSON type of parameter at index in this array
         *
         * @param index of item
         * @return type of parameter
         */
        virtual Type getType(int index) const;

        /**
         * @brief return true if the parameter @a key is present
         *
         * @param key name of parameter
         * @return true if named parameter is present in object
         */
        virtual bool has(const std::string& key) const;

        /**
         * @brief return number of elements in a JSON array or keys in a JSON Object
         *
         * result is undetermined on a non object type.

         * @return number of elements in the array or keys in an object
         */
        virtual std::size_t length() const;

        /**
         * @brief return true is key has a JSON null value
         *
         * If @a key does not exist false is returned. Key must exist and have
         * a value of JSON null. Use @see bool has(const std::string&) const
         * to detect if a parameter is present or not.
         *
         * @param key name of parameter to check for
         * @return true if @a key exists and is a JSON null
         */
        virtual bool isNull(const std::string& key) const;

        /**
         * @brief return true is key has a JSON null value
         *
         * If @a index does not exist false is returned. Index must exist and have
         * a value of JSON null. Use @see size_t length() const
         * to detect if an index is present or not.
         *
         * @param index of parameter to check for
         * @return true if @a index exists and is a JSON null
         */
        virtual bool isNull(int index) const;

        /**
         * @brief get a JSON boolean parameter as a bool
         *
         * If the parameter @a key is present and a boolean the value of the parameter is returned else
         * this method throws an Exception.
         *
         * @param key name of parameter
         * @return return boolean value of parameter @a key
         */
        virtual bool getBoolean(const std::string& key) const;

        /**
         * @brief get a JSON number parameter as a double
         *
         * If the parameter @a key is present and a number the value of the parameter is returned else
         * this method throws an Exception.
         *
         * @param key name of parameter
         * @return return double value of parameter @a key
         */
        virtual double getDouble(const std::string& key) const;

        /**
         * @brief get a JSON number parameter as a integer
         *
         * If the parameter @a key is present and a number the value of the parameter is returned else
         * this method throws an Exception.
         *
         * @param key name of parameter
         * @return return integer value of parameter @a key
         */
        template<typename INTTYPE>
        INTTYPE getInt(const std::string& key) const
        {
            JTRACE;
            cJSON *elem = getItem(key);

            switch(elem->type)
            {
            case cJSON_Number:
                return static_cast<INTTYPE>(elem->valueint);
            }
            THROW_MSG(Exception, "JSON element is not a number: {\"" << key << "\"}");
            /* notreached */
            return static_cast<INTTYPE>(0);
        }

        /**
         * @brief get a JSON number parameter as a long
         *
         * If the parameter @a key is present and a number the value of the parameter is returned else
         * this method throws an Exception.
         *
         * @param key name of parameter
         * @return return long value of parameter @a key
         */
        virtual long getLong(const std::string& key) const;

        /**
         * @brief get a JSON string parameter as a string
         *
         * If the parameter @a key is present and a string the value of the parameter is returned.
         * If the parameter is present and is not a string, the json value of the element is returned.
         * If the key is not present this method throws an Exception.
         *
         * @param key name of parameter
         * @return return string value of parameter @a key
         */
        virtual std::string getString(const std::string& key) const;

        /**
         * @brief get a JSON array parameter as an JSONArray
         *
         * If the parameter @a key is present and an array the value of the parameter is returned else
         * this method throws an Exception.
         * <p>
         * A shallow copy should be used for reading or writing the JSON while the parent remains valid,
         * it is quickest. A deep copy should be used if the JSON element is required after the parent
         * object is destroyed.
         *
         * @param key name of parameter
         * @param value JSON array value will be returned in
         * @param copy perform a deep copy of the JSON node - this node will own the referenced parse tree
         */
        void getJSONArray(const std::string& key, JSON& value, bool copy = false) const;

        /**
         * @brief get a JSON object parameter as an JSONObject
         *
         * If the parameter @a key is present and an object the value of the parameter is returned else
         * this method throws an Exception.
         * <p>
         * A shallow copy should be used for reading or writing the JSON while the parent remains valid,
         * it is quickest. A deep copy should be used if the JSON element is required after the parent
         * object is destroyed.
         *
         * @param key name of parameter
         * @param value JSON object value will be returned in
         * @param copy perform a deep copy of the JSON node - this node will own the referenced parse tree
         */
        void getJSONObject(const std::string& key, JSON& value, bool copy = false) const;

        /**
         * @brief get optional JSON boolean parameter as a bool returning false if it is not present
         *
         * If the parameter @a key is present and is a boolean the value of the parameter is returned else
         * false is returned.
         *
         * @param key name of parameter
         * @param defaultValue returned if no key found
         * @return return boolean value of parameter @a key if present else return defaultValue
         */
        bool optBoolean(const std::string& key, bool defaultValue) const;

        /**
         * @brief get optional JSON number parameter as a double returning 0 if it is not present
         *
         * If the parameter @a key is present and is a number the value of the parameter is returned as a double else
         * 0 is returned.
         *
         * @param key name of parameter
         * @param defaultValue returned if no key found
         * @return return double value of parameter @a key if present else return defaultValue
         */
        double optDouble(const std::string& key, double defaultValue) const;

        /**
         * @brief get optional JSON number parameter as an integer returning 0 if it is not present
         *
         * If the parameter @a key is present and is a number the value of the parameter is returned as an integer else
         * 0 is returned.
         *
         * @param key name of parameter
         * @param defaultValue returned if no key found
         * @return return integer value of parameter @a key if present else return defaultValue
         */
        template<typename INTTYPE>
        INTTYPE optInt(const std::string& key, INTTYPE defaultValue) const
        {
            JTRACE;
            cJSON *elem = getItem(key, true);
            if(0 == elem)
                return defaultValue;

            switch(elem->type)
            {
            case cJSON_Number:
                return static_cast<INTTYPE>(elem->valueint);
            }
            return defaultValue;
        }

        /**
         * @brief get optional JSON number parameter as a long returning 0 if it is not present
         *
         * If the parameter @a key is present and is a number the value of the parameter is returned as a long else
         * 0 is returned.
         *
         * @param key name of parameter
         * @param defaultValue returned if no key found
         * @return return long value of parameter @a key if present else return defaultValue
         */
        long optLong(const std::string& key, long defaultValue) const;

        /**
         * @brief get optional JSON string parameter as a string returning an empty string if it is not present
         *
         * If the parameter @a key is present and is a string the value of the parameter is returned.
         * If the parameter is present and is not a string, the json value of the element is returned.
         * If the key is not present an empty string is returned.
         *
         * @param key name of parameter
         * @param defaultValue returned if no key found
         * @return return string value of parameter @a key if present else return defaultValue
         */
        std::string optString(const std::string& key, const std::string& defaultValue) const;

        /**
         * @brief get optional JSON array parameter as a JSONArray returning JSONArray::Null if it is not present
         *
         * If the parameter @a key is present and is an array the value of the parameter is returned else
         * JSONArray::Null is returned.
         * <p>
         * A shallow copy should be used for reading or writing the JSON while the parent remains valid,
         * it is quickest. A deep copy should be used if the JSON element is required after the parent
         * object is destroyed.
         *
         * @param key name of parameter
         * @param value JSON array value will be returned in
         * @param copy perform a deep copy of the JSON node - this node will own the referenced parse tree
         * @return return true if key is present
         */
        bool optJSONArray(const std::string& key, JSON& value, bool copy = false) const;

        /**
         * @brief get optional JSON object parameter as a JSONObject returning JSONObject::Null if it is not present
         *
         * If the parameter @a key is present and is an object the value of the parameter is returned else
         * JSONObject::Null is returned.
         * <p>
         * A shallow copy should be used for reading or writing the JSON while the parent remains valid,
         * it is quickest. A deep copy should be used if the JSON element is required after the parent
         * object is destroyed.
         *
         * @param key name of parameter
         * @param value JSON object value will be returned in
         * @param copy perform a deep copy of the JSON node - this node will own the referenced parse tree
         * @return return true if key is present
         */
        bool optJSONObject(const std::string& key, JSON& value, bool copy = false) const;

        /**
         * @brief get a JSON boolean parameter as a bool
         *
         * If the parameter @a index is present and a boolean the value of the parameter is returned else
         * this method throws an Exception.
         *
         * @param index of parameter
         * @return return boolean value of parameter at @a index
         */
        virtual bool getBoolean(int index) const;

        /**
         * @brief get a JSON number parameter as a double
         *
         * If the parameter @a index is present and a number the value of the parameter is returned else
         * this method throws an Exception.
         *
         * @param index of parameter
         * @return return double value of parameter at @a index
         */
        virtual double getDouble(int index) const;

        /**
         * @brief get a JSON number parameter as an int
         *
         * If the parameter @a index is present and a number the value of the parameter is returned else
         * this method throws an Exception.
         *
         * @param index of parameter
         * @return return the int value of parameter at @a index
         */
        template<typename INTTYPE>
        INTTYPE getInt(int index) const
        {
            JTRACE;
            cJSON *elem = getItem(index);

            switch(elem->type)
            {
            case cJSON_Number:
                return static_cast<INTTYPE>(elem->valueint);
            }
            THROW_MSG(Exception, "JSON element is not a number: [" << index << "]");
            /* notreached */
            return static_cast<INTTYPE>(0);
        }

        /**
         * @brief get a JSON number parameter as a long
         *
         * If the parameter @a index is present and a number the value of the parameter is returned else
         * this method throws an Exception.
         *
         * @param index of parameter
         * @return return long value of parameter at @a index
         */
        virtual long getLong(int index) const;

        /**
         * @brief get a JSON string parameter as a string
         *
         * If the parameter @a index is present and a string the value of the parameter is returned.
         * If the parameter is present and is not a string, the json value of the element is returned.
         * If the index is not present this method throws an Exception.
         *
         * @param index of parameter
         * @return return the string value of parameter at @a index
         */
        virtual std::string getString(int index) const;

        /**
         * @brief get a JSON array parameter as an JSONArray
         *
         * If the parameter @a index is present and an array the value of the parameter is returned else
         * this method throws an Exception.
         * <p>
         * A shallow copy should be used for reading or writing the JSON while the parent remains valid,
         * it is quickest. A deep copy should be used if the JSON element is required after the parent
         * object is destroyed.
         *
         * @param index of parameter
         * @param value JSON array value will be returned in
         * @param copy perform a deep copy of the JSON node - this node will own the referenced parse tree
         */
        void getJSONArray(int index, JSON& value, bool copy = false) const;

        /**
         * @brief get a JSON object parameter as an JSONObject
         *
         * If the parameter @a index is present and an object the value of the parameter is returned else
         * this method throws an Exception.
         * <p>
         * A shallow copy should be used for reading or writing the JSON while the parent remains valid,
         * it is quickest. A deep copy should be used if the JSON element is required after the parent
         * object is destroyed.
         *
         * @param index of parameter
         * @param value JSON object value will be returned in
         * @param copy perform a deep copy of the JSON node - this node will own the referenced parse tree
         */
        void getJSONObject(int index, JSON& value, bool copy = false) const;

        /**
         * @brief get optional JSON boolean parameter as a bool returning false if it is not present
         *
         * If the parameter @a index is present and is a boolean the value of the parameter is returned else
         * false is returned.
         *
         * @param index of parameter
         * @param defaultValue returned if no index out of range
         * @return return boolean value of parameter @a index if present else return defaultValue
         */
        bool optBoolean(int index, bool defaultValue) const;

        /**
         * @brief get optional JSON number parameter as a double returning 0 if it is not present
         *
         * If the parameter @a index is present and is a number the value of the parameter is returned as a double else
         * 0 is returned.
         *
         * @param index of parameter
         * @param defaultValue returned if no index out of range
         * @return return double value of parameter @a index if present else return defaultValue
         */
        double optDouble(int index, double defaultValue) const;

        /**
         * @brief get optional JSON number parameter as an integer returning 0 if it is not present
         *
         * If the parameter @a index is present and is a number the value of the parameter is returned as an integer else
         * 0 is returned.
         *
         * @param index of parameter
         * @param defaultValue returned if no index out of range
         * @return return integer value of parameter @a index if present else return defaultValue
         */
        template<typename INTTYPE>
        INTTYPE optInt(int index, INTTYPE defaultValue) const
        {
            JTRACE;
            cJSON *elem = getItem(index, true);
            if(0 == elem)
                return defaultValue;

            switch(elem->type)
            {
            case cJSON_Number:
                return static_cast<INTTYPE>(elem->valueint);
            }
            return defaultValue;
        }

        /**
         * @brief get optional JSON number parameter as a long returning 0 if it is not present
         *
         * If the parameter @a index is present and is a number the value of the parameter is returned as a long else
         * 0 is returned.
         *
         * @param index of parameter
         * @param defaultValue returned if no index out of range
         * @return return long value of parameter @a index if present else return defaultValue
         */
        long optLong(int index, long defaultValue) const;

        /**
         * @brief get optional JSON string parameter as a string returning an empty string if it is not present
         *
         * If the parameter @a index is present and is a string the value of the parameter is returned.
         * If the parameter is present and is not a string, the json value of the element is returned.
         * If the index is not present an empty string is returned.
         *
         * @param index of parameter
         * @param defaultValue returned if no index out of range
         * @return return string value of parameter @a index if present else return defaultValue
         */
        std::string optString(int index, const std::string& defaultValue) const;

        /**
         * @brief get optional JSON array parameter as a JSONArray returning JSONArray::Null if it is not present
         *
         * If the parameter @a index is present and is an array the value of the parameter is returned else
         * JSONArray::Null is returned.
         * <p>
         * A shallow copy should be used for reading or writing the JSON while the parent remains valid,
         * it is quickest. A deep copy should be used if the JSON element is required after the parent
         * object is destroyed.
         *
         * @param index of parameter
         * @param value JSON array value will be returned in
         * @param copy perform a deep copy of the JSON node - this node will own the referenced parse tree
         * @return return true if index is valid
         */
        bool optJSONArray(int index, JSON& value, bool copy = false) const;

        /**
         * @brief get optional JSON object parameter as a JSONObject returning JSONObject::Null if it is not present
         *
         * If the parameter @a index is present and is an object the value of the parameter is returned else
         * JSONObject::Null is returned.
         * <p>
         * A shallow copy should be used for reading or writing the JSON while the parent remains valid,
         * it is quickest. A deep copy should be used if the JSON element is required after the parent
         * object is destroyed.
         *
         * @param index of parameter
         * @param value JSON object value will be returned in
         * @param copy perform a deep copy of the JSON node - this node will own the referenced parse tree
         * @return return true if index is valid
         */
        bool optJSONObject(int index, JSON& value, bool copy = false) const;

        /**
         * Add a boolean item to this JSON object.
         *
         * @param key name to add to object
         * @param value boolean
         */
        void addItem(const std::string& key, bool value);

        /**
         * Add a double item to this JSON object.
         *
         * @param key name to add to object
         * @param value double
         */
        void addItem(const std::string& key, double value);

        /**
         * Add a integer item to this JSON object.
         *
         * @param key name to add to object
         * @param value integer
         */
        template<typename INTTYPE>
        void addItem(const std::string& key, INTTYPE value)
        {
            JTRACE;

            if(0 == m_node)
                THROW_MSG(Exception, "JSON node NULL");

            if(m_node->type != cJSON_Object)
                THROW_MSG(Exception, "expected JSON object");

            cJSON_AddNumberToObject(m_node, key.c_str(), static_cast<int64_t>(value));
        }

        /**
         * Add a long item to this JSON object.
         *
         * @param key name to add to object
         * @param value long
         */
        void addItem(const std::string& key, long value);

        /**
         * Add a string item to this JSON object.
         *
         * @param key name to add to object
         * @param value string
         */
        void addItem(const std::string& key, const std::string& value);

        /**
         * Add an aggregate item to this JSON object. This JSON object will take
         * ownership of the underlying cJSON object tree.
         *
         * @param key name of array or object
         * @param value JSON array or object
         */
        void addItem(const std::string& key, const JSON& value);

        /**
         * Add a null item to this JSON object.
         *
         * @param key name to add to object
         */
        void addNullItem(const std::string& key);

        /**
         * Append a boolean item to this JSON array.
         *
         * @param value boolean
         */
        void addItem(bool value);

        /**
         * Append a double item to this JSON array.
         *
         * @param value double
         */
        void addItem(double value);

        /**
         * Append a integer item to this JSON array.
         *
         * @param value integer
         */
        template<typename INTTYPE>
        void addItem(INTTYPE value)
        {
            JTRACE;

            if(0 == m_node)
                THROW_MSG(Exception, "JSON node NULL");

            if(m_node->type != cJSON_Array)
                THROW_MSG(Exception, "expected JSON array");

            cJSON* elem = cJSON_CreateNumber(static_cast<int64_t>(value));
            if(0 == elem)
                THROW_MSG(Exception, "no memory while allocating JSON element for appending");

            cJSON_AddItemToArray(m_node, elem);
        }

        /**
         * Append a long item to this JSON array.
         *
         * @param value long
         */
        void addItem(long value);

        /**
         * Append a string item to this JSON array.
         *
         * @param value string
         */
        void addItem(const std::string& value);

        /**
         * Append an aggregate item to this JSON array. This JSON object will take
         * ownership of the underlying cJSON object tree.
         *
         * @param value JSON array or object
         */
        void addItem(const JSON& value);

        /**
         * Append a null item to this JSON array.
         */
        void addItem();

        /**
         * Replace a boolean item in this JSON array.
         *
         * @param key name of value
         * @param value boolean
         */
        void replaceItem(const std::string& key, bool value);

        /**
         * Replace a double number item in this JSON array.
         *
         * @param key name of value
         * @param value double number
         */
        void replaceItem(const std::string& key, double value);

        /**
         * Replace an integer number item in this JSON array.
         *
         * @param key name of value
         * @param value integer number
         */
        template<typename INTTYPE>
        void replaceItem(const std::string& key, INTTYPE value)
        {
            JTRACE;

            if(0 == m_node)
                THROW_MSG(Exception, "JSON node NULL");

            if(m_node->type != cJSON_Object)
                THROW_MSG(Exception, "expected JSON object");

            cJSON* elem = cJSON_CreateNumber(static_cast<int64_t>(value));
            if(0 == elem)
                THROW_MSG(Exception, "no memory while allocating JSON object element: {\"" << key << "\"}");

            cJSON_ReplaceItemInObject(m_node, key.c_str(), elem);
        }


        /**
         * Replace an integer number item in this JSON array.
         *
         * @param key name of value
         * @param value integer number
         */
        void replaceItem(const std::string& key, std::int32_t value);

        /**
         * Replace an integer number item in this JSON array.
         *
         * @param key name of value
         * @param value integer number
         */
        void replaceItem(const std::string& key, std::int64_t value);

        /**
         * Replace an long integer number item in this JSON array.
         *
         * @param key name of value
         * @param value long integer number
         */
        void replaceItem(const std::string& key, long value);

        /**
         * Replace an string item in this JSON array.
         *
         * @param key name of value
         * @param value string
         */
        void replaceItem(const std::string& key, const std::string& value);

        /**
         * Replace a null item in this JSON array.
         *
         * @param key name of value
         */
        void replaceNullItem(const std::string& key);

        /**
         * Replace the value associated with key in this JSON object. This JSON object will take
         * ownership of the underlying cJSON object tree.
         *
         * @param key name of value
         * @param value new JSON array or object to associate with key
         */
        void replaceItem(const std::string& key, const JSON& value);

        /**
         * Replace a boolean item in this JSON array.
         *
         * @param index of array or object
         * @param value boolean
         */
        void replaceItem(int index, bool value);

        /**
         * Replace a double number item in this JSON array.
         *
         * @param index of array or object
         * @param value double number
         */
        void replaceItem(int index, double value);

        /**
         * Replace an integer number item in this JSON array.
         *
         * @param index of array or object
         * @param value integer number
         */
        template<typename INTTYPE>
        void replaceItem(int index, INTTYPE value)
        {
            JTRACE;

            if(0 == m_node)
                THROW_MSG(Exception, "JSON node NULL");

            if(m_node->type != cJSON_Array)
                THROW_MSG(Exception, "expected JSON array");

            cJSON* elem = cJSON_CreateNumber(static_cast<int64_t>(value));
            if(0 == elem)
                THROW_MSG(Exception, "no memory while allocating JSON array element: [" << index << "]");

            cJSON_ReplaceItemInArray(m_node, index, elem);
        }


        /**
         * Replace an long integer number item in this JSON array.
         *
         * @param index of array or object
         * @param value long integer number
         */
        void replaceItem(int index, long value);

        /**
         * Replace an string item in this JSON array.
         *
         * @param index of array or object
         * @param value string
         */
        void replaceItem(int index, const std::string& value);

        /**
         * Replace a null item in this JSON array.
         *
         * @param index of array or object
         */
        void replaceItem(int index);

        /**
         * Replace an aggregate item in this JSON array. This JSON object will take
         * ownership of the underlying cJSON object tree.
         *
         * @param index of array or object
         * @param value JSON array or object
         */
        void replaceItem(int index, const JSON& value);

        /**
         * @brief remove the key from the object
         *
         * The first key found and the value are removed from the
         * cJSON object tree and all memory associated with the node
         * is freed
         *
         * @param key of item to be removed
         */
        void removeItem(const std::string& key);

        /**
         * @brief remove the index from the array
         *
         * The index and the value are removed from the
         * cJSON array and all memory associated with the node
         * is freed
         *
         * @param index of item to be removed
         */
        void removeItem(int index);

        JSON& operator=(const JSON& rhs);

    public:

        /**
         * Create a JSON object of type null
         *
         * This is a primitive JSON type and cannot be extended
         */
        JSON(bool trace = false);

        /**
         * Create a JSON object of type bool
         *
         * This is a primitive JSON type and cannot be extended
         */
        JSON(bool val, bool trace = false);

        /**
         * Create a JSON object of type number from integer
         *
         * This is a primitive JSON type and cannot be extended
         */
        template<typename INTTYPE>
        JSON(INTTYPE val, bool trace = false)
            : m_trace(trace)
            , m_next(0)
            , m_prev(0)
            , m_self(this)
            , m_root(cJSON_CreateNumber(val))
            , m_node(m_root)
        {
            JTRACE;
            JLOG("constructed with cJSON_CreateNumber");
        }

        /**
         * Create a JSON object of type number from double
         *
         * This is a primitive JSON type and cannot be extended
         */
        JSON(double val, bool trace = false);

        /**
         * Used to create a JSON parse tree with a received
         * JSON UTF8 string. May be any valid JSON type.
         *
         * @param jsonstr JSON formatted UTF8 string
         */
        JSON(const std::string& val, bool trace = false);

        /**
         * Used to create a JSON parse tree with a received
         * JSON UTF8 string. May be any valid JSON type.
         *
         * @param jsonstr JSON formatted UTF8 string
         */
        JSON(const char* val, bool trace = false);

        /**
         * Used to create a JSON parse tree with a received
         * JSON UTF8 string. May be any valid JSON type.
         *
         * @param jsonstr JSON formatted UTF8 string
         */
        JSON(char* val, bool trace = false);

        /**
         * Convert to compressed JSON (no whitespace)
         *
         * @return a compressed JSON UTF8 string
         */
        std::string toString() const;

        /**
         * Pretty print to stream
         *
         * @param os output stream
         * @param obj to print
         * @return output stream
         */
        friend std::ostream& operator<<(std::ostream& os, const JSON& obj);

        /**
         * @brief get an enumerator to enumerate the children of an object or array
         *
         * TODO: convert from a specific class to a parametised Enumeration<> template
         *
         * @return an Enumeration initialised to the first element in the container
         */
        Enumeration keys() const;

        /**
         * @brief is this object empty
         *
         * @return true is there is no parse tree
         */
        bool isEmpty() const;

        /**
         * @brief is this object a null
         *
         * @return true is this object is a JSON null
         */
        bool isNull() const;
    };

} /* namespace JSON */
#endif /* JSON__JSON_H_ */
