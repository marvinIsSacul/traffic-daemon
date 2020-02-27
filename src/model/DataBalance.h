#ifndef DATABALANCE_H
#define DATABALANCE_H

#include <cstdint>

#include "Model.h"
#include "nullable/Nullable.h"


namespace model{

class DataBalance : public Model
{
    public:

        DataBalance(){
            this->setData({
              {"id", nullptr},                  // integer
              {"user_id", nullptr},            // integer
              {"purchase_id", nullptr},         // integer
              {"traffic_filter_id", nullptr},   // integer
              {"bytes_balance", nullptr},       // integer-64
            });
            this->getDatabaseConnector()->setEndPoint("http://127.0.0.1:8000/api/databalances");
        }

        void setId(int32_t id) { this->getData()["id"] = id; }
        nullable::Nullable<int32_t> getId(void) { return this->getData()["id"]; }

        void setPurchaseId(int32_t purchase_id) { this->getData()["purchase_id"] = purchase_id; }
        nullable::Nullable<int32_t> getPurchaseId() { return this->getData()["purchase_id"]; }

        void setUserId(int32_t user_id) { this->getData()["user_id"] = user_id; }
        nullable::Nullable<int32_t> getUserId(void) { return this->getData()["user_id"]; }

        void setTrafficFilterId(int32_t traffic_filter_id) { this->getData()["traffic_filter_id"] = traffic_filter_id; }
        nullable::Nullable<int32_t> getTrafficFilterId(void) { return this->getData()["traffic_filter_id"]; }

        void setBytesBalance(int64_t bytes_balance) { this->getData()["bytes_balance"] = bytes_balance; }
        nullable::Nullable<int64_t> getBytesBalance(void) { return this->getData()["bytes_balance"]; }
};

}

#endif // DATABALANCE_H
