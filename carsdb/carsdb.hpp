
using namespace eosio;

class [[eosio::contract]] carsdb : public eosio::contract {
public:
	using contract::contract;
	carsdb(name receiver, name code, datastream<const char*> ds): contract(receiver, code,ds) {}
	
	[[eosio::action]]
	void insert( name user,std::string brand,std::string gov_number,std::string color,std::string engine_volume);


	[[eosio::action]]
	void erase(uint64_t key);

	[[eosio::action]]
	void modify(name user, uint64_t key, std::string brand, std::string gov_number, std::string color, std::string engine_volume);


	[[eosio::action]]
	void carauction(name user, std::string brand, std::string gov_number, std::string color, std::string engine_volume, uint64_t key);

	[[eosio::action]]
	void takewinner(name user, uint64_t key);

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

