// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: <STUDENT NUMBER>
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------
// This file contains declarations for the helper
// functions used for initialising and running 371pass.
// -----------------------------------------------------

#ifndef _371PASS_H
#define _371PASS_H

#include <string>

#include "lib_cxxopts.hpp"
#include "wallet.h"

namespace App {

// TODO: Enter your student number here!
const std::string STUDENT_NUMBER = "987654";

// Enums (short for enumerations) are similar to their Java implementation.
// It is a user-defined type, used to assign names to internal constants
// in code, instead of simply passing in integers/strings etc.
//
// For example, functions can take a value/constant from a specific enum
// and use it in a switch statement, doing different things
//
// As such, it is a useful way for us to specify information in both a
// machine and human-readable format.
//
// Unlike Java, enums in C++ only map to integer values. You can either let
// the compiler generate the values automatically, in which it allocates a
// unique integer (0-indexed). Or, you can set the value by giving the name
// followed by = <value> (e.g. CREATE=0).
//
// This enum specifies the four different values we support in the action
// program argument.
enum Action { CREATE, READ, UPDATE, DELETE };

int run(int argc, char *argv[]);

cxxopts::Options cxxoptsSetup();

App::Action parseActionArgument(cxxopts::ParseResult &args);

std::string getJSON(Wallet &w);
std::string getJSON(Wallet &w, const std::string &c);
std::string getJSON(Wallet &w, const std::string &c, const std::string &i);
std::string getJSON(Wallet &wObj, const std::string &c, const std::string &i,
                    const std::string &e);

} // namespace App

#endif // _371PASS_H