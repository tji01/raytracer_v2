

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

void pixel(int index, unsigned char* data, float r, float g, float b);
float map(float value, float input_min, float input_max, float output_min, float output_max);
int indexToPix_X(int i);
int indexToPix_Y(int i);
void draw(int index, unsigned char* image, ray* r, obj* objects);
void reflect(ray* r, ray* norm, point* collision);
float intersect(ray* r, obj* objects, int* index);

float top = 512;
float side = 512;
int shadow = 0;
int NUM_OBJ = 0;
float channels = 3;

float light[3] = {0, 0, 3};
point theLight;
vector zero;


int main(int argc, char* argv[])
{
    
    
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
    
    obj objects[4096];

    char mem[2048];

    memset(objects, 0, sizeof(obj)*4096);
    memset(mem, 0, 2048);

    NUM_OBJ = loadObjects(mem, 2048, objects, 4096, argv[3]);

    printf("done\n");

    printf("Generating Image...");


    unsigned char* image = malloc(sizeof(unsigned char) * side * top * channels);

    point imageMidpoint;
    newPoint(0, 0, 2, &imageMidpoint);

    vector camera_perspective;
    newVector(0, 0, 2, &camera_perspective);

    point origin;
    newPoint(0, 0, 5, &origin);

    newPoint(light[0], light[1], light[2], &theLight);
    newVector(0, 0, 0, &zero);

    point pix;
    vector v1;
    ray r;
    for(int i = 0; i < (side * top * channels); i = i+channels)
    {
        //generate the ray

        // get the location of the current pixel
        newPoint(
            map(((i/3)%(int)top)+.5, 0, side, -1, 1),
            map(((i/3)/side)+.5, 0, top, 1, -1),
            -2,
            &pix
        );

        vectorFromPoints(&imageMidpoint, &pix, &v1);
        vectorSum(&v1, &camera_perspective);
        newRay(&origin, &v1, &r);
        
        normalize(r.v);

        draw(i, image, &r, objects);


    }

    stbi_write_png("reference.png", (int)top, (int) side, 3, image, top*3);

}




/* 
 * Takes two ranges and a float that exists in the input range and projects it to the output range.
 *
 * Input: input range Min/Max, output range Min/Max, input value
 * Output: float value equivalent to the input value's position on the input range on the output range.
*/
float map(float value, float inputMin, float inputMax, float outputMin, float outputMax)
{
    return  ((value - inputMin) / (inputMax - inputMin)) * (outputMax - outputMin) + outputMin;
}

/*
 * Assigns the appropriate rgb values to the channels at the index in the argument array. Intended to recieve
   the index corrosponding to the r channel.
 * Input:   index, unsigned char* array, float r, g, b.
 * Output:  void
 */
void pixel(int index, unsigned char* data, float r, float g, float b)
{
    data[index]     = r;
    data[index+1]   = g;
    data[index+2]   = b;
}

/*
    Returns the vertical pixel value on recieving the red channel index of image array
*/
int indexToPix_X(int i)
{
    return ((i/3)%(int)side);
}

/*
    Returns the horozontal pixel value on recieving the red channel index of the image array.
*/
int indexToPix_Y(int i)
{
    return ((i/3)/top);
}


float intersect(ray* r, obj* objects, int* index)
{
    //cycle through objects, find smallest positive t value at ray  

    float min = (float)__INT_MAX__;
    float t;
    int t_index = -1;

    for(int i = 0; i< NUM_OBJ; i++)
    {
        t = intersectTriangle(objects[i].location, r);
        if(t<0) continue;

        if((t<=min) & (t>0))
        {
            min = t;
            t_index = i;
        }
    }
    *index = t_index;
    return min;
}

void reflect(ray* r, ray* norm, point* collision)
{
    //r = d + -2(d dot n) n
    normalize(r->v);
    normalize(norm->v);

    point y_init;
    newPoint(r->v->x, r->v->y, r->v->z, &y_init);
    scale(2, r->v);

    vector neg2_r_dot_norm_norm;
    newPoint(r->v->x, r->v->y, r->v->z, (point*)&neg2_r_dot_norm_norm);
    scale(-2 * dot(r->v, norm->v), norm->v);

    vector y;
    newVector(r->v->x, r->v->y, r->v->z, &y);
    vectorSum(&y, &neg2_r_dot_norm_norm);
    scale(.9999, &y);

    //return new ray
    newRay(collision, &y, r);
}

