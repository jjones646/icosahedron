// Draw an Icosahedron
// ECE4893/8893 Project 4
// YOUR NAME HERE

#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

#define NFACE 20
#define NVERTEX 12

#define X (0.525731112119133606) 
#define Z (0.850650808352039932)

// These are the 12 vertices for the icosahedron
static GLfloat vdata[NVERTEX][3] = {    
   {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},    
   {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},    
   {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0} 
};

// These are the 20 faces.  Each of the three entries for each 
// vertex gives the 3 vertices that make the face.
static GLint tindices[NFACE][3] = { 
   {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},    
   {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},    
   {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6}, 
   {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };

int testNumber; // Global variable indicating which test number is desired
 
// Test cases.  Fill in your code for each test case
// draw the icosahedron in 3d space & display from any viewing angle
void Test1(void)
{

}

// Test1 + update at 10Hz rotating ~1 degree on both X & Y for each update
void Test2(void)
{
}

// Subdivide into smaller triangles
void Test3(void)
{
}

// Test3 + rotation and updating at 10Hz like Test2
void Test4(void)
{

}

// Add 2nd argument for the depth of division - don't divide past 5
void Test5(int depth)
{

}

// Test 5 + rotation and updating at 10Hz like Test2
void Test6(int depth)
{

}

int main(int argc, char** argv)
{
  if (argc < 2)
    {
      std::cout << "Usage: ./icosahedron <testnumber>" << endl;
      exit(1);
    }
  // Set the global test number
  testNumber = atol(argv[1]);
  // Initialize glut  and create your window here
  // Set your glut callbacks here
  // Enter the glut main loop here

  switch(testNumber){
    case 1:
    break;
    case 2:
    break;
    case 3:
    break;
    case 4:
    break;
    case 5:
    break;
    case 6:
    break;
    default:
    break;
  }

  return 0;
}

