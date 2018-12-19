#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/time.hpp>
//#include <ctime>

using namespace eosio;

class [[eosio::contract]] carsdb : public eosio::contract {
public:
	using contract::contract;
	carsdb(name receiver, name code, datastream<const char*> ds): contract(receiver, code,ds) {}
	
	[[eosio::action]]
	void insert( name user,std::string brand,std::string gov_number,std::string color,std::string engine_volume) {
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


	[[eosio::action]]
	void erase(uint64_t key) {
		require_auth(_self);
		car_index cars(_self, _code.value);
		auto iterator = cars.find(key);
		eosio_assert(iterator != cars.end(), "Record does not exist");
		cars.erase(iterator);
	}

	[[eosio::action]]
	void modify(name user, uint64_t key, std::string brand, std::string gov_number, std::string color, std::string engine_volume) {
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


	[[eosio::action]]
	void carauction(){
		action(
			permission_level{get_self(), "active"_n},
			"auctionf"_n,
			"version"_n,// function the want to call from another contract
			std::make_tuple()
			).send();


	}


//int64_t eosio::microseconds::time_point;

private:
struct [[eosio::table]] car {
		uint64_t key;
		name user;
		std::string brand;
		std::string gov_number;
		std::string color;
		std::string engine_volume;

		
		uint64_t primary_key() const { return key;}
		EOSLIB_SERIALIZE(car, (key)(user)(brand)(gov_number)(color)(engine_volume))

	};

	typedef eosio::multi_index<"cars"_n, car> car_index; 
};



EOSIO_DISPATCH(carsdb, (insert)(erase)(modify)(carauction))

