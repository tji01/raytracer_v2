

/*

Raytracer
CS 4611 Computer Graphics
Tyler Iverson


Renders objects defined in this file

depends on structs.h, lin_alg.h, and stb_image_write.h

*/







#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"
#include "lin_alg.h"
#include <stdio.h>

#define NUM_OBJ 8

void pixel(int index, unsigned char* data, float r, float g, float b);
float map(float value, float input_min, float input_max, float output_min, float output_max);
int indexToPix_X(int i);
int indexToPix_Y(int i);
void draw(int index, unsigned char* image, ray* r, obj** objects);


float light[3] = {3, 8, -15};

float top = 512;
float side = 512;
int shadow = 0;
float channels = 3;


int main(int argc, char *argv[])
{
    if (argc == 1) goto begin;
    top = atof((argv[1]));
    side = atof((argv[2]));
    
    begin:
    printf("Generating %f x %f image... ", side, top);
    //float center = (((side*top)/2)*channels) + ((top/2)*channels);
    //float value;
    //int bias = 0;

    unsigned char *image = malloc(sizeof(unsigned char)* side * top * channels);



    //middle of image, 2d point on image plane (0, 0, 2)
    point* imageMidpoint = newPoint(0, 0, 2);
    vector* camera = newVector(0, 0, 2);
    point* origin = newPoint(0, 0, 0);
    
    //balls
    point* ball1pos = newPoint(0, 0, -16);
    point* ball2pos = newPoint(3, -1, -14);
    point* ball3pos = newPoint(-3, -1, -14);

    float ball1rad = 2;
    float ball2rad = 1;
    float ball3rad = 1;

    ball* ball1 = newBall(ball1pos, ball1rad);
    ball* ball2 = newBall(ball2pos, ball2rad);
    ball* ball3 = newBall(ball3pos, ball3rad);

    //ball ball_

    //triangles
    //back wall
    point* tri1A = newPoint(-8, -2, -20);
    point* tri1B = newPoint(8,-2,-20);
    point* tri1C = newPoint(8,10,-20);
    
    point* tri2A = newPoint(-8, -2, -20);
    point* tri2B = newPoint(8,10,-20);
    point* tri2C = newPoint(-8,10,-20);

    tri* tri1 = newTriangle(tri1A, tri1B, tri1C);
    tri* tri2 = newTriangle(tri2A, tri2B, tri2C);

    //floor
    point* tri3A = newPoint(-8, -2, -20);
    point* tri3B = newPoint(8,-2,-10);
    point* tri3C = newPoint(8,-2,-20);
    
    point* tri4A = newPoint(-8, -2, -20);
    point* tri4B = newPoint(-8,-2,-10);
    point* tri4C = newPoint(8,-2,-10);
    
    tri* tri3 = newTriangle(tri3A, tri3B, tri3C);
    tri* tri4 = newTriangle(tri4A, tri4B, tri4C);

    //right wall
    point* tri5A = newPoint(8,-2,-20);
    point* tri5B = newPoint(8,-2,-10);
    point* tri5C = newPoint(8,10,-20);

    tri* tri5 = newTriangle(tri5A, tri5B, tri5C);


    

    obj* tri1_obj = newObject(tri1, TRIANGLE, 0, 0, 255, 0);
    obj* tri2_obj = newObject(tri2, TRIANGLE, 0, 0, 255, 0);

    obj* tri3_obj = newObject(tri3, TRIANGLE, 255, 255, 255, 0);
    obj* tri4_obj = newObject(tri4, TRIANGLE, 255, 255, 255, 0);

    obj* tri5_obj = newObject(tri5, TRIANGLE, 255, 0, 0, 0);


    obj* ball1_obj = newObject(ball1, SPHERE, 0, 0, 0, 1);
    obj* ball2_obj = newObject(ball2, SPHERE, 0, 0, 0, 1);
    obj* ball3_obj = newObject(ball3, SPHERE, 255, 0, 0, 0);


    obj** objects_array = malloc(sizeof(obj**) * NUM_OBJ);

    objects_array[0] = tri1_obj;
    objects_array[1] = tri2_obj;
    objects_array[2] = tri3_obj;
    objects_array[3] = tri4_obj;
    objects_array[4] = tri5_obj;
    objects_array[5] = ball1_obj;
    objects_array[6] = ball2_obj;
    objects_array[7] = ball3_obj;
    
    


    for (int i = 0; i < (side * top * channels); i = i+channels)
    {
        //1. Generate Ray


        //find vector from origin to pixel        
        //get point pixel is at
        point* pix = newPoint(
                            map(((i/3)%(int)top)+0.5, 0, side, -1, 1),//x
                            map(((i/3)/side)+0.5, 0, top, 1, -1),//y 
                            -2                                   //z
                            );
        //2d vector on image plane
        vector* v1 = vectorFromPoints(imageMidpoint, pix);
        vector* v3 = vectorSum(v1, camera);            
        ray* r = newRay(origin, v3);
        normalize(r->v);

        //2. intersect and draw  objects
        
        draw(i, image, r, objects_array);
        
        


        

        freePoint(pix);
        freeVector(v1);
        freeVector(v3);
        freeRay(r);
    }
    printf("writing image... ");
    stbi_write_png("reference.png", (int)top, (int) side, 3, image, top*3);
    //image frees
    free(image);
    freeVector(camera);
    freePoint(imageMidpoint);
    freePoint(origin);

    //ball frees

    freePoint(ball1pos);
    freePoint(ball2pos);
    freePoint(ball3pos);
    
    freeBall(ball1);
    freeBall(ball2);
    freeBall(ball3);


    //tri frees
    freePoint(tri1A);
    freePoint(tri1B);
    freePoint(tri1C);
    
    freePoint(tri2A);
    freePoint(tri2B);
    freePoint(tri2C);

    freePoint(tri3A);
    freePoint(tri3B);
    freePoint(tri3C);

    freePoint(tri4A);
    freePoint(tri4B);
    freePoint(tri4C);
    
    freePoint(tri5A);
    freePoint(tri5B);
    freePoint(tri5C);

    freeTriangle(tri1);
    freeTriangle(tri2);
    freeTriangle(tri3);
    freeTriangle(tri4);
    freeTriangle(tri5);

    //obj frees
    freeObject(ball1_obj);
    freeObject(ball2_obj);
    freeObject(ball3_obj);

    freeObject(tri1_obj);
    freeObject(tri2_obj);
    freeObject(tri3_obj);
    freeObject(tri4_obj);
    freeObject(tri5_obj);

    free(objects_array);
    printf("done.\n");
}








