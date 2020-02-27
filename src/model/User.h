#ifndef USER_MODEL_H
#define USER_MODEL_H

#include <cstdint>
#include <string>

#include "Model.h"
#include "nullable/Nullable.h"


namespace model{


/** @class User
 *
 * The user model which inherits the generic (typeless) model. This user model helps with strict typing of fields.
 */

class User : public Model
{
    public:

        User(){
            this->setData({
              {"id", nullptr},              // integer
              {"username", nullptr},        // string
              {"mac_address", nullptr},     // string
              {"password", nullptr},        // string
              {"created_at", nullptr},      // integer (UNIX timestamp)
              {"updated_at", nullptr},      // integer (UNIX timestamp)
            });
            this->getDatabaseConnector()->setEndPoint("http://127.0.0.1:8000/api/users");
        }

        void setId(int32_t id){ this->getData()["id"] = id; }
        nullable::Nullable<int32_t> getId(void) { return this->getData()["id"]; }

        void setUsername(const std::string &username){ this->getData()["username"] = username; }
        nullable::Nullable<std::string> getUsername(void) { return this->getData()["username"]; }

        void setMacAddress(const std::string &mac_address){ this->getData()["mac_address"] = mac_address; }
        nullable::Nullable<std::string> getMacAddress(void) { return this->getData()["mac_address"]; }

        void setPassword(const std::string &password){ this->getData()["password"] = password; }
        nullable::Nullable<std::string> getPassword(void) { return this->getData()["password"]; }

        void setCreatedAt(int32_t created_at){ this->getData()["created_at"] = created_at; }
        nullable::Nullable<int32_t> getCreatedAt(void) { return this->getData()["created_at"]; }

        void setUpdatedAt(int32_t updated_at){ this->getData()["updated_at"] = updated_at; }
        nullable::Nullable<int32_t> getUpdatedAt(void) { return this->getData()["updated_at"]; }
};

}

#endif // USER_MODEL_H
