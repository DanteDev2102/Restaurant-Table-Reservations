#pragma once

#include <memory>

#include "reservation.hpp"
#include "db.hpp"

class Application {
	private:
		int qtyTables = 0;
		std::unique_ptr<Database> db;
		
	public:
		Application(int qty);
		void setQtyTables (int qty);
		int getQtyTables();
		void configQtyTables();
		bool createReservation(int table, int qty, const std::string& name, const std::string& dni, const std::string& day);
};
