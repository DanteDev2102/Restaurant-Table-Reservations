#pragma once

#include <memory>
#include "utils/utility.hpp"

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
		void updateFunction(Reservations& reservationList);
		void deleteFunction(Reservations& reservationList, Reservations& cancelledList);
		void showCancelledReservations(Reservations& cancelledList);
		json parseToJson(const int& table, const std::string& name, const std::string& dni, const std::string& date, const int& qty);
        void loadReservations(Reservations& reservationList);
        bool deleteReservationRecord(int table, string date); //Borrado de reservas cuando terminan de comer
};
