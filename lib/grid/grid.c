#include "grid.h"
#include "../../settings/settings.h"
#include "../person/person.h"

#include <stdlib.h>

int can_move_left(int row_index, int col_index)
{
    return col_index - 1 >= 0;
}
int can_move_right(int row_index, int col_index)
{
    return col_index + 1 < GRID_WIDTH;
}
int can_move_up(int row_index, int col_index)
{
    return row_index - 1 >= 0;
}
int can_move_down(int row_index, int col_index)
{
    return row_index + 1 < GRID_HEIGHT;
}

void move_person(Person *person, Grid *grid)
{

    int row_index = person->position[0];
    int col_index = person->position[1];
    int direction = rand() % 4; // 0 - left, 1 - right, 2 - up, 3 - down
    switch (direction)
    {
    case 0:
        if (can_move_left(row_index, col_index))
            col_index--;
        break;
    case 1:
        if (can_move_right(row_index, col_index))
            col_index++;
        break;
    case 2:
        if (can_move_up(row_index, col_index))
            row_index--;
        break;
    case 3:
        if (can_move_down(row_index, col_index))
            row_index++;
        break;
    default:
        break;
    }

    // Remove the person from their current cell
    grid->data[person->position[0]][person->position[1]]--;

    // Update the person's position
    person->position[0] = row_index;
    person->position[1] = col_index;

    // Update the grid cell
    grid->data[person->position[0]][person->position[1]]++;
}