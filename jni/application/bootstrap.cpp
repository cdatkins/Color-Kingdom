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

class End_State : public Gamestate_Base {

public:
	End_State(int score_, int highscore_)
		:score(score_),highscore(highscore_)
	{}
	~End_State(){}
private:
	
	void on_push() {
    //get_Window().mouse_grab(true);
    get_Window().mouse_hide(true);
    //get_Game().joy_mouse.enabled = false;
	
	}

  void on_pop() {
    //get_Window().mouse_grab(false);
    get_Window().mouse_hide(false);
    //get_Game().joy_mouse.enabled = true;
  }
	
	void on_key(const SDL_KeyboardEvent &event) {
		switch (event.keysym.sym) {
		  case SDLK_SPACE:
			  get_Game().pop_state();
		  default:
			  Gamestate_Base::on_key(event);
	  }
	}

	void render() {
		
	  Video &vr = get_Video();
	  vr.set_2d(make_pair(Point2f(0,0),Point2f(854.0f, 480.0f)),true);

	  render_image("end",Point2f(0,0),Point2f(854,480)); 

	  int congrats_width = get_Fonts()["title_2"].get_text_width("THANKS FOR PLAYING!");
	  get_Fonts()["title_2"].render_text("THANKS FOR PLAYING!", Point2f(427-(congrats_width/2), 0),get_Colors()["white"]);
	  
	  if(score == highscore) {
		  int highscore_width = get_Fonts()["title_2"].get_text_width("NEW HIGHSCORE!");
		  int highscore_height = get_Fonts()["title_2"].get_text_height();
		  get_Fonts()["title_2"].render_text("NEW HIGHSCORE!",Point2f(427-(highscore_width/2),240),get_Colors()["white"]);
		  int score_width = get_Fonts()["title_2"].get_text_width(itoa(score));
		  get_Fonts()["title_2"].render_text(itoa(score),Point2f(427-(score_width/2),240+highscore_height+5),get_Colors()["white"]);
	  }
	  else {
		  int font_height = get_Fonts()["title"].get_text_height() + 5;
		  int width_f = get_Fonts()["title"].get_text_width("HighScore:  " + itoa(highscore));
		  int width_s = get_Fonts()["title"].get_text_width("Score:  " + itoa(score));
		  get_Fonts()["title"].render_text("HighScore:  " + itoa(highscore),Point2f(427-(width_f/2),240),get_Colors()["white"]);
		  get_Fonts()["title"].render_text("Score:  " + itoa(score),Point2f(427-(width_s/2),240+font_height),get_Colors()["white"]);
	  }

		  int final_width = get_Fonts()["title"].get_text_width("Press SpaceBar to Continue...");
		  int font_height2 = get_Fonts()["title"].get_text_height()+10;
		  get_Fonts()["title"].render_text("Press SpaceBar to Continue...",Point2f(427-(final_width/2),480-font_height2),get_Colors()["white"]);
	}

	int score, highscore;
};


class Play_State : public Gamestate_Base {
  Play_State(const Play_State &);
  Play_State operator=(const Play_State &);

public:
  Play_State()
	  : m_time_passed(0.0f), map("1") 
  {
    
	set_pausable(true);
	logic = new OrbLogic();
	item = NULL;
	
	std::ifstream infile;
	highscore = 0;
	infile.open("../assets/highscore.txt");
	if(infile.is_open()) 
		infile >> highscore;
	item_timer.start();
	get_Sound().set_BGM("../assets/sfx/2ndBallad");
	get_Sound().set_BGM_looping(true);
	get_Sound().set_BGM_gain(.5f);
	get_Sound().play_BGM();
	
  }

  ~Play_State() {
	 delete logic;
	 if(item != NULL)
		delete item;
  }

private:
  
void perform_logic() {
	
	const float time_passed = chrono.seconds();
    const float time_step = time_passed - m_time_passed;
    m_time_passed = time_passed;
	
	//Checks if the player has lost all lives
	if(player.num_lives <= 0) {
		std::ofstream outfile;
		outfile.open("../assets/highscore.txt");
		if(outfile.is_open()) 
			outfile << highscore;
		get_Game().pop_state();
		get_Game().push_state(new End_State(player.score,highscore));
	}
	
	//Extra life everytime the player recieves 750 points
	//if(player.score > 0 && player.score%750 == 0 )
		//player.num_lives++;
	
	logic->Collision(map);
	logic->Collision(player);
	map.Collision(player);
	player.Update(time_step);
	logic->Update(time_step);

	//Item generation
	/*if(item != NULL) {
		map.Collision(*item);
		player.Collision(item);
		item->Update(time_step);
		
		if(item->done) {
			delete item;
			item = NULL;
			item_timer.start();
		}
	}
	else if( item_timer.seconds() >= 10){
		
		switch(random.rand_lt(2)) {
		case 0:
			if(random.rand_lt(5) == 1) {
				switch (random.rand_lt(3)) {
				case 0:
					item = new Item(Item::RED, Item::MULTI);
					item_timer.stop();
					item_timer.reset();
					break;
				case 1:
					item = new Item(Item::GREEN, Item::MULTI);
					item_timer.stop();
					item_timer.reset();
					break;
				case 2:
					item = new Item(Item::BLUE, Item::MULTI);
					item_timer.stop();
					item_timer.reset();
					break;
				}
			}
			break;
		case 1:
			break;
		}
	}*/
	
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
	get_Sound().stop_BGM();
  }

  void render() {
	  Video &vr = get_Video();
	  vr.set_2d(make_pair(Point2f(0,0),Point2f(854.0f, 480.0f)),true);
	  Zeni::render_image("background",Zeni::Point2f(0,0), Zeni::Point2f(854,480));
	  map.RenderMap(vr);
	  player.Render();
	  logic->Render();

	  if(item !=NULL)
		item->Render();

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
  Item * item;
  Chronometer<Time> chrono, item_timer;
  float m_time_passed;
  int highscore;

  Random  random;
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
	render_image("logo", Point2f(200.0f, 25.0f), Point2f(600.0f, 225.0f));
  }
};


class Bootstrap {
  class Gamestate_One_Initializer : public Gamestate_Zero_Initializer {
    virtual Gamestate_Base * operator()() {
      Window::set_title("COLOR RUSH");

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
