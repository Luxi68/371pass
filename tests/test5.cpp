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
// This file contains tests loading JSON files into
// the Wallet.
// -----------------------------------------------------

#include "../src/lib_catch.hpp"

#include <fstream>
#include <string>

#include "../src/lib_cxxopts.hpp"
#include "../src/lib_cxxopts_argv.hpp"

#include "../src/wallet.h"

SCENARIO("A Wallet object can load from a JSON file", "[wallet]") {

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

    WHEN("a new empty Wallet object is constructed") {

      Wallet wObj1{};
      REQUIRE(wObj1.empty());

      AND_WHEN("the load function is called with the file path") {

        THEN("the file will be imported without an exception") {

          REQUIRE_NOTHROW(wObj1.load(filePath));
          REQUIRE(wObj1.size() == 2);

          REQUIRE_NOTHROW(wObj1.getCategory("Websites"));
          REQUIRE(wObj1.getCategory("Websites").size() == 3);

          REQUIRE_NOTHROW(wObj1.getCategory("Websites").getItem("Google"));
          REQUIRE(wObj1.getCategory("Websites").getItem("Google").size() == 3);
          REQUIRE(
              wObj1.getCategory("Websites").getItem("Google").getEntry("url") ==
              "https://www.google.com/");
          REQUIRE(wObj1.getCategory("Websites")
                      .getItem("Google")
                      .getEntry("username") == "example@gmail.com");
          REQUIRE(wObj1.getCategory("Websites")
                      .getItem("Google")
                      .getEntry("password") == "pass1234");

          REQUIRE_NOTHROW(wObj1.getCategory("Websites").getItem("Facebook"));
          REQUIRE(wObj1.getCategory("Websites").getItem("Facebook").size() ==
                  3);
          REQUIRE(wObj1.getCategory("Websites")
                      .getItem("Facebook")
                      .getEntry("url") == "https://www.facebook.com/");
          REQUIRE(wObj1.getCategory("Websites")
                      .getItem("Facebook")
                      .getEntry("username") == "example@gmail.com");
          REQUIRE(wObj1.getCategory("Websites")
                      .getItem("Facebook")
                      .getEntry("password") == "pass1234fb");

          REQUIRE_NOTHROW(wObj1.getCategory("Websites").getItem("Twitter"));
          REQUIRE(wObj1.getCategory("Websites").getItem("Twitter").size() == 3);
          REQUIRE(wObj1.getCategory("Websites")
                      .getItem("Twitter")
                      .getEntry("url") == "https://www.twitter.com/");
          REQUIRE(wObj1.getCategory("Websites")
                      .getItem("Twitter")
                      .getEntry("username") == "example@gmail.com");
          REQUIRE(wObj1.getCategory("Websites")
                      .getItem("Twitter")
                      .getEntry("password") == "r43rfsffdsfdsf");

          REQUIRE_NOTHROW(wObj1.getCategory("Bank Accounts"));
          REQUIRE(wObj1.getCategory("Bank Accounts").size() == 1);
          REQUIRE_NOTHROW(
              wObj1.getCategory("Bank Accounts").getItem("Starling"));
          REQUIRE(
              wObj1.getCategory("Bank Accounts").getItem("Starling").size() ==
              3);
          REQUIRE(wObj1.getCategory("Bank Accounts")
                      .getItem("Starling")
                      .getEntry("Name") == "Mr John Doe");
          REQUIRE(wObj1.getCategory("Bank Accounts")
                      .getItem("Starling")
                      .getEntry("Account Number") == "12345678");
          REQUIRE(wObj1.getCategory("Bank Accounts")
                      .getItem("Starling")
                      .getEntry("Sort Code") == "12-34-56");

        } // THEN

      } // AND_WHEN

    } // WHEN

  } // GIVEN

} // SCENARIO
