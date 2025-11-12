#pragma once

#include <string>
#include <mutex>
#include "json.hpp"

using json = nlohmann::json;

class Database {
	public:

	    explicit Database(const std::string& filepath);
	    
    	bool createRecord(const std::string& table, const std::string& id, const json& data);

    	json readRecord(const std::string& table, const std::string& id);

    	json readTable(const std::string& table);

    	bool updateRecord(const std::string& table, const std::string& id, const json& data);

    	bool deleteRecord(const std::string& table, const std::string& id);

	private:
    	std::string filepath_;     
    	json db_data_;            
    	std::mutex db_mutex_;     

    	void load();

    	bool save();
};
