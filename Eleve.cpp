#pragma warning(disable : 4996)

#include "G2D.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// information de partie
struct GameData
{
  int idFrame = 0;
  int HeighPix = 800;
  int WidthPix = 600;
  V2 BallPos = V2(550, 30);
  V2 BallSpeed = V2(400, 400);

  V2 LP[11] = {V2(600, 550), V2(585, 596), V2(542, 638), V2(476, 671),
               V2(392, 692), V2(300, 700), V2(207, 692), V2(123, 671),
               V2(57, 638), V2(14, 596), V2(0, 550)};
};

GameData G;

V2 Rebond(V2 v, V2 n)
{
  n.normalize();
  return v - 2 * (v * n) * n;
}

void render()
{

  G2D::ClearScreen(Color::Black);

  G2D::DrawStringFontMono(V2(80, G.HeighPix - 70), string("Super Flipper"), 50,
                          5, Color::Blue);

  G2D::DrawRectangle(V2(0, 0), V2(G.WidthPix, G.HeighPix), Color::Green);

  G2D::DrawCircle(G.BallPos, 15, Color::Red, true);

  G2D::DrawCircle(V2(200, 400), 40, Color::Blue, true);
  G2D::DrawCircle(V2(400, 400), 40, Color::Blue, true);
  G2D::DrawCircle(V2(300, 550), 40, Color::Blue, true);

  for (int i = 0; i < 11; i++)
    G2D::DrawLine(G.LP[i], G.LP[i + 1], Color::Green);

  G2D::Show();
}

void Logic()
{
  G.idFrame += 1;
  double dt = G2D::ElapsedTimeFromLastCallbackSeconds();

  G.BallPos.x += G.BallSpeed.x * dt;
  G.BallPos.y += G.BallSpeed.y * dt;

  if (G.BallPos.y > G.HeighPix - 15)
  {
    G.BallPos.y = G.HeighPix - 15;
    G.BallSpeed = Rebond(G.BallSpeed, V2(0, -1));
  }
  else if (G.BallPos.y < 15)
  {
    G.BallPos.y = 15;
    G.BallSpeed = Rebond(G.BallSpeed, V2(0, 1));
  }
  else if (G.BallPos.x < 15)
  {
    G.BallPos.x = 15;
    G.BallSpeed = Rebond(G.BallSpeed, V2(1, 0));
  }
  else if (G.BallPos.x > G.WidthPix - 15)
  {
    G.BallPos.x = G.WidthPix - 15;
    G.BallSpeed = Rebond(G.BallSpeed, V2(-1, 0));
  }
}

int main(int argc, char *argv[])
{
  // V2 A(5, 10);
  // V2 B = V2(6, 15);
  // V2 AB = B - A;
  // cout << AB << endl;
  // V2 u(5, 7);
  // V2 v(2, 3);
  // cout << "Produit scalaire  : " << (u * v) << endl;
  // cout << "Produit vectoriel : " << (u ^ v) << endl;

  G2D::InitWindow(argc, argv, V2(G.WidthPix, G.HeighPix), V2(200, 200),
                  string("Super Flipper 600 !!"));

  G2D::Run(Logic, render);
}
