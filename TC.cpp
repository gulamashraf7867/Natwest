#include "CLI.h"
#include <gtest/gtest.h>
namespace {

class CLITesting : public ::testing::Test 
{

protected:
  void SetUp() { 
	cli.getNotesName();
	cli.notesName;
}
void TearDown() {}
Cli Cli;
};
TEST_F(CLITesting, CheckFileName) {
    notesName = "pythonNotes";
 // EXPECT_NE((Trip*)NULL, trips.findTripById("SG304"));
  EXPECT_EQ("pythonNotes", cli.getNotesName();
}
}
