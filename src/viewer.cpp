#include "viewer.hpp"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>

Monitor::Monitor():scaleIndex(1.0){
}

void Monitor::updateStarting(int x,int y){
      startingX = x;
      startingY = y;
  }
double Monitor::getScaleIndex(){
  return scaleIndex;
}
void Monitor::updateEnding(int x,int y){
    endingX=x;
    endingY=y;
  }
  void Monitor::setStart(int start){
    startingTime = start;
    direction = 0;//clear direction.
    XangleDiff=0;
    YangleDiff=0;
    ZangleDiff=0;
    gravityTimer.disconnect();
  }
  bool Monitor::gravityUpdate(){
    if(direction==1){
    currentXRatio+=XangleDiff;
    currentYRatio+=YangleDiff;
    currentZRatio+=ZangleDiff;
  }
  else if (direction==2){
    currentXRatio-=XangleDiff;
    currentYRatio-=YangleDiff;
    currentZRatio-=ZangleDiff;
  }
    return true;
  }
  void Monitor::setEnd(int end,int x){
    timeDiff = end - startingTime;
    startingTime = 0;
    std::cerr<<"Time diff is"<<timeDiff<<std::endl;
    XangleDiff/=(double)timeDiff;
    YangleDiff/=(double)timeDiff;
    ZangleDiff/=(double)timeDiff;
    if (x<=startingX){
      direction = 2;//ccw
    }
    else{
      direction = 1;//cw
    }
  }
  int Monitor::getStarting(){
    return startingTime;
  }
 double Monitor::getXRatio(){
    return currentXRatio;
  }
double Monitor::getYRatio(){
    return currentYRatio;
  }
 double Monitor::getZRatio(){
    return currentZRatio;
  }
  void Monitor::reset(){
    currentXRatio=0;
    currentYRatio=0;
    currentZRatio=0;
    scaleIndex=1.0;
    gravityTimer.disconnect();
  }
void Monitor::setButtonType(int type){
     if(type>=1&&type<=3){
      buttonFlag = type;
     }
  }
void Monitor::updateScale(){
    double distance = sqrt((startingX-endingX)*(startingX-endingX) + (startingY-endingY)*(startingY-endingY));
      distance/=(double)1800;
      std::cerr<<"ratio is "<<distance<<std::endl;
      if(startingX<endingX){ //scale up
         if(scaleIndex+distance>=2.0){
           scaleIndex=2.0;
         }
         else{
            scaleIndex+=distance;
         }
      }
      else{//scale down.
          if(scaleIndex-distance<0.5){
            scaleIndex=0.5;
          }
          else{
            scaleIndex-=distance;
          }
      }
  }

  void Monitor::updateRatio(){
    double distance = sqrt((startingX-endingX)*(startingX-endingX) + (startingY-endingY)*(startingY-endingY));
    if(startingX<endingX){
      switch(buttonFlag)
      {
        case 1:
        currentXRatio += (double)distance/800;
        XangleDiff+= (double)distance/800;
         break;
        case 2:
        currentYRatio += (double)distance/800;
        YangleDiff+= (double)distance/800;
          break;
          case 3:
          currentZRatio += (double)distance/800;
          ZangleDiff+= (double)distance/800;
          break;
          default:
          break;
      }
      }
    else{
      switch(buttonFlag)
      {
        case 1:
        currentXRatio -= (double)distance/800;
        XangleDiff+=(double)distance/800;
         break;
        case 2:
        currentYRatio -= (double)distance/800;
        YangleDiff+= (double)distance/800;
          break;
          case 3:
          currentZRatio -= (double)distance/800;
          ZangleDiff+= (double)distance/800;
          break;
          default:
          break;
      }
    }
  }
Viewer::Viewer():game(10,20),currentMode(face)
{ 
  Glib::RefPtr<Gdk::GL::Config> glconfig;

  // Ask for an OpenGL Setup with
  //  - red, green and blue component colour
  //  - a depth buffer to avoid things overlapping wrongly
  //  - double-buffered rendering to avoid tearing/flickering
  glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB |
                                     Gdk::GL::MODE_DEPTH |
                                     Gdk::GL::MODE_DOUBLE);
  if (glconfig == 0) {
    // If we can't get this configuration, die
    std::cerr << "Unable to setup OpenGL Configuration!" << std::endl;
    abort();
  }

  // Accept the configuration
  set_gl_capability(glconfig);

  // Register the fact that we want to receive these events
  add_events(Gdk::BUTTON1_MOTION_MASK    |
             Gdk::BUTTON2_MOTION_MASK    |
             Gdk::BUTTON3_MOTION_MASK    |
             Gdk::BUTTON_PRESS_MASK      | 
             Gdk::BUTTON_RELEASE_MASK    |
             Gdk::VISIBILITY_NOTIFY_MASK);
  game.reset();

}

