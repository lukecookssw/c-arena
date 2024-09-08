#ifndef PERSON_H
#define PERSON_H

typedef struct Person
{
    char name[50];
    int position[2];        // coords (x, y) of the person's position in the grid
    int happiness;          // will be between 0 - 100
} Person;

#endif