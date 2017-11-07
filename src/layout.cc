//
//  layout.cpp
//  layout++
//
//  Created by Andrei Kashcha on 5/21/15.
//  Copyright (c) 2015 Andrei Kashcha. All rights reserved.
//

#include "layout.h"
#include <iostream>
#include <cmath>
#include <map>

Layout::Layout() :tree(settings) {}

void Layout::init(int* bodyIds, size_t bodyIdSize, int* links, long size) {
  random = Random(42);
  initBodies(bodyIds, bodyIdSize, links, size);

  // Now the graph is initialized. Let's make sure we get
  // good initial positions:
  setDefaultBodiesPositions();
}

void Layout::loadPositionsFromArray(int *initialPositions) {
  for (size_t i = 0; i < bodies.size(); ++i) {
    Vector2 initialPos(initialPositions[i * 3 + 0], //+ Random::nextDouble(),
                       initialPositions[i * 3 + 1] //+ Random::nextDouble(),
                       );
    bodies[i].setPos(initialPos);
  }
}

void Layout::setDefaultBodiesPositions() {
  size_t maxBodyId = bodies.size();
  for (size_t i = 0; i < maxBodyId; ++i) {
    Body *body = &(bodies[i]);
    if (!body->positionInitialized()) {
      Vector2 initialPos(random.nextDouble() * log(maxBodyId) * 100,
                         random.nextDouble() * log(maxBodyId) * 100);
      bodies[i].setPos(initialPos);
    }
    Vector2 *sourcePos = &(body->pos);
    // init neighbours position:
    for (size_t j = 0; j < body->springs.size(); ++j) {
      if (!bodies[body->springs[j]].positionInitialized()) {
        Vector2 neighbourPosition(
                                  sourcePos->x + random.next(settings.springLength) - settings.springLength/2,
                                  sourcePos->y + random.next(settings.springLength) - settings.springLength/2
                                  );
        bodies[j].setPos(neighbourPosition);
      }
    }
  }
}

void Layout::initBodies(int* bodyIds, size_t bodyIdSize, int* links, long size) {

  std::map<int, size_t> bodyMap;

  bodies.reserve(bodyIdSize);
  for (int i = 0; i < bodyIdSize; i++) {
    bodies.push_back(Body(bodyIds[i]));
    bodyMap[bodyIds[i]] = i;
  }

  // Now that we have bodies, let's add links:
  for (int i = 0; i < size; i+=2) {
    bodies[bodyMap.at(links[i])].springs.push_back(bodyMap.at(links[i+1]));
    bodies[bodyMap.at(links[i+1])].incomingCount += 1;
  }

  // Finally, update body mass based on total number of neighbours:
  for (size_t i = 0; i < bodies.size(); i++) {
    Body *body = &(bodies[i]);
    // TODO: Dividing by 2 rather than 3 due to changing to 2d only?
    body->mass = 1 + (body->springs.size() + body->incomingCount)/3.0;
  }
}

void Layout::setBodiesWeight(int *weights) {
    // FIXME: Verify that size of the weights matches size of the bodies.
    // Unfortunately current graph format does not properly store nodes without
    // edges.
    for (size_t i = 0; i < bodies.size(); i++) {
        Body *body = &(bodies[i]);
        body->mass = weights[i];
    }
}

size_t Layout::getBodiesCount() {
  return bodies.size();
}

bool Layout::step() {
  accumulate();
  double totalMovement = integrate();
  // cout << totalMovement << " move" << endl;
  return totalMovement < settings.stableThreshold;
}

void Layout::accumulate() {
  tree.insertBodies(bodies);

  #pragma omp parallel for
  for (size_t i = 0; i < bodies.size(); i++) {
    Body* body = &bodies[i];
    body->force.reset();

    tree.updateBodyForce(&(*body));
    updateDragForce(&(*body));
  }

  #pragma omp parallel for
  for (size_t i = 0; i < bodies.size(); i++) {
    Body* body = &bodies[i];
    updateSpringForce(&(*body));
  }
}

double Layout::integrate() {
  double dx = 0, tx = 0,
  dy = 0, ty = 0,
  timeStep = settings.timeStep;

//dx should be private or defined inside loop
 //tx need to be reduction variable, or its value will be unpredictable.
  #pragma omp parallel for reduction(+:tx,ty) private(dx,dy)
  for (size_t i = 0; i < bodies.size(); i++) {
    Body* body = &bodies[i];
    double coeff = timeStep / body->mass;

    body->velocity.x += coeff * body->force.x;
    body->velocity.y += coeff * body->force.y;

    double vx = body->velocity.x,
    vy = body->velocity.y,
    v = sqrt(vx * vx + vy * vy);

    if (v > 1) {
      body->velocity.x = vx / v;
      body->velocity.y = vy / v;
    }

    dx = timeStep * body->velocity.x;
    dy = timeStep * body->velocity.y;

    body->pos.x += dx;
    body->pos.y += dy;

    tx += abs(dx); ty += abs(dy);
  }

  return (tx * tx + ty * ty)/bodies.size();
}

void Layout::updateDragForce(Body *body) {
  body->force.x -= settings.dragCoeff * body->velocity.x;
  body->force.y -= settings.dragCoeff * body->velocity.y;
}

void Layout::updateSpringForce(Body *source) {

  Body *body1 = source;
  for (size_t i = 0; i < source->springs.size(); ++i){
    Body *body2 = &(bodies[source->springs[i]]);

    double dx = body2->pos.x - body1->pos.x;
    double dy = body2->pos.y - body1->pos.y;
    double r = sqrt(dx * dx + dy * dy);

    if (r == 0) {
      dx = (random.nextDouble() - 0.5) / 50;
      dy = (random.nextDouble() - 0.5) / 50;
      r = sqrt(dx * dx + dy * dy);
    }

    double d = r - settings.springLength;
    double coeff = settings.springCoeff * d / r;

    body1->force.x += coeff * dx;
    body1->force.y += coeff * dy;

    body2->force.x -= coeff * dx;
    body2->force.y -= coeff * dy;
  }
}
