
// time == time , every time you add bidder

// 



#include <eosiolib/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] auction : public eosio::contract {

public:
	using contract::contract;

	auction(name receiver, name code, datadstream<const char*> ds): contract(receiver, code, ds) {}


	[[eosio::action]]
	void start_auction(uint64_t key,name user, uint64_t time ) {
		require_auth(name("carsdb"));
		std::list<int> bid ; 
		int winner;
		if (time_comp()+time*60000000 == time_comp()) {

			printf("Auction end\n");
		}
		else { 


		}

	}

	//[[eosio::action]]
	void take_participation (name user, uint64_t token_bid)  {
		// car_on_auction_index cars(_self, _self.value);
		// auto iterator = cars.find(time.value);
	
		user_bid_index users(_code, _code.value);
		auto iterator = users.find(user.value);

		users.emplace(name user, [&](auto& row) {

			row.user = user;
			row.token_bid = token_bid;

	
	});

}
private: 
	// struct  [[eosio::table]] car_on_auction 
	// {
	// 		name user;
	// 		uint64_t key;
	// 		uint64_t time;

	// 		uint64_t primary_key() const { return key;}
	// 		EOSLIB_SERIALIZE(car_on_auction, (user)(key)(time))
		
	// };
   float time_comp() {
   float sec, minutes, hours, day,years;
   sec = current_time()/1000000;
   minutes = sec/60;
   hours = minutes/60;
   day = hours/24;
   years = day/365;
 //  printf("Year: %f, Day: %f, Hours: %f, Minutes: %f, Seconds: %f", years, day, hours, minutes, sec);
   return minutes;
}
	struct [[eosio::table]] user_bid
	{	
		name user;
		uint64_t token_bid;

		uint64_t primary_key() const { return key;}
		EOSLIB_SERIALIZE(user_bid, (user)(token_bid))

	};

	//typedef eosio::multi_index<"cars_auction"_n, car_on_auction> car_on_auction_index;
	typedef eosio::multi_index<"user_and_bid"_n, user_bid> user_bid_index;
};



EOSIO_DISPATCH(auction, (start_auctiont)(take_participation))