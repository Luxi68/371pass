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
// This file contains tests saving JSON files from a
// the Wallet.
// -----------------------------------------------------

#include "../src/lib_catch.hpp"

#include <fstream>
#include <sstream>
#include <string>

#include "../src/lib_cxxopts.hpp"
#include "../src/lib_cxxopts_argv.hpp"

#include "../src/wallet.h"

SCENARIO("A Wallet object can save to a JSON file", "[wallet]") {

  const std::string filePath = "./tests/testdatabasealt.json";

  auto fileExists = [](const std::string &path) {
    return std::ifstream(path).is_open();
  };

  auto readFileContents = [](const std::string &path) {
    // Not a robust way to do this, but here it doesn't matter so much, if it
    // goes wrong we'll fail the test anyway…
    std::stringstream ss{std::stringstream::out};
    ss << std::ifstream(path).rdbuf();
    return ss.str();
  };

  auto writeFileContents = [](const std::string &path,
                              const std::string &contents) {
    // Not a robust way to do this, but here it doesn't matter so much, if it
    // goes wrong we'll fail the test anyway…
    std::ofstream f{path};
    f << contents;
  };

  GIVEN("a valid path to an empty database JSON file") {

    // Reset the file...
    REQUIRE(fileExists(filePath));
    REQUIRE_NOTHROW(writeFileContents(filePath, "{}"));

    WHEN("a new empty Wallet object is constructed") {

      Wallet wObj1{};
      REQUIRE(wObj1.empty());

      const std::string ident1 = "ident1";
      const std::string ident2 = "ident2";

      const std::string key1 = "key1";
      const std::string key2 = "key2";

      const std::string value1 = "value1";
      const std::string value2 = "value2";

      AND_WHEN("the Wallet object is populated with dummy data") {

        Item iObj1{ident1};
        Item iObj2{ident2};

        iObj1.addEntry(key1, value1);
        iObj1.addEntry(key2, value2);
        iObj2.addEntry(key1, value1);

        REQUIRE(iObj1.size() == 2);
        REQUIRE(iObj2.size() == 1);

        Category cObj1{ident1};
        Category cObj2{ident2};

        cObj1.addItem(iObj1);
        cObj1.addItem(iObj2);
        cObj2.addItem(iObj1);

        REQUIRE(cObj1.size() == 2);
        REQUIRE(cObj2.size() == 1);

        wObj1.addCategory(cObj1);
        wObj1.addCategory(cObj2);

        REQUIRE(wObj1.size() == 2);

        THEN("the saving to file will not throw an exception") {

          REQUIRE_NOTHROW(wObj1.save(filePath));

          AND_THEN("the file contents will be correct") {

            REQUIRE(
                readFileContents(filePath).find(
                    "{\"ident1\":{\"ident1\":{\"key1\":\"value1\",\"key2\":"
                    "\"value2\"},\"ident2\":{\"key1\":\"value1\"}},\"ident2\":{"
                    "\"ident1\":{\"key1\":\"value1\",\"key2\":\"value2\"}}}") ==
                0);

          } // AND_THEN

        } // THEN

      } // AND_WHEN

    } // WHEN

  } // GIVEN

} // SCENARIO