Viewer::~Viewer()
{
  // Nothing to do here right now.
}

void Viewer::invalidate()
{
  //Force a rerender
  Gtk::Allocation allocation = get_allocation();
  get_window()->invalidate_rect( allocation, false);
  
}
void Viewer::resetMonitor()
{
  monitor.reset();
}
void Viewer::turnOnWiredMode()
{
  currentMode = wired;
}
void Viewer::turnOnFaceMode()
{
  currentMode = face;
}
void Viewer::on_realize()
{
  // Do some OpenGL setup.
  // First, let the base class do whatever it needs to
  Gtk::GL::DrawingArea::on_realize();
  
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();
  
  if (!gldrawable)
    return;

  if (!gldrawable->gl_begin(get_gl_context()))
    return;

  // Just enable depth testing and set the background colour.
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.7, 0.7, 1.0, 0.0);

  gldrawable->gl_end();
}
void Viewer::draw_coordinate(){
  glBegin(GL_LINES);
  glVertex3d(0.0,0.0,0.0);
  glVertex3d(0.0,50.0,0.0);//Y coordinate axis
  glVertex3d(0.0,0.0,0.0);
  glVertex3d(50.0,0.0,0.0);//X coordinate axis
  glVertex3d(0.0,0.0,0.0);
  glVertex3d(0.0,0.0,50.0);//Z coordinate axis
  glEnd();
}
void Viewer::draw_Cube(int colorIndex)
{
  colorIndex=colorIndex+2;
  glBegin(GL_QUADS);
  //left column;
  // glPushMatrix();
  glColor3d(0.15*colorIndex,0.8,0.07*colorIndex);
  // glNormal3d(0,0,1);
  // glVertex3d(0.0,1.0,0.0);
  // glVertex3d(1.0,1.0,0.0);
  // glVertex3d(1.0,0.0,0.0);
  // glVertex3d(0.0,0.0,0.0);//starting point.Front face.    
  //front face decoration.
  glNormal3d(0,0,1);//front face.
  glVertex3d(0.1,0.1,0.0);
  glVertex3d(0.9,0.1,0.0);
  glVertex3d(0.9,0.9,0.0);
  glVertex3d(0.1,0.9,0.0);
  //front top decoration.
  // glNormal3d
  glNormal3d(0,1,1);
  glVertex3d(0.1,0.9,0.0); //Front top dec.
  glVertex3d(0.9,0.9,0.0);
  glVertex3d(1.0,1.0,-0.1);
  glVertex3d(0.0,1.0,-0.1);

  glNormal3d(-1,0,1);
  glVertex3d(0.0,0.0,-0.1);//Front left dec.
  glVertex3d(0.1,0.1,0.0);
  glVertex3d(0.1,0.9,0.0);
  glVertex3d(0.0,1.0,-0.1);

  glNormal3d(0,-1,1);
  glVertex3d(0.0,0.0,-0.1);//Front bot dec.
  glVertex3d(1.0,0.0,-0.1);
  glVertex3d(0.9,0.1,0.0);
  glVertex3d(0.1,0.1,0.0);

  glNormal3d(1,0,1);
  glVertex3d(0.9,0.1,0.0); //front Right dec.
  glVertex3d(1.0,0.0,-0.1);
  glVertex3d(1.0,1.0,-0.1);
  glVertex3d(0.9,0.9,0.0);

  //front dec end.

  //Top face.
  glColor3d(0.09*colorIndex,0.01*colorIndex,0.1*colorIndex);
  glNormal3d(0,1,0);
  // glVertex3d(0.0,1.0,0.0);//Top face.
  // glVertex3d(0.0,1.0,-1.0);
  // glVertex3d(1.0,1.0,-1.0);
  // glVertex3d(1.0,1.0,0.0);
  glVertex3d(0.0,1.0,-0.1);//Top face.
  glVertex3d(1.0,1.0,-0.1);
  glVertex3d(1.0,1.0,-0.9);
  glVertex3d(0.0,1.0,-0.9);

 
  glColor3d(0.04*colorIndex,0.0,0.0914*colorIndex);
  glNormal3d(-1,0,0);
  // glVertex3d(0.0,0.0,0.0);//Left face.
  // glVertex3d(0.0,1.0,0.0);
  // glVertex3d(0.0,1.0,-1.0);
  // glVertex3d(0.0,0.0,-1.0);

  glVertex3d(0.0,0.0,-0.1); //Left face.
  glVertex3d(0.0,1.0,-0.1);
  glVertex3d(0.0,1.0,-0.9);
  glVertex3d(0.0,0.0,-0.9);

  glColor3d(0.0*colorIndex,0.124*colorIndex,0.025*colorIndex);
  glNormal3d(1,0,0);
  // glVertex3d(1.0,1.0,0.0);//Right face.
  // glVertex3d(1.0,1.0,-1.0);
  // glVertex3d(1.0,0.0,-1.0);
  // glVertex3d(1.0,0.0,0.0);
  glVertex3d(1.0,0.0,-0.1);//Right Face.
  glVertex3d(1.0,0.0,-0.9);
  glVertex3d(1.0,1.0,-0.9);
  glVertex3d(1.0,1.0,-0.1);

  glColor3d(0.148*colorIndex,0.7*colorIndex,0.0912*colorIndex);
  glNormal3d(0,-1,0);
  // glVertex3d(0.0,0.0,0.0);//Buttom face
  // glVertex3d(0.0,0.0,-1.0);
  // glVertex3d(1.0,0.0,-1.0);
  // glVertex3d(1.0,0.0,0.0);
  glVertex3d(0.0,0.0,-0.1);
  glVertex3d(0.0,0.0,-0.9);
  glVertex3d(1.0,0.0,-0.9);
  glVertex3d(1.0,0.0,-0.1);

  glColor3d(0.1*colorIndex,0.03184*colorIndex,0.125*colorIndex);
  glNormal3d(0,0,-1);
  // glVertex3d(0.0,0.0,-1.0);//Back.
  // glVertex3d(0.0,1.0,-1.0);
  // glVertex3d(1.0,1.0,-1.0);
  // glVertex3d(1.0,0.0,-1.0);
  glVertex3d(0.1,0.1,-1.0); //Back face.
  glVertex3d(0.1,0.9,-1.0);
   glVertex3d(0.9,0.9,-1.0);
  glVertex3d(0.9,0.1,-1.0);

  glNormal3d(0,1,-1);
  glVertex3d(0.1,0.9,-1.0); //Back top dec.
  glVertex3d(0.0,1.0,-0.9);
  glVertex3d(1.0,1.0,-0.9);
  glVertex3d(0.9,0.9,-1.0);
  
    glNormal3d(-1,0,-1);
   glVertex3d(0.0,0.0,-0.9);//Back left dec.
   glVertex3d(0.0,1.0,-0.9);
   glVertex3d(0.1,0.9,-1.0);
  glVertex3d(0.1,0.1,-1.0);
   
   glNormal3d(0,-1,-1);
  glVertex3d(0.0,0.0,-0.9);//Back bot dec.
  glVertex3d(0.1,0.1,-1.0);
  glVertex3d(0.9,0.1,-1.0);
  glVertex3d(1.0,0.0,-0.9);
 
  glNormal3d(1,0,-1);
  glVertex3d(0.9,0.1,-1.0); //Back Right dec.
  glVertex3d(0.9,0.9,-1.0);
    glVertex3d(1.0,1.0,-0.9);
  glVertex3d(1.0,0.0,-0.9);

  
   

  
  glEnd();
}

