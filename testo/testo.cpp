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

void testo::eras(uint64_t key) {
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

void testo::finish(uint64_t lot) {

		bid_index bids(_self, _self.value);

		balance_index balances(_self, _self.value);
		auto itbal = balances.cbegin();
		auto it = bids.get_index<"lot"_n>();
		auto itt = it.find(lot);

		car_index cars(_self, _self.value);
		auto itcar = cars.find(lot);

		itbal = balances.find(itcar->user.value);
		balances.modify(itbal, get_self(), [&](auto& row ){
			row.amount += itt->bidd;
		});

		
		sendback(itt->user, lot);

}

bool testo::checknotnull(uint64_t lot) {
	car_index cars (_self, _self.value);
	auto it = cars.find(lot);
	return ( it->times == 0 ?  true : false );
}

bool testo::checktime( uint64_t lot) {
	car_index cars(_self, _self.value);
	auto iterator = cars.find(lot);

	return (iterator->times > (now() / 60) ? true : false );

}

bool testo::checkbal(name user, asset bidd) {

	balance_index balances(_self, _self.value);
	auto itbal = balances.find(user.value);

	return (itbal->amount < bidd ? true : false);
}

void testo::pbid(name user, asset bidd , uint64_t lot ) {

	bid_index bids(_self, _self.value);
	auto it = bids.get_index<"lot"_n>();

	auto itt = it.find(lot);
	auto er = bids.cbegin(); 
	balance_index balances(_self, _self.value);
	auto itbal = balances.find(user.value);
	

	if (checknotnull(lot) == true) { print("Car not on auct"); return;}
	else if(checkbal(user, bidd)== true) {print("Your balance is lesser than your bidd"); return;}
	else if (checktime(lot) == false) {
		finish(lot); 
		

		return;
	}



	itbal = balances.find(user.value);


	

	if (itt == it.end() ) {

		bids.emplace("testo"_n, [&](auto & row) {
			row.key = bids.available_primary_key();
			row.user = user;
			row.bidd = bidd;
			row.lot = lot;
		}) ;

		balances.modify(itbal, user, [&](auto & row) {
			row.amount -= bidd;
		});
	}
	else if (bidd > itt->bidd) {
		itbal = balances.find(itt->user.value);
		balances.modify(itbal, user, [&](auto & row) {
			row.amount += itt->bidd;
		});


		er= bids.find(itt->key);
		bids.erase(er);

		bids.emplace("testo"_n, [&](auto & row ) {

			row.key = bids.available_primary_key();
			row.user = user;
			row.bidd = bidd;
			row.lot = lot;
		});
		itbal = balances.find(user.value);
		balances.modify(itbal, user, [&](auto & row) {
			row.amount -= bidd;
		});


	}
	else {
		print("your bid is smaller than heighest, bid more than: ", itt->bidd);
	}


}