#pragma warning(disable : 4996)

#include "G2D.h"
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

// 0 : orthogonal
// + : same direction
// - : opposite direction
// float sameDirection(V2 v1, V2 v2) { return v1 * v2; }

bool sameSide(V2 A, V2 B, V2 M, V2 P) {
  V2 AM = M - A;
  V2 AP = P - A;
  V2 AB = B - A;
  float r1 = AB ^ AM;
  float r2 = AB ^ AP;
  return r1 * r2 >= 0;
}

bool interSegmentSegment(V2 A, V2 B, V2 E, V2 F) {
  return (!sameSide(A, B, E, F) && !sameSide(E, F, A, B));
}
// V2 Rot90(V2 v) { return V2(-v.y, v.x); }

float dist(V2 A, V2 B, V2 P) {
  V2 AB(B - A);
  V2 N(AB.y, -AB.x);
  N.normalize();
  V2 AP(P - A);
  return abs(AP * N);
}

float dist(V2 A, V2 B) { return V2(B - A).norm(); }

/**
 * Allonge un segment a gauche et a droite de r
 * @param A extremité gauche
 * @param B extremité droite
 * @param r rayon
 **/
V2 allongeSegment(V2 A, V2 B, float r) {
  V2 T(B - A);
  T.normalize();
  V2 A2 = A - r * T;
  V2 B2 = B + r * T;
  return V2(B2 - A2);
}
bool interSegmentBall(V2 A, V2 B, V2 C) {
  V2 AB(B - A);
  V2 AC(C - A);
  V2 BC(C - B);
  V2 BA(A - B);
  float r = 15;
  if (dist(A, C) <= r) {
    return true;
  }
  if (dist(B, C) <= r) {
    return true;
  }
  if (AB * AC >= 0 && BA * BC >= 0) {

    if (dist(AB, C) <= r) {
      return true;
    }
  }
  return false;
}

struct Cible {
  int x1, y1, x2, y2, nbLines;
  bool isActive;

  Cible(int _x1, int _y1, int _x2, int _y2) {
    x1 = _x1;
    y1 = _y1;
    x2 = _x2;
    y2 = _y2;
    nbLines = 20;
    isActive = true;
  }
  V2 getA() { return V2(x1, y1); }
  V2 getB() { return V2(x2, y2); }

  V2 prolongeA(float r) {
    V2 A = getA();
    V2 B = getB();
    V2 T(B - A);
    T.normalize();
    V2 A2 = A - r * T;
    return A2;
  }
  V2 prolongeB(float r) {
    V2 A = getA();
    V2 B = getB();
    V2 T(B - A);
    T.normalize();
    V2 B2 = B + r * T;
    return B2;
  }
  bool getIsActive() { return isActive; }
  void setIsActive(bool _isActive) { isActive = _isActive; }

  void drawCible(void) {
    if (isActive) {
      for (int i = 0; i < nbLines; i++) {
        G2D::DrawLine(V2(x1 - i, y1), V2(x2 - i, y2), Color::Red);
      }
    } else {
      for (int i = 0; i < nbLines; i++) {

        G2D::DrawLine(V2(x1 - i, y1), V2(x2 - i, y2), Color::Green);
      }
    }
  }
};

struct Bumper {
  float x, y, r, time;
  bool collision;
  int state;

  Bumper(float _x, float _y, float _r) {
    x = _x;
    y = _y;
    r = _r;
    collision = false;
    time = 0;
    state = 0;
  }
  void setTime(float _time) { time = _time; }
  float getTime(void) { return time; }
  void setState(int _state) { state = _state; }
  bool getCollision(void) { return collision; }
  void setCollision(bool _collision) { collision = _collision; }

