#ifndef GRID_H
#define GRID_H

#include "../../settings/settings.h"
#include "../person/person.h"


typedef struct Grid
{
    int data[GRID_WIDTH][GRID_HEIGHT]; // 2D array with 100 rows and 100 columns
    
} Grid;

void move_person(Person *person, Grid *grid);

#endif