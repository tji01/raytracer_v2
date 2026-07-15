

/*

Raytracer 2
Post Graduation; based on CS 4611 Computer Graphics project
Tyler Iverson


Renders objects defined in external files

depends on structs.h, lin_alg.h, and stb_image_write.h

*/



#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"
#include "lin_alg.h"
#include "obj_loader.h"

#ifndef STDIO
#define STDIO
#include <stdio.h>
#endif

#ifndef STRUCTS
#define STRUCTS
#include "structs.h"
#endif



int main(int argc, char* argv[])
{
    float top = 512;
    float side = 512;
    /*
    int shadow = 0;
    float channels = 3;

    float light[3] = {3, 8, -15};
    */
    /*
        read arguments
            dimensions of image to create
            file of objects to render

        if file of objects given:
            load objects into memory (only triangles and spheres)
            
        else
            what is default behavior?

    */

    if(argc != 4) 
    {
        printf("Provide only image dimensions and obj file name \n./ray <x> <y> <filename>\n");
        exit(1);
    }

    
    top = atof((argv[1]));
    side = atof((argv[2]));

    begin:
    printf("Loading objects...");
    
    char objects[4096];
    char mem[2048];
    loadObjects(mem, 2048, objects, 4096, "icosahedron.obj");

    printf("done\n");
}


/*
    
*/