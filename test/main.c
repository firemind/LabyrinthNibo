#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"
#include "../labyrinth.h"
#include "../calculate.h"

int init_suite1(void)
{
  return 0;
}

int clean_suite1(void)
{
  return 0;
}

void testBestPath(void)
{
   fillLabyrinth();
   CU_ASSERT(labyrinth[0][0].actions == 2);
   CU_ASSERT(labyrinth[5][2].value == 0);
   CU_ASSERT(labyrinth[5][2].actions == 12);

   calculateValues();
   CU_ASSERT(labyrinth[0][0].value == 100);
   CU_ASSERT(labyrinth[3][1].value == 100);

   printLabyrinth();
   cleanup();
}

void testBestPathWithTraps(void)
{
   fillLabyrinth();
   labyrinth[4][1].value = -99; // This field is a trap
   CU_ASSERT(labyrinth[0][0].actions == 2);
   CU_ASSERT(labyrinth[5][2].value == 0);
   CU_ASSERT(labyrinth[5][2].actions == 12);

   calculateValues();
   CU_ASSERT(labyrinth[0][0].value == 100);
   CU_ASSERT(labyrinth[3][1].value == 0); // This should not be part of the best path
   CU_ASSERT(labyrinth[7][1].value == 100); // This should now be part of the best path

   printLabyrinth();
   cleanup();
}


int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("My Suite", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   if (NULL == CU_add_test(pSuite, "test calculateValues in standard labyrinth)", testBestPath))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   if (NULL == CU_add_test(pSuite, "test calculateValues with traps ", testBestPathWithTraps))
   {
     CU_cleanup_registry();
     return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();


   return CU_get_error();
}
