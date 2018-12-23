
using namespace eosio;

class [[eosio::contract]] testsen : public eosio::contract {
public:
	using contract::contract;
	testsen(name receiver, name code, datastream<const char*> ds): contract(receiver, code, ds) {}

	[[eosio::action]]
	void add (name user , std::string brand );

	[[eosio::action]]
	void take(name user , uint64_t key);

	[[eosio::action]]
	void erase(uint64_t key);

	[[eosio::action]]
	void auction(name user, uint64_t key);

	void sendcar( name user, std::string brand, uint64_t key);
private:

	struct [[eosio::table]] car {
		uint64_t key;
		name user;
		std::string brand;
		uint64_t primary_key() const { return key;}

		EOSLIB_SERIALIZE(car, (key)(user)(brand))

	};

	typedef eosio::multi_index<"cars"_n, car> car_index;

};

EOSIO_DISPATCH(testsen, (add)(take)(erase)(auction))

