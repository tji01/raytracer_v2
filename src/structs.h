

/*









*/



enum type {
    SPHERE,
    TRIANGLE
};


// a point in space
typedef struct Point
{
    float* x;
    float* y;
    float* z;

}point;

typedef struct Vector
{
    float* x;
    float* y;
    float* z;

}vector;

typedef struct Ray
{
    vector* v;
    point* o;

}ray;

typedef struct Sphere
{
    point* position;
    float* radius;
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
Returns the address of a point struct given its coordinates
Inpupt: float x, y, z coordinates
Output: point* 
 */
point* newPoint(float x, float y, float z)
{
    point* addr = malloc(sizeof(point));
    addr->x = malloc(sizeof(float));
    addr->y = malloc(sizeof(float));
    addr->z = malloc(sizeof(float));

    *(addr->x) = x;
    *(addr->y) = y;
    *(addr->z) = z;

    return addr;

}


/*
Frees the contents of the point struct and the struct itself upon recieving the address for the struct
*/
void freePoint(point* addr)
{
    free(addr->x);
    free(addr->y);
    free(addr->z);
    free(addr);
}

//creates and initializes a new vector struct and returns its address
//Use freeVector on returned address
vector* newVector(float x, float y, float z)
{
    vector* addr = malloc(sizeof(vector));
    addr->x = malloc(sizeof(float));
    addr->y = malloc(sizeof(float));
    addr->z = malloc(sizeof(float));

    *(addr->x) = x;
    *(addr->y) = y;
    *(addr->z) = z;

    return addr;
}

//Frees the vector at addr and its contents
void freeVector(vector* addr)
{
    free(addr->x);
    free(addr->y);
    free(addr->z);
    free(addr);
}


/* newRay 

    Args:   point* origin
            vector* direction
    Return: ray* (address of new Ray struct)


    Malloc's and initializes a ray struct upon being given a point struct and vector struct. The point and vector structs do
    not compose the ray struct; they are just copied and new structs are created for the ray o and v. This is to
    prevent double free's. This means that the user is still responsible for freeing origin and direction.

    The ray, origin point and direction vector are all free'd using...

    freeRay() to free after use

*/
ray* newRay(point* origin, vector* direction)
{
    ray* addr = malloc(sizeof(ray));
    addr->o = newPoint(*(origin->x), *(origin->y), *(origin->z));
    addr->v = newVector(*(direction->x), *(direction->y), *(direction->z));
    return addr;
}


/* freeRay

    Args:   ray* addr (address of Ray struct)
    return: void
    Frees the ray struct at addr
*/
void freeRay(ray* addr)
{
    freePoint(addr->o);
    freeVector(addr->v);
    free(addr);
}

/* newBall

    Args:   point* position (sphere's location)
            float radius (sphere's radius)
    Returns pointer to Sphere struct

    Use freeBall to free after use
*/

ball* newBall(point* position, float radius)
{
    ball* addr = malloc(sizeof(ball));
    addr->position = newPoint(*(position->x), *(position->y), *(position->z));
    addr->radius = malloc(sizeof(float));
    

    *(addr->radius) = radius;
    return addr;
}


/*freeBall
    frees a Sphere struct 
    
    Args:   ball* addr
*/
void freeBall(ball* addr)
{
    freePoint(addr->position);
    free(addr->radius);
    free(addr);
}


/* newTriangle
    allocates memory for a Triangle struct (and its components) and returns its address.

    args:   point* a, b, c (vertices of the triangle)

    return: Triangle*

    Use freeTriangle() to free after use. User responsible for freeing argument point* a, b, c. points in Triangle struct are copies

*/
tri* newTriangle(point* a, point* b, point* c)
{
    tri* addr = malloc(sizeof(tri));

    addr->a = newPoint(*(a->x), *(a->y), *(a->z));
    addr->b = newPoint(*(b->x), *(b->y), *(b->z));
    addr->c = newPoint(*(c->x), *(c->y), *(c->z));

    return addr;
}

/* freeTriangle

    Frees memory allocated by newTriangle

    args:   struct Triangle *t
    return: void

*/
void freeTriangle(tri* t)
{
    freePoint(t->a);
    freePoint(t->b);
    freePoint(t->c);
    free(t);
}

/* new Object

    Allocates memory for an Object struct
    args:   void* location (object's location in memory)
            enum type kind (the kind of object being referred to)
    return: struct Object*
        
    Use freeObject to free after use

*/
obj* newObject(void* location, enum type kind, int r, int g, int b, int reflective)
{
    obj* addr = malloc(sizeof(obj));

    addr->location = location;
    addr->kind = kind;
    addr->color.r = r;
    addr->color.g = g;
    addr->color.b = b;
    addr->reflective = reflective;
    return addr;
}

/* freeObject

    frees memory alloc'd by newObject

    args:   struct Object* addr 
    return: void


*/
void freeObject(obj* addr)
{
    free(addr);
}