void shade_reflect(color* c, ray* _ray, float t, obj* j, int val, obj* objects)
{
    //find collision point
    vector rayVector;
    newVector(_ray->v->x, _ray->v->y, _ray->v->z, &rayVector);
    
    point r_terminal;
    scale(t, (vector*)&r_terminal);

    ray norm;
    newRay(&r_terminal, &zero, &norm);
    
    point p;
    newPoint(_ray->o->x, _ray->o->y, _ray->o->z, &p);

    ray r;
    newRay(&p, &rayVector, &r);

    //find normal vector
    vector a;
    vectorFromPoints( ((tri*)j->location)->a, ((tri*)j->location)->b, &a);

    vector b;
    vectorFromPoints( ((tri*)j->location)->a, ((tri*)j->location)->c, &b);

    cross(&a, &b);
    newRay(&r_terminal, &a, &norm);
    
    normalize(norm.v);

    //if object is reflective, do reflecting or diffuse

    int count = 0;
    if(j->reflective == 0) goto shading;
    else
    {
        while((j->reflective == 1) & (count<10))
        {
            /* if t is not positive we hit nothing */

            ray reflection;
            newRay(r.o, r.v, &reflection);

            reflect(&r, &norm, &r_terminal);

            int i = 0;
            t = intersect(&r, objects, &i);
            if(t == (float)__INT_MAX__)
            {
                c->r = 0;
                c->g = 0;
                c->b = 0;

                return;
            }
            //update object
            if(i>=0)j = &objects[i];

            vector scale_t_r;
            newVector(r.v->x, r.v->y, r.v->z, &scale_t_r);
            scale(t, &scale_t_r);

            point addr;
            newPoint(r_terminal.x, r_terminal.y, r_terminal.z, &addr);

            vectorSum((vector*)&r_terminal, &scale_t_r);

            //update norm
            vectorFromPoints( ((tri*)j->location)->a, ((tri*)j->location)->b, &a);
            vectorFromPoints( ((tri*)j->location)->a, ((tri*)j->location)->c, &b); 
            
            vector address;
            newVector(norm.v->x, norm.v->y, norm.v->z, &address);

            cross(&a, &b);
            newVector(a.x, a.y, a.z, norm.v);

            normalize(norm.v);
            count++;
        }

        //update color
        c->r = j->color.r;
        c->g = j->color.g;
        c->b = j->color.b;

        
    }
    shading:

    //diffuse shading, shadow
    //find vector toward light
    shadow = 0;

    
    

    vector toLight;
    vectorFromPoints(&r_terminal, &theLight, &toLight);

    normalize(&toLight);

    vector bumpedCollision;
    newVector(r_terminal.x, r_terminal.y, r_terminal.z, &bumpedCollision);
    scale(.99999, &bumpedCollision);

    ray lightRay;
    newRay((point*)&bumpedCollision, &toLight, &lightRay);


    //is there a shadow?
    int dummy;
    float shadow_t = intersect(&lightRay, objects, &dummy);

    if((shadow_t != (float)__INT_MAX__) )//if there is a shadow
    {
        c->r = (c->r * .2);
        c->g = (c->g * .2);
        c->b = (c->b * .2);

        return;
    }

    //find percent shaded, dot of normalized vectors
    float shade = dot(norm.v, &toLight);

    shade = clamp(.2, 1, shade);

    //multiply each color channel in c by clamped dot

    c->r = (c->r * shade);
    c->g = (c->g * shade);
    c->b = (c->b * shade);

}

void draw(int index, unsigned char* image, ray* r, obj* objects)
{
    
    //draw pixel
    int object;
    int val;
    float min = intersect(r, objects, &object);


    if(min == (float)__INT_MAX__) 
    {
        pixel(index, image, 0, 0, 0);
    }
    else 
    {
        color c;
        c.r = objects[object].color.r;
        c.g = objects[object].color.g;
        c.b = objects[object].color.b;

        val = index;

        /*if(objects[object]->reflective == 0)*/ 
        shade_reflect(&c, r, min, &objects[object], val, objects);
        

        

        pixel(index, image, c.r, 
                            c.g, 
                            c.b);
    }

}