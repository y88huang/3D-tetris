#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <gtkmm.h>
#include <gtkglmm.h>
#include "game.hpp"
class Monitor {
  int startingX;
  int startingY;
  int endingX;
  int endingY;
  int buttonFlag; //1,2,3;
  double currentXRatio;
  double currentYRatio;
  double currentZRatio;
  int startingTime;
  int timeDiff;
  double XangleDiff;
  double YangleDiff;
  double ZangleDiff;
  double scaleIndex;//range from [0.5,10.0]
  int direction;//0-no rotation,1 clock-wise,2-counter-clockwise.

public:
  Monitor();
   sigc::connection gravityTimer;
  void updateStarting(int x,int y);
  void updateEnding(int x,int y);
  double getXRatio();
  double getYRatio();
  double getZRatio();
  double getScaleIndex();
  int getStarting();
  bool gravityUpdate();
  void setStart(int start);
  void setEnd(int end,int x);
  void getTimeDiff();
    void reset();
  void setButtonType(int type);
  void updateRatio();
  void updateScale();
};

// The "main" OpenGL widget
class Viewer : public Gtk::GL::DrawingArea {
public:
  Viewer();
  virtual ~Viewer();

  // A useful function that forces this widget to rerender. If you
  // want to render a new frame, do not call on_expose_event
  // directly. Instead call this, which will cause an on_expose_event
  // call when the time is right.
  void invalidate();
  void turnOnWiredMode();//switch from wired mode to face mode or the other way around.
  void turnOnFaceMode();
  void resetMonitor();
  Game game;
protected:
  //Temp parameters.
  
  // Events we implement
  // Note that we could use gtkmm's "signals and slots" mechanism
  // instead, but for many classes there's a convenient member
  // function one just needs to define that'll be called with the
  // event.
  void draw_Cube(int colorIndex);
  void draw_coordinate();
  void draw_the_wall();
  void draw_stick();

  // Called when GL is first initialized
  virtual void on_realize();
  // Called when our window needs to be redrawn
  virtual bool on_expose_event(GdkEventExpose* event);
  // Called when the window is resized
  virtual bool on_configure_event(GdkEventConfigure* event);
  // Called when a mouse button is pressed
  virtual bool on_button_press_event(GdkEventButton* event);
  // Called when a mouse button is released
  virtual bool on_button_release_event(GdkEventButton* event);
  // Called when the mouse moves
  virtual bool on_motion_notify_event(GdkEventMotion* event);

private:
  Monitor monitor;
  enum drawMode {wired, face};
  drawMode currentMode;
};

#endif
