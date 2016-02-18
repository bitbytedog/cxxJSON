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
#include <cassert>
#include <JSON/JSON.h>

namespace JSON
{

    using namespace std;

    JSON::JSON(bool trace)
        : m_trace(trace)
        , m_next(0)
        , m_prev(0)
        , m_self(this)
        , m_root(cJSON_CreateNull())
        , m_node(m_root)
    {
        JTRACE;
        JLOG("constructed with cJSON_CreateNull");
    }

    JSON::JSON(bool val, bool trace)
        : m_trace(trace)
        , m_next(0)
        , m_prev(0)
        , m_self(this)
        , m_root(cJSON_CreateBool(val))
        , m_node(m_root)
    {
        JTRACE;
        JLOG("constructed with cJSON_CreateBool");
    }

    JSON::JSON(double val, bool trace)
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

    JSON::JSON(Type type, bool trace)
        : m_trace(trace)
        , m_next(0)
        , m_prev(0)
        , m_self(this)
        , m_root(0)
        , m_node(0)
    {
        JTRACE;
        switch(type)
        {
        case JNONE:
            /*
             * This is an empty JSON object i.e. no value as opposed to a null value
             */
            break;
        case JARRAY:
            JLOG("constructed with cJSON_CreateArray");
            m_root = cJSON_CreateArray();
            break;
        case JOBJECT:
            JLOG("constructed with cJSON_CreateObject");
            m_root = cJSON_CreateObject();
            break;
        case JBOOLEAN:
            JLOG("constructed with cJSON_CreateBool");
            m_root = cJSON_CreateBool(false);
            break;
        case JSTRING:
            JLOG("constructed with cJSON_CreateString");
            m_root = cJSON_CreateString("");
            break;
        case JNUMBER:
            JLOG("constructed with cJSON_CreateNumber");
            m_root = cJSON_CreateNumber(0);
            break;
        case JNULL:
            JLOG("constructed with cJSON_CreateNull");
            m_root = cJSON_CreateNull();
            break;
        default:
            THROW_MSG(Exception, "unexpected type for this constructor");
        }
        m_node = m_root;
    }

    JSON::JSON(const string& val, bool trace)
        : m_trace(trace)
        , m_next(0)
        , m_prev(0)
        , m_self(this)
        , m_root(val.empty() ? cJSON_CreateNull() : cJSON_Parse(val.c_str()))
        , m_node(m_root)
    {
        JTRACE;
        /*
         * cJSON supports a method cJSON_GetErrorPtr(), but this relies on a global
         * const char* ep in cJSON.c - therefore it is not thread safe, we will ignore this
         * function.
         */
        if(0 == m_root)
            THROW_MSG(Exception, "parse error: " << val);

        JLOG("constructed with cJSON_Parse");
    }

    JSON::JSON(const char* val, bool trace)
        : m_trace(trace)
        , m_next(0)
        , m_prev(0)
        , m_self(this)
        , m_root(val == NULL || *val == '\0' ? cJSON_CreateNull() : cJSON_Parse(val))
        , m_node(m_root)
    {
        JTRACE;
        /*
         * cJSON supports a method cJSON_GetErrorPtr(), but this relies on a global
         * const char* ep in cJSON.c - therefore it is not thread safe, we will ignore this
         * function.
         */
        if(0 == m_root)
            THROW_MSG(Exception, "parse error: " << val);

        JLOG("constructed with cJSON_Parse");
    }

    JSON::JSON(char* val, bool trace)
        : m_trace(trace)
        , m_next(0)
        , m_prev(0)
        , m_self(this)
        , m_root(val == NULL || *val == '\0' ? cJSON_CreateNull() : cJSON_Parse(val))
        , m_node(m_root)
    {
        JTRACE;
        /*
         * cJSON supports a method cJSON_GetErrorPtr(), but this relies on a global
         * const char* ep in cJSON.c - therefore it is not thread safe, we will ignore this
         * function.
         */
        if(0 == m_root)
            THROW_MSG(Exception, "parse error: " << val);

        JLOG("constructed with cJSON_Parse");
    }


