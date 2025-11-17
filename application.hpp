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
};