//      ~*~supporting functions~*~



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


float intersect(ray* r, obj** objects, int* index)
{
    //cycle through objects, find smallest positive t value at ray  
    float min = (float)__INT_MAX__;
    float t;
    int t_index = -1;
    //int val = 0;
    for(int i = 0; i < NUM_OBJ; i++)
    {
        
        if(objects[i]->kind == TRIANGLE)
        {
            t = intersectTriangle(objects[i]->location, r);
            if(t<0) continue;
        }
        else if(objects[i]->kind == SPHERE)
        {
            t = intersectBall(objects[i]->location, r);
            if(t<0) continue;
        }
        else printf("What shape is this?\n");

        if ((t <= min) & (t > 0))
        {
            min = t;
            t_index = i;
        }


        


    } 
    *(index) = t_index;
    return min;
  
}


/* reflection

~
//r = d +  -2(d dot n) n
    vector norm_rv = *(r->v);
    vector norm_norm = *(norm->v);
    ray* addr = newRay((point*)vectorSum(((vector*)collision), r->v), &norm_norm);
    //2d_terminal = collision + d

    normalize(&norm_norm);
    normalize(&norm_rv);
    //printf("yup");
    float scalar = -2 * dot(&norm_rv, &norm_norm);
    addr->v = vectorSum(scale(scalar, &norm_norm), norm->v);
    //freeRay(r);
    //point *addr_terminal = (point*) vectorSum((scale(2, &norm_rv)), addr->v);



    //addr->v = vectorFromPoints(collision, addr_terminal);
    return addr;
~

args:       ray* r, norm
returns:    address of reflected ray

DESTRUCTIVE
frees passed in ray r
free the returned ray with freeRay
*/
ray* reflection(ray* r, ray* norm, point* collision)
{
    //r = d + -2(d dot n) n
    normalize(r->v);
    normalize(norm->v);
    point *y_init = (point*) scale(2, r->v);
    //vector *r_dot_norm = 
    vector *neg2_r_dot_norm_norm = scale(-2 * dot(r->v, norm->v), norm->v);
    vector *y = vectorSum(r->v, neg2_r_dot_norm_norm);
    vector* scale_y = scale(.9999, y);
    ray *r_new = newRay(collision, scale_y);
    freeVector(neg2_r_dot_norm_norm);
    freePoint(y_init);
    freeVector(y);
    freeVector(scale_y);
    return r_new;
}

