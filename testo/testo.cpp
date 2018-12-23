#include <eosiolib/print.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>

#include "testo.hpp"

void testo::addcar(name user , std::string brand, uint64_t key) {
	car_index carsd(_self, _self.value);
	auto iterator = carsd.find(user.value);
	carsd.emplace("testo"_n, [&](auto & row) {
		row.user = user;
		row.key = key;
		row.brand = brand;
		row.times = 0;
	});

}

void testo::erase(uint64_t key) {
	bid_index bids(_self, _code.value);
	auto itr = bids.find(key);

	bids.erase(itr);


}



void testo::auction (name user, uint64_t key, uint64_t times ) {

	uint64_t t = times + (now() / 60);
	car_index carsd(_code,  _code.value);
	auto iterator = carsd.find(key);
	print("Car number:" , iterator->key, "is on auction!");
	carsd.modify(iterator, user , [&] (auto & row) {
		row.times = t;
	});

}

void testo::pbid (name user, asset bidd, uint64_t lot) {

	car_index carsd(_code, _code.value);
	bid_index bids(_code, _code.value);
	balance_index balances(_code, _code.value);
	auto itcar = carsd.find(lot);
	auto itbal = balances.find(user.value);
	auto itbid = bids.cbegin();

	if (itcar->times <= (now() / 60)) {
		print("This lot time has expired");
	}
	else
	{
		if (itbid == bids.cend() && itbid == bids.cbegin()) {
			bids.emplace("testo"_n, [&](auto & row) {
				row.key = bids.available_primary_key();
				row.user = user;
				row.bidd = bidd;
				row.lot = lot;
			});

			balances.modify(itbal, user , [&](auto & row) {
				row.amount -= bidd;
			});
		}
		else
		{

			if (findheighest(lot) == 0) {
				bids.emplace("testo"_n, [&](auto & row) {
					row.key = bids.available_primary_key();
					row.user = user;
					row.bidd = bidd;
					row.lot = lot;
				});

			}
			else {
				itbid = bids.find(findheighest(lot));
				if (itbal->amount < bidd || itbid->bidd >= bidd )
				{
					print("Your bidd is greater than amount of your tokens or is less than heighest bid");
				}
				else
				{
					itbal = balances.find(itbid->user.value);
					balances.modify(itbal, user, [&](auto & row) {
						row.amount += itbid->bidd;
					});

					bids.erase(itbid);


					bids.emplace("testo"_n, [&](auto & row) {
						row.key = bids.available_primary_key();
						row.user = user;
						row.bidd = bidd;
						row.lot = lot;
					});
					itbal = balances.find(user.value);
					balances.modify(itbal, user , [&](auto & row) {
						row.amount -= bidd;
					});
				}
			}
		}
	}
}

void testo::status(uint64_t lot) {


	car_index cars(get_self(), get_self().value);
	bid_index bids(get_self(), get_self().value);
	auto itcar = cars.find(lot);
	auto itbid = bids.find(findheighest(lot));

	if ((now() / 60) > itcar->times && itcar->times != 0) {
		print(itbid->user);
		sendback(itbid->user, lot);

		/*
				itbid = bids.find(itcar->user.value);

				bids.modify(itbid, itcar->user, [&](auto& row){
					row.amount +=itbid->bidd;
				});
				row.amount +=itbid->bidd;
				*/
	}
	else {
		print("Lot not yet on auction");
	}

}




void testo::addbalance(name from, name to, asset amount, std::string m) {
	balance_index balances(get_self(), get_self().value);
	auto iterator = balances.find(from.value);
	if (iterator == balances.end())
	{
		balances.emplace(get_self(), [&from, &amount](auto & row)
		{
			row.user = from;
			row.amount = amount;
		});
	}
	else
	{
		balances.modify(iterator, get_self(), [&amount](auto & row)
		{
			row.amount += amount;
		});
	}

}


void testo::sendback (name user, uint64_t key) {

	action sendd = action (
	                   permission_level{get_self() , "active"_n},
	                   "testsen"_n,
	                   "take"_n,
	                   std::make_tuple(user, key )
	               );
	sendd.send();


};

uint64_t testo::findheighest(uint64_t lot) {
	asset hibid ;
	uint64_t hikey = 0;
	bid_index bids(_code, _code.value);
	for (auto iterator = bids.cbegin(); iterator != bids.cend(); iterator++ ) {
		if (iterator->lot == lot) {
			return iterator->key;
		}

	}

	return hikey;
}
