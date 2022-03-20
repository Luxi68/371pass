// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 853761
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "371pass.h"
#include "lib_json.hpp"
#include "lib_cxxopts.hpp"
#include "wallet.h"

// TODO Complete this function. You have been provided some skeleton code which
//  retrieves the database file name from cxxopts.
//  1. Load the database file by calling load() on a Wallet object
//  2. Parse the 'action' argument to decide what action should be taken (
//     create, read, update, or delete). Read is the easiest to implement, and
//     update is the hardest. The details of how these arguments work is in the
//     coursework specification.
//  4. Save the updated Wallet object to the JSON file if there have been
//     changes (calling save() on the Wallet object).
//
// Some commented out code has been provided. Using some of this will be
// demonstrated in the coursework video on Canvas. Remember, this coursework is
// meant to be challenging and testing your understanding of programming in C++.
// Part of the challenge is figuring things out on your own. That is a major
// part of software development.
//
// Example:
//  int main(int argc, char *argv[]) { return App::run(argc, argv); }
int App::run(int argc, char *argv[]) {
	auto options = App::cxxoptsSetup();
	auto args = options.parse(argc, argv);

	// Print the help usage if requested
	if (args.count("help")) {
		std::cout << options.help() << '\n';
		return 0;
	}

	// Open the database and construct the Wallet
	const std::string db = args["db"].as<std::string>();
	Wallet wObj{};
	// Only uncomment this once you have implemented the load function!
	wObj.load(db);
	
	// Trying to parse action to catch any exeptions
	try {
		parseActionArgument(args);
	} catch (std::invalid_argument const&) {
		std::cerr << "Error: invalid action argument(s)." << std::endl;
		return 1;
	} catch (cxxopts::option_has_no_value_exception const&) {
		std::cerr << "Error: missing action argument(s)." << std::endl;
		return 1;
	} catch (cxxopts::missing_argument_exception const&) {
		std::cerr << "Error: missing action argument(s)." << std::endl;
		return 1;
	}

	const Action a = parseActionArgument(args);
	const Objs o = parseObjsArgument(args);
	switch (a) {
	case Action::CREATE:
		switch (o) {
		case Objs::NONE: //trying to creating with no arguments
			std::cerr << "Error: missing category, item or entry argument(s)." << std::endl;
			return 1;

		case Objs::CATEGORY: {
			const std::string category = args["category"].as<std::string>();

			Category cObj{category};
			// std::cout << category << std::endl;
			// std::cout << cObj.getIdent() << std::endl;
			try {
				wObj.addCategory(cObj);
			} catch (std::runtime_error const&) {
				std::cerr << "Error: failed to insert category " << category << " into wallet." << std::endl;
				return 1;
			}

			wObj.save(db);
			break;
		}
		case Objs::ITEM: {
			const std::string category = args["category"].as<std::string>();
			const std::string item = args["item"].as<std::string>();

			Category cObj{category};
			Item iObj{item};

			cObj.addItem(iObj);
			try {
				wObj.addCategory(cObj);
			} catch (std::runtime_error const&) {
				std::cerr << "Error: failed to insert category " << category << " into wallet." << std::endl;
				return 1;
			}

			wObj.save(db);
			break;
		}
		case Objs::ENTRY: {
			const std::string category = args["category"].as<std::string>();
			const std::string item = args["item"].as<std::string>();
			const auto entry = splitStr((args["entry"].as<std::string>()), ',');

			Category cObj{category};
			Item iObj{item};

			if(entry.size() == 2) { //expected output
				iObj.addEntry(entry.at(0), entry.at(1));
			} else if (entry.size() == 1) { //if entry is missing value
				iObj.addEntry(entry.at(0), "");
			} else { //TODO add option of creating more than one entry pairs
				std::cerr << "Error: invalid entry argument(s)." << std::endl;
				return 1;
			}
			cObj.addItem(iObj);

			try {
				wObj.addCategory(cObj);
			} catch (std::runtime_error const&) {
				std::cerr << "Error: failed to insert category " << category << " into wallet." << std::endl;
				return 1;
			}			

			wObj.save(db);
			break;
		}
		default:
			std::cerr << "Error: unexpected arguments for create action." << std::endl;
			return 1;
		}
		break;

	case Action::READ:
		switch (o) {
		case Objs::NONE:
			std::cout << nlohmann::json::parse(getJSON(wObj)) << std::endl;
			break;
			
		case Objs::CATEGORY: {
			const std::string category = args["category"].as<std::string>();
			std::cout << nlohmann::json::parse(getJSON(wObj, category)) << std::endl;
			break;
		}
		case Objs::ITEM: {
			const std::string category = args["category"].as<std::string>();
			const std::string item = args["item"].as<std::string>();
			std::cout << nlohmann::json::parse(getJSON(wObj, category, item)) << std::endl;
			break;
		}
		case Objs::ENTRY: {
			const std::string category = args["category"].as<std::string>();
			const std::string item = args["item"].as<std::string>();
			const std::string entry = args["entry"].as<std::string>();
			std::cout << getJSON(wObj, category, item, entry) << std::endl;
			break;
		}
		default:
			std::cerr << "Error: unexpected arguments for read action." << std::endl;
			return 1;
		}
		break;

	case Action::UPDATE:
		throw std::runtime_error("update not implemented");
		break;

	case Action::DELETE:
		throw std::runtime_error("delete not implemented");
		break;

	default:
		throw std::runtime_error("Unknown action not implemented");
	}

	return 0;
}

