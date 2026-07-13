/*

obj_loader.h

this file defines a function that loads basic objects from an .obj file

Only the vertices and faces are read from the file

*/



#ifndef STDIO
#define STDIO
#include <stdio.h>
#endif

#ifndef STRUCTS
#define STRUCTS
#include "structs.h"
#endif

#include <string.h>


/*
    read obj file for triangles and load them in array
    Memory must be pre allocated and the size must be provided

    args:   void* mem
            unsigned int mem_size
            const char* filename

    return: int number of bytes used

*/
int loadObjects
(void* mem, unsigned int mem_size, const char* filename)
{
    /*
        open file

        read from file and write to memory
            count bytes written

        return count of bytes

    */
    FILE *file;
    file = fopen(filename, "r");

    if(file == NULL) 
    {
        printf("No Such File\n");
        exit(1);
    }

    /*
        file format
        #comment
        #geometric vertices
        v <x y z> [w] where w scales the point (default 1)
        ...
        #texture coordinates in <u> [v w]
        vt not used (skipped in algo)
        ...
        #vertex normals
        vn <x y z>
        ...
        #parameter space vertices (skipped)

        #faces
        f <vertex_index> / [texture index] / <normal_index>
        #if no texture, just leave blank like so: 1//1 2//1 3//1

    */

    char buffer[256];

    point* vertices = mem;


    //for each line
    int v_i = 0;
    int n_i = 0;
    while(fgets(buffer, 256, file)!=NULL)//get line
    {
        printf("%s", buffer);
        char* str = strtok(&buffer, " ");//get each word in line

        //if the first word of the line is "v"
        if(strcmp((const char*) str, "v") == 0)
        {
            for(int i = 0; i < 3; i++)//get the three points of the vertex
            {
                vertices[v_i].x = (float) atof(strtok(NULL, " "));
                vertices[v_i].y = (float) atof(strtok(NULL, " "));
                vertices[v_i].z = (float) atof(strtok(NULL, " "));
                v_i++;
            }
            continue; //move to next line
        }

        /*
        //if the first word of the line is "vn"
        if(strcmp((const char*) str, "vn") == 0)
        {
            for(int i = 0; i < 3; i++)//get the normal vertex for the point
            {
                normals[n_i].x = (float) atof(strtok(NULL, " "));
                normals[n_i].y = (float) atof(strtok(NULL, " "));
                normals[n_i].z = (float) atof(strtok(NULL, " "));
                n_i++;
            }
            continue; // move to next line
        }
        */

        //if the first word of the line is "f"
        else if(strcmp((const char*) str, "f") == 0)
        {
            //link the geometry face with its component points; ie create a triangle

        }

        else continue;
        
    }
    fclose(file);
}