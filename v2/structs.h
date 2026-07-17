

/*

structs.h 

This file defines structures and functions useful for the organization of the program

*/

/*
    The type of geometry to render, likely going to be deprecated

*/
enum type {
    SPHERE,
    TRI
};

/*
    Defines a point in space. 

    float x, y, z
*/
typedef struct Point
{
    float x;
    float y;
    float z;

}point;

/*
    Defines a direction in space.

    float x, y, z
*/
typedef struct Vector
{
    float x;
    float y;
    float z;

}vector;

/*
    Defines a loacation and direction in space

    vector* v
    point* o

*/
typedef struct Ray
{
    vector* v;
    point* o;
}ray;


typedef struct Sphere
{
    point* position;
    float radius;
}ball;

/*
    Defines a triangle's location in vector space.

    point* a, b, c

*/
typedef struct Triangle
{
    point* a;
    point* b;
    point* c;

}tri;

/*
    Defines the color a geometric object should color a pixel.

    int r, g, b
*/
typedef struct Color
{
    int r;
    int g;
    int b;
}color;

/* struct Object

    This struct is explicitly to make a bunch of different types of objects iterable in an array.

    It holds an address and the type of object located at that address.

    enum type kind
    void* location
    int reflective
    color color

*/
typedef struct Object
{
    enum type kind;
    void* location;
    int reflective;
    color color; //this maybe should be a pointer
}obj;

/*
    populates the provided address with a point using the provided coords
    Input:  float x, y, z
            point* a
    output: void
*/
void newPoint(float x, float y, float z, point* a)
{
    a->x = x;
    a->y = y;
    a->z = z;
}

/*
    populates a provided vector struct with the provided components
    input:  float x, y, z
            vector* v
    output: void
*/
void newVector(float x, float y, float z, vector* v)
{
    v->x = x;
    v->y = y;
    v->z = z;
}

/*
    initializes provided ray structure using the provided point and
    vector. The point is the origin and the vector is the direction.
    input:  point* origin
            vector* direction
            ray* r
    output: void
*/
void newRay(point* origin, vector* direction, ray* r)
{
    r->o = origin;
    r->v = direction;
}

/*
    initializes provided sphere structure
    input:  point* position
            float radius
            ball* b
    output: void

*/
void newBall(point* position, float radius, ball* b)
{
    b->position = position;
    b->radius = radius;
}

/*
    initializes triangle structure

    input:  point* a, b, c
            Triangle* t

    output: void
*/
void newTriangle(point* a, point* b, point* c, tri* t)
{
    t->a = a;
    t->b = b;
    t->c = c;
}

/*
    initializes object structure

    args:   void* location  (object's location in memory)
            enum type kind
            int r, j, b     (color values)
            int reflective  (if object is revlective)
            obj* j
    return: void
*/

void newObject(void* location, enum type kind, int r, int g, int b, int reflective, obj* j)
{
    j->location = location;
    j->kind = kind;
    j->color.r = r;
    j->color.g = g;
    j->color.b = b;
    j->reflective = reflective;
}

