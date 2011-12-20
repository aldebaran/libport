/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/damerau-levenshtein-distance.hh>

#include <libport/unit-test.hh>

using libport::test_suite;

void check_hard() // Not really checked
{
  std::string s1("In information theory and computer science,"
                 " Damerau-Levenshtein distance is a \"distance\" "
                 "(string metric) between two strings, i.e., finite sequence "
                 "of symbols, given by counting the minimum number of "
                 "operations needed to transform one string into the other, "
                 "where an operation is defined as an insertion, deletion, or "
                 "substitution of a single character, or a transposition of "
                 "two characters. Damerau in his seminal paper not only "
                 "distinguished these four edit operations but also stated "
                 "that they correspond to more than 80% of all human "
                 "misspellings. Damerau concentrated on single-character "
                 "misspellings. Edit distance was introduced by Levenshtein, "
                 "who generalized this concept with multiple edit operations, "
                 "but did not include transpositions in the set of basic "
                 "operations. While the original motivation was to measure "
                 "distance between human misspellings to improve applications "
                 "such as spell checkers, Damerau-Levenshtein distance has "
                 "also seen uses in biology to measure the variation between "
                 "DNA.");
  std::string s2("The above-described pseudo-code calculates only restricted "
                 "edit distance. Damerau?Levenshtein distance plays an "
                 "important role in natural language processing. In natural "
                 "languages, strings are short and the number of errors "
                 "(misspellings) rarely exceeds 2. In such circumstances, "
                 "restricted and real edit distance differ very rarely. That "
                 "is why this limitation is not very important. However, one "
                 "must remember that restricted edit distance does not always "
                 "satisfy the triangle inequality and, thus, cannot be used "
                 "with metric trees. An extension of the edit distance "
                 "algorithm, that does satisfy the triangle inequality is "
                 "described in the paper F.J. Damerau. A technique for "
                 "computer detection and correction of spelling errors, "
                 "Communications of the ACM, 1964");
  for (int i = 0; i < 100; ++i)
    libport::damerau_levenshtein_distance(s1, s2);
}

#define CHECK(S1, S2, D)						\
  BOOST_CHECK_EQUAL(libport::damerau_levenshtein_distance(S1, S2), D)

void check_deletion()
{
  CHECK("azertyuiop", "aeryuop", 3u);
}

void check_insertion()
{
  CHECK("aeryuop", "azertyuiop", 3u);
}

void check_substitution()
{
  CHECK("azertyuiopqsdfghjklmwxcvbn,", "qwertyuiopasdfghjkl;zxcvbnm", 6u);
}

void check_transposition()
{
  CHECK("1234567890", "1324576809", 3u);
}

test_suite*
init_test_suite()
{
  test_suite* suite =
    BOOST_TEST_SUITE("libport::damerau-levenshtein-distance");
//  suite->add(BOOST_TEST_CASE(check_hard));
  suite->add(BOOST_TEST_CASE(check_deletion));
  suite->add(BOOST_TEST_CASE(check_insertion));
  suite->add(BOOST_TEST_CASE(check_substitution));
  suite->add(BOOST_TEST_CASE(check_transposition));
  return suite;
}
