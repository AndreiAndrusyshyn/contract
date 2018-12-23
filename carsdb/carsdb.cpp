#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/time.hpp>

#include "carsdb.hpp"

	
	void carsdb::insert( name user,std::string brand,std::string gov_number,std::string color,std::string engine_volume) {
		require_auth(user);
		car_index cars(_code, _code.value);
		auto iterator = cars.find(user.value);
				cars.emplace(user, [&](auto& row) {
				row.user = user;
				row.key = cars.available_primary_key();
				row.brand = brand;
				row.gov_number = gov_number;
				row.color = color;
				row.engine_volume = engine_volume;
			});
						
	}


	void carsdb::erase(uint64_t key) {
		require_auth(_self);
		car_index cars(_self, _code.value);
		auto iterator = cars.find(key);
		eosio_assert(iterator != cars.end(), "Record does not exist");
		cars.erase(iterator);
	}


	void carsdb::modify(name user, uint64_t key, std::string brand, std::string gov_number, std::string color, std::string engine_volume) {
		require_auth(_self);
		car_index cars(_self, _self.value);
		auto iterator = cars.find(key);
		cars.modify(iterator, user , [&] (auto& row) {
			row.brand = brand;
			row.gov_number = gov_number;
			row.color = color;
			row.engine_volume = engine_volume;
		});
	}


	void carsdb::carauction(name user, std::string brand, std::string gov_number, std::string color, std::string engine_volume, uint64_t key){
		action(
			permission_level{get_self(), "active"_n},
			"auction"_n,
			"add"_n,// function the want to call from another contract
			std::make_tuple(user, brand, gov_number,color, engine_volume, key)
			).send();


	}

	void cradb::takewinner(name user, uint64_t key) {
		car_index cars(self, _self.value);
		auto iterator = cars.find(key);

		cars.emplace(iterator, user, [&](auto& row){
			row.user = user;
		});
	}


