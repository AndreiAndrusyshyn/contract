#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
using namespace eosio;

class [[eosio::contract]] testsen : public eosio::contract {
public:
	using contract::contract; 
	testsen(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}

[[eosio::action]] 
void add (name user , std::string brand ) { 
require_auth(user);
car_index cars(_code, _code.value);
	auto iterator = cars.find(user.value);
	cars.emplace(user , [&](auto& row) {
		row.user = user;
		row.key = cars.available_primary_key();
		row.brand = brand;
	});

	print("Dfsfs");
}

[[eosio::action]]
void take(name user , uint64_t key) {

	car_index cars(_code, _code.value);
	auto iterator = cars.find(key);
	cars.modify(iterator, user , [&] (auto& row) {
			row.user = user;
		});
}


[[eosio::action]]
void tacut(name user , uint64_t key) {

	car_index cars(_code, _code.value);
	auto iterator = cars.find(key);
	
	senddcar( user, iterator->brand, key);

}

[[eosio::action]]
void erasee(uint64_t key) {
	car_index cars(_self, _code.value);
	auto iterator = cars.find(key);

	cars.erase(iterator);
}




private:


void senddcar( name user, std::string brand, uint64_t key)  {
	action sendd=action (
	permission_level{get_self(), "active"_n},
	"testo"_n,
	"add"_n,
	std::make_tuple(user,brand, key ) 
	);
	sendd.send();

}
struct [[eosio::table]] car {
		uint64_t key;
		name user;
		std::string brand;
		uint64_t primary_key() const { return key;}

	//	EOSLIB_SERIALIZE(car, (key)(user)(brand))

	};

	typedef eosio::multi_index<"cars"_n, car> car_index; 

};

EOSIO_DISPATCH(testsen, (add)(tacut)(erasee))