// Create a cxxopts instance. You do not need to modify this function.
//
// Example:
//  auto options = App::cxxoptsSetup();
//  auto args = options.parse(argc, argv);
cxxopts::Options App::cxxoptsSetup() {
	cxxopts::Options cxxopts("371pass", "Student ID: " + STUDENT_NUMBER + "\n");

	cxxopts.add_options()(
		"db", "Filename of the 371pass database",
		cxxopts::value<std::string>()->default_value("database.json"))(

		"action", "Action to take, can be: 'create', 'read', 'update', 'delete'.",
		cxxopts::value<std::string>())(

		"category",
		"Apply action to a category (e.g., if you want to add a category, set the"
		" action argument to 'add' and the category argument to your chosen"
		" category identifier).",
		cxxopts::value<std::string>())(

		"item",
		"Apply action to an item (e.g., if you want to add an item, set the "
		"action argument to 'add', the category argument to your chosen category "
		"identifier and the item argument to the item identifier).",
		cxxopts::value<std::string>())(

		"entry",
		"Apply action to an entry (e.g., if you want to add an entry, set the "
		"action argument to 'add', the category argument to your chosen category "
		"identifier, the item argument to your chosen item identifier, and the "
		"entry argument to the string 'key,value'). If there is no comma, an "
		"empty entry is inserted. If you are simply retrieving an entry, set the "
		"entry argument to the 'key'. If you are updating an entry key, use a : "
		"e.g., oldkey:newkey,newvalue.",
		cxxopts::value<std::string>())(

		"h,help", "Print usage.");

	return cxxopts;
}

// TODO Rewrite this function so that it works. This function should
//  case-insensitively check the action argument retrieved from cxxopts and
//  convert this to a value from the ACTION enum. If an invalid value is given
//  in a string, throw an std::invalid_argument exception.
//
// Example:
//  auto options = App::cxxoptsSetup();
//  auto args = options.parse(argc, argv);
//  App::Action action = parseActionArgument(args);
App::Action App::parseActionArgument(cxxopts::ParseResult &args) {
	std::string input = args["action"].as<std::string>();

	std::for_each(input.begin(), input.end(), [](char &c)
				{ c = ::tolower(c); });

	if (input == "create") {
		return Action::CREATE;
	} else if (input == "read") {
		return Action::READ;
	} else if (input == "update") {
		return Action::UPDATE;
	} else if (input == "delete") {
		return Action::DELETE;
	} else {
		throw std::invalid_argument("action");
	}	
}

