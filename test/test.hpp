#ifndef test_h
#define test_h

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <iostream>
#include <string>

#include "utils.hpp"


class FrechetTestCase : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(FrechetTestCase);
CPPUNIT_TEST(testDiscreteDist);
CPPUNIT_TEST(testContinuousDist);
CPPUNIT_TEST(testMeanCurve);
CPPUNIT_TEST_SUITE_END();

public:
    void setUp() {}
    void tearDown() {}

protected:
    void testDiscreteDist();
    void testContinuousDist();
    void testMeanCurve();
};


#endif