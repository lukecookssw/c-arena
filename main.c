
#include "settings/settings.h"
#include "lib/arena/arena.h"
#include "lib/person/person.h"
#include "lib/grid/grid.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_grid(Grid *grid)
{
    // TODO: This doesn't work the way I hoped it would.
    // It consumes the initial "./main" command as well
    printf("\033[H");   // Move the cursor to the top-left corner

    // Print the grid with colors
    for (int i = 0; i < GRID_WIDTH; i++)
    {
        for (int j = 0; j < GRID_HEIGHT; j++)
        {
            if (grid->data[i][j] < 2)
            {
                printf(GREEN "%4d " RESET, grid->data[i][j]);
            }
            else if (grid->data[i][j] >= 2 && grid->data[i][j] < 3)
            {
                printf(YELLOW "%4d " RESET, grid->data[i][j]);
            }
            else
            {
                printf(RED "%4d " RESET, grid->data[i][j]);
            }
        }
        printf("\n");
    }

    fflush(stdout); // Ensure the output is printed immediately
}

void create_people(Person *people_arr, int count)
{
    for (int i = 0; i < count; i++)
    {
        Person person = {
            .happiness = 50};
        sprintf(person.name, "Person %d", i);
        people_arr[i] = person;
    }
}

void add_people_to_grid(Person *people, Grid *grid, int people_count, int grid_height, int grid_width)
{
    int row_index = 0;
    int col_index = 0;
    for (int i = 0; i < people_count; i++)
    {
        // move the person to one adjacent cell
        row_index = (row_index + 1) % GRID_WIDTH;
        col_index = (col_index + 1) % GRID_HEIGHT;

        // Place a person in the current cell
        grid->data[row_index][col_index]++; // increment the value in the grid cell

        // Update the person's position
        people[i].position[0] = row_index;
        people[i].position[1] = col_index;
    }
}



void tick(Person *people, Grid *grid, int people_count)
{
    int tick_count = 0;

    while (tick_count < 10)
    {
        // Move people around the grid
        for (int i = 0; i < people_count; i++)
        {
            move_person(&people[i], grid);
        }
        tick_count++;

        print_grid(grid);
        // sleep for 10 seconds
        sleep(3);
    }
}

int main()
{

    Arena arena;
    size_t arena_size = (NUM_PEOPLE * sizeof(Person)) + sizeof(Grid);
    arena_init(&arena, arena_size); // Initialize arena with enough memory

    Person *people = (Person *)arena_alloc(&arena, NUM_PEOPLE * sizeof(Person));
    create_people(people, NUM_PEOPLE);

    Grid *grid = (Grid *)arena_alloc(&arena, sizeof(Grid));
    // Initialize the data array to 0
    memset(grid->data, 0, sizeof(grid->data));

    // add people to the grid in random positions
    add_people_to_grid(people, grid, NUM_PEOPLE, GRID_HEIGHT, GRID_WIDTH);

    // main game loop
    tick(people, grid, NUM_PEOPLE);

    arena_free(&arena); // Free the arena memory

    return 0;
}