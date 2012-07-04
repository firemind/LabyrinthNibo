#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"
#include "../labyrinth.h"

int init_suite1(void)
{
  return 0;
}

int clean_suite1(void)
{
  return 0;
}

void testFillLabyrinth(void)
{
   fillLabyrinth();
   
   CU_ASSERT(labyrinth[5][2].value == 100);
   CU_ASSERT(labyrinth[5][2].actions == 12);

   calculateValues();

   CU_ASSERT(labyrinth[0][0].value == 37);
   
   // calculation should not change value terminal states
   CU_ASSERT(labyrinth[death_x][death_y].value == -99);
   CU_ASSERT(labyrinth[goal_x][goal_y].value == 100);

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

   /* add the tests to the suite */
   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
   if (NULL == CU_add_test(pSuite, "test of fillLabyrinth()", testFillLabyrinth))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();

   printLabyrinth();

   return CU_get_error();
}
