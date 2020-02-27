#include "Database_Curl.h"

#include <iostream>
#include <sstream>


namespace data{


size_t get_response(void *buffer, size_t , size_t nmemb, void *userp);


Database_Curl::Database_Curl() : curl{nullptr}, headers{NULL}, isGet{false}, isException{false}, errorBuf{""}{

}

void Database_Curl::init() throw (exception::DatabaseException) {
    this->curl = curl_easy_init();

    if (this->curl == nullptr){
        throw exception::DatabaseException("error initialising CURL");
    }

    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Accept: application/json");
    this->setOption(CURLOPT_HTTPHEADER, headers);
    this->setOption(CURLOPT_WRITEFUNCTION, get_response);
    this->setOption(CURLOPT_WRITEDATA, this);
    this->setOption(CURLOPT_ERRORBUFFER, this->errorBuf);
    this->setInit();
}

void Database_Curl::query(const std::string &) throw(exception::DatabaseException) {

}



void Database_Curl::update(const nlohmann::json &data) throw (exception::DatabaseException){
    assertInit();

    const std::string &jsonData = data.dump();

    std::stringstream urlPart;
    urlPart << '/' << data["id"];

    this->setOption(CURLOPT_POSTFIELDS, jsonData.c_str());
    this->setOption(CURLOPT_CUSTOMREQUEST, "PUT");
    this->setOption(CURLOPT_POSTFIELDSIZE, (long) jsonData.size());
    this->setOption(CURLOPT_URL, _url(urlPart.str()).c_str());

    CURLcode result = curl_easy_perform(curl);

    if (result != CURLE_OK)
        throw exception::DatabaseException(errorBuf);
}

void Database_Curl::insert(const nlohmann::json &data) throw (exception::DatabaseException){
    assertInit();

    const std::string &jsonData = data.dump();

    this->setOption(CURLOPT_POST, 1);
    this->setOption(CURLOPT_POSTFIELDS, jsonData.c_str());
    this->setOption(CURLOPT_POSTFIELDSIZE, (long) jsonData.size());
    this->setOption(CURLOPT_URL, _url().c_str());

    if (curl_easy_perform(curl) != CURLE_OK){
        throw exception::DatabaseException(this->errorBuf);
    }
    else{
       // printf("%s\n", jsonData.c_str());
    }
}

nlohmann::json Database_Curl::get(int64_t id) throw (exception::DatabaseException){
    std::stringstream urlPart;

    urlPart << '/' << id;

    isGet = true;

    this->setOption(CURLOPT_CUSTOMREQUEST, "GET");
    this->setOption(CURLOPT_URL, _url(urlPart.str()).c_str());

    const int result = curl_easy_perform(curl);

    if (result != CURLE_OK)
        throw exception::DatabaseException(errorBuf);

    return makeJson();
}

nlohmann::json Database_Curl:: get(const nlohmann::json &data) throw (exception::DatabaseException){
    return this->get(int64_t(data["id"]));
}

void Database_Curl::remove(const nlohmann::json &/*data*/) throw(exception::DatabaseException)
{
    /// @todo
}

void Database_Curl::clear(){
    if (curl != nullptr){
        curl_easy_cleanup(curl);
        curl = nullptr;
    }

    if (headers != NULL){
        curl_slist_free_all(headers);
        headers = NULL;
    }
}

void Database_Curl::assertInit() throw(exception::DatabaseException){
    if (this->curl == nullptr)
        throw exception::DatabaseException("CURL is not initialized");
}

size_t get_response(void *buffer, size_t , size_t nmemb, void *userp){
    char *data = (char*)buffer;
    std::string str;
    str.reserve(nmemb);

    Database_Curl *instance = static_cast<Database_Curl*>(userp);

    instance->isException = false;

    for (size_t i = 0; i < nmemb; ++i)
        str.push_back(data[i]);

    try{
        instance->jsonObj = nlohmann::json::parse(str);
    }
    catch (...){
        std::cerr << "EXCEPTION";
        std::cerr << typeid(*instance).name() << "::" << __func__ << std::endl;
        std::cerr << std::endl;
        instance->isException = true;
    }

    return nmemb;
}

nlohmann::json Database_Curl::makeJson(){
    auto dataObj = this->jsonObj["data"];   /// assumes the actual data is stored inside a "data" object.

    return dataObj;
}

}
