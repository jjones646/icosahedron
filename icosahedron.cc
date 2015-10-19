// Draw an Icosahedron
// ECE4893/8893 Project 4
// Jonathan Jones

#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <memory.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

#define X (0.525731112119133606f)
#define Z (0.850650808352039932f)

static const float DEG2RAD = ((M_PI) / 180);
static const size_t NFACE = 20;
static const size_t NVERTEX = 12;
static int updateRate = 100;
static int testNumber = 1;  // Global variable indicating which test number is desired
static int depth = 1;
static bool shouldRotate = false;

// These are the 12 vertices for the icosahedron
static GLfloat vdata[NVERTEX][3] = {
  { -X, 0.0, Z}, {X, 0.0, Z}, { -X, 0.0, -Z}, {X, 0.0, -Z},
  {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
  {Z, X, 0.0}, { -Z, X, 0.0}, {Z, -X, 0.0}, { -Z, -X, 0.0}
};

// These are the 20 faces.  Each of the three entries for each
// vertex gives the 3 vertices that make the face.
static GLint tindices[NFACE][3] = {
  {0, 4, 1}, {0, 9, 4}, {9, 5, 4}, {4, 5, 8}, {4, 8, 1},
  {8, 10, 1}, {8, 3, 10}, {5, 3, 8}, {5, 2, 3}, {2, 7, 3},
  {7, 10, 3}, {7, 6, 10}, {7, 11, 6}, {11, 0, 6}, {0, 1, 6},
  {6, 1, 10}, {9, 0, 11}, {9, 11, 2}, {9, 2, 5}, {7, 2, 11}
};

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

/* Normalize a vector of non-zero length */
void Normalize(GLfloat n[3])
{
  // find the normalization value
  GLfloat d = sqrt(
                n[0] * n[0] +
                n[1] * n[1] +
                n[2] * n[2]
              );

  // set the results to the output vector that gets returned
  n[0] /= d; n[1] /= d; n[2] /= d;
}

/* Normalize a vector of non-zero length */
void Normalize(const GLfloat* in, GLfloat* out, const size_t sz)
{
  float inner_val = 0;
  // find the normalization value
  for ( size_t i = 0; i < sz; ++i )
    inner_val += in[i] * in[i];

  GLfloat d = sqrt(inner_val);

  // make a copy for our return vector
  memcpy(out, in, sizeof(GLfloat) * sz);

  // set the results to the output vector that gets returned
  for ( size_t i = 0; i < sz; ++i )
    out[i] /= d;
}

/* Take the normalized cross product for 2 non-parallel vectors */
void NormCrossProd(const GLfloat* u, const GLfloat* v, GLfloat* n_out, const size_t sz)
{
  GLfloat n[sz];
  // make a copy for our return vector
  memcpy(n, n_out, sizeof(GLfloat) * sz);

  for ( size_t i = 0; i < sz; ++i ) {
    // make our initial index assumptions
    const size_t ii[2] = { (i + 1) % sz, (i + 2) % sz };
    // compute our normalized indexs
    n[i] = u[ii[0]] * v[ii[1]] - u[ii[1]] * v[ii[0]];
  }
  // normalize it
  Normalize(n, n_out, sz);
}

void NormFace(const GLfloat* v1, const GLfloat* v2, const GLfloat* v3, const size_t sz)
{
  GLfloat d1[sz], d2[sz], n[sz];
  // take the difference between the vectors
  for (size_t k = 0; k < sz; k++) {
    d1[k] = v1[k] - v2[k];
    d2[k] = v2[k] - v3[k];
  }

  NormCrossProd(d1, d2, n, sz);
  glNormal3fv(n);
}

/* draw triangle using face normals */
// void drawTriangleFlat(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3])
void DrawTriangle(const GLfloat* v1, const GLfloat* v2, const GLfloat* v3, const size_t sz, const float colors[4])
{
  // let openGL know we're about to draw a triangle
  glBegin(GL_TRIANGLES);
  // draw a normalized face
  NormFace(v1, v2, v3, sz);
  // now set the color
  glColor3f(colors[0], colors[1], colors[2]);
  // set the vertex points
  glVertex3fv(v1); glVertex3fv(v2); glVertex3fv(v3);
  // officially finish this segment of the drawing
  glEnd();

  // draw the lines
  glLineWidth(2.0);
  // white lines
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINES);
  glVertex3fv(v1); glVertex3fv(v2);
  glEnd();
}

/*
 * Recursively subdivide face N times
 * and draw the resulting triangles.
 */
