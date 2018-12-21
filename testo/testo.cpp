#include "testo.hpp"


#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/print.hpp>



void testo::add(name user , std::string brand, uint64_t key) {
	car_index cars(get_self(), get_self().value);
	auto iterator = cars.find(user.value); 
	cars.emplace("testo"_n, [&](auto & row) { 
		row.user = user;
		row.key = key;
		row.brand = brand;
		row.times = 0;
	});
	print("gi");
}

void testo::eras(name key) {

	balance_index balances(_self, _code.value);
	auto iterator = balances.find(key.value);

	balances.erase(iterator);

}



void testo::auction (name user, uint64_t key, uint64_t times ) {
	uint64_t t = times + (now() / 60);
	car_index cars(_code,  _code.value);
	auto iterator = cars.find(key);
	print("Car number:" , iterator->key, "is on auction!");
	cars.modify(iterator, user , [&] (auto & row) {
		row.times = t;
	});

}

void testo::pbid (name user, asset bidd, uint64_t lot) {

	car_index cars(_code, _code.value);
	balance_index balances(_code, _code.value);
	bid_index bids(_code, _code.value);
	auto iteratorbal = balances.find(user.value);
	auto iteratorbid = bids.cbegin();
	auto iterato = cars.cbegin();



	for (; iterato != cars.cend();) { // find - table operation is not allowed
		if (iterato->key == lot && iterato->times <= (now() / 60))  {

			print("Lot time expired!");
			break;

		}
		else if (iterato->key == lot) {
			if (iteratorbal->amount < bidd) {
				print("not so fast");
				break;
			}
			else {
				print(findheighest(lot));

				bids.emplace("testo"_n, [&](auto & row) {
					row.key = bids.available_primary_key();
					row.user = user;
					row.bidd = bidd;
					row.lot = lot;
				});

				balances.modify(iteratorbal, "testo"_n, [&](auto & row ) {
					row.amount = row.amount - bidd;
				});


			}
		}
		else { print("Sorry, this lot: ", lot, " is not on auction");}
		iterato++;
	}
}

void testo::status(uint64_t lot) {
	/*
			car_index cars(_code, _code.value);
			bid_index bids(_code, _code.value);
			auto iter = bids.find(findheighest(lot));
			auto iterator = cars.find(lot);

			if (iterator->times < (now() / 60) ) {

				print("heighest bid:", iter->bidd, "winner is:" , iter->user);

				sendback(iter->user, iterator->key);
			}
			else {

				print("heighest bid:", iter->bidd);

			}
	*/
}


void testo::addbalance(name from, name to, asset amount, std::string m) {
	balance_index balances(get_self(), get_self().value);
	auto iterator = balances.find(from.value);
	if (iterator == balances.end())
	{
		balances.emplace(get_self(), [&from, &amount](auto& row)
		{
			row.user = from;
			row.amount = amount;
		});
	}
	else
	{
		balances.modify(iterator, get_self(), [&amount](auto& row)
		{
			row.amount += amount;
		});
	}

}


void testo::sendback (name user, uint64_t key) {

	action sendd = action (
	                   permission_level{get_self() , "active"_n},
	                   "testsend"_n,
	                   "take"_n,
	                   std::make_tuple(user, key )
	               );
	sendd.send();


};

asset testo::findheighest(uint64_t lot) {
	asset hibid ;
	uint64_t hikey = 0;
	bid_index bids(_code, _code.value);
	for (auto iterator = bids.cbegin(); iterator != bids.cend(); iterator++ ) {
		if (iterator->lot == lot && iterator->bidd > hibid) {
			hikey = iterator->key;
			hibid = iterator->bidd;
		}

	}

	return hibid;
}
