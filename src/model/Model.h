#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <ostream>
#include <nlohmann/json.hpp>

#include "../data/Database_Curl.h"


/**
 * @class Model
 * The generic model. This model can be used on its own. But it is typeless.
*/
class Model {
private:
    data::Database *_dc;
    nlohmann::json _data;
    std::string _dataHash;
    bool _isProtectingFields = true;

public:
    Model(data::Database *dc = nullptr) : _dc{nullptr} {
        if (dc == nullptr)
            this->setDatabaseConnector(new data::Database_Curl());
        if (!this->getDatabaseConnector()->isInit())
            this->getDatabaseConnector()->init();
    }

    virtual ~Model(){ clearDatabaseConnector(); }

    virtual void update() throw(exception::DatabaseException){
        this->getDatabaseConnector()->update(this->getData());
    }

    virtual void save() throw(exception::DatabaseException){
        this->getDatabaseConnector()->insert(this->getData());
    }

    virtual void remove() throw(exception::DatabaseException){
        this->getDatabaseConnector()->remove(this->getData());
    }

    virtual void fetch(int64_t id) throw(exception::DatabaseException) {
        this->setData(this->getDatabaseConnector()->get(id));
        this->_dataHash = this->_data.dump();  // store new data "hash"
    }

    /** \brief Asserts whether data fetched from the database is still as was when fetched. still pristine.
     *
     * \return bool
     *
     */
    virtual bool isFetched() const {
        return this->getData().dump() == this->_dataHash;
    }

    virtual void setDatabaseConnector(data::Database *dc){
        this->_dc = dc;
    }

    virtual data::Database *getDatabaseConnector(void){
        return this->_dc;
    }

    virtual const nlohmann::json &operator[](const std::string &key) const {
        return this->_data[key];
    }

    nlohmann::json &operator[](const std::string &key) {
        return this->_data[key];
    }

    virtual void clearDatabaseConnector(){
        if (this->_dc != nullptr){
            delete this->_dc;
            this->_dc = nullptr;
        }
    }

    friend std::ostream& operator << (std::ostream &os, const Model &model){
        return os << model.getData();
    }


protected:

    virtual nlohmann::json &getData(void) final { return this->_data; }

    virtual const nlohmann::json &getData(void) const final { return this->_data; }

    virtual void setData(const nlohmann::json &data) throw (std::exception) final { this->_data = data; }
};




#endif // MODEL_H_INCLUDED
