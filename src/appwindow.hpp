#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <gtkmm.h>
#include "viewer.hpp"

class AppWindow : public Gtk::Window {
public:
  AppWindow();
  bool rerender();
  bool tickGame();
  void newGame();
  // void wired();
  // void face();

  void slowSpeed();
  void medianSpeed();
  void fastSpeed();
protected:
  virtual bool on_key_press_event( GdkEventKey *ev );

private:
  // A "vertical box" which holds everything in our window
  Gtk::VBox m_vbox;

  // The menubar, with all the menus at the top of the window
  Gtk::MenuBar m_menubar;
  // Each menu itself
  Gtk::Menu m_menu_app;
  Gtk::Menu m_mode;
  Gtk::Menu m_speed;
  Gtk::RadioButtonGroup modeGroup;
  Gtk::RadioButtonGroup speedGroup;
  sigc::connection conn;
  int dropSpeed; //the current drop speed. default 300;
  // The main OpenGL area
  Viewer m_viewer;
};

#endif
