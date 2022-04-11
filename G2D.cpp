#include "G2D.h"

#ifdef __linux__
#include "GL/glut.h"
#endif

#ifdef _WIN32
#include "glut.h"
#endif

#include <chrono>
#include <cmath>
#include <map>
#include <string>
#include <vector>

using namespace std;

const Color Color::Black = Color(0, 0, 0);
const Color Color::White = Color(1, 1, 1);
const Color Color::Red = Color(1, 0, 0);
const Color Color::Green = Color(0, 1, 0);
const Color Color::Blue = Color(0, 0, 1);
const Color Color::Magenta = Color(1, 0, 1);
const Color Color::Cyan = Color(0, 1, 1);
const Color Color::Yellow = Color(1, 1, 0);
const Color Color::Gray = Color(0.5, 0.5, 0.5);

/////////////////////////////////////////////////////////////
//
//	    Geometry
//
/////////////////////////////////////////////////////////////

void G2D::DrawRectangle(V2 P1, V2 Size, Color c, bool fill) {
  glLineWidth(1);
  glColor4d(c.R, c.G, c.B, c.A);
  glDisable(GL_TEXTURE_2D);

  if (fill)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glRecti(P1.x, P1.y, (P1.x + Size.x), (P1.y + Size.y));
}

void G2D::DrawLine(V2 P1, V2 P2, Color c) {
  glLineWidth(1);
  glColor4d(c.R, c.G, c.B, c.A);

  glBegin(GL_LINES);
  glVertex2f(P1.x, P1.y);
  glVertex2f(P2.x, P2.y);
  glEnd();
}

void G2D::DrawCircle(V2 C, float r, Color c, bool fill) {
  vector<V2> LPoints;

  int lineAmount = r / 4; //  nb of triangles used to draw circle
  if (lineAmount < 20)
    lineAmount = 20;

  // GLfloat radius = 0.8f; //radius
  double step = 2 * PI / lineAmount;

  for (int i = 0; i <= lineAmount; i++)
    LPoints.push_back(V2(C.x + r * cos(i * step), C.y + r * sin(i * step)));

  G2D::DrawPolygon(LPoints, c, fill);
}

void G2D::DrawPolygon(vector<V2> &PointList, Color c, bool fill) {
  glLineWidth(1);
  if (fill)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glColor4d(c.R, c.G, c.B, c.A);

  glBegin(GL_POLYGON);
  for (V2 P : PointList)
    glVertex2f(P.x, P.y);
  glEnd();
}

/////////////////////////////////////////////////////////////
//
//	    Font
//
/////////////////////////////////////////////////////////////

void DrawString(V2 pos, string text, float fontSize, float thickness, Color c,
                bool FontMono) {
  glColor4f(c.R, c.G, c.B, c.A);

  // EPAISSEUR de la font
  glLineWidth(thickness);

  glPushMatrix();
  glTranslatef(pos.x, pos.y, 0);
  glScalef(1 / 152.38, 1 / 152.38, 1 / 152.38);
  glScalef(fontSize, fontSize, fontSize);
  const char *cc = text.c_str();
  for (char *p = (char *)cc; *p; p++) {
    if (FontMono)
      glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p);
    else
      glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
  }
  glLineWidth(1);
  glPopMatrix();
}

void G2D::DrawStringFontMono(V2 pos, string text, float fontSize,
                             float thickness, Color c) {
  DrawString(pos, text, fontSize, thickness, c, true);
}

/////////////////////////////////////////////////////////////
//
//	    Key Management
//
/////////////////////////////////////////////////////////////

void keyboard(unsigned char key, int x, int y) {
  if (key == 0x1b)
    exit(0); // ESC
}

/////////////////////////////////////////////////////////////
//
//	    window size
//
/////////////////////////////////////////////////////////////

int Wwidth, Wheight;

void reshape(int w, int h) { glutReshapeWindow(Wwidth, Wheight); }

/////////////////////////////////////////////////////////////
//
//	    Init
//
/////////////////////////////////////////////////////////////

void G2D::ClearScreen(Color c) {
  glClearColor(c.R, c.G, c.B, c.A);
  glClear(GL_COLOR_BUFFER_BIT);
}

void G2D::Show() {
  // glFlush(); single buffer
  glutSwapBuffers(); // double buffered
}

void G2D::InitWindow(int &argc, char **&argv, V2 WindowSize, V2 WindowStartPos,
                     std::string name) {
  glutInit(&argc, argv);

  Wwidth = WindowSize.x;
  Wheight = WindowSize.y;

  glutInitWindowPosition(WindowStartPos.x, WindowStartPos.y);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(Wwidth, Wheight);
  glutCreateWindow(name.c_str());
}

void (*_animCallBack)(void);

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::duration<double> duration;

auto T0 = Time::now();
auto TPreviousFrame = Time::now();
auto TNewFrame = Time::now();

bool FirstFrame = true;

void anim(int value) {
  // timing
  if (FirstFrame) {
    T0 = Time::now(); // d�marrage du jeu
    TPreviousFrame = Time::now();
    TNewFrame = Time::now();
    FirstFrame = false;
  } else {
    TPreviousFrame = TNewFrame;
    TNewFrame = Time::now();
  }

  _animCallBack();
  glutPostRedisplay();
  glutTimerFunc(10, anim, value);
}

double G2D::ElapsedTimeFromStartSeconds() {
  duration elapsed = TNewFrame - T0;
  return elapsed.count();
}

double G2D::ElapsedTimeFromLastCallbackSeconds() {
  duration elapsed = TNewFrame - TPreviousFrame;
  return elapsed.count();
}

void G2D::Run(void animCallBack(), void render()) {

  glLoadIdentity();
  glOrtho(0, Wwidth, 0, Wheight, -1, 10); // donne l'echelle x et y
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glEnable(GL_LINE_SMOOTH);
  glLineWidth(1); // not avaliable on all platform
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  // glEnable(GL_POLYGON_SMOOTH); � �viter rendu erron�

  glutReshapeFunc(reshape);

  // callbacks
  glutKeyboardFunc(keyboard);

  // logic
  _animCallBack = animCallBack;
  glutDisplayFunc(render);
  glutTimerFunc(100, anim, 0);
  glutMainLoop();
}