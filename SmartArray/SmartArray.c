// Cassidy Lyons


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SmartArray.h"

// Dynamically allocate space for a new SmartyArray
SmartArray *createSmartArray(int length)
{
    SmartArray *s = NULL;
    int i;
    s = malloc(sizeof(SmartArray));
    if(s == NULL)
    {
        return NULL;
    }
       if(length < DEFAULT_INIT_LEN)
    {
        length = DEFAULT_INIT_LEN;
    }
    s->size = 0;
    s->capacity = length;

    // An array that stores "length" (integer amount of) char pointers
    s->array = malloc(sizeof(char*) * (length));

    // Make sure all the pointers initialized to NULL
    for(i = 0; i < length; i++)
    {
        s->array[i] = NULL;
    }
    printf("-> Created new SmartArray of size %d.\n", s->capacity);
    return s;
}

// Free any dynamically allocated memory associated with the SmartArray struct
SmartArray *destroySmartArray(SmartArray *smarty)
{
    // To return if passed an already NULL struct pointer
    // Free if not NULL
    if(smarty == NULL)
        return NULL;
    else
    {
        free(smarty->array);
        free(smarty);
    }
    return NULL;
}

// Dynamically allocate a new array of length passed in. Copy smarty->array conents into new array, free memory assosiated with smarty
// update smartyArray size & capacity members if needed
SmartArray *expandSmartArray(SmartArray *smarty, int length)
{
    int i, size;
    char **newArray;
    newArray = NULL;

    // To hold "length" (integer amount of) char pointers
    newArray = malloc(sizeof(char*) * length);
    if(newArray == NULL)
    {
       return NULL;
    }
    if(smarty == NULL)
    {
        return NULL;
    }
    else
    {
        // Copy smarty->array current contents over to newArray
        size = getSize(smarty);
        for(i = 0; i < size; i++)
        {
            newArray[i] = smarty->array[i];
        }

        // Now make sure remaining unassigned pointers in newArray are initialized to NULL
        for(i = size; i < length; i++)
            {
                newArray[i] = NULL;
            }
    }
    // It is now safe to free old contents of smarty->array and then update size & capacity members
    free(smarty->array);
    smarty->capacity = length;
    printf("->Expanded SmartArray to size %d\n", length);

    // smarty->array will now point to base address of newArray, which holds the copies of smartys previous content
    smarty->array = newArray;
    return smarty;
}

// Trim the capacity down to match the size if smarty->capacity < smarty->size
SmartArray *trimSmartArray(SmartArray *smarty)
{
    int i, size;
    char **newArray = NULL;
    if(smarty == NULL)
    {
        return NULL;
    }
    size = getSize(smarty);
    newArray = malloc(sizeof(char*) * size);
    if(newArray == NULL)
    {
        free(newArray);
        return NULL;
    }
    if (smarty->capacity > size )
    {
        // Copy smarty->array current contents over to newArray
        for(i = 0; i < size; i++)//changed from  > smarty->capacity
        {
            newArray[i] = smarty->array[i];
        }
        // It is now safe to free smarty->array current contents, update members, and then point to
        // base address of newArray, which holds a copy of smarty->array contents
        free(smarty->array);
        smarty->capacity = size;
        smarty->array = newArray;
        printf("-> Trimmed SmartArray to size %d.\n", size);
    }
    return smarty;
}

// Make a copy of str and then put it into next free cell of smarty->array
char *put(SmartArray *smarty, char *str)
{
    // First make sure smarty and str are not NULL before attempting to manipulate values
    if(smarty == NULL || str == NULL)
        return NULL;
    int size = getSize(smarty), exp = ((smarty->capacity * 2) + 1);

    // This variable will hold the length of a str
    unsigned long len;
    char *copy = NULL;
    len = strlen(str);

    // copy needs room to copy "str's" string of characters, strlen(str) + 1 to accomodate null terminator
    copy = malloc(sizeof(char) * (len + 1));
    if(copy == NULL)
    {
        free(copy);
        return NULL;
    }
    strcpy(copy, str);

   // Check if smarty->array is full before attempting to add copy of str.
   // If so, make room and come back
   if(size >= smarty->capacity)
    {
        expandSmartArray(smarty, exp);
        smarty->array[size] = copy;
    }
    else
    {
        smarty->array[size] = copy;
    }
    // smarty->size is initially 0 and free. Increment size and next cell with be free on return
    smarty->size++;
    return smarty->array[size];
}