// A function which checks the arguments relating to category, item and entry
// values and converts this into a value from the OBJS enum to determine what
// specific actions are to be taken.
App::Objs App::parseObjsArgument(cxxopts::ParseResult &args){
	bool hasCategoryInput = true;
	try {args["category"].as<std::string>();
	} catch (cxxopts::option_has_no_value_exception const&) {
		hasCategoryInput = false;
	} catch (cxxopts::missing_argument_exception const&) {
		std::cerr << "Error: missing category argument(s)." << std::endl;
		exit(1);
	}

	bool hasItemInput = true;
	try {args["item"].as<std::string>();
	} catch (cxxopts::option_has_no_value_exception const&) {
		hasItemInput = false;
	} catch (cxxopts::missing_argument_exception const&) {
		std::cerr << "Error: missing item argument(s)." << std::endl;
		exit(1);
	}

	bool hasEntryInput = true;
	try {args["entry"].as<std::string>();
	} catch (cxxopts::option_has_no_value_exception const&) {
		hasEntryInput = false;
	} catch (cxxopts::missing_argument_exception const&) {
		std::cerr << "Error: missing entry argument(s)." << std::endl;
		exit(1);
	}

	if(hasCategoryInput) {
		if(hasItemInput) {
			if(hasEntryInput) {
				// 111
				return Objs::ENTRY;
			} else {
				// 110
				return Objs::ITEM;
			}
		} else {
			if (hasEntryInput) {
				// 101
				// return Objs::MISSITE;
				std::cerr << "Error: missing item argument(s)." << std::endl;
				exit(1);
			} else {
				// 100
				return Objs::CATEGORY;
			}
		}
	} else {
		if(hasItemInput) {
			// 011, 010
			// return Objs::MISSCAT;
			std::cerr << "Error: missing category argument(s)." << std::endl;
			exit(1);
		} else {
			if(hasEntryInput) {
				// 001
				// return Objs::MISSCAT;
				std::cerr << "Error: missing category argument(s)." << std::endl;
				exit(1);
			} else {
				//000
				return Objs::NONE;
			}
		}
	}
}

// TODO Write a function, getJSON, that returns a std::string containing the
//  JSON representation of a Wallet object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  Wallet wObj{};
//  std::cout << getJSON(wObj);
std::string App::getJSON(Wallet &wObj) {
	// Only uncomment this once you have implemented the functions used!
	return wObj.str();
}

// TODO Write a function, getJSON, that returns a std::string containing the
//  JSON representation of a specific Category in a Wallet object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  Wallet wObj{};
//  std::string c = "category argument value";
//  std::cout << getJSON(wObj, c);
std::string App::getJSON(Wallet &wObj, const std::string &c) {
	// Only uncomment this once you have implemented the functions used!
	try {
		const auto cObj = wObj.getCategory(c);
		return cObj.str();
	} catch(std::out_of_range const&) {
		std::cerr << "Error: invalid category argument(s)." << std::endl;
		exit(1);
	}
}

// TODO Write a function, getJSON, that returns a std::string containing the
//  JSON representation of a specific Item in a Wallet object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  Wallet wObj{};
//  std::string c = "category argument value";
//  std::string i = "item argument value";
//  std::cout << getJSON(wObj, c, i);
std::string App::getJSON(Wallet &wObj, const std::string &c,
						 const std::string &i) {
	// Only uncomment this once you have implemented the functions used!
	try {
		const auto cObj = wObj.getCategory(c);
		try {
			const auto iObj = cObj.getItem(i);
			return iObj.str();
		} catch (std::out_of_range const&) {
			std::cerr << "Error: invalid item argument(s)." << std::endl;
			exit(1);
		}
	} catch(std::out_of_range const&) {
		std::cerr << "Error: invalid category argument(s)." << std::endl;
		exit(1);
	}
}

// TODO Write a function, getJSON, that returns a std::string containing the
//  JSON representation of a specific Entry in a Wallet object.
//
// This function has been implemented for you, but you may wish to adjust it.
// You will have to uncomment the code, which has been left commented to ensure
// the coursework framework compiles (i.e., it calls functions that you must
// implement, once you have implemented them you may uncomment this function).
//
// Example:
//  Wallet wObj{};
//  std::string c = "category argument value";
//  std::string i = "item argument value";
//  std::string e = "entry argument value";
//  std::cout << getJSON(wObj, c, i, e);
std::string App::getJSON(Wallet &wObj, const std::string &c,
						 const std::string &i, const std::string &e) {
	// Only uncomment this once you have implemented the functions used!
	try {
		const auto cObj = wObj.getCategory(c);
		try {
			const auto iObj = cObj.getItem(i);
			try {
				return iObj.getEntry(e);
			} catch(std::out_of_range const&) {
				std::cerr << "Error: invalid entry argument(s)." << std::endl;
				exit(1);
			}
		} catch (std::out_of_range const&) {
			std::cerr << "Error: invalid item argument(s)." << std::endl;
			exit(1);
		}
	} catch(std::out_of_range const&) {
		std::cerr << "Error: invalid category argument(s)." << std::endl;
		exit(1);
	}
}

// This function takes in a string and the requested delimiter and returns
// the result in a vector
std::vector<std::string> App::splitStr(std::string str, const char delim) {
	std::stringstream sstr(str);
	std::vector<std::string> output;

	while (sstr.good()){
		std::string substr;
		getline(sstr, substr, delim);

		output.push_back(substr);
	}
	return output;
}