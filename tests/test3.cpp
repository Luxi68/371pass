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
// Items in the Category container.
// -----------------------------------------------------

#include "../src/lib_catch.hpp"

#include <string>

#include "../src/lib_cxxopts.hpp"
#include "../src/lib_cxxopts_argv.hpp"

#include "../src/category.h"

SCENARIO("An empty Category can be constructed successfully", "[category]") {

  const std::string ident = "Test";

  GIVEN("the category identifier '" + ident + "'") {

    WHEN("a new Category object is constructed") {

      Category cObj1{ident};

      THEN("it will contain zero Items/be empty") {

        REQUIRE(cObj1.size() == 0);
        REQUIRE(cObj1.empty());

      } // THEN

    } // WHEN

  } // GIVEN

} // SCENARIO

SCENARIO("Items can be added to and retrieved from a Category", "[category]") {

  const std::string ident = "Test";

  GIVEN("an empty Category object with identifier '" + ident + "'") {

    Category cObj1{ident};

    REQUIRE(cObj1.empty());

    AND_GIVEN("an empty Item with identifier '" + ident + "'") {

      Item iObj1{ident};

      REQUIRE(iObj1.empty());

      WHEN("adding the Item") {

        THEN("true is returned") {

          REQUIRE(cObj1.addItem(iObj1) == true);

          AND_THEN("the Category contains 1 Item") {

            REQUIRE(cObj1.size() == 1);
            REQUIRE_FALSE(cObj1.empty());

            AND_THEN("getting the Item using the ident '" + ident +
                     "' will return the expected object") {

              REQUIRE(cObj1.getItem(ident) == iObj1);
              REQUIRE_FALSE(cObj1.empty());

            } // AND_THEN

          } // AND_THEN

          AND_GIVEN("another empty Item with the same ident, '" + ident + "'") {

            Item iObj2{ident};

            REQUIRE(iObj2.empty());

            WHEN("adding the Item") {

              THEN("false is returned and the size of the Category will not "
                   "change") {

                REQUIRE(cObj1.addItem(iObj2) == false);
                REQUIRE(cObj1.size() == 1);
                REQUIRE_FALSE(cObj1.empty());

              } // THEN

            } // WHEN

          } // AND_GIVEN

          const std::string ident3 = "Test2";

          AND_GIVEN("another empty Item with the a different ident, '" +
                    ident3 + "'") {

            Item iObj3{ident3};

            REQUIRE(iObj3.empty());

            WHEN("adding the Item") {

              THEN("true is returned and the size of the Category will be 2") {

                REQUIRE(cObj1.addItem(iObj3) == true);
                REQUIRE(cObj1.size() == 2);
                REQUIRE_FALSE(cObj1.empty());

                AND_THEN("getting the Item using the ident '" + ident3 +
                         "' will return the expected object") {

                  REQUIRE(cObj1.getItem(ident3) == iObj3);

                } // AND_THEN

              } // THEN

            } // WHEN

          } // AND_GIVEN

        } // THEN

      } // WHEN

    } // AND_GIVEN

  } // GIVEN

} // SCENARIO

SCENARIO("Items can be added to and deleted from a Category", "[category]") {

  const std::string ident = "Test";

  GIVEN("a empty Category object with identifier '" + ident + "'") {

    Category cObj1{ident};

    REQUIRE(cObj1.empty());

    AND_GIVEN("an empty Item with identifier '" + ident + "") {

      Item iObj1{ident};

      REQUIRE(iObj1.empty());

      WHEN("the entry is added") {

        THEN("true is returned") {

          REQUIRE(cObj1.addItem(iObj1) == true);

          AND_THEN("the Category contains 1 Item") {

            REQUIRE(cObj1.size() == 1);
            REQUIRE_FALSE(cObj1.empty());

            WHEN("getting the Item using the ident '" + ident + "'") {

              THEN("the Item object is returned") {

                REQUIRE(cObj1.getItem(ident) == iObj1);

              } // THEN

            } // WHEN

            WHEN("deleting a non-existent Item with ident 'blah'") {

              THEN("an std::out_of_range exception is thrown and the Category "
                   "will not change") {

                REQUIRE_THROWS_AS(cObj1.deleteItem("blah"), std::out_of_range);
                REQUIRE(cObj1.getItem(ident) == iObj1);
                REQUIRE(cObj1.size() == 1);

              } // THEN

            } // WHEN

            WHEN("deleting the added Item with ident '" + ident) {

              THEN("true is returned and the Category will be empty") {

                REQUIRE(cObj1.deleteItem(ident) == true);
                REQUIRE_THROWS_AS(cObj1.getItem(ident), std::out_of_range);
                REQUIRE(cObj1.size() == 0);

              } // THEN

            } // WHEN

          } // AND_THEN

        } // THEN

      } // WHEN

    } // AND_GIVEN

  } // GIVEN

} // SCENARIO
