#ifndef TRAFFICFILTER_H_INCLUDED
#define TRAFFICFILTER_H_INCLUDED


#include <cstdint>

#include "Model.h"
#include "nullable/Nullable.h"


namespace model{

class TrafficFilter : public Model
{
    public:

        TrafficFilter(){
            this->setData({
              {"id", nullptr},                      // integer
              {"ip_address", nullptr},              // string
              {"time_start", nullptr},              // integer
              {"time_end", nullptr},                // integer
              {"description", nullptr},             // string
              {"created_at", nullptr}               // integer
            });
            this->getDatabaseConnector()->setEndPoint("http://127.0.0.1:8000/api/trafficfilters");
        }

        void setId(int32_t id) { this->getData()["id"] = id; }
        nullable::Nullable<int32_t> getId() { return this->getData()["id"]; }

        void setIpAddress(const std::string &ip_address){ this->getData()["ip_address"] = ip_address; }
        nullable::Nullable<std::string> getIpAddress(void) { return this->getData()["ip_address"]; }

        void setTimeStart(int32_t time_start) { this->getData()["time_start"] = time_start; }
        nullable::Nullable<int32_t> getTimeStart() { return this->getData()["time_start"]; }

        void setTimeEnd(int32_t time_end) { this->getData()["time_end"] = time_end; }
        nullable::Nullable<int32_t> getTimeEnd() { return this->getData()["time_end"]; }

        void setTrafficFilterId(const std::string &description) { this->getData()["description"] = description; }
        nullable::Nullable<std::string> getDescription() { return this->getData()["description"]; }

        void setCreatedAt(int32_t created_at) { this->getData()["created_at"] = created_at; }
        nullable::Nullable<int32_t> getCreatedAt() { return this->getData()["created_at"]; }
};

}



#endif // TRAFFICFILTER_H_INCLUDED
