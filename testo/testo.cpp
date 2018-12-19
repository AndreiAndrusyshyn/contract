#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/print.hpp>
using namespace eosio;

class [[eosio::contract]] testo: public eosio::contract {
public:
	using contract::contract; 
	testo(name receiver, name code, datastream<const char*> ds):contract(receiver, code, ds) {}

[[eosio::action]]
void add(name user , std::string brand, uint64_t key) {
	//name u = user;
	car_index cars(_self, _self.value);
	auto iterator = cars.find(user.value); /// CHANGED!!!!
	cars.emplace("testo"_n, [&](auto& row) {  //because payer for ram is contract testo!
		row.user = user;
		row.key = key;
		row.brand = brand;
		row.times = 0;
	});

	print("gi");
}
[[eosio::action]]
void erase(uint64_t key, uint64_t swit) {
		bid_index bids(_self, _code.value);
		auto iterator = bids.find(key);
		//car_index cars(_self, _code.value);
		//auto iterat = cars.find(key);
	//switch(swit) {
		//case 4: 
		
		
		//eosio_assert(iterator != bids.end(), "Record does not exist");
		bids.erase(iterator);
		//case 2:
		
		
		//eosio_assert(iterat != cars.end(), "Record does not exist");
		//cars.erase(iterat);

	//}
}


[[eosio::action]]
void auction (name user, uint64_t key, uint64_t times ) {
		uint64_t t = times + (now()/60);
		car_index cars(_code,  _code.value);
		auto iterator = cars.find(key);
		print("Car number:" ,iterator->key, "is on auction!");
		cars.modify(iterator, user , [&] (auto& row) {
			row.times = t;
		});
	
}

[[eosio::action]]
void pbid (name user, asset bidd, uint64_t lot) {
	//std::string bido = asset_to_string(bidd);
car_index cars(_code, _code.value);
auto iterato = cars.find(lot);
if (iterato->times <= (now()/60))  {

	print("Lot time expired!");


}
else { 
	sendmoney(user, "auction"_n , bidd, "m");
	bid_index bids(_code, _code.value);
	
	//auto iterator = bids.find(findheighest(lot));
	auto iterator = bids.cbegin();
	while(iterator != bids.cend()) {
	if(iterator->bidd > bidd) {
		print("bid more than:", iterator->bidd);
	}
	else {
	bids.emplace(user, [&](auto& row){
		row.key = bids.available_primary_key();
		row.user = user;
		row.bidd = bidd;
		row.lot = lot;

	});
}
}
}
	
}


[[eosio::action]]

void pra (name user, asset bidr) {
//std::string blah = bidd;

bid_index bids(_code, _code.value);

auto ite = bids.cbegin();
//print(blah);
while(ite != bids.end()) {
	if(ite->bidd == bidr) {
		print("YEAAHH");
	}
    else { 
    	print("NOO");
   }
   ite++;
}



}


[[eosio::action]]
void ptime ( ) {

	uint64_t  d = now();
	uint64_t sec_n = d % 60;
	uint64_t min_co = now()/60;
	uint64_t min_n = (now() / 60) % 60;
	uint64_t houdr_n = (now() / 3600) % 60;
	if(houdr_n > 24) {
		houdr_n = houdr_n - 24;
		houdr_n = 12 + houdr_n;

	}
	print("Hours: ", houdr_n, " Minutes: ", min_n, " Seconds: ", sec_n, "min_co" , min_co);
}

[[eosio::action]]
void status(uint64_t lot) { 

car_index cars(_code, _code.value);
bid_index bids(_code, _code.value);
auto iter =bids.find(findheighest(lot));
auto iterator = cars.find(lot);

if (iterator->times < (now()/60) ) {
	
		print("heighest bid:", iter->bidd, "winner is:" ,iter->user);
		
		sendback(iter->user, iterator->key);
}
else { 

	print("heighest bid:", iter->bidd); 

}

}
private:

	void sendbablo(name from,name to,  asset amoun, std::string d) {

		action sendd=action (
	permission_level{get_self() , "eosio.code"_n},
	"eosio.token"_n,
	"transfer"_n,
	std::make_tuple(from,to,  amoun ,d ) 
	);
	sendd.send();



	};


void sendback (name user, uint64_t key) {

action sendd=action (
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
	for(auto iterator= bids.cbegin(); iterator != bids.cend(); iterator++ ) {
		if(iterator->lot==lot && iterator->bidd > hibid) {
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


EOSIO_DISPATCH(testo, (add)(auction)(erase)(pbid)(ptime)(status)(pra))