void Subdivide(const GLfloat v1[3], const GLfloat v2[3], const GLfloat v3[3], const size_t depth, const float colors[4])
{
  const size_t pts = 3;
  GLfloat v12[pts], v23[pts], v31[pts];

  if ( depth == 0 ) {
    DrawTriangle(v1, v2, v3, pts, colors);
    return;
  }

  // compute the midpoints for all sides
  for ( size_t i = 0; i < pts; ++i ) {
    v12[i] = (v1[i] + v2[i]) / 2.0;
    v23[i] = (v2[i] + v3[i]) / 2.0;
    v31[i] = (v3[i] + v1[i]) / 2.0;
  }
  // extrude midpoints to lie on unit sphere
  Normalize(v12); Normalize(v23); Normalize(v31);

  float colors_div[3][4] = { 0 };
  for ( size_t i = 0; i < 3; ++i )
    for ( size_t j = 0; j < 3; ++j )
      colors_div[i][j] = colors[j] / 3;

  // recursively subdivide the triangle
  Subdivide(v1, v12, v31, depth - 1, colors_div[0]); Subdivide(v2, v23, v12, depth - 1, colors_div[1]);
  Subdivide(v3, v31, v23, depth - 1, colors_div[2]); Subdivide(v12, v23, v31, depth - 1, colors);
}

void Display(void)
{
  static bool colorsSet = false;
  static float colors[20][4] = { 0 };

  // clear all
  glClear(GL_COLOR_BUFFER_BIT);
  // Clear the matrix
  glLoadIdentity();
  // Set the viewing transformation
  gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  // translate so we can see everything
  const size_t window_x = glutGet(GLUT_WINDOW_X);
  const size_t window_y = glutGet(GLUT_WINDOW_Y);
  glTranslatef(window_x, window_y, 0.0);
  // scale our object
  glScalef(window_x, window_x, 0.0);
  // glScalef(100, 100, 0.0);

  // rotate if we need to
  static GLfloat rotAng = 0.0;
  if ( shouldRotate == true ) {
    glRotatef(rotAng, 1.0, 0.0, 0.0);
    glRotatef(rotAng, 0.0, 1.0, 0.0);

    if ( rotAng > 1.0 )
      rotAng = 0.0;
    else
      rotAng += 0.1;
  }

  for (size_t i = 0; i < 20; i++) {
    // set the colors initially if we need to
    if ( colorsSet == false ) {
      for ( size_t j = 0; j < 3; ++j )
        colors[i][j] = ((float) rand() / (RAND_MAX));
    }

    Subdivide(&vdata[tindices[i][0]][0],
              &vdata[tindices[i][1]][0],
              &vdata[tindices[i][2]][0],
              depth,
              colors[i]);
  }

  if ( colorsSet == false )
    colorsSet = true;

  // Flush buffer
  glutSwapBuffers();  // for double buffering
}

void Init(void)
{
  //select clearing (background) color
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
}

void Reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, (GLdouble)w, (GLdouble)0.0, h, (GLdouble)w, (GLdouble) - w);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void timer(int)
{
  glutPostRedisplay();
  glutTimerFunc(1000.0 / updateRate, timer, 0);
}

void showUsageAndExit(void)
{
  std::cout << "\tUsage: ./icosahedron <testnumber> [depth]" << std::endl;
  exit(1);
}

int main(int argc, char** argv)
{
  if ( argc < 1 )
  {
    showUsageAndExit();
  } else {
    // Set the global test numbers to
    testNumber = atol(argv[1]);

    // check for a valid test number
    if (testNumber < 1 || testNumber > 6) {
      std::cout << "--  Invalid test number " << +testNumber << ". Valid test numbers range from 1 to 6." << std::endl;
      exit(2);
    }

    // if we are running test 5 or 6, we'll need a 2nd argument
    if (testNumber > 4) {
      if ( argc < 3 ) {
        showUsageAndExit();
      } else if ( argc > 3 ) {
        std::cout << "--  Invalid number of arguments. Ignoring all excess arguments." << std::endl;
      }
      depth = atol(argv[2]);
      // check for a valid division value
      if (depth > 5) {
        std::cout << "--  Max depth exceeded. Adjusting depth from " << +depth << " to 5." << std::endl;
        depth = 5;
      } else if (depth < 1) {
        std::cout << "--  Min depth is 1. Adjusting depth from " << +depth << " to 1." << std::endl;
        depth = 1;
      }
    } else {
      if ( argc != 2 ) {
        std::cout << "--  Invalid number of arguments. Ignoring all excess arguments." << std::endl;
      }
    }
  }

  // Initialize glut  and create your window here
  glutInit(&argc, argv);
  // double buffering
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  // get our screen dimensions & set our window name depending on what are parameters are
  const size_t display_width = glutGet(GLUT_SCREEN_WIDTH);
  const size_t display_height = glutGet(GLUT_SCREEN_HEIGHT);
  std::ostringstream name_strm;
  name_strm << "Icosahedron\t(Test: " << testNumber << ", Depth: " << depth << ")";
  std::string window_name = name_strm.str();

  glutInitWindowSize(display_width / 4, display_height / 2);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("");
  glutSetWindowTitle(window_name.c_str());

  switch (testNumber) {
  case 1:
    shouldRotate = false;
    break;

  case 2:
    shouldRotate = true;
    break;

  case 3:
    shouldRotate = false;
    break;

  case 4:
    shouldRotate = true;
    break;

  case 5:
    shouldRotate = false;
    break;

  case 6:
    shouldRotate = true;
    break;

  default:
    showUsageAndExit();
    break;
  }

  // Set your glut callbacks here
  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutTimerFunc(1000.0 / updateRate, timer, 0);

  // Enter the glut main loop here
  Init();
  glutMainLoop();
  return 0;
}
