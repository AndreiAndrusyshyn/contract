
#include <eosiolib/print.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>

using namespace eosio;

class [[eosio::contract]] testo : public eosio::contract 
{
public:
	using contract::contract;

	[[eosio::action]]
	void add(name user, std::string brand, uint64_t key);

	[[eosio::action]]
	void eras(name key);

	[[eosio::action]]
	void auction(name user, uint64_t key, uint64_t times);

	[[eosio::action]]
	void pbid(name user, asset bidd, uint64_t lot);

	[[eosio::action]]
	void status(uint64_t lot);

	[[eosio::action]]
	void addbalance(name from, name to, asset amount, std::string m);
	
	void sendback(name user, uint64_t key);

	asset findheighest(uint64_t lot);

private:
	
	struct [[eosio::table]] bid {
		uint64_t key;
		name user;
		asset bidd;
		uint64_t lot;
		uint64_t primary_key() const {return key;}
		uint64_t by_lot() const {return lot;}

		EOSLIB_SERIALIZE(bid, (key)(user)(bidd)(lot))

	};
	typedef eosio::multi_index<"bids"_n, bid, indexed_by<"lot"_n, const_mem_fun<bid, uint64_t, &bid::by_lot>>> bid_index;

	struct [[eosio::table]] balance
	{	
		name user;
		asset amount;
		uint64_t primary_key() const {return user.value;}

		EOSLIB_SERIALIZE(balance, (user)(amount))
		
	};
	typedef eosio::multi_index<"balances"_n, balance> balance_index;

	struct [[eosio::table]] car
	{
		uint64_t key;
		name user;
		std::string brand;
		uint64_t times;
		uint64_t primary_key() const {return key;}
		uint64_t by_time() const {return times;}

		EOSLIB_SERIALIZE(car, (key)(user)(times))
	};
	typedef eosio::multi_index<"cars"_n, car, indexed_by<"times"_n, const_mem_fun<car, uint64_t, &car::by_time>>> car_index;

};

extern "C" {

	void apply( uint64_t receiver, uint64_t code, uint64_t action ) {

		if (code == name("eosio.token"_n).value && action == name("transfer"_n).value) {
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

