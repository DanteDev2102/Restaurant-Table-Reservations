#include <fstream>
#include <iostream>

#include "db.hpp"

Database::Database(const std::string& filepath) : filepath_(filepath) {
    std::lock_guard<std::mutex> lock(db_mutex_);
    load();
}

void Database::load() {
    std::ifstream file(filepath_);
    if (!file.is_open()) {
        db_data_ = json::object();
        std::cerr << "Advertencia: El archivo '" << filepath_ << "' no existe. Se creará uno nuevo." << std::endl;
        return;
    }

    try {
        file >> db_data_;
    } catch (json::parse_error& e) {
        std::cerr << "Error: Falla al parsear JSON en '" << filepath_ << "'. " << e.what() << std::endl;
        db_data_ = json::object();
    }
}

bool Database::save() {
    std::ofstream file(filepath_);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo '" << filepath_ << "' para escritura." << std::endl;
        return false;
    }

    try {
        file << db_data_.dump(4);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error: Falla al escribir JSON en el archivo. " << e.what() << std::endl;
        return false;
    }
}


bool Database::createRecord(const std::string& table, const std::string& id, const json& data) {
    std::lock_guard<std::mutex> lock(db_mutex_);

    if (!db_data_.contains(table)) {
        db_data_[table] = json::object();
    }

    if (db_data_[table].contains(id)) {
        return false;
    }

    db_data_[table][id] = data;
    return save();
}

json Database::readRecord(const std::string& table, const std::string& id) {
    std::lock_guard<std::mutex> lock(db_mutex_);

    if (db_data_.contains(table) && db_data_[table].contains(id)) {
        return db_data_[table][id];
    }

    return json(); 
}

json Database::readTable(const std::string& table) {
    std::lock_guard<std::mutex> lock(db_mutex_);

    if (db_data_.contains(table)) {
        return db_data_[table];
    }
    
    return json();
}

bool Database::updateRecord(const std::string& table, const std::string& id, const json& data) {
    std::lock_guard<std::mutex> lock(db_mutex_);

    if (!db_data_.contains(table) || !db_data_[table].contains(id)) {
        return false;
    }

    db_data_[table][id] = data;
    return save();
}

bool Database::deleteRecord(const std::string& table, const std::string& id) {
    std::lock_guard<std::mutex> lock(db_mutex_);

    if (!db_data_.contains(table) || !db_data_[table].contains(id)) {
        return false;
    }

    db_data_[table].erase(id);
    return save();
}
