// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: Martin Porcheron
//         m.a.w.porcheron@swansea.ac.uk
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------
// Catch2 — https://github.com/catchorg/Catch2
// Catch2 is licensed under the BOOST license
// -----------------------------------------------------
// This file tests 371pass output for the database
// argument and 'read' value of the action argument.
// This test works by capturing the stdout of your
// program.
// -----------------------------------------------------

#include "../src/lib_catch.hpp"

#include <fstream>
#include <iostream>
#include <string>

#include "../src/lib_cxxopts.hpp"
#include "../src/lib_cxxopts_argv.hpp"

#include "../src/371pass.h"

// Redirect std::cout to a buffer
// by Björn Pollex
// via https://stackoverflow.com/a/5419388
// licensed under CC BY-SA 3.0.
class CoutRedirect {
private:
  std::streambuf *old;

public:
  CoutRedirect(std::streambuf *new_buffer)
      : old(std::cout.rdbuf(new_buffer)) { /* do nothing */
  }

  ~CoutRedirect() { std::cout.rdbuf(old); }
};

SCENARIO("The database and action program arguments can be parsed correctly "
         "such that a file can be opened, read, parsed, and output to stdout",
         "[args]") {

  const std::string filePath = "./tests/testdatabasealt.json";

  auto fileExists = [](const std::string &path) {
    return std::ifstream(path).is_open();
  };

  auto writeFileContents = [](const std::string &path,
                              const std::string &contents) {
    // Not a robust way to do this, but here it doesn't matter so much, if it
    // goes wrong we'll fail the test anyway…
    std::ofstream f{path};
    f << contents;
  };

  GIVEN("a valid path to a reset database JSON file") {

    // Reset the file...
    REQUIRE(fileExists(filePath));
    REQUIRE_NOTHROW(writeFileContents(
        filePath, "{\"Bank Accounts\":{\"Starling\":{\"Account "
                  "Number\":\"12345678\",\"Name\":\"Mr John Doe\",\"Sort "
                  "Code\":\"12-34-56\"}},\"Websites\":{\"Facebook\":{"
                  "\"password\":\"pass1234fb\",\"url\":\"https://"
                  "www.facebook.com/"
                  "\",\"username\":\"example@gmail.com\"},\"Google\":{"
                  "\"password\":\"pass1234\",\"url\":\"https://www.google.com/"
                  "\",\"username\":\"example@gmail.com\"},\"Twitter\":{"
                  "\"password\":\"r43rfsffdsfdsf\",\"url\":\"https://"
                  "www.twitter.com/\",\"username\":\"example@gmail.com\"}}}"));

    WHEN("the db program argument is '" + filePath +
         "' and the action program argument is 'read'") {

      Argv argvObj({"test", "--db", filePath.c_str(), "--action", "read"});
      auto **argv = argvObj.argv();
      auto argc = argvObj.argc();

      std::stringstream buffer;
      CoutRedirect originalBuffer{buffer.rdbuf()};

      THEN("the expected JSON output is printed to stdout") {

        REQUIRE_NOTHROW(App::run(argc, argv));

        std::string output = buffer.str();

        REQUIRE(
            output.find(
                "{\"Bank Accounts\":{\"Starling\":{\"Account "
                "Number\":\"12345678\",\"Name\":\"Mr John Doe\",\"Sort "
                "Code\":\"12-34-56\"}},\"Websites\":{\"Facebook\":{"
                "\"password\":\"pass1234fb\",\"url\":\"https://"
                "www.facebook.com/"
                "\",\"username\":\"example@gmail.com\"},\"Google\":{"
                "\"password\":\"pass1234\",\"url\":\"https://www.google.com/"
                "\",\"username\":\"example@gmail.com\"},\"Twitter\":{"
                "\"password\":\"r43rfsffdsfdsf\",\"url\":\"https://"
                "www.twitter.com/\",\"username\":\"example@gmail.com\"}}}") ==
            0);

      } // THEN

    } // WHEN

  } // GIVEN

} // SCENARIO