  V2 getV2(void) { return V2(x, y); }
  void drawBumper(void) { G2D::DrawCircle(getV2(), r, Color::Blue, true); }
  /**
   * @brief mets à jours l'état et dessine le nouveau flash
   * @param time le temps écoulé depuis la dernière collision
   * @param _collision true si la collision est réalisée, false sinon
   */
  void setAndDrawState(float time, bool _collision) {
    int nbState = 35;
    float T = 2.0 / nbState;
    float dt = G2D::ElapsedTimeFromLastCallbackSeconds();

    if ((time == 0 && _collision) || time != 0) {

      // std::cout << "time : " << time << "collision " << _collision <<
      // std::endl;
      setTime(time + dt);
      for (int i = 0; i < nbState; i++) {
        if (time >= nbState * T) {
          setTime(0);
          setState(0);
        } else if (time >= i * T && time < (i + 1) * T) {
          setState(i);
          drawCurrentState(nbState);
          break;
        }
      }
    }
  }
  /**
   * @brief dessine l'état courant du flash
   * @param rayon la différence entre le rayon du flash et le rayon du bumper
   */
  void drawBorder(int rayon) {
    G2D::DrawCircle(getV2(), r + rayon, Color::Red, true);
    G2D::DrawCircle(getV2(), r, Color::Blue, true);
  }
  void drawCurrentState(int nbState) { drawBorder(nbState - state); }
};

// information de partie
struct GameData {
  int idFrame = 0;
  bool collision = false;
  int HeighPix = 800;
  int WidthPix = 600;
  V2 BallPos = V2(550, 30);
  V2 BallSpeed = V2(400, 400);

  Bumper bumper1 = Bumper(200, 400, 40);
  Bumper bumper2 = Bumper(400, 400, 40);
  Bumper bumper3 = Bumper(300, 550, 40);

  Bumper bumpers[3] = {bumper1, bumper2, bumper3};

  Cible cibleGauche1 = Cible(50, 500, 120, 640);
  Cible cibleGauche2 = Cible(50, 300, 120, 440);
  Cible cibleGauche3 = Cible(50, 100, 120, 240);

  Cible cibleDroite1 = Cible(450, 640, 550, 500);
  Cible cibleDroite2 = Cible(450, 440, 550, 300);
  Cible cibleDroite3 = Cible(450, 240, 550, 100);

  Cible cibles[6] = {cibleGauche1, cibleGauche2, cibleGauche3,
                     cibleDroite1, cibleDroite2, cibleDroite3};

  V2 LP[11] = {V2(600, 550), V2(585, 596), V2(542, 638), V2(476, 671),
               V2(392, 692), V2(300, 700), V2(207, 692), V2(123, 671),
               V2(57, 638),  V2(14, 596),  V2(0, 550)};
};

GameData G;

V2 Rebond(V2 v, V2 n) {
  n.normalize();
  return v - 2 * (v * n) * n;
}

V2 rotate(V2 v, float angle) {
  float x = v.x * cos(angle) - v.y * sin(angle);
  float y = v.x * sin(angle) + v.y * cos(angle);
  V2 vbis = V2(x, y);
  // std::cout << "alpha=" << angle * 180 / PI << "°";
  // std::cout << " v=" << v;
  // std::cout << " v'=" << vbis << endl;
  return vbis;
}

// float dist(V2 A, V2 B) { return (B - A).norm(); }

bool pointInCercle(V2 C, float r, V2 P) { return (dist(C, P) <= r); }

bool detectCircleCollision(V2 ball, Bumper bumper) {
  const int R = bumper.r + 15;
  return dist(ball, V2(bumper.x, bumper.y)) <= R;
}

/**
 * @brief calcule les nouvelles coordonnées de la balle
 */
void nextMove(void) {
  double dt = G2D::ElapsedTimeFromLastCallbackSeconds();
  G.BallPos.x += G.BallSpeed.x * dt;
  G.BallPos.y += G.BallSpeed.y * dt;
}
/**
 * @brief calcule la nouvelle vitesse dans le cas d'une collision contre un des
 * 4 murs
 */
void handleWall(float x, float y) {
  float alpha = (rand() % 601 - 300) / 100.0;
  if (G.BallPos.y > G.HeighPix - 15) {
    G.BallPos.x = x;
    G.BallPos.y = y;
    G.BallSpeed = Rebond(G.BallSpeed, V2(0, -1));
    G.BallSpeed = rotate(G.BallSpeed, alpha * PI / 180);
  } else if (G.BallPos.y < 15) {
    G.BallPos.x = x;
    G.BallPos.y = y;
    G.BallSpeed = Rebond(G.BallSpeed, V2(0, 1));
    G.BallSpeed = rotate(G.BallSpeed, alpha * PI / 180);
  } else if (G.BallPos.x < 15) {
    G.BallPos.x = x;
    G.BallPos.y = y;
    G.BallSpeed = Rebond(G.BallSpeed, V2(1, 0));
    G.BallSpeed = rotate(G.BallSpeed, alpha * PI / 180);
  } else if (G.BallPos.x > G.WidthPix - 15) {
    G.BallPos.x = x;
    G.BallPos.y = y;
    G.BallSpeed = Rebond(G.BallSpeed, V2(-1, 0));
    G.BallSpeed = rotate(G.BallSpeed, alpha * PI / 180);
  }
}
/**
 * @brief calcule la nouvelle vitesse dans le cas d'une collision contre un des
 * bumper
 */