    JSON::JSON(istream& is, bool trace)
        : m_trace(trace)
        , m_next(0)
        , m_prev(0)
        , m_self(this)
    {
        /*
         * read in the string and parse, we don't assume
         * the istream has a fixed length, for example it
         * could be a network socket
         */
        std::string val;
        char buf[4096];
        while (is.read(buf, sizeof(buf)))
            val.append(buf, sizeof(buf));
        val.append(buf, is.gcount());
        m_root = val.empty() ? cJSON_CreateNull() : cJSON_Parse(val.c_str());
        m_node = m_root;
    }

    JSON::JSON(const JSON& other)
        : m_trace(other.m_trace)
        , m_next(0)
        , m_prev(0)
        , m_self(this)
        , m_root(0)
        , m_node(other.m_node)
    {
        JTRACE;

        addSelf(&other);
    }

    JSON::~JSON()
    {
        JTRACE;

        removeSelf();
    }

    void
    JSON::addSelf(const JSON* node) const
    {
        JTRACE;

        /*
         * insert in chain after node
         */
        m_self->m_next = node->m_next;
        m_self->m_prev = node->m_self;

        m_prev->m_next = this->m_self;
        if(m_next)
            m_next->m_prev = this->m_self;
    }

    void
    JSON::removeSelf()
    {
        JTRACE;

        if(m_root)
        {
            /*
             * Only the head node can own a parse tree
             */
            assert(m_prev == 0);

            if(m_next)
            {
                m_next->m_root = m_root;
                m_next->m_prev = 0;
            }
            else
            {
                JLOG("calling cJSON_Delete");
                cJSON_Delete(m_root);
            }
            m_root = 0;
        }
        else
        {
            if(m_next)
                m_next->m_prev = m_prev;
            if(m_prev)
                m_prev->m_next = m_next;
        }
        m_next = 0;
        m_prev = 0;
        m_node = 0;
    }

    void
    JSON::parse(const char* json)
    {
        JTRACE;

        if(m_node)
            removeSelf();

        JLOG("calling cJSON_Parse");
        m_root = cJSON_Parse(json);
        m_node = m_root;
    }

