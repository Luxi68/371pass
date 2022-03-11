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
// argument and 'create' value of the action argument.
// This test works by calling on your program to create
// a category, item, and an entry, and each time
// calls upon your program to load the JSON file to
// check the category/item/entry exists.
// -----------------------------------------------------

#include "../src/lib_catch.hpp"

#include <fstream>
#include <iostream>
#include <string>

#include "../src/lib_cxxopts.hpp"
#include "../src/lib_cxxopts_argv.hpp"

#include "../src/371pass.h"

SCENARIO(
    "The database and action program arguments can be parsed correctly "
    "such that a file can be opened, read, parsed, data added, and written to",
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

    const std::string testCategory = "TestCategory";
    const std::string testItem = "TestItem";
    const std::string testEntryKey = "TestEntryKey";
    const std::string testEntryValue = "TestEntryValue";
    const std::string testEntry = testEntryKey + "," + testEntryValue;

    WHEN("the db program argument is '" + filePath +
         "', the action program argument is 'create'") {

      AND_WHEN("and the category program argument is '" + testCategory + "'") {

        Argv argvObj({"test", "--db", filePath.c_str(), "--action", "create",
                      "--category", testCategory.c_str()});
        auto **argv = argvObj.argv();
        auto argc = argvObj.argc();

        THEN("no exception is thrown") {

          REQUIRE_NOTHROW(App::run(argc, argv));

          AND_WHEN("loading the saved file into a new Wallet object") {

            Wallet wObj1{};
            REQUIRE(wObj1.empty());
            REQUIRE_NOTHROW(wObj1.load(filePath));

            THEN("the new Wallet will contain the Category object") {

              REQUIRE_NOTHROW(wObj1.getCategory(testCategory));
              REQUIRE(wObj1.getCategory(testCategory).size() == 0);

            } // THEN

            AND_WHEN("and the item program argument is '" + testItem + "'") {

              Argv argvObj({"test", "--db", filePath.c_str(), "--action",
                            "create", "--category", testCategory.c_str(),
                            "--item", testItem.c_str()});
              auto **argv = argvObj.argv();
              auto argc = argvObj.argc();

              THEN("no exception is thrown") {

                REQUIRE_NOTHROW(App::run(argc, argv));

                AND_WHEN("loading the saved file into a new Wallet object") {

                  Wallet wObj2{};
                  REQUIRE(wObj2.empty());
                  REQUIRE_NOTHROW(wObj2.load(filePath));

                  THEN("the new Wallet will contain the Category and Item "
                       "objects") {

                    REQUIRE_NOTHROW(wObj2.getCategory(testCategory));
                    REQUIRE(wObj2.getCategory(testCategory).size() == 1);
                    REQUIRE_NOTHROW(
                        wObj2.getCategory(testCategory).getItem(testItem));
                    REQUIRE(wObj2.getCategory(testCategory)
                                .getItem(testItem)
                                .size() == 0);

                  } // THEN

                  AND_WHEN("and the entry program argument is '" + testEntry +
                           "'") {

                    Argv argvObj({
                        "test",
                        "--db",
                        filePath.c_str(),
                        "--action",
                        "create",
                        "--category",
                        testCategory.c_str(),
                        "--item",
                        testItem.c_str(),
                        "--entry",
                        testEntry.c_str(),
                    });
                    auto **argv = argvObj.argv();
                    auto argc = argvObj.argc();

                    THEN("no exception is thrown") {

                      REQUIRE_NOTHROW(App::run(argc, argv));

                      AND_WHEN("loading the saved file into a new Wallet "
                               "object") {

                        Wallet wObj3{};
                        REQUIRE(wObj3.empty());
                        REQUIRE_NOTHROW(wObj3.load(filePath));

                        THEN("the new Wallet will contain the Category and "
                             "Item objects and entry") {

                          REQUIRE_NOTHROW(wObj3.getCategory(testCategory));
                          REQUIRE(wObj3.getCategory(testCategory).size() == 1);
                          REQUIRE_NOTHROW(wObj3.getCategory(testCategory)
                                              .getItem(testItem));
                          REQUIRE(wObj3.getCategory(testCategory)
                                      .getItem(testItem)
                                      .size() == 1);
                          REQUIRE(wObj3.getCategory(testCategory)
                                      .getItem(testItem)
                                      .getEntry(testEntryKey) ==
                                  testEntryValue);

                        } // THEN

                      } // AND_WHEN

                    } // THEN

                  } // AND_WHEN

                } // AND_WHEN

              } // THEN

            } // AND_WHEN

          } // AND_WHEN

        } // THEN

      } // AND_WHEN

    } // WHEN

  } // GIVEN

} // SCENARIO