/* This file is part of the Zenipex Library (zenilib).
 * Copyleft (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * This source file is simply under the public domain.
 */

#include <zenilib.h>
#include <player.h>
#include <map.h>
#include <orb.h>
#include <vector>
#include <time.h>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Zeni;

class Play_State : public Gamestate_Base {
  Play_State(const Play_State &);
  Play_State operator=(const Play_State &);

public:
  Play_State()
	  : m_time_passed(0.0f), map("1")
  {
    set_pausable(true);

	red1 = new Orb(Orb::RED,Zeni::Point2f(-20.0f,45.0f));
	red2 = new Orb(Orb::RED,Zeni::Point2f(40.0f,-20.0f));
	red3 = new Orb(Orb::RED,Zeni::Point2f(870.0f,200.0f));

	green1 = new Orb(Orb::GREEN,Zeni::Point2f(870.0f,360.0f));
	green2 = new Orb(Orb::GREEN,Zeni::Point2f(240.0f,-20.0f));

	blue1 = new Orb(Orb::BLUE,Zeni::Point2f(-20.0f,400.0f));

	black1 = new Orb(Orb::BLACK,Zeni::Point2f(500.0f,-20.0f));
	black2 = new Orb(Orb::BLACK,Zeni::Point2f(870.0f,45.0f));
	black3 = new Orb(Orb::BLACK,Zeni::Point2f(-20.0f,80.0f));
	//black4 = new Orb(Orb::BLACK,Zeni::Point2f(70.0f,-20.0f));
  }

  ~Play_State() {
	  delete red1;
	  delete red2;
	  delete red3;
	  delete green1;
	  delete green2;
	  delete blue1;
	  delete black1;
	  delete black2;
	  delete black3;
	  //delete black4;
  }

private:
  void perform_logic() {
	
	const float time_passed = chrono.seconds();
    const float time_step = time_passed - m_time_passed;
    m_time_passed = time_passed;
	
	map.Collision(player);
	
	map.Collision(*red1);
	map.Collision(*red2);
	map.Collision(*red3);

	map.Collision(*green1);
	map.Collision(*green2);

	map.Collision(*blue1);

	map.Collision(*black1);
	map.Collision(*black2);
	map.Collision(*black3);
	//map.Collision(*black4);
	
	player.Update(time_step);
	
	red1->Update(time_step);
	red2->Update(time_step);
	red3->Update(time_step);

	green1->Update(time_step);
	green2->Update(time_step);

	blue1->Update(time_step);

	black1->Update(time_step);
	black2->Update(time_step);
	black3->Update(time_step);
	//black4->Update(time_step);
	
  }
  
  void on_key(const SDL_KeyboardEvent &event) {
  
	  switch (event.keysym.sym) {
		  case SDLK_LEFT:
			  player.move_left = (event.type == SDL_KEYDOWN);
			  break;
		  case SDLK_RIGHT:
			  player.move_right = (event.type == SDL_KEYDOWN);
			  break;
		  case SDLK_UP:
			  player.jump = (event.type == SDL_KEYDOWN);
			  break;
		  case SDLK_DOWN:
			  player.release_orb = (event.type == SDL_KEYUP);
			  break;
		  default:
			  Gamestate_Base::on_key(event);
	  }
  }

  void on_push() {
    //get_Window().mouse_grab(true);
    get_Window().mouse_hide(true);
    //get_Game().joy_mouse.enabled = false;
	chrono.start();
  }

  void on_pop() {
    //get_Window().mouse_grab(false);
    get_Window().mouse_hide(false);
    //get_Game().joy_mouse.enabled = true;
	chrono.stop();
  }

  void render() {
	  Video &vr = get_Video();
	  vr.set_2d(make_pair(Point2f(0,0),Point2f(854.0f, 480.0f)),true);
	  map.RenderMap(vr);
	  player.Render();

	  red1->Render();
	  red2->Render();
	  red3->Render();

	  green1->Render();
	  green2->Render();

	  blue1->Render();

	  black1->Render();
	  black2->Render();
 	  black3->Render();
	  //black4->Render();
	  
  }
  

  Player player;
  
  Orb * red1;
  Orb * red2;
  Orb * red3;

  Orb * green1;
  Orb * green2;

  Orb * blue1;

  Orb * black1;
  Orb * black2;
  Orb * black3;
  Orb * black4;
  
  Map map;
  Chronometer<Time> chrono;
  float m_time_passed;
};

class Instructions_State : public Widget_Gamestate {
  Instructions_State(const Instructions_State &);
  Instructions_State operator=(const Instructions_State &);

public:
  Instructions_State()
    : Widget_Gamestate(make_pair(Point2f(0.0f, 0.0f), Point2f(854.0f, 480.0f)))
  {
  }

private:
  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
      get_Game().pop_state();
  }

  void render() {
    Widget_Gamestate::render();

    Zeni::Font &fr = get_Fonts()["title"];

    fr.render_text(
#if defined(_WINDOWS)
                   "ALT+F4"
#elif defined(_MACOSX)
                   "Apple+Q"
#else
                   "Ctrl+Q"
#endif
                           " to Quit",
                   Point2f(400.0f, 300.0f - 0.5f * fr.get_text_height()),
                   get_Colors()["title_text"],
                   ZENI_CENTER);
  }
};

class Title_State_Custom : public Title_State<Play_State, Instructions_State> {
public:
  Title_State_Custom()
    : Title_State<Play_State, Instructions_State>("")
  {
    m_widgets.unlend_Widget(title);
  }
 
  void render() {
    Title_State<Play_State, Instructions_State>::render();
 
    //render_image("logo", Point2f(200.0f, 25.0f), Point2f(600.0f, 225.0f));
  }
};

class Bootstrap {
  class Gamestate_One_Initializer : public Gamestate_Zero_Initializer {
    virtual Gamestate_Base * operator()() {
      Window::set_title("zenilib Application");

      get_Joysticks();
      get_Video();
      get_Textures();
      get_Fonts();
      get_Sounds();
      get_Game().joy_mouse.enabled = true;

      return new Title_State_Custom;
    }
  } m_goi;

public:
  Bootstrap() {
    g_gzi = &m_goi;
  }
} g_bootstrap;

int main(int argc, char **argv) {
  return zenilib_main(argc, argv);
}
