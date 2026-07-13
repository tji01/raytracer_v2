

/*

structs.h 

This file defines structures and functions useful for the organization of the program

*/


enum type {
    SPHERE,
    TRI
};

// a point in space
typedef struct Point
{
    float x;
    float y;
    float z;

}point;

typedef struct Vector
{
    float x;
    float y;
    float z;

}vector;

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

typedef struct Triangle
{
    point* a;
    point* b;
    point* c;

}tri;

typedef struct Color
{
    int r;

    int g;
    int b;
}color;

/* struct Object

    This struct is explicitly to make a bunch of different types of objects iterable in an array.

    It holds an address and the type of object located at that address.

*/
typedef struct Object
{
    enum type kind;
    void* location;
    int reflective;
    color color;
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

