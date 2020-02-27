#ifndef USERMAP_H_INCLUDED
#define USERMAP_H_INCLUDED


#include <cstdint>

#include "Model.h"
#include "nullable/Nullable.h"


namespace model{

class UserMap : public Model
{
    public:

        UserMap(){
            this->setData({
              {"id", nullptr},                  // integer
              {"mac_address", nullptr},         // string
              {"ip_address", nullptr},         // string
              {"port", nullptr},                // integer (16-bit)
              {"created_at", nullptr},          // integer
              {"updated_at", nullptr},          // integer
            });
            this->getDatabaseConnector()->setEndPoint("http://192.168.0.100:8000/api/usermaps");
        }

        void setId(int32_t id) { this->getData()["id"] = id; }
        nullable::Nullable<int32_t> getId(void) { return this->getData()["id"]; }

        void setMacAddress(const std::string &mac_address) { this->getData()["mac_address"] = mac_address; }
        nullable::Nullable<std::string> getMacAddress() { return this->getData()["mac_address"]; }

        void setIpAddress(const std::string &ip_address) { this->getData()["ip_address"] = ip_address; }
        nullable::Nullable<std::string> getIpAddress(void) { return this->getData()["ip_address"]; }

        void setPort(uint16_t port) { this->getData()["port"] = port; }
        nullable::Nullable<uint16_t> getPort(void) { return this->getData()["port"]; }

        void setCreatedAt(int32_t created_at) { this->getData()["created_at"] = created_at; }
        nullable::Nullable<int32_t> getCreatedAt(void) { return this->getData()["created_at"]; }

        void setUpdatedAt(int32_t updated_at) { this->getData()["updated_at"] = updated_at; }
        nullable::Nullable<int32_t> getUpdatedAt(void) { return this->getData()["updated_at"]; }
};

}


#endif // USERMAP_H_INCLUDED
