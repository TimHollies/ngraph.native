//
//  layout.h
//  layout++
//
//  Created by Andrei Kashcha on 5/21/15.
//  Copyright (c) 2015 Andrei Kashcha. All rights reserved.
//

#ifndef __layout____layout__
#define __layout____layout__

#include <vector>
#include "primitives.h"
#include "quadTree.h"
#include "Random.h"


class Layout {
  Random random;
  std::vector<Body> bodies;
  LayoutSettings settings;
  QuadTree tree;
  
  void accumulate();
  double integrate();
  void updateDragForce(Body *body);
  void updateSpringForce(Body *spring);

  void initBodies(int* bodyIds, size_t bodyIdSize, int *links, long size);

  void setDefaultBodiesPositions();
  void loadPositionsFromArray(int *initialPositions);
  
public:
  Layout();
  void init(int* bodyIds, size_t bodyIdSize, int *links, long size);
  void setBodiesWeight(int *weights);
  bool step();
  size_t getBodiesCount();
  std::vector<Body> *getBodies() { return &bodies; };
};

#endif /* defined(__layout____layout__) */