    cJSON*
    JSON::getItem(const string& key, bool optional, bool copy) const
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Object)
            THROW_MSG(Exception, "expected JSON object");

        cJSON *elem = cJSON_GetObjectItem(m_node, key.c_str());;
        if(0 == elem && !optional)
            THROW_MSG(Exception, "mandatory JSON object element missing: " << key);

        if(copy)
        {
            char* json = cJSON_PrintUnformatted(elem);
            elem = cJSON_Parse(json);
            cJSON_free(json);
        }
        return elem;
    }

    cJSON*
    JSON::getItem(int index, bool optional, bool copy) const
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Array)
            THROW_MSG(Exception, "expected JSON array");

        cJSON *elem = cJSON_GetArrayItem(m_node, index);
        if(0 == elem && !optional)
            THROW_MSG(Exception, "mandatory JSON array index missing: " << index);

        if(copy)
        {
            char* json = cJSON_PrintUnformatted(elem);
            elem = cJSON_Parse(json);
            cJSON_free(json);
        }
        return elem;
    }

    JSON::Type
    JSON::getType() const
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        switch(m_node->type)
        {
        case cJSON_False:
        case cJSON_True:
            return JBOOLEAN;
        case cJSON_NULL:
            return JNULL;
        case cJSON_Number:
            return JNUMBER;
        case cJSON_String:
            return JSTRING;
        case cJSON_Array:
            return JARRAY;
        case cJSON_Object:
            return JOBJECT;
        }
        /*
         * shouldn't get here, but all JSON data types are UTF-8 strings
         */
        return JSTRING;
    }

    JSON::Type
    JSON::getType(const std::string& key) const
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Object)
            THROW_MSG(Exception, "expected JSON object");

        cJSON *elem = getItem(key.c_str(), true);

        if(0 == elem)
            return JNONE;

        switch(elem->type)
        {
        case cJSON_False:
        case cJSON_True:
            return JBOOLEAN;
        case cJSON_NULL:
            return JNULL;
        case cJSON_Number:
            return JNUMBER;
        case cJSON_String:
            return JSTRING;
        case cJSON_Array:
            return JARRAY;
        case cJSON_Object:
            return JOBJECT;
        }
        /*
         * shouldn't get here, but all JSON data types are UTF-8 strings
         */
        return JSTRING;
    }

    JSON::Type
    JSON::getType(int index) const
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Array)
            THROW_MSG(Exception, "expected ARRAY object");

        cJSON *elem = getItem(index, true);

        if(0 == elem)
            return JNONE;

        switch(elem->type)
        {
        case cJSON_False:
        case cJSON_True:
            return JBOOLEAN;
        case cJSON_NULL:
            return JNULL;
        case cJSON_Number:
            return JNUMBER;
        case cJSON_String:
            return JSTRING;
        case cJSON_Array:
            return JARRAY;
        case cJSON_Object:
            return JOBJECT;
        }
        /*
         * shouldn't get here, but all JSON data types are UTF-8 strings
         */
        return JSTRING;
    }

    bool
    JSON::has(const std::string& key) const
    {
        JTRACE;
        cJSON *elem = getItem(key, true);

        return 0 != elem;
    }

    size_t
    JSON::length() const
    {
        JTRACE;
        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        return cJSON_GetArraySize(m_node);
    }

    bool
    JSON::isNull(const std::string& key) const
    {
        JTRACE;
        cJSON *elem = getItem(key, true);

        return 0 != elem && cJSON_NULL == elem->type;
    }

    bool
    JSON::isNull(int index) const
    {
        JTRACE;
        cJSON *elem = getItem(index, true);

        return 0 != elem && cJSON_NULL == elem->type;
    }

    bool
    JSON::getBoolean(const std::string& key) const
    {
        JTRACE;
        cJSON *elem = getItem(key);

        switch(elem->type)
        {
        case cJSON_False:
            return false;
        case cJSON_True:
            return true;
        }
        THROW_MSG(Exception, "JSON element is not boolean: {\"" << key << "\"}");
        /* notreached */
        return false;
    }

    double
    JSON::getDouble(const std::string& key) const
    {
        JTRACE;
        cJSON *elem = getItem(key);

        switch(elem->type)
        {
        case cJSON_Number:
            return elem->valuedouble;
        }
        THROW_MSG(Exception, "JSON element is not a number: {\"" << key << "\"}");
        /* notreached */
        return 0;
    }

    long
    JSON::getLong(const std::string& key) const
    {
        JTRACE;
        cJSON *elem = getItem(key);

        switch(elem->type)
        {
        case cJSON_Number:
            return (long)elem->valueint;
        }
        THROW_MSG(Exception, "JSON element is not a number: {\"" << key << "\"}");
        /* notreached */
        return 0;
    }

    std::string
    JSON::getString(const std::string& key) const
    {
        JTRACE;
        cJSON *elem = getItem(key);

        switch(elem->type)
        {
        case cJSON_String:
            return elem->valuestring;
        }
        char* json = cJSON_PrintUnformatted(elem);
        string jsonstring = json;
        cJSON_free(json);
        return jsonstring;
    }

    void
    JSON::getJSONArray(const std::string& key, JSON& value, bool copy) const
    {
        JTRACE;
        cJSON *elem = getItem(key, false, copy);

        if(elem->type != cJSON_Array)
        {
            if(copy)
                cJSON_Delete(elem);

            THROW_MSG(Exception, "JSON element is not an array: {\"" << key << "\"}");
        }

        value.removeSelf();
        value.m_node = elem;
        if(copy)
            value.m_root = elem;
        else
            value.addSelf(this);
    }

    void
    JSON::getJSONObject(const std::string& key, JSON& value, bool copy) const
    {
        JTRACE;
        cJSON *elem = getItem(key, false, copy);

        if(elem->type != cJSON_Object)
        {
            if(copy)
                cJSON_Delete(elem);

            THROW_MSG(Exception, "JSON element is not an object: {\"" << key << "\"}");
        }

        value.removeSelf();
        value.m_node = elem;
        if(copy)
            value.m_root = elem;
        else
            value.addSelf(this);
    }

    bool
    JSON::optBoolean(const std::string& key, bool defaultValue) const
    {
        JTRACE;
        cJSON *elem = getItem(key, true);
        if(0 == elem)
            return defaultValue;

        switch(elem->type)
        {
        case cJSON_True:
            return true;
        case cJSON_False:
            return false;
        }
        return false;
    }

    double
    JSON::optDouble(const std::string& key, double defaultValue) const
    {
        JTRACE;
        cJSON *elem = getItem(key, true);
        if(0 == elem)
            return defaultValue;

        switch(elem->type)
        {
        case cJSON_Number:
            return elem->valuedouble;
        }
        return defaultValue;
    }

    long
    JSON::optLong(const std::string& key, long defaultValue) const
    {
        JTRACE;
        cJSON *elem = getItem(key, true);
        if(0 == elem)
            return defaultValue;

        switch(elem->type)
        {
        case cJSON_Number:
            return (long)elem->valueint;
        }
        return defaultValue;
    }

    std::string
    JSON::optString(const std::string& key, const std::string& defaultValue) const
    {
        JTRACE;
        cJSON *elem = getItem(key, true);
        if(0 == elem)
            return defaultValue;

        switch(elem->type)
        {
        case cJSON_String:
            return elem->valuestring;
        }
        char* json = cJSON_PrintUnformatted(elem);
        string jsonstring = json;
        cJSON_free(json);
        return jsonstring;
    }

    bool
    JSON::optJSONArray(const std::string& key, JSON& value, bool copy) const
    {
        JTRACE;
        cJSON *elem = getItem(key, true, copy);
        if(0 != elem)
        {
            switch(elem->type)
            {
            case cJSON_Array:
                value.removeSelf();
                value.m_node = elem;
                if(copy)
                    value.m_root = elem;
                else
                    value.addSelf(this);
                return true;
            }
        }
        return false;
    }

    bool
    JSON::optJSONObject(const std::string& key, JSON& value, bool copy) const
    {
        JTRACE;
        cJSON *elem = getItem(key, true, copy);
        if(0 != elem)
        {
            switch(elem->type)
            {
            case cJSON_Object:
                value.removeSelf();
                value.m_node = elem;
                if(copy)
                    value.m_root = elem;
                else
                    value.addSelf(this);
                return true;
            }
        }
        return false;
    }

    bool
    JSON::getBoolean(int index) const
    {
        JTRACE;
        cJSON *elem = getItem(index);

        switch(elem->type)
        {
        case cJSON_False:
            return false;
        case cJSON_True:
            return true;
        }
        THROW_MSG(Exception, "JSON element is not boolean: [" << index << "]");
        /* notreached */
        return false;
    }

    double
    JSON::getDouble(int index) const
    {
        JTRACE;
        cJSON *elem = getItem(index);

        switch(elem->type)
        {
        case cJSON_Number:
            return elem->valuedouble;
        }
        THROW_MSG(Exception, "JSON element is not a number: [" << index << "]");
        /* notreached */
        return 0;
    }

    long
    JSON::getLong(int index) const
    {
        JTRACE;
        cJSON *elem = getItem(index);

        switch(elem->type)
        {
        case cJSON_Number:
            return (long)elem->valueint;
        }
        THROW_MSG(Exception, "JSON element is not a number: [" << index << "]");
        /* notreached */
        return 0;
    }

    std::string
    JSON::getString(int index) const
    {
        JTRACE;
        cJSON *elem = getItem(index);

        switch(elem->type)
        {
        case cJSON_String:
            return elem->valuestring;
        }
        string strValue;
        char* json = cJSON_PrintUnformatted(elem);
        strValue = json;
        cJSON_free(json);

        return strValue;
    }

    void
    JSON::getJSONArray(int index, JSON& value, bool copy) const
    {
        JTRACE;
        cJSON *elem = getItem(index, false, copy);

        if(elem->type != cJSON_Array)
        {
            if(copy)
                cJSON_Delete(elem);

            THROW_MSG(Exception, "JSON element is not an array: [" << index << "]");
        }

        value.removeSelf();
        value.m_node = elem;
        if(copy)
            value.m_root = elem;
        else
            value.addSelf(this);
    }

    void
    JSON::getJSONObject(int index, JSON& value, bool copy) const
    {
        JTRACE;
        cJSON *elem = getItem(index, false, copy);

        if(elem->type != cJSON_Object)
        {
            if(copy)
                cJSON_Delete(elem);

            THROW_MSG(Exception, "JSON element is not an object: [" << index << "]");
        }

        value.removeSelf();
        value.m_node = elem;
        if(copy)
            value.m_root = elem;
        else
            value.addSelf(this);
    }

    bool
    JSON::optBoolean(int index, bool defaultValue) const
    {
        JTRACE;
        cJSON *elem = getItem(index, true);
        if(0 == elem)
            return defaultValue;

        switch(elem->type)
        {
        case cJSON_True:
            return true;
        case cJSON_False:
            return false;
        }
        return defaultValue;
    }

    double
    JSON::optDouble(int index, double defaultValue) const
    {
        JTRACE;
        cJSON *elem = getItem(index, true);
        if(0 == elem)
            return defaultValue;

        switch(elem->type)
        {
        case cJSON_Number:
            return elem->valuedouble;
        }
        return defaultValue;
    }

    long
    JSON::optLong(int index, long defaultValue) const
    {
        JTRACE;
        cJSON *elem = getItem(index, true);
        if(0 == elem)
            return defaultValue;

        switch(elem->type)
        {
        case cJSON_Number:
            return (long)elem->valueint;
        }
        return defaultValue;
    }

    std::string
    JSON::optString(int index, const std::string& defaultValue) const
    {
        JTRACE;
        cJSON *elem = getItem(index, true);
        if(0 == elem)
            return defaultValue;

        switch(elem->type)
        {
        case cJSON_String:
            return elem->valuestring;
        }
        char* json = cJSON_PrintUnformatted(elem);
        string jsonstring = json;
        cJSON_free(json);
        return jsonstring;
    }

    bool
    JSON::optJSONArray(int index, JSON& value, bool copy) const
    {
        JTRACE;
        cJSON *elem = getItem(index, true);
        if(0 != elem)
        {
            switch(elem->type)
            {
            case cJSON_Array:
                value.removeSelf();
                value.m_node = elem;
                if(copy)
                    value.m_root = elem;
                else
                    value.addSelf(this);
                return true;
            default:
                if(copy)
                    cJSON_Delete(elem);
            }
        }
        return false;
    }

    bool
    JSON::optJSONObject(int index, JSON& value, bool copy) const
    {
        JTRACE;
        cJSON *elem = getItem(index, true);
        if(0 != elem)
        {
            switch(elem->type)
            {
            case cJSON_Object:
                value.removeSelf();
                value.m_node = elem;
                if(copy)
                    value.m_root = elem;
                else
                    value.addSelf(this);
                return true;
            default:
                if(copy)
                    cJSON_Delete(elem);
            }
        }
        return false;
    }

    void
    JSON::addItem(const string& key, bool value)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Object)
            THROW_MSG(Exception, "expected JSON object");

        cJSON_AddBoolToObject(m_node, key.c_str(), value);
    }

    void
    JSON::addItem(const string& key, double value)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Object)
            THROW_MSG(Exception, "expected JSON object");

        cJSON_AddNumberToObject(m_node, key.c_str(), value);
    }

    void
    JSON::addItem(const string& key, long value)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Object)
            THROW_MSG(Exception, "expected JSON object");

        cJSON_AddNumberToObject(m_node, key.c_str(), value);
    }

    void
    JSON::addItem(const string& key, const string& value)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Object)
            THROW_MSG(Exception, "expected JSON object");

        cJSON_AddStringToObject(m_node, key.c_str(), value.c_str());
    }

    void
    JSON::addItem(const string& key, const JSON& value)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Object)
            THROW_MSG(Exception, "expected JSON object");

        if(0 == value.m_node)
            THROW_MSG(Exception, "child JSON node NULL");

        if(!value.m_root)
            THROW_MSG(Exception, "child does not own JSON node");

        cJSON_AddItemToObject(m_node, key.c_str(), value.m_node);
        value.m_self->m_root = 0;
        value.addSelf(this);
    }

    void
    JSON::addNullItem(const string& key)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Object)
            THROW_MSG(Exception, "expected JSON object");

        cJSON_AddNullToObject(m_node, key.c_str());
    }

    void
    JSON::addItem(bool value)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Array)
            THROW_MSG(Exception, "expected JSON array");

        cJSON* elem = cJSON_CreateBool(value);
        if(0 == elem)
            THROW_MSG(Exception, "no memory while allocating JSON element");

        cJSON_AddItemToArray(m_node, elem);
    }

    void
    JSON::addItem(double value)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Array)
            THROW_MSG(Exception, "expected JSON array");

        cJSON* elem = cJSON_CreateNumber(value);
        if(0 == elem)
            THROW_MSG(Exception, "no memory while allocating JSON element");

        cJSON_AddItemToArray(m_node, elem);
    }

    void
    JSON::addItem(long value)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Array)
            THROW_MSG(Exception, "expected JSON array");

        cJSON* elem = cJSON_CreateNumber(value);
        if(0 == elem)
            THROW_MSG(Exception, "no memory while allocating JSON element");

        cJSON_AddItemToArray(m_node, elem);
    }

    void
    JSON::addItem(const string& value)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Array)
            THROW_MSG(Exception, "expected JSON array");

        cJSON* elem = cJSON_CreateString(value.c_str());
        if(0 == elem)
            THROW_MSG(Exception, "no memory while allocating JSON element");

        cJSON_AddItemToArray(m_node, elem);
    }

    void
    JSON::addItem(const JSON& value)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Array)
            THROW_MSG(Exception, "expected JSON array");

        if(0 == value.m_node)
            THROW_MSG(Exception, "child JSON node NULL");

        if(!value.m_root)
            THROW_MSG(Exception, "child does not own JSON node");

        cJSON_AddItemToArray(m_node, value.m_node);
        value.m_self->m_root = 0;
        value.addSelf(this);
    }

    void
    JSON::addItem()
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Array)
            THROW_MSG(Exception, "expected JSON array");

        cJSON* elem = cJSON_CreateNull();
        if(0 == elem)
            THROW_MSG(Exception, "no memory while allocating JSON element");

        cJSON_AddItemToArray(m_node, elem);
    }

    void
    JSON::replaceItem(const string& key, bool value)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Object)
            THROW_MSG(Exception, "expected JSON object");

        cJSON* elem = cJSON_CreateBool(value);
        if(0 == elem)
            THROW_MSG(Exception, "no memory while allocating JSON object element: {\"" << key << "\"}");

        cJSON_ReplaceItemInObject(m_node, key.c_str(), elem);
    }

    void
    JSON::replaceItem(const string& key, double value)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Object)
            THROW_MSG(Exception, "expected JSON object");

        cJSON* elem = cJSON_CreateNumber(value);
        if(0 == elem)
            THROW_MSG(Exception, "no memory while allocating JSON object element: {\"" << key << "\"}");

        cJSON_ReplaceItemInObject(m_node, key.c_str(), elem);
    }

    void
    JSON::replaceItem(const string& key, long value)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Object)
            THROW_MSG(Exception, "expected JSON object");

        cJSON* elem = cJSON_CreateNumber(value);
        if(0 == elem)
            THROW_MSG(Exception, "no memory while allocating JSON object element: {\"" << key << "\"}");

        cJSON_ReplaceItemInObject(m_node, key.c_str(), elem);
    }

    void
    JSON::replaceItem(const string& key, const string& value)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Object)
            THROW_MSG(Exception, "expected JSON object");

        cJSON* elem = cJSON_CreateString(value.c_str());
        if(0 == elem)
            THROW_MSG(Exception, "no memory while allocating JSON object element: {\"" << key << "\"}");

        cJSON_ReplaceItemInObject(m_node, key.c_str(), elem);
    }

    void
    JSON::replaceNullItem(const string& key)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Object)
            THROW_MSG(Exception, "expected JSON object");

        cJSON* elem = cJSON_CreateNull();
        if(0 == elem)
            THROW_MSG(Exception, "no memory while allocating JSON object element: {\"" << key << "\"}");

        cJSON_ReplaceItemInObject(m_node, key.c_str(), elem);
    }

    void
    JSON::replaceItem(const string& key, const JSON& value)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Object)
            THROW_MSG(Exception, "expected JSON object");

        if(0 == value.m_node)
            THROW_MSG(Exception, "child JSON node NULL");

        if(!value.m_root)
            THROW_MSG(Exception, "child does not own JSON node");

        cJSON_ReplaceItemInObject(m_node, key.c_str(), value.m_node);
        value.m_self->m_root = 0;
        value.addSelf(this);
    }

    void
    JSON::replaceItem(int index, bool value)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Array)
            THROW_MSG(Exception, "expected JSON array");

        cJSON* elem = cJSON_CreateBool(value);
        if(0 == elem)
            THROW_MSG(Exception, "no memory while allocating JSON array element: [" << index << "]");

        cJSON_ReplaceItemInArray(m_node, index, elem);
    }

    void
    JSON::replaceItem(int index, double value)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Array)
            THROW_MSG(Exception, "expected JSON array");

        cJSON* elem = cJSON_CreateNumber(value);
        if(0 == elem)
            THROW_MSG(Exception, "no memory while allocating JSON array element: [" << index << "]");

        cJSON_ReplaceItemInArray(m_node, index, elem);
    }

    void
    JSON::replaceItem(int index, long value)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Array)
            THROW_MSG(Exception, "expected JSON array");

        cJSON* elem = cJSON_CreateNumber(value);
        if(0 == elem)
            THROW_MSG(Exception, "no memory while allocating JSON array element: [" << index << "]");

        cJSON_ReplaceItemInArray(m_node, index, elem);
    }

    void
    JSON::replaceItem(int index, const string& value)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Array)
            THROW_MSG(Exception, "expected JSON array");

        cJSON* elem = cJSON_CreateString(value.c_str());
        if(0 == elem)
            THROW_MSG(Exception, "no memory while allocating JSON array element: [" << index << "]");

        cJSON_ReplaceItemInArray(m_node, index, elem);
    }

    void
    JSON::replaceItem(int index)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Array)
            THROW_MSG(Exception, "expected JSON array");

        cJSON* elem = cJSON_CreateNull();
        if(0 == elem)
            THROW_MSG(Exception, "no memory while allocating JSON array element: [" << index << "]");

        cJSON_ReplaceItemInArray(m_node, index, elem);
    }

    void
    JSON::replaceItem(int index, const JSON& value)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Array)
            THROW_MSG(Exception, "expected JSON array");

        if(0 == value.m_node)
            THROW_MSG(Exception, "child JSON node NULL");

        if(!value.m_root)
            THROW_MSG(Exception, "child does not own JSON node");

        cJSON_ReplaceItemInArray(m_node, index, value.m_node);
        value.m_self->m_root = 0;
        value.addSelf(this);
    }

    void
    JSON::removeItem(const string& key)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Object)
            THROW_MSG(Exception, "expected JSON object");

        cJSON_DeleteItemFromObject(m_node, key.c_str());
    }

    void
    JSON::removeItem(int index)
    {
        JTRACE;

        if(0 == m_node)
            THROW_MSG(Exception, "JSON node NULL");

        if(m_node->type != cJSON_Array)
            THROW_MSG(Exception, "expected JSON array");

        cJSON_DeleteItemFromArray(m_node, index);
    }

    JSON&
    JSON::operator=(const JSON& rhs)
    {
        JTRACE;
        if(this != &rhs)
        {
            if(m_root != 0)
                removeSelf();
            m_node = rhs.m_node;
            addSelf(&rhs);
        }
        return *this;
    }

    string
    JSON::toString() const
    {
        JTRACE;

        string jsonstring;
        if(0 != m_node)
        {
            char* json = cJSON_PrintUnformatted(m_node);
            jsonstring = json;
            cJSON_free(json);
        }
        return jsonstring;
    }

    Enumeration
    JSON::keys() const
    {
        JTRACE;

        return Enumeration(0 != m_node ? m_node->child : 0);
    }

    bool
    JSON::isEmpty() const
    {
        JTRACE;
        return 0 == m_node || ((m_node->type == cJSON_Array || m_node->type == cJSON_Object) && 0 == m_node->child);
    }

    bool
    JSON::isNull() const
    {
        JTRACE;
        return this->getType() == JSON::JNULL;
    }

    ostream& operator<<(ostream &os, const JSON &obj)
    {

        if(0 != obj.m_node)
        {
            char* json = cJSON_Print(obj.m_node);
            os << json;
            cJSON_free(json);
        }
        return os;
    }

} /* namespace JSON */