void handleBumperCollision(Bumper &bumper, float x, float y) {
  if (detectCircleCollision(G.BallPos, bumper)) {
    // ? collision donc retourne la balle a la position avant collision
    G.BallPos.x = x;
    G.BallPos.y = y;
    // ? vecteur normal
    float nx = bumper.x - G.BallPos.x;
    float ny = bumper.y - G.BallPos.y;

    G.BallSpeed = Rebond(G.BallSpeed, V2(nx, ny));
  }
}

/**
 * @brief Gère la collision pour les 3 bumpers
 * @param x ancienne coordonnée
 * @param y ancienne coordonnée
 */
void handleBumpers(float x, float y) {
  // ? pour chacun des bumpers
  for (int i = 0; i < 3; i++) {
    Bumper &bumper = G.bumpers[i];
    handleBumperCollision(bumper, x, y);
  }
}

/**
 * @brief synchroniser la collision des bumper entre la fonction logique et
 * render pour déclencher le flash
 */
void synchronizeBumperCollision(void) {
  for (int i = 0; i < 3; i++) {
    G.bumpers[i].collision = false;
    if (detectCircleCollision(G.BallPos, G.bumpers[i])) {
      G.bumpers[i].collision = true;
    }
  }
}

void handleCibleCollision(V2 n, int x, int y) {
  n.normalize();
  G.BallPos.x = x;
  G.BallPos.y = y;
  G.BallSpeed = Rebond(G.BallSpeed, n);
}

void handleCible(int x, int y) {
  V2 E = V2(x, y);
  V2 F = V2(G.BallPos.x, G.BallPos.y);

  for (int i = 0; i < 6; i++) {
    Cible &cible = G.cibles[i];
    if (cible.getIsActive()) {

      for (int j = 0; j < cible.nbLines; j++) {
        V2 A = V2(cible.x1 - j, cible.y1);
        V2 B = V2(cible.x2 - j, cible.y2);
        V2 AB = B - A;
        V2 n = V2(y, -x);
        if (interSegmentSegment(cible.prolongeA(15), cible.prolongeB(15), E,
                                F)) {
          std::cout << "collision cible" << i << " " << dist(A, B, F) << endl;
          handleCibleCollision(n, x, y);
          cible.setIsActive(false);
          return;
        }
      }
    }
  }
}
void render() {
  G2D::ClearScreen(Color::Black);

  G2D::DrawStringFontMono(V2(80, G.HeighPix - 70), string("Super Flipper"), 50,
                          5, Color::Blue);

  G2D::DrawRectangle(V2(0, 0), V2(G.WidthPix, G.HeighPix), Color::Green);

  G2D::DrawCircle(G.BallPos, 15, Color::Red, true);

  for (int i = 0; i < 3; i++) {
    Bumper &bumper = G.bumpers[i];
    bumper.drawBumper();
    if (bumper.collision) {
      bumper.setAndDrawState(0, true);
    } else {
      bumper.setAndDrawState(bumper.getTime(), false);
    }
  }
  for (int i = 0; i < 6; i++) {
    Cible &cible = G.cibles[i];
    cible.drawCible();
  }
  for (int i = 0; i < 11; i++)
    G2D::DrawLine(G.LP[i], G.LP[i + 1], Color::Green);

  G2D::Show();
}

void Logic() {
  G.idFrame += 1;
  float x = G.BallPos.x;
  float y = G.BallPos.y;
  nextMove();
  synchronizeBumperCollision();
  handleCible(x, y);
  handleWall(x, y);
  handleBumpers(x, y);
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
