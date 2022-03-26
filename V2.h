#pragma once

#include <cmath>
#include <iostream>
struct V2 {
  float x, y;

  V2(float _x, float _y) {
    x = _x;
    y = _y;
  }

  float norm() { return sqrt(x * x + y * y); }
  void normalize() {
    float n = norm();
    x /= n;
    y /= n;
  }
};

// comparaison sur des flottants... traitement sp�cial

bool operator==(const V2 &a, const V2 &b);

// red�finition des op�rateurs standards

V2 operator+(const V2 &a, const V2 &b);
V2 operator-(const V2 &a, const V2 &b);
V2 operator*(float a, const V2 &b);
V2 operator*(const V2 &a, float b);
float operator*(const V2 &a, const V2 &b);
float operator^(const V2 &a, const V2 &b);
std::ostream &operator<<(std::ostream &os, V2 &t);

V2 operator/(const V2 &a, float b);
