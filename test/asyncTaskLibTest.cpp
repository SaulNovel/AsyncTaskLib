#include <gtest/gtest.h>
#include <Task/Scheduler.h>

#include <cstdlib>
#include <vector>
#include <unordered_set>

using std::vector;
using std::unordered_set;
using std::unordered_map;
using std::unique_ptr;


TEST(AsyncTaskLibTest, Start_Complete) 
{

}


int main(int ac, char* av[])
{
        testing::InitGoogleTest(&ac, av);
        return RUN_ALL_TESTS();
} 
