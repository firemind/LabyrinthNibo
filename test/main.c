#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"

#include "../helper.h"
#include "../labyrinth.h"
#include "../calculate.h"

/* Dummy functions for testing */
void copro_setSpeed(int l, int r){}; 
void copro_stopImmediate(){}; 
void delay(int i){}; 
void _delay_ms(int i){}; 
#include "../move.h"

/* Dummy functions for testing */
struct field hidden_laby[LAB_WIDTH][LAB_HEIGHT];
int wallLeft(){
  switch(current_direction){
    case WEST:
      return !(hidden_laby[current_position.x][current_position.y].actions & SOUTH);
    case EAST:
      return !(hidden_laby[current_position.x][current_position.y].actions & NORTH);
    case SOUTH:
      return !(hidden_laby[current_position.x][current_position.y].actions & EAST);
    case NORTH:
      return !(hidden_laby[current_position.x][current_position.y].actions & WEST);
  }
}; 
int wallRight(){
  switch(current_direction){
    case WEST:
      return !(hidden_laby[current_position.x][current_position.y].actions & NORTH);
    case EAST:
      return !(hidden_laby[current_position.x][current_position.y].actions & SOUTH);
    case SOUTH:
      return !(hidden_laby[current_position.x][current_position.y].actions & WEST);
    case NORTH:
      return !(hidden_laby[current_position.x][current_position.y].actions & EAST);
  }
}; 
int wallFront(){
  switch(current_direction){
    case WEST:
      return !(hidden_laby[current_position.x][current_position.y].actions & WEST);
    case EAST:
      return !(hidden_laby[current_position.x][current_position.y].actions & EAST);
    case SOUTH:
      return !(hidden_laby[current_position.x][current_position.y].actions & SOUTH);
    case NORTH:
      return !(hidden_laby[current_position.x][current_position.y].actions & NORTH);
  }
}; 
int groundBlack(){return (current_position.x == 5 && current_position.y == 2); }
#include "../phase1.h"

#include "../phase2.h"

int init_suite1(void)
{
  return 0;
}

int clean_suite1(void)
{
  return 0;
}

void testMoving(void)
{
   CU_ASSERT(current_position.x == 0);
   CU_ASSERT(current_position.y == 0);

   move_forward();

   CU_ASSERT(current_position.x == 0);
   CU_ASSERT(current_position.y == 1);

   turn_left();

   CU_ASSERT(current_direction == EAST);

   move_forward();
   move_forward();

   CU_ASSERT(current_position.x == 2);
   CU_ASSERT(current_position.y == 1);
}

void testBestPath(void)
{
   struct environment env;
   cleanup(&env);
   env.goal_x = 5;
   env.goal_y = 2;
   fillLabyrinth(labyrinth);
   CU_ASSERT(labyrinth[0][0].actions == 2);
   CU_ASSERT(labyrinth[5][2].value == 0);
   CU_ASSERT(labyrinth[5][2].actions == 12);

   calculateValues(&env);
   CU_ASSERT(labyrinth[0][0].value == 100);
   CU_ASSERT(labyrinth[3][1].value == 100);

   printLabyrinth(labyrinth);
   cleanup(&env);
}

void testBestPathWithTraps(void)
{
   struct environment env;
   cleanup(&env);
   env.goal_x = 5;
   env.goal_y = 2;
   fillLabyrinth(labyrinth);
   labyrinth[4][1].value = -99; // This field is a trap
   CU_ASSERT(labyrinth[0][0].actions == 2);
   CU_ASSERT(labyrinth[5][2].value == 0);
   CU_ASSERT(labyrinth[5][2].actions == 12);

   calculateValues(&env);
   CU_ASSERT(labyrinth[0][0].value == 100);
   // This should no longer be part of the optimal path
   CU_ASSERT(labyrinth[3][1].value == 0); 
   // This should now be part of the optimal path
   CU_ASSERT(labyrinth[7][1].value == 100); 

   printLabyrinth(labyrinth);
   cleanup(&env);
}

void testPhase1(void)
{
  fillLabyrinth(hidden_laby);
  current_position.x = 0;
  current_position.y = 0;
  current_direction = SOUTH;
  doPhase1();
  printLabyrinth(labyrinth);
}

void testPhase2(void)
{
  printLabyrinth(labyrinth);
  doPhase2();
  printLabyrinth(labyrinth);
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

   if (NULL == CU_add_test(pSuite, "test moving and turning", testMoving))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   if (NULL == CU_add_test(pSuite, "test calculateValues in standard labyrinth", testBestPath))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   if (NULL == CU_add_test(pSuite, "test calculateValues with traps", testBestPathWithTraps))
   {
     CU_cleanup_registry();
     return CU_get_error();
   }

   if (NULL == CU_add_test(pSuite, "test phase 1 (moving on left-hand rule and cartographing)", testPhase1))
   {
     CU_cleanup_registry();
     return CU_get_error();
   }

   if (NULL == CU_add_test(pSuite, "test phase 2 (using A* to find best way back)", testPhase2))
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