void Viewer::draw_the_wall(){
  glPushMatrix();
  glTranslated(-1.0,-1.0,0.0);
  glScaled(12.0,1.0,1.0);
  draw_Cube(1.0);
  // glTranslated(1.0,1.0,0.0);
  // glTranslated(-1.0,0.0,0.0);
  // glScaled(1.0,20.0,1.0);
  glPopMatrix();
  glPushMatrix();
  glTranslated(-1.0,0.0,0.0);
  glScaled(1.0,24.0,1.0);
  draw_Cube(1.0);
  glPopMatrix();
  glPushMatrix();
  glTranslated(10.0,0.0,0.0);
  glScaled(1.0,24.0,1.0);
  draw_Cube(1.0);
  glPopMatrix();
}

bool Viewer::on_expose_event(GdkEventExpose* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;

  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  // Clear the screen

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Modify the current projection matrix so that we move the 
  // camera away from the origin.  We'll draw the game at the
  // origin, and we need to back up to see it.
  if(currentMode==wired){
glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}
else if(currentMode==face){
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
} 

  glEnable(GL_COLOR_MATERIAL);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glTranslated(0.0, 0.0, -40.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();


  // Not implemented: set up lighting (if necessary)
   glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  GLfloat lightpos[] = {0.0, 0.0, 15.0, 1.0};
glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,1.0);
  GLfloat spe[] = {1.0f,1.0f,1.0f,1.0f};
  glLightfv(GL_LIGHT0,GL_SPECULAR,spe);
//   GLfloat cyan[] = {0.f, .8f, .8f, 1.f};
// glMaterialfv(GL_FRONT, GL_SPECULAR, spe);
//   glColorMaterial(GL_FRONT,GL_SPECULAR);
  // Not implemented: scale and rotate the scene
  glScaled(1.0*monitor.getScaleIndex(),1.0*monitor.getScaleIndex(),1.0*monitor.getScaleIndex());
  // You'll be drawing unit cubes, so the game will have width
  // 10 and height 24 (game = 20, stripe = 4).  Let's translate
  // the game so that we can draw it starting at (0,0) but have
  // it appear centered in the window.
    glRotated(monitor.getXRatio()*360,1.0,0.0,0.0);
   glRotated(monitor.getYRatio()*360,0.0,1.0,0.0);
    glRotated(monitor.getZRatio()*360,0.0,0.0,1.0);
    glTranslated(0.0, -12.0, 0.0);
  glTranslated(-5.0,0.0,0.0);
  glColor3d(1.0,0.0,1.0);
  draw_coordinate();
  glColor3d(1.0, 1.0, 0.0);
  // We pushed a matrix onto the PROJECTION stack earlier, we 
  // need to pop it.
  draw_the_wall();
  // draw_the_wall();
  for(int i=0;i<10;i++){
    for(int j=0;j<24;j++){
      int colorIndex = game.get(j,i);
      if(colorIndex!=-1){
        // std::cerr<<"color index is "<<colorIndex<<std::endl;
        glPushMatrix();
        glTranslated((double)i,(double)j,0.0);
        draw_Cube(colorIndex);
        glPopMatrix();
      }
    }
  }
  
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  // Swap the contents of the front and back buffers so we see what we
  // just drew. This should only be done if double buffering is enabled.
  gldrawable->swap_buffers();

  gldrawable->gl_end();

  return true;
}

