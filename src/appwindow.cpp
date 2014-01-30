#include "appwindow.hpp"
#include <iostream>

AppWindow::AppWindow()
{
  set_title("488 Tetrominoes on the Wall");

  // A utility class for constructing things that go into menus, which
  // we'll set up next.
  using Gtk::Menu_Helpers::MenuElem;
  using Gtk::Menu_Helpers::RadioMenuElem;
  // Set up the application menu
  // The slot we use here just causes AppWindow::hide() on this,
  // which shuts down the application.

  dropSpeed = 500;
  m_menu_app.items().push_back(MenuElem("_New game", Gtk::AccelKey("n"),
    sigc::mem_fun(*this,&AppWindow::newGame)));
  m_menu_app.items().push_back(MenuElem("_Reset",Gtk::AccelKey("r"),
    sigc::mem_fun(m_viewer,&Viewer::resetMonitor)));
  m_menu_app.items().push_back(MenuElem("_Quit", Gtk::AccelKey("q"),
    sigc::mem_fun(*this, &AppWindow::hide)));
   m_mode.items().push_back(RadioMenuElem(modeGroup,"_Face",Gtk::AccelKey("f"),
    sigc::mem_fun(m_viewer,&Viewer::turnOnFaceMode)));
  m_mode.items().push_back(RadioMenuElem(modeGroup,"_Wire-frame",Gtk::AccelKey("w"),
    sigc::mem_fun(m_viewer,&Viewer::turnOnWiredMode)));

 
  m_speed.items().push_back(RadioMenuElem(speedGroup,"_Slow",Gtk::AccelKey("1"),
    sigc::mem_fun(*this,&AppWindow::slowSpeed)));
  m_speed.items().push_back(RadioMenuElem(speedGroup,"_Medium",Gtk::AccelKey("2"),
    sigc::mem_fun(*this,&AppWindow::medianSpeed)));
  m_speed.items().push_back(RadioMenuElem(speedGroup,"_Fast",Gtk::AccelKey("3"),
    sigc::mem_fun(*this,&AppWindow::fastSpeed)));
  // Set up the menu bar
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Application", m_menu_app));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Draw Mode", m_mode));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Speed",m_speed));
  // Pack in our widgets

  // First add the vertical box as our single "top" widget
  add(m_vbox);

  // Put the menubar on the top, and make it as small as possible
  m_vbox.pack_start(m_menubar, Gtk::PACK_SHRINK);

  // Put the viewer below the menubar. pack_start "grows" the widget
  // by default, so it'll take up the rest of the window.
  m_viewer.set_size_request(300, 600);
  m_vbox.pack_start(m_viewer);
  show_all();
  sigc::slot0<bool>time_slot = sigc::mem_fun(*this,&AppWindow::rerender);
  const int oneOverSixty = 16;
  Glib::signal_timeout().connect(time_slot,oneOverSixty);

  sigc::slot0<bool>tick_time_slot = sigc::mem_fun(*this,&AppWindow::tickGame);
  conn=Glib::signal_timeout().connect(tick_time_slot,dropSpeed);
}

bool AppWindow::rerender(){
  m_viewer.invalidate();
  return true;
}

// void AppWindow::face(){
//   m_viewer.turnOnFaceMode();
// }
void AppWindow::slowSpeed(){
  dropSpeed=500;
  conn.disconnect();
  sigc::slot0<bool>tick_time_slot = sigc::mem_fun(*this,&AppWindow::tickGame);
  conn=Glib::signal_timeout().connect(tick_time_slot,dropSpeed);
}
void AppWindow::medianSpeed(){
  dropSpeed=300;
  conn.disconnect();
  sigc::slot0<bool>tick_time_slot = sigc::mem_fun(*this,&AppWindow::tickGame);
  conn=Glib::signal_timeout().connect(tick_time_slot,dropSpeed);
}
void AppWindow::fastSpeed(){
  dropSpeed=100;
  conn.disconnect();
  sigc::slot0<bool>tick_time_slot = sigc::mem_fun(*this,&AppWindow::tickGame);
  conn=Glib::signal_timeout().connect(tick_time_slot,dropSpeed);
}
// void AppWindow::wired(){
//   m_viewer.turnOnWiredMode();
// }
bool AppWindow::tickGame(){
  m_viewer.game.tick();
  return true;
}
void AppWindow::newGame(){
  m_viewer.game.reset();
}

bool AppWindow::on_key_press_event( GdkEventKey *ev )
{
        // This is a convenient place to handle non-shortcut
        // keys.  You'll want to look at ev->keyval.

	// An example key; delete and replace with the
	// keys you want to process
        if( ev->keyval == 't' ) {
                std::cerr << "Hello!" << std::endl;
                return true;
        } 
        else if(ev->keyval == 65361){
              std::cerr<<"moving left!"<<std::endl;
            return m_viewer.game.moveLeft();
        }
        else if(ev->keyval ==65362){
           std::cerr<<"moving up!"<<std::endl;
           return m_viewer.game.rotateCCW();
        }
        else if(ev->keyval == 65363){
          std::cerr<<"moving right!"<<std::endl;
          return m_viewer.game.moveRight();
                 }
        else if(ev->keyval == 65364){
           std::cerr<<"moving down!"<<std::endl;
          return m_viewer.game.rotateCW();
                  }
        else if(ev->keyval == 32){
          std::cerr<<"using space!"<<std::endl;
          return m_viewer.game.drop();
        }
        else {
                return Gtk::Window::on_key_press_event( ev );
        }
}
