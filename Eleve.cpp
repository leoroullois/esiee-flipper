#pragma warning(disable : 4996)

#include "G2D.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Bumper {
  float x, y, r;
  bool visible;
  Bumper(float _x, float _y, float _r, bool _visible) {
    x = _x;
    y = _y;
    r = _r;
    visible = _visible;
  }
  V2 getV2(void) { return V2(x, y); }
  void drawBumper(void) { G2D::DrawCircle(getV2(), r, Color::Blue, visible); }
  void drawBorder(void) {
    // TODO: draw border
    float rayon = r + 100;
    std::cout << "rayon " << rayon << endl;
    G2D::DrawCircle(getV2(), rayon, Color::Red, true);
    G2D::Show();
  }
};
// information de partie
struct GameData {
  int idFrame = 0;
  int HeighPix = 800;
  int WidthPix = 600;
  V2 BallPos = V2(550, 30);
  V2 BallSpeed = V2(400, 400);

  Bumper bumper1 = Bumper(200, 400, 40, true);
  Bumper bumper2 = Bumper(400, 400, 40, true);
  Bumper bumper3 = Bumper(300, 550, 40, true);

  Bumper bumpers[3] = {bumper1, bumper2, bumper3};

  V2 LP[11] = {V2(600, 550), V2(585, 596), V2(542, 638), V2(476, 671),
               V2(392, 692), V2(300, 700), V2(207, 692), V2(123, 671),
               V2(57, 638),  V2(14, 596),  V2(0, 550)};
};

GameData G;

V2 Rebond(V2 v, V2 n) {
  n.normalize();
  return v - 2 * (v * n) * n;
}

void rotate(V2 &v, float angle) {
  float x = v.x * cos(angle) - v.y * sin(angle);
  float y = v.x * sin(angle) + v.y * cos(angle);
  v.x = x;
  v.y = y;
}

// 0 : orthogonal
// + : same direction
// - : opposite direction
float sameDirection(V2 v1, V2 v2) { return v1 * v2; }

bool sameSide(V2 AB, V2 M, V2 P) {
  float r1 = AB ^ M;
  float r2 = AB ^ P;
  return r1 * r2 > 0;
}
V2 Rot90(V2 v) { return V2(-v.y, v.x); }

float dist(V2 A, V2 B, V2 P) {
  V2 AB(B - A);
  V2 N(AB.y, -AB.x);
  N.normalize();
  V2 AP(P - A);
  return abs(AP * N);
}
float dist(V2 A, V2 B) { return (B - A).norm(); }

bool pointInRectangle(float x1, float x2, float y1, float y2, float xP,
                      float yP) {
  return (xP >= x1 && xP <= x2 && yP >= y1 && yP <= y2);
}

bool pointInCercle(V2 C, float r, V2 P) { return (dist(C, P) <= r); }

bool detectCircleCollision(V2 ball, Bumper bumper) {
  const int R = bumper.r + 15;
  return dist(ball, V2(bumper.x, bumper.y)) <= R;
}

void render() {
  std::cout << G.idFrame << endl;
  G2D::ClearScreen(Color::Black);

  G2D::DrawStringFontMono(V2(80, G.HeighPix - 70), string("Super Flipper"), 50,
                          5, Color::Blue);

  G2D::DrawRectangle(V2(0, 0), V2(G.WidthPix, G.HeighPix), Color::Green);

  G2D::DrawCircle(G.BallPos, 15, Color::Red, true);

  for (int i = 0; i < 3; i++) {
    G.bumpers[i].drawBumper();
    // G.bumpers[i].drawBorder();
  }

  for (int i = 0; i < 11; i++)
    G2D::DrawLine(G.LP[i], G.LP[i + 1], Color::Green);

  G2D::Show();
}

void nextMove(void) {
  double dt = G2D::ElapsedTimeFromLastCallbackSeconds();
  G.BallPos.x += G.BallSpeed.x * dt;
  G.BallPos.y += G.BallSpeed.y * dt;
}

void Logic() {
  G.idFrame += 1;
  if (G.BallPos.y > G.HeighPix - 15) {
    G.BallPos.y = G.HeighPix - 15;
    G.BallSpeed = Rebond(G.BallSpeed, V2(0, -1));
  } else if (G.BallPos.y < 15) {
    G.BallPos.y = 15;
    G.BallSpeed = Rebond(G.BallSpeed, V2(0, 1));
  } else if (G.BallPos.x < 15) {
    G.BallPos.x = 15;
    G.BallSpeed = Rebond(G.BallSpeed, V2(1, 0));
  } else if (G.BallPos.x > G.WidthPix - 15) {
    G.BallPos.x = G.WidthPix - 15;
    G.BallSpeed = Rebond(G.BallSpeed, V2(-1, 0));
  }
  for (int i = 0; i < 3; i++) {
    if (detectCircleCollision(G.BallPos, G.bumpers[i])) {
      G.BallSpeed = Rebond(G.BallSpeed, V2(G.bumpers[i].x - G.BallPos.x,
                                           G.bumpers[i].y - G.BallPos.y));
      G.bumpers[i].drawBorder();
    }
  }
  nextMove();
}

void test1(void) {
  V2 A(5, 10);
  V2 B = V2(6, 15);
  V2 AB = B - A;
  cout << AB << endl;
  V2 u(5, 7);
  V2 v(2, 3);
  cout << "Produit scalaire  : " << (u * v) << endl;
  cout << "Produit vectoriel : " << (u ^ v) << endl;
}
void test_collision(void) {
  V2 n(-1, 1);
  V2 r = Rebond(V2(1, 0), n);
  cout << r << endl;
  r = Rebond(V2(1, -1), n);
  cout << r << endl;
  r = Rebond(V2(-1, 1), n);
  cout << r << endl;
  r = Rebond(V2(0, 1), n);
  cout << r << endl;

  n = V2(0, 1);
  r = Rebond(V2(1, -1), n);
  cout << r << endl;
  r = Rebond(V2(0, -1), n);
  cout << r << endl;
  r = Rebond(V2(-1, 1), n);
  cout << r << endl;
  r = Rebond(V2(-1, -1), n);
  cout << r << endl;
}

int main(int argc, char *argv[]) {
  // test1();
  // test_collision();
  G2D::InitWindow(argc, argv, V2(G.WidthPix, G.HeighPix), V2(200, 200),
                  string("Super Flipper 600 !!"));

  G2D::Run(Logic, render);
}
