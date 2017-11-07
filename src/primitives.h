//
//  primitives.h
//  layout++
//
//  Created by Andrei Kashcha on 5/21/15.
//  Copyright (c) 2015 Andrei Kashcha. All rights reserved.
//

#ifndef layout___primitives_h
#define layout___primitives_h
#include <cmath>        // std::abs
#include <vector>

using namespace std;

struct LayoutSettings {
  double stableThreshold = 0.009;
  double gravity = -1.2;
  double theta = 1.2;
  double dragCoeff = 0.02;
  double springCoeff = 0.0008;
  double springLength = 30;
  double timeStep = 20;
};

struct Vector2 {
  double x = 0.0;
  double y = 0.0;

  Vector2(double _x, double _y) :
  x(_x), y(_y) {};

  Vector2() {}

  void reset () {
    x = y = 0;
  }

  bool sameAs(const Vector2 &other) {

    double dx = std::abs(x - other.x);
    double dy = std::abs(y - other.y);

    return (dx < 1e-8 && dy < 1e-8);
  }
};

struct Body {
  Vector2 pos;
  Vector2 prevPos;
  Vector2 force;
  Vector2 velocity;
  double mass = 1.0;

  vector<int> springs; // these are outgoing connections.
  // This is just a number of incoming connections for this body,
  // so we can count its mass appropriately.
  int incomingCount = 0;

  Body() { }
  Body(Vector2 _pos): pos(_pos), prevPos(_pos) {}

  void setPos(const Vector2 &_pos) {
    pos = _pos;
    prevPos = _pos;
  }

  bool positionInitialized() {
    return pos.x != 0 || pos.y != 0;
  }
};
class NotEnoughQuadSpaceException: public exception {};

#endif
