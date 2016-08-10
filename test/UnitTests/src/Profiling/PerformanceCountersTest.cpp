/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#include <gtest/gtest.h>
#include <osre/Profiling/PerformanceCounters.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::Profiling;

class PerformanceCountersTest : public ::testing::Test {
};

TEST_F( PerformanceCountersTest, createTest ) {
    bool ok( true );
    ok = PerformanceCounters::create();
    EXPECT_TRUE( ok );
    ok = PerformanceCounters::create();
    EXPECT_FALSE( ok );
    ok = PerformanceCounters::destroy();
    EXPECT_TRUE( ok );
    ok = PerformanceCounters::destroy();
    EXPECT_FALSE( ok );
}

TEST_F( PerformanceCountersTest, regiserCounterTest ) {
    bool ok = PerformanceCounters::create();

    ui32 v( 0 );
    ok = PerformanceCounters::queryCounter( "test", v );
    EXPECT_FALSE( ok );

    ok = PerformanceCounters::registerCounter( "test" );
    EXPECT_TRUE( ok );

    ok = PerformanceCounters::queryCounter( "test", v );
    EXPECT_TRUE( ok );

    ok = PerformanceCounters::unregisterCounter( "test" );
    EXPECT_TRUE( ok );

    ok = PerformanceCounters::unregisterCounter( "test" );
    EXPECT_FALSE( ok );
}

} // Namespace UnitTest
} // Namespace OSRE
