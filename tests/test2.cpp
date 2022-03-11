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
// This file contains tests adding, getting and deleting
// entries in the Item container.
// -----------------------------------------------------

#include "../src/lib_catch.hpp"

#include <string>

#include "../src/lib_cxxopts.hpp"
#include "../src/lib_cxxopts_argv.hpp"

#include "../src/item.h"

SCENARIO("An empty Item can be constructed successfully", "[item]") {

  const std::string ident = "Test";

  GIVEN("the item identifier '" + ident + "'") {

    WHEN("a new Item object is constructed") {

      Item iObj1{ident};

      THEN("it will contain zero entries/be empty") {

        REQUIRE(iObj1.size() == 0);
        REQUIRE(iObj1.empty());

      } // THEN

    } // WHEN

  } // GIVEN

} // SCENARIO

SCENARIO("Entries can be added to and retrieved from an Item", "[item]") {

  const std::string ident = "Test";

  GIVEN("an empty Item object with identifier '" + ident + "'") {

    Item iObj1{ident};

    REQUIRE(iObj1.empty());

    std::string key = "url";
    std::string value = "https://www.google.com/";

    AND_GIVEN("an entry with key '" + key + "' and value '" + value + "'") {

      WHEN("the entry is added") {

        THEN("true is returned") {

          REQUIRE(iObj1.addEntry(key, value) == true);

          AND_THEN("the Item contains 1 entry") {

            REQUIRE(iObj1.size() == 1);
            REQUIRE_FALSE(iObj1.empty());

            AND_THEN("getting the entry using the key '" + key +
                     "' will return the expected value") {

              REQUIRE(iObj1.getEntry(key) == value);

            } // AND_THEN

          } // AND_THEN

          AND_GIVEN("another entry with the same key, '" + key + "'") {

            WHEN("adding the entry") {

              THEN("false is returned and the size of the Item will not "
                   "change") {

                REQUIRE_FALSE(iObj1.addEntry(key, value));
                REQUIRE(iObj1.size() == 1);
                REQUIRE_FALSE(iObj1.empty());

              } // THEN

            } // WHEN

          } // AND_GIVEN

          key = "username";
          value = "myusername";

          AND_GIVEN("another entry with a different key, '" + key +
                    "', and value '" + value + "'") {

            WHEN("adding the entry") {

              THEN("true is returned and the size of the Item will be 2") {

                REQUIRE(iObj1.addEntry(key, value) == true);
                REQUIRE(iObj1.size() == 2);
                REQUIRE_FALSE(iObj1.empty());

                AND_THEN("getting the entry using the key '" + key +
                         "' will return the expected value") {

                  REQUIRE(iObj1.getEntry(key) == value);

                } // AND_THEN

              } // THEN

            } // WHEN

          } // AND_GIVEN

        } // THEN

      } // WHEN

    } // AND_GIVEN

  } // GIVEN

} // SCENARIO

SCENARIO("Entries can be added to and deleted from an Item", "[item]") {

  const std::string ident = "Test";

  GIVEN("an empty Item object with identifier '" + ident + "'") {

    Item iObj1{ident};

    REQUIRE(iObj1.empty());

    std::string key = "url";
    std::string value = "https://www.google.com/";

    AND_GIVEN("an entry with key '" + key + "' and value '" + value + "'") {

      WHEN("the entry is added") {

        THEN("true is returned") {

          REQUIRE(iObj1.addEntry(key, value) == true);

          AND_THEN("the Item contains 1 entry") {

            REQUIRE(iObj1.size() == 1);
            REQUIRE_FALSE(iObj1.empty());

            WHEN("getting the entry using the key '" + key + "'") {

              THEN("the expected value will be returned") {

                REQUIRE(iObj1.getEntry(key) == value);

              } // THEN

            } // WHEN

            WHEN("deleting a non-existent entry with key 'blah'") {

              THEN("the Item will not change") {

                REQUIRE_THROWS_AS(iObj1.getEntry("blah"), std::out_of_range);
                REQUIRE(iObj1.getEntry(key) == value);
                REQUIRE(iObj1.size() == 1);

              } // THEN

            } // WHEN

            WHEN("deleting the added entry with key '" + key + "'") {

              THEN("the Item will be empty") {

                REQUIRE(iObj1.deleteEntry(key));
                REQUIRE(iObj1.size() == 0);

              } // THEN

            } // WHEN

          } // AND_THEN

        } // THEN

      } // WHEN

    } // AND_GIVEN

  } // GIVEN

} // SCENARIO