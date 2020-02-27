#ifndef CURL_H
#define CURL_H

#include "Database.h"

#include <curl/curl.h>

namespace data{

class Database_Curl : public Database
{
    public:
        Database_Curl();
        virtual ~Database_Curl(){ clear(); }

        Database_Curl(const Database_Curl&) = delete;
        Database_Curl(const Database_Curl&&) = delete;
        Database_Curl &operator=(const Database_Curl&) = delete;
        Database_Curl &operator=(const Database_Curl&&) = delete;

        virtual void init() throw(exception::DatabaseException) override;
        virtual void update(const nlohmann::json &data) throw(exception::DatabaseException) override;
        virtual void insert(const nlohmann::json &data) throw(exception::DatabaseException) override;
        virtual void remove(const nlohmann::json &data) throw(exception::DatabaseException) override;
        virtual nlohmann::json get(int64_t id) throw (exception::DatabaseException) override;
        virtual nlohmann::json get(const nlohmann::json &data) throw(exception::DatabaseException) override;

        virtual void query(const std::string &sql) throw(exception::DatabaseException) override;

    private:

        CURL *curl;
        struct curl_slist *headers;
        nlohmann::json jsonObj;
        bool isGet;
        bool isException;
        char errorBuf[CURL_ERROR_SIZE + 1];

        friend size_t get_response(void *buffer, size_t , size_t nmemb, void *userp);

        std::string _url(const std::string &part = ""){ return this->getEndPoint() + part; }
        void clear();
        void assertInit() throw(exception::DatabaseException);

        nlohmann::json makeJson();

        template<typename O, typename D>
        void setOption(O option, D data) throw(exception::DatabaseException){
            if (curl_easy_setopt(this->curl, option, data) != CURLE_OK)
                throw exception::DatabaseException(this->errorBuf);
        }
};

}

#endif // CURL_H