/*

~ old reflection propagation logic

if (count>1) printf("yup");
        //if ray hits a diffuse object or nothing, do stuff and break
        //if(t<0) goto color_black_and_return;//if hit nothing
        //if()
        if(j->reflective == 0) goto shading;//if not reflective
        
        //reflect ray 
        r = reflection(r, norm, r_terminal);

        //intersect ray reflection

        int i = 0;
        t = intersect(r, objects, &i);
        //update collision 
        if(t>0) 
        {
            //freePoint(collision);
            //freePoint((point*)r_terminal);
        
            r_terminal = scale(t, r->v);
            //collision = newPoint(*(r_terminal->x), *(r_terminal->y), *(r_terminal->z));

        }

        if(i>0) j = objects[i];
        //else goto color_black_and_return;
        //update color
        c->r = j->color.r;
        c->g = j->color.g;
        c->b = j->color.b;
        count++;
    }
    if(count > 9) 
    {
        color_black_and_return:
        c->r = 0;
        c->g = 255;
        c->b = 0;

        //freePoint(collision);
        freePoint(r_terminal);
        freeVector(rayVector);
        freeRay(norm);
        freePoint(theLight);
        return;
    }

~



    Modifies a color struct to apply to a pixel for shading

    args:   color* c        the color to modify
            ray* r:         the ray that struck the object 
            float t:        the distance from the ray* r's origin the object is struck
            obj* j:         the distinct object struck
    
    return: void        

*/
void shade_reflect(color* c, ray* _ray, float t, obj* j, int val, obj** objects)
{
    
    //find collision point
    point* theLight = newPoint(light[0], light[1], light[2]);
    vector *rayVector = newVector(*(_ray->v->x), *(_ray->v->y), *(_ray->v->z));
    point *r_terminal = (point*)scale(t, rayVector);
    //point *collision = newPoint(*(r_terminal->x), *(r_terminal->y), *(r_terminal->z));
    vector *zero = newVector(0, 0, 0);
    ray* norm = newRay(r_terminal, zero);
    freeVector(zero);

    //make a freeable copy of ray
    /*
    point p;
    p.x = _ray->o->x;
    p.y = _ray->o->y;
    p.z = _ray->o->z;
    vector v;
    v.x = _ray->v->x;
    v.y = _ray->v->y;
    v.z = _ray->v->z;
    */
    point* p = newPoint(*(_ray->o->x), *(_ray->o->y), *(_ray->o->z));
    vector* v = newVector(*(_ray->v->x), *(_ray->v->y), *(_ray->v->z));
    ray *r = newRay(p, v);
    freePoint(p);
    freeVector(v);


    
    

    //find normal vector  
    if(j->kind == SPHERE)
    {
        vector* vec_to_free = norm->v;

        norm->v = vectorFromPoints(((ball*)j->location)->position, r_terminal);
        freeVector(vec_to_free);
    }   
    else if(j->kind == TRIANGLE)
    {
        
        vector *a = vectorFromPoints(((tri*)j->location)->a, ((tri*)j->location)->b);
        vector *b = vectorFromPoints(((tri*)j->location)->a, ((tri*)j->location)->c);

        vector* vec_to_free = norm->v;
        norm->v = cross(a, b);
        freeVector(vec_to_free);
        freeVector(a);
        freeVector(b);
    }
    normalize(norm->v);


    //if object is reflective, reflect ray <10x or until a diffuse object is hit
    int count = 0;
    
    if(j->reflective == 0) goto shading;
    else 
    {
        while((j->reflective == 1) & (count<10)) 
        {
            /*if (val == (((side*top+ (512*2))/2)*channels) + (((top + 90)/2)*channels) )
            {
                printf("reflected, %d\n", j->kind);
            } 
            //if t is not positive we hit nothing.*/
            
            //reflect ray
            ray* free_r = r;
            r = reflection(r, norm, r_terminal);
            freeRay(free_r);
            //intersect new ray with objects
            int i = 0;
            t = intersect(r, objects, &i);
            if(t == (float)__INT_MAX__)
            {
                //printf("yup");
                c->r = 0;
                c->g = 0;
                c->b = 0;

                //freeRay(norm);
                freePoint(r_terminal);
                freeVector(rayVector);
                freeRay(norm);
                freePoint(theLight);
                freeRay(r);
                return;
            }
            //update object
            if(i>=0)j = (objects[i]);

            vector *scale_t_r = scale(t, r->v);
            point* addr = r_terminal;
            r_terminal = (point*)vectorSum((vector*)r_terminal, scale_t_r);
            freeVector(scale_t_r);
            freePoint(addr);
            //update norm
            if(j->kind == SPHERE)
            {
                vector* addr = norm->v;
                norm->v = vectorFromPoints(((ball*)j->location)->position, r_terminal);
                freeVector(addr);

            }   
            else if(j->kind == TRIANGLE)
            {
                
                vector *a = vectorFromPoints(((tri*)j->location)->a, ((tri*)j->location)->b);
                vector *b = vectorFromPoints(((tri*)j->location)->a, ((tri*)j->location)->c);

                vector* addr = norm->v;
                norm->v = cross(a, b);
                freeVector(addr);
                freeVector(a);
                freeVector(b);
            }
            normalize(norm->v);
            count++;
        }
        //update color
        c->r = j->color.r;
        c->g = j->color.g;
        c->b = j->color.b;

        
    }


    shading:

    //find vector toward light

    vector* toLight = vectorFromPoints(r_terminal, theLight); 
    normalize(toLight);
    vector* bumpedCollision = (vector*) scale(.99999, (vector*) r_terminal);
    ray* lightRay = newRay((point*)bumpedCollision, toLight);

    

    //see if there is a shadow upon this point using collision and lightRay
    shadow = 1;
    int dummy;
    float shadow_t = intersect(lightRay, objects, &dummy);
    shadow = 0;
    if((shadow_t != (float)__INT_MAX__) )//if there is a shadow
    {
        c->r = (c->r * .2);
        c->g = (c->g * .2);
        c->b = (c->b * .2);

        
        freeRay(lightRay);
        freeVector(bumpedCollision);
        //freePoint(collision);
        freePoint(r_terminal);
        freeVector(rayVector);
        freeRay(norm);
        freePoint(theLight);
        freeVector(toLight);
        freeRay(r);
        return;
    }

    //find dot product of normalized vectors ("percent" shaded)
    float shade = dot(norm->v, toLight);
    //clamp dot from .2 to 1
    shade = clamp(.2, 1, shade);

    //multiply each color channel in c by clamped dot

    c->r = (c->r * shade);
    c->g = (c->g * shade);
    c->b = (c->b * shade);

    //freePoint(collision);
    freeVector(bumpedCollision);
    freeRay(lightRay);
    freePoint(r_terminal);
    freeVector(rayVector);
    freeRay(norm);
    freePoint(theLight);
    freeVector(toLight);
    freeRay(r);
}



void draw(int index, unsigned char* image, ray* r, obj** objects)
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
        c.r = objects[object]->color.r;
        c.g = objects[object]->color.g;
        c.b = objects[object]->color.b;

        val = index;

        /*if(objects[object]->reflective == 0)*/ 
        shade_reflect(&c, r, min, objects[object], val, objects);
        

        

        pixel(index, image, c.r, 
                            c.g, 
                            c.b);
    }
    


}


