#include <simple2d.h>
#include <chipmunk/chipmunk.h>

// Set coordinates for shapes

int circle_x = 50;
int circle_y = 50;

int line_x1 = 0;
int line_y1 = 300;
int line_x2 = 400;
int line_y2 = 350;

int quad_x1 = 437;
int quad_y1 = 430;
int quad_x2 = 493;
int quad_y2 = 406;
int quad_x3 = 687;
int quad_y3 = 444;
int quad_x4 = 462;
int quad_y4 = 560;

// Create Simple 2D and Chipmunk objects and set values

S2D_Window *window;
cpFloat timeStep = 1.0 / 60.0;
cpBody *ballBody;
cpSpace *space;


// The Simple 2D update loop, used to update the game state; this will try to
// run 60 times per second, by default; not guaranteed to be a fixed-size time;
// step (although highly recommended for accurate physics simulations)
void update() {

  // Get and set the ball position and velocity
  cpVect pos = cpBodyGetPosition(ballBody);
  cpVect vel = cpBodyGetVelocity(ballBody);
  circle_x = pos.x;
  circle_y = pos.y;

  // Simulate all the objects in the space by stepping forward through time in
  // small increments called "steps"
  cpSpaceStep(space, timeStep);
}


// The Simple 2D render loop, used to render the game scene
void render() {

  // Draw the static body quad
  S2D_DrawQuad(
    quad_x1, quad_y1, 0.5, 0.5, 1.0, 1.0,
    quad_x2, quad_y2, 0.5, 0.5, 1.0, 1.0,
    quad_x3, quad_y3, 0.5, 0.5, 1.0, 1.0,
    quad_x4, quad_y4, 0.5, 0.5, 1.0, 1.0
  );

  // Draw the static line
  S2D_DrawLine(
    line_x1, line_y1, line_x2, line_y2, 2,
    1.0, 0, 0, 1.0,
    1.0, 0, 0, 1.0,
    1.0, 0, 0, 1.0,
    1.0, 0, 0, 1.0
  );

  // Draw the dynamic ball
  S2D_DrawCircle(circle_x, circle_y, 10, 20, 1.0, 1.0, 1.0, 1.0);
}


// The mouse event handler; when a mouse button is down, subtract x and y
// velocity from ball
void on_mouse(S2D_Event e) {
  switch (e.type) {
    case S2D_MOUSE_DOWN:
      cpBodySetVelocity(ballBody,
        cpv(cpBodyGetVelocity(ballBody).x - 50, cpBodyGetVelocity(ballBody).y - 200));
      break;
  }
}


int main() {

  // Print Simple 2D diagnostics (additional info about the window)
  S2D_Diagnostics(true);

  // Create a gravity vector
  cpVect gravity = cpv(0, 300);

  // Create an empty space and set the gravity
  space = cpSpaceNew();
  cpSpaceSetGravity(space, gravity);

  // Add a static line segment for the ball to interact with / roll off of;
  // attach it to a static body to tell Chipmunk it shouldn't be movable; set
  // the friction and elasticity (bounciness)
  cpShape *line = cpSegmentShapeNew(cpSpaceGetStaticBody(space), cpv(line_x1, line_y1), cpv(line_x2, line_y2), 0);
  cpShapeSetFriction(line, 1);
  cpShapeSetElasticity(line, 0.5);
  cpSpaceAddShape(space, line);

  // Create vertices for a quadrilateral for the ball to interact with
  cpVect quad_verts[] = {
    cpv(quad_x1, quad_y1),
    cpv(quad_x2, quad_y2),
    cpv(quad_x3, quad_y3),
    cpv(quad_x4, quad_y4)
  };

  // Create the quad polygon shape, attach to static body; set the friction and
  // elasticity (bounciness)
  cpShape *quad = cpPolyShapeNew(cpSpaceGetStaticBody(space), 4, quad_verts, cpTransformIdentity, 0);
  cpShapeSetFriction(quad, 1);
  cpShapeSetElasticity(quad, 0.5);
  cpSpaceAddShape(space, quad);

  // Create a ball that will interact with the line and quad; `cpBody` will hold
  // the physical properties of the object, such as mass, position, velocity,
  // angle, etc.; then, attach collision shapes to the cpBody to give it a size
  // and shape

  // set the radius and mass
  cpFloat radius = 10;
  cpFloat mass = 1;

  // The moment of inertia is like mass for rotation; use the `cpMomentFor*()`
  // functions to help you approximate it
  cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero);

  // The `cpSpaceAdd*()` functions return the thing that you are adding
  ballBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));

  // Set the ball position
  cpBodySetPosition(ballBody, cpv(50, 15));

  // Create the collision shape for the ball; you can create multiple collision
  // shapes that point to the same body, they will all be attached to the body
  // and move around to follow it.
  cpShape *ballShape = cpSpaceAddShape(space, cpCircleShapeNew(ballBody, radius, cpvzero));
  cpShapeSetFriction(ballShape, 0.7);

  // Set elasticity (bounce) of the ball
  cpShapeSetElasticity(ballShape, 1.0);

  // Create the Simple 2D window
  window = S2D_CreateWindow(
    "Chipmunk S2D test", 800, 600, update, render, 0
  );

  // Attach the mouse event handler
  window->on_mouse = on_mouse;

  // Show the window
  S2D_Show(window);

  return 0;
}
