#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

#include "testsen.hpp"


void testsen::add (name user , std::string brand ) {
	require_auth(user);
	car_index cars(_code, _code.value);
	auto iterator = cars.find(user.value);
	cars.emplace(user , [&](auto & row) {
		row.user = user;
		row.key = cars.available_primary_key();
		row.brand = brand;
	});
}

void testsen::erase(uint64_t key) {
	car_index cars(_self, _code.value);
	auto iterator = cars.find(key);

	cars.erase(iterator);
}


void testsen::auction(name user, uint64_t key) {
	car_index cars(_code, _code.value);
	auto iterator = cars.find(key);

	testsen::sendcar(user, iterator->brand, key);
}

void testsen::take(name user, uint64_t key) {
	car_index cars(_code, _code.value);
	auto iterator = cars.find(key);

	cars.modify(iterator, iterator->user, [&](auto&row) {
		row.user = user;
	});



}

void testsen::sendcar( name user, std::string brand, uint64_t key)  {
	action sendd = action (
	                   permission_level{get_self(), "active"_n},
	                   "testo"_n,
	                   "addcar"_n,
	                   std::make_tuple(user, brand, key )
	               );
	sendd.send();

}


