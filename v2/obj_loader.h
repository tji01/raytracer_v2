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

    args:   void* points_triangles
            unsigned int pt_size
            void* object_array
            unsigned int oa_size
            const char* filename

    return: int size of object array
            fills mem with points, triangles, and objects defined in the input file
            object structs are only populated with type and memory location
            user must define color and reflectivity 

*/
int loadObjects
(void* points_triangles, unsigned int pt_size, void* object_array, unsigned int oa_size, const char* filename)
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

    point* vertices = points_triangles;
    tri* triangles;
    //for each line
    int v_i = 0;
    int n_i = 0;
    int t_i = 0;
    char* line_context;
    while(fgets(buffer, 256, file)!=NULL)//get line
    {

        

        

        char* obj_data_type = strtok_r(buffer, " ", &line_context);//get each word in line

        //if the first word of the line is "v"
        if(strcmp((const char*) obj_data_type, "v") == 0)
        {
            //get the three floats of the vertex
            
            vertices[v_i].x = (float) atof(strtok_r(NULL, " ", &line_context));
            vertices[v_i].y = (float) atof(strtok_r(NULL, " ", &line_context));
            vertices[v_i].z = (float) atof(strtok_r(NULL, " ", &line_context));
            v_i++;
            
            continue; //move to next line
        }

        /*
        //if the first word of the line is "vn" 
        if(strcmp((const char*) obj_data_type, "vn") == 0)
        {
            for(int i = 0; i < 3; i++)//get the vertex normal for the point
            {
                normals[n_i].x = (float) atof(strtok_r(NULL, " ", &line_context));
                normals[n_i].y = (float) atof(strtok_r(NULL, " ", &line_context));
                normals[n_i].z = (float) atof(strtok_r(NULL, " ", &line_context));
                n_i++;
            }
            continue; //move to next line
        }
        */

        
        //if the first word of the line is "f"
        else if(strcmp((const char*) obj_data_type, "f") == 0)
        {
            triangles = (tri*)(vertices + v_i + n_i);
            
            //link the geometry face with its component points; ie create a triangle
            //get first data point
            char* a = strtok_r(NULL, " ", &line_context); //string of x/y/z or x//z
            char* b = strtok_r(NULL, " ", &line_context);
            char* c = strtok_r(NULL, " ", &line_context);
            char* a_context = NULL;
            char* b_context = NULL;
            char* c_context = NULL;

            char* slash;
            if((slash = strstr(a, "//")) == NULL)
            {
                
            }
            else
            {
                int point_one = atoi(strtok_r(a, "/", &a_context));
                int point_two = atoi(strtok_r(b, "/", &b_context));
                int point_three = atoi(strtok_r(c, "/", &c_context));

                triangles[t_i].a = &(vertices[point_one - 1]);
                triangles[t_i].b = &(vertices[point_two - 1]);
                triangles[t_i].c = &(vertices[point_three - 1]);
            }
            t_i++;
        }

        else continue;
        
    }

    //for each triangle, create an object struct
    obj* objects = (obj*)object_array;
    int i = 0;
    for(; i < t_i; i++)
    {
        //#0B78B3
        objects[i].kind = TRI;
        objects[i].location = &(triangles[i]);
        objects[i].color.r = 0x0b;
        objects[i].color.g = 0x78;
        objects[i].color.g = 0xb3;
        objects[i].reflective = 0;

    }

    fclose(file);

    return i;
}