bool Viewer::on_configure_event(GdkEventConfigure* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;
  
  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  // Set up perspective projection, using current size and aspect
  // ratio of display

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, event->width, event->height);
  gluPerspective(40.0, (GLfloat)event->width/(GLfloat)event->height, 0.1, 1000.0);

  // Reset to modelview matrix mode
  
  glMatrixMode(GL_MODELVIEW);

  gldrawable->gl_end();

  return true;
}

bool Viewer::on_button_press_event(GdkEventButton* event)
{
  std::cerr << "Stub: Button " << event->button << " pressed at" << event->x<<" ,"<< event->y<<std::endl;
  monitor.updateStarting(event->x,event->y);
  monitor.setButtonType(event->button);
  monitor.setStart(event->time);
  std::cerr << "time is " <<event->time<<std::endl;
  // monitor.isRotating = true;
  return true;
}

bool Viewer::on_button_release_event(GdkEventButton* event)
{
  std::cerr << "Stub: Button " << event->button << " released" << std::endl;
 
  monitor.setEnd(event->time,event->x);
  if(event->x-monitor.getStarting()>5){
  sigc::slot0<bool>gravityTimeSlot = sigc::mem_fun(monitor,&Monitor::gravityUpdate);
  monitor.gravityTimer=Glib::signal_timeout().connect(gravityTimeSlot,10);
}
   monitor.updateEnding(0,0);
  monitor.updateStarting(0,0);
  return true;
}

bool Viewer::on_motion_notify_event(GdkEventMotion* event)
{
  std::cerr << "Stub: Motion at " << event->x << ", " << event->y << std::endl;
  monitor.updateEnding(event->x,event->y);
  if(event->state & GDK_SHIFT_MASK){
    monitor.updateScale();
    return true;
  }
  monitor.updateRatio();
  // invalidate();
    return true;
}
