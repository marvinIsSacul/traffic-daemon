#ifndef DATABASE_H
#define DATABASE_H

#include <cstdint>
#include <string>
#include <nlohmann/json.hpp>

#include "../exception/DatabaseException.h"


namespace data{

class Database
{
    public:

        Database(): _isInit{false} {}
        virtual ~Database() {}
        virtual void init() throw(exception::DatabaseException) = 0;
        virtual bool isInit() const noexcept { return this->_isInit; }

        /**
         * Performs an update query.
         * @param data The data.
        */
        virtual void update(const nlohmann::json& data) throw (exception::DatabaseException) = 0;
        virtual void remove(const nlohmann::json& data) throw (exception::DatabaseException) = 0;
        virtual void insert(const nlohmann::json& data) throw (exception::DatabaseException) = 0;

        virtual void query(const std::string &sql) throw(exception::DatabaseException) = 0;

        virtual nlohmann::json get(const nlohmann::json& data) throw (exception::DatabaseException) {
            try{
                const int64_t id = data["id"];
                return this->get(id);
            }catch (const std::exception &e){
                throw exception::DatabaseException(e.what());
            }
        }

        virtual nlohmann::json get(int64_t id) throw (exception::DatabaseException) = 0;

        virtual void setEndPoint(const std::string &endPoint) { this->_endPoint = endPoint; }
        virtual std::string getEndPoint(void) const { return this->_endPoint; }

    private:

        std::string _endPoint;
        bool _isInit;

    protected:
        virtual void setInit() final { this->_isInit = true; }
};

}

#endif // DATABASE_H