// Get element at specified index without going out of bounds
char *get(SmartArray *smarty, int index)
{
    int  size = getSize(smarty);

    // Make sure index is not out of bounds before attempting to retrieve
    if(smarty == NULL)
        return NULL;
    if(index < 0)
        return NULL;
    if(smarty->array[index] == NULL)
        return NULL;
    if(index > size)
        return NULL;

    return smarty->array[index];
}

// Replace string at specified index with a copy of str, but only if string at index is already valid
char *set(SmartArray *smarty, int index, char *str)
{
    char *copy = NULL;
    int size;
    if(smarty == NULL)
    {
        return NULL;
    }
    size = getSize(smarty);
    // If string at index is NULL, less than zero, or larger than smarty->size, it must be out of bounds (skip to return)
    // if not out of bounds, safe to replace with copy of str
    if (smarty->array[index] != NULL && index <= size && index >= 0)
    {
        // Only allocate enough room for the copy of str
        copy = malloc(sizeof(char) * (strlen(str)) + 1);
        if (copy == NULL)
        {
            free(copy);
            return NULL;
        }
        strcpy(copy, str);
        smarty->array[index] = copy;
        return smarty->array[index];
    }
    else
       return NULL;
    }

// Insert a copy of str into specified index of smarty->array. If index is invalid insert into first empty position in array
// Shift all strings to the rightto the right of index right in both cases
char *insertElement(SmartArray *smarty, int index, char *str)
{
    char *copy = NULL;
    int i, size;
    // Do not add a NULL string
    if(str == NULL)
    {
        return NULL;
    }
    if(smarty == NULL)
    {
        return NULL;
    }
    size = getSize(smarty);
    copy = malloc(sizeof(char) * strlen(str) + 1);
    if(copy == NULL)
    {
        free(copy);
        return NULL;
    }
        strcpy(copy, str);
    // Make sure smarty->array is not full before inserting the copy of str, if it is double the size and come back
    if(size >= smarty->capacity)
    {
        expandSmartArray(smarty,(smarty->capacity * 2));
    }
    // If index is out of bounds, make room and add it to first valid position
    // Otherwise, make room then add at index and shift remaining elements to the right
    if(index < 0 || index > size)
    {
        smarty->size++;
        size = getSize(smarty);
        for(i = 0; i < size; i++)
        {
            if(smarty->array[i] != NULL)
            {
                smarty->array[i] = copy;
                break;
            }
        }
    }
    else
    {
        smarty->size++;
        size = getSize(smarty);
       // Make room by shifting elements that are to the right of index right one space, and then replacing element at index
        for(i = size; i > index; i--)
       {
           smarty->array[i] = smarty->array[i - 1];
       }
        smarty->array[index] = copy;
    }
    return smarty->array[index];
}

// Remove element at specified index if index is valid
int removeElement(SmartArray *smarty, int index)
{
    int size, i;
    if(smarty == NULL)
    {
        return 0;
    }
    size = getSize(smarty);
    // If index is out of bounds, do nothing
    if(index >= size || index < 0)
    {
       return 0;
    }
    smarty->array[index] = NULL;
    // After removing element, shift remaining elements left to avoid leaving an empty space
    for(i = index; i < size; i++)
    {
        smarty->array[i] = smarty->array[i+1];
    }

    // Decrease size by 1 and update capacity since we removed one of smarty->array's elements
    smarty->size--;
    smarty->capacity = smarty->size;

    return 1;
}

// function to use instead of accessing smarty->size throughout program
int getSize(SmartArray *smarty)
{
    int size;
    if(smarty == NULL)
    {
        return -1;
    }
    size = smarty->size;
    return size;
}
// Print contents of smarty->array
void printSmartArray(SmartArray *smarty)
{
    int i, size = getSize(smarty);
    if(smarty == NULL || smarty->array == NULL)
    {
        printf("(empty array)\n");
        return;
    }
    if(size == 0)
    {
       printf("(empty array)\n");
       return;
    }
    for(i = 0; i < size; i++)
    {
           printf("%s\n", smarty->array[i]);
    }
    return;
}
// Returns a number between 0.0 and 5.0, inclusively, which represents a measure of difficulty based on effort exerted
double difficultyRating(void)
{

    return 2.8;
}

// Returns a number > 0.0 to represent all hours spent on this program
double hoursSpent(void)
{
    return 10.37;
}
