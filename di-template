#include <string>

/*
	Attempt to create dependency injections for easier testing without using dynamic polimorphism

	Implementation is seperated into helper classes from the real usage classes.
	This is to avoid using complex template types or auto to hold the instances

	Updise:
		- No vtables
		- No Interfaces
	Downside:
		- A new class has to be used if the helper/base class is required to be used with different dependecies.
		- coupeling must be decided at compile time
*/

////////////////////////////////////////////////
//
//		DB objects
//

struct user_obj
{
	std::string user_no;
	std::string twitter_id;
	//+++

	void update() {}
	bool read() { return true; }
};

struct user_game
{
	std::string user_no;
	std::string game_name;

	void update() {}
	bool read() { return true; }
};
struct user_badge
{
	std::string user_no;
	std::string badge;

	void update() {}
	bool read() { return true; }
};


////////////////////////////////////////////////
//
//		Extern api
//

struct twiter_api
{
	void send_message(const std::string& user_id, const std::string& message)
	{
		// sending message to twitter
	}
};


////////////////////////////////////////////////
////////////////////////////////////////////////
//
//		Components
//
template<typename t, typename g>
struct move_game_helper
{
	void move(const std::string& from_user_no, const std::string& to_user_no)
	{
		g game;
		game.user_no = from_user_no;
		if (!game.read())				// real life would be multiple 
			return;
		game.user_no = to_user_no;
		game.update();
		// some special stuff only for games
	}	
};
template<typename g>
struct move_game_base : public move_game_helper<move_game_base<g>, g> {};

// Implementations
struct move_game : public move_game_helper<move_game, user_game> {};





template<typename t, typename b>
struct move_badges_helper
{
	void move(const std::string& from_user_no, const std::string& to_user_no)
	{
		b badge;
		badge.user_no = from_user_no;
		if (!badge.read())				// real life would be multiple 
			return;
		badge.user_no = to_user_no;
		badge.update();
		// some special stuff only for badges
	}
};
template<typename b>
struct move_badge_base : public move_badges_helper<move_badge_base<b>, b> {};

// Implementations
struct move_badge : public move_badges_helper<move_badge, user_badge> {};


// twitter_message_helper can easely be used other places. 
template<typename t, typename u, typename tw>
struct twitter_message_helper
{
	void message(const std::string& user_no, const std::string& message)
	{
		u user;
		user.user_no = user_no;
		user.read();

		tw api;
		api.send_message(user.twitter_id, message);
	}
};
template<typename u, typename tw>
struct twitter_message_base : public twitter_message_helper<twitter_message_base<u, tw>, u, tw> {};

// Implementations
struct twitter_message : public twitter_message_helper<twitter_message, user_obj, twiter_api> {};



////////////////////////////////////////////////
////////////////////////////////////////////////
//
//		Composition
//
template<typename t, typename g, typename b, typename tw>
struct merge_user_helper
{
	void merge(const std::string& from_user_no, const std::string& to_user_no)
	{
		g move_game;
		move_game.move(from_user_no, to_user_no);

		b move_badge;
		move_badge.move(from_user_no, to_user_no);
		

		tw twitter_move_message;
		twitter_move_message.message(from_user_no, "I was move to user: "+to_user_no);
		twitter_move_message.message(to_user_no, "user: " + from_user_no + " was moved to me");

	}
};
//template<typename g, typename b, typename tw>
//struct merge_user_base : public merge_user_helper<merge_user_base<g, b, tw>, g, b, tw> {};

// Implementations
struct merge_user : public merge_user_helper<merge_user, move_game, move_badge, twitter_message> {};
// add new empty classes for other special implementations of merge_user_detail


////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
//
//		TESTING classes - used by unit-tests
//

// mock objects
// Mocked objects return known predefined data without accesssing third party services 
struct user_obj_mock
{
	std::string user_no;
	std::string twitter_id;
	//+++

	void update() {}
	bool read() { return true; }
};
struct user_game_mock
{
	std::string user_no;
	std::string game_name;

	void update() {}
	bool read() { return true; }
};
struct user_badge_mock
{
	std::string user_no;
	std::string badge;

	void update() {}
	bool read() { return true; }
};

struct twiter_api_mock
{
	void send_message(const std::string& user_id, const std::string& message)
	{
		// do not send message to twitter. simulate sending it only
	}
};

// classes to be tested are injected with mock object so not to hit db or other services

// Test classes for each component
struct move_badge_test : public move_badges_helper<move_badge_test, user_badge_mock> {};
struct move_game_test : public move_game_helper<move_game_test, user_game_mock> {};
struct twitter_message_test : public twitter_message_helper<twitter_message_test, user_obj_mock, twiter_api_mock> {};


// Test classes for composition object. 
// One injected with pre made comosition test classes and the other injected with custom mock objects
struct merge_user_test : public merge_user_helper<merge_user_test, move_game_test, move_badge_test, twitter_message_test> {};
struct merge_user_custom_test :		
	public merge_user_helper<
		merge_user_custom_test,
		move_game_base<user_game_mock>, 
		move_badge_base<user_badge_mock>,
		twitter_message_base<user_obj_mock, twiter_api_mock>
							
	> {};


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

int main()
{

	merge_user merge;
	merge.merge("123", "321");

	merge_user_test merge_test;
	merge_test.merge("123", "321");
	
	merge_user_custom_test custom_merge_test;
	custom_merge_test.merge("123", "321");

	return 0;
}
