#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/print.hpp>
using namespace eosio;


class [[eosio::contract]] testo: public eosio::contract {
public:
	using contract::contract;
	testo(name receiver, name code, datastream<const char*> ds): contract(receiver, code, ds) {}

	[[eosio::action]]
	void add(name user , std::string brand, uint64_t key) {
		car_index cars(_self, _self.value);
		auto iterator = cars.find(user.value); /// CHANGED!!!!
		cars.emplace("testo"_n, [&](auto & row) { //because payer for ram is contract testo!
			row.user = user;
			row.key = key;
			row.brand = brand;
			row.times = 0;
		});

		print("gi");
	}
	[[eosio::action]]
	void eras(uint64_t key) {

		balance_index balances(_self, _code.value);
		auto iterator = balances.find(key);

		balances.erase(iterator);

	}


	[[eosio::action]]
	void auction (name user, uint64_t key, uint64_t times ) {
		uint64_t t = times + (now() / 60);
		car_index cars(_code,  _code.value);
		auto iterator = cars.find(key);
		print("Car number:" , iterator->key, "is on auction!");
		cars.modify(iterator, user , [&] (auto & row) {
			row.times = t;
		});

	}

	[[eosio::action]]
	void pbid (name user, asset bidd, uint64_t lot) {
		car_index cars(_code, _code.value);
		auto iterato = cars.find(lot);
		if (iterato->times <= (now() / 60))  {

			print("Lot time expired!");


		}
		else {

			bid_index bids(_code, _code.value);

			auto iterator = bids.find(user.value);

			if (iterator == bids.cend() && iterator == bids.cbegin()) {
				bids.emplace(user, [&](auto & row) {
					row.key = bids.available_primary_key();
					row.user = user;
					row.bidd = bidd;
					row.lot = lot;

				});
			}
			else {
				iterator = bids.cbegin();
				for (; iterator != bids.cend(); ) {
					if (iterator != bids.cend() && iterator->lot == lot && iterator->bidd >= bidd) {
						print("bid more than:", iterator->bidd);

						break;
					}
					else {
						bids.emplace(user, [&](auto & row) {
							row.key = bids.available_primary_key();
							row.user = user;
							row.bidd = bidd;
							row.lot = lot;

						});
					}
				}


				iterator++;
			}
		}


	}

	[[eosio::action]]
	void status(uint64_t lot) {

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

	}


	[[eosio::action]]
	void addbalance(name from, name to, asset quantity, std::string m) {
		balance_index balances(_code, _code.value);
		frist.key = balances.available_primary_key();
		frist.user = from;
		frist.quan = quantity;

		memcpy(&frist_copy, &frist, sizeof(frist));

		auto iterator = balances.find(frist_copy.user.value);

		if(iterator == balances.cend()) {
				balances.emplace("testo"_n, [&](auto& row){
					row.key = frist_copy.user;
					row.user = frist_copy.user;
					row.amount = frist_copy.quan;

				});

		} 

		else {	
			balances.modify(iterator, "testo"_n, [&](auto& row) {
				row.amount = row.amount+frist_copy.quan;
			});

		}

	}

private:

	struct b
	{
		uint64_t key;
		name user;
		asset quan;
	} frist, frist_copy ;



	void sendback (name user, uint64_t key) {

		action sendd = action (
		                   permission_level{get_self() , "active"_n},
		                   "testsend"_n,
		                   "take"_n,
		                   std::make_tuple(user, key )
		               );
		sendd.send();


	};

	uint64_t findheighest(uint64_t lot) {
		asset hibid ;
		uint64_t hikey = 0;
		bid_index bids(_code, _code.value);
		for (auto iterator = bids.cbegin(); iterator != bids.cend(); iterator++ ) {
			if (iterator->lot == lot && iterator->bidd > hibid) {
				hikey = iterator->key;
				hibid = iterator->bidd;
			}

		}

		return hikey;

	};

	struct [[eosio::table]] bid {
		uint64_t key;
		name user ;
		asset bidd;
		uint64_t lot;
		uint64_t primary_key() const { return key;}
		//asset by_bid() {return bidd;}
		uint64_t by_lot() const {return lot;}


	};
	typedef eosio::multi_index<"bids"_n, bid, indexed_by<"lot"_n, const_mem_fun<bid, uint64_t, &bid::by_lot>>>bid_index;


	struct [[eosio::table]] balance
	{
		name key;
		name user;
		asset amount;
		uint64_t primary_key() const {return key.value;}

	};

	typedef eosio::multi_index<"balances"_n, balance> balance_index;

	struct [[eosio::table]] car {
		uint64_t key;
		name user;
		std::string brand;
		uint64_t times;
		uint64_t primary_key() const { return key;}
		uint64_t get_secondary_1() const { return times;}

	};

	typedef eosio::multi_index<"cars"_n, car, indexed_by<"times"_n, const_mem_fun<car, uint64_t, &car::get_secondary_1>>> car_index;

};


extern "C" {

	void apply( uint64_t receiver, uint64_t code, uint64_t action ) {

		if (code == name("eosio.token").value && action == name("transfer").value) {
			execute_action(name(receiver), name(receiver), &testo::addbalance );

		}
		else if (code == receiver && action == name("add").value) {
			execute_action(name(receiver), name(code), &testo::add );

		}
		else if (code == receiver && action == name("auction").value) {
			execute_action(name(receiver), name(code), &testo::auction );

		}

		else if (code == receiver && action == name("eras").value) {
			execute_action(name(receiver), name(code), &testo::eras );

		}
		else if (code == receiver && action == name("pbid").value) {
			execute_action(name(receiver), name(code), &testo::pbid );

		}
		else if (code == receiver && action == name("status").value) {
			execute_action(name(receiver), name(code), &testo::status );

		}
		else if (code == receiver && action == name("addbalance").value) {
			execute_action(name(receiver), name(code), &testo::addbalance );

		}


	}
}

