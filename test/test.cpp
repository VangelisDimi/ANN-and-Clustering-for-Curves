#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <vector>
#include "test.hpp"

#include <iostream>
using namespace std;

// Register test fixture
CPPUNIT_TEST_SUITE_REGISTRATION(FrechetTestCase);


void FrechetTestCase::testDiscreteDist()
{
    CPPUNIT_ASSERT(getDiscreteFrechetDistance( {{2,3},{3,4},{5,6}} , {{2,3},{3,4},{5,6}} ) == 0);
}

void FrechetTestCase::testMeanCurve()
{
    vector<vector<float>> vec={{2,3},{3,4},{5,6}};
    vector<vector<float>> mean=MeanCurve(vec,vec);
    CPPUNIT_ASSERT_MESSAGE("Mean of two same vectors is not the same size as those vectors",vec.size()==mean.size());

    int equal_elements=0;
    for(int i=0;i<vec.size();i++)
    {
        if(vec[i][0]==mean[i][0] && vec[i][1]==mean[i][1]) equal_elements++;
    }
    CPPUNIT_ASSERT_MESSAGE("Mean of two same vectors is not same as those vectors",equal_elements == vec.size());
}


// the main method
int main(int argc, char *argv[])
{
    CppUnit::TestResultCollector result;

    CppUnit::TestResult controller;
    controller.addListener(&result);

    CppUnit::BriefTestProgressListener progress;
    controller.addListener(&progress);

    CppUnit::TestRunner runner;
    runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
    runner.run(controller);

    CppUnit::TextOutputter outputter(&result, std::cerr);
    outputter.write();

    return result.wasSuccessful() ? 0 : 1;
}