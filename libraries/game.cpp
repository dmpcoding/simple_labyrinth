#include "game.h"

P_Game::P_Game(){

    player.setRadius(10);
    player.setFillColor(sf::Color::Red);
		player.setPosition(0,15);
		meta_texture.loadFromFile("x.png");
		meta.setTexture(meta_texture);
		speed = 0.2;
		map_file.open("libraries/levels_info");
		actual_level = 0;
		map_file >> number_of_levels;
		for(int i = 0; i < number_of_levels;i++){

			std::string level_file;
			map_file >> level_file;
			levels.push_back("libraries/"+level_file);

		}
		map_file.close();

}

void P_Game::draw(sf::RenderTarget& target,sf::RenderStates states)const{

	for(sf::RectangleShape x: game_map)
		target.draw(x,states);

	target.draw(meta,states);
	target.draw(player,states);
	sf::Time time = clock.getElapsedTime();
	int m = time.asSeconds()/60;
	int s = int(time.asSeconds())%60;
	sf::Text textTime(std::to_string(m)+":"+std::to_string(s),*font,16);
	textTime.setPosition(game_window->getSize().x-textTime.getGlobalBounds().width,
	0);
	target.draw(textTime,states);

}

void P_Game::refresh(){

	sf::CircleShape test = player;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
		test.move(speed,0);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
		test.move(0,speed);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
		test.move(-speed,0);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
		test.move(0,-speed);
	}

	bool off_the_screen = (test.getPosition().x > 0
	&& test.getPosition().y > 0
	&& test.getPosition().x < game_window->getSize().x-test.getGlobalBounds().width
  && test.getPosition().y < game_window->getSize().y-test.getGlobalBounds().height);

	if(off_the_screen){

		bool collision = false;

			for(int i = 0;i < game_map.size()&&!collision;i++)
				collision = game_map[i].getGlobalBounds().intersects(
					test.getGlobalBounds());

					if(collision == false){
						player = test;
					}
	}

	sf::Rect<float> meta_test = meta.getGlobalBounds();
	meta_test.left+= player.getGlobalBounds().width;
	if(player.getGlobalBounds().intersects(meta_test)){
		resetClock(false);
		actual_level = (actual_level == number_of_levels) ? actual_level = 0 :actual_level+1;
		readMapFromFile(levels[actual_level]);

	}

}

void P_Game::resetClock(bool rest_levels){

		clock.restart();
		player.setPosition(0,15);
		if(rest_levels == true)
			actual_level = 0;


}

void P_Game::readMapFromFile(std::string file){

	game_map.clear();

 	map_file.open(file);
	int lines = 0;
	map_file >> lines;

	for (int i = 0;i<lines;i++){
		map_file >> value.x;
		map_file >> value.y;
		map_file >> value.w;
		map_file >> value.h;
		map_file >> value.r;

		addMapElement(value);

	}

	int x = 0;
	int y = 0;

	map_file >> x;
	map_file >> y;

	setUpMapEnd(x,y);

	map_file.close();

}

void P_Game::setUpMap(int i,int x,int y,int w,int h,int r){

	game_map[i].setPosition(x,y);
	game_map[i].setSize(sf::Vector2f(w,h));
	game_map[i].setRotation(r);
	game_map[i].setFillColor(sf::Color::White);

}

void P_Game::addMapElement(int x,int y,int w,int h,int r){

	sf::RectangleShape a;
	game_map.push_back(a);
	setUpMap(game_map.size()-1,x,y,w,h,r);

}

void P_Game::addMapElement(map_element_value values){

	addMapElement(values.x,values.y,values.w,values.h,values.r);

}

void P_Game::setUpMapEnd(int x,int y){

	meta.setPosition(x,y);

}
