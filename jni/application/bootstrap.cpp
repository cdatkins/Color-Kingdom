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
#include <orb_logic.h>


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
	logic = new OrbLogic();
	std::ifstream infile;
	highscore = 0;
	infile.open("../assets/highscore.txt");
	if(infile.is_open()) 
		infile >> highscore;
  }

  ~Play_State() {
	 delete logic;
  }

private:
  void perform_logic() {
	
	const float time_passed = chrono.seconds();
    const float time_step = time_passed - m_time_passed;
    m_time_passed = time_passed;
	
	if(player.num_lives <= 0) {
		std::ofstream outfile;
		outfile.open("../assets/highscore.txt");
		if(outfile.is_open()) 
			outfile << highscore;
		get_Game().pop_state();
	}
	
	logic->Collision(map);
	logic->Collision(player);
	map.Collision(player);
	player.Update(time_step);
	logic->Update(time_step);
	
	if(player.score > highscore) 
		highscore = player.score;

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
		  case SDLK_SPACE:
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
	  logic->Render();
	  
	  for(int i = 1; i <= player.num_lives; i++) {
			Zeni::Point2f point(854-i*32,0);
			Zeni::render_image("life",point,point+Zeni::Vector2f(32,32));
	  }
	  int font_height = get_Fonts()["title"].get_text_height() + 10;

	  get_Fonts()["title"].render_text("HighScore:  " + itoa(highscore),Point2f(0,0),get_Colors()["white"]);
	  get_Fonts()["title"].render_text("Score:  " + itoa(player.score),Point2f(0,font_height),get_Colors()["white"]);
  }
  

  Player player; 
  OrbLogic *logic;
  Map map;
  Chronometer<Time> chrono;
  float m_time_passed;
  int highscore;
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
