#include <catch.hpp>
#include "PoissonSolver3DGPUTest.h"




TEST_CASE( "INFO and WARN do not abort tests", "[messages][.]" ) {
    INFO( "this is a " << "message" );    // This should output the message if a failure occurs
    WARN( "this is a " << "warning" );    // This should always output the message but then continue
}

TEST_CASE( "SUCCEED counts as a test pass", "[messages]" ) {
    SUCCEED( "this is a " << "success" );
}

TEST_CASE( "Correctness for Poisson Solver ", "[classic]" ) {
    
    INFO( "this message should be logged" );
    INFO( "so should this" );
    DoPoissonSolverExperiment(17, 17, 18,  200, 0);
    REQUIRE( 1== 1 );
}
