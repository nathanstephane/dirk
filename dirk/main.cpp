#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

# define PI     3.14159265358979323846 
const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH = SCREEN_HEIGHT * 2;
const int MAP_SIZE = 8;
const int TILE_SIZE = static_cast<int>((SCREEN_WIDTH / 2) / MAP_SIZE);
const int MAX_DEPTH = static_cast<int>(MAP_SIZE * TILE_SIZE); // 8 * 480 / 8 = 480
const int PREVIEW_SCREEN_WIDTH = 480;
const float FOV = PI / 3;
const float HALF_FOV = FOV / 2;

const int CASTED_RAYS = 120;
const float STEP_ANGLE = FOV / CASTED_RAYS;

// source
float player_x = PREVIEW_SCREEN_WIDTH / 2;
float player_y = SCREEN_HEIGHT / 2;
float player_angle = PI;
void DrawMap(char (*map)[8],sf::RenderWindow& win)
{

    for (int i = 0; i < 8;i++)
    {
        for (int j = 0; j < 8; j++)
        {
            //square index
            int square = i * MAP_SIZE + j;
            sf::RectangleShape rec(sf::Vector2f(TILE_SIZE-2, TILE_SIZE-2));
            sf::Color rec_color = map[i][j] == '#' ? sf::Color(200, 200, 200) : sf::Color(100, 100, 100);
            rec.setFillColor(rec_color);
            rec.setPosition(j * TILE_SIZE, i * TILE_SIZE);
            win.draw(rec);
            
        }
    }

    //player
    sf::CircleShape player(10.0f);
    player.setFillColor(sf::Color::Red);
    player.setPosition(player_x-10,player_y-10);
    win.draw(player);

    //player direction
    std::vector<sf::Vertex> line;
    line.push_back(sf::Vertex(sf::Vector2f( player.getPosition().x+10, player.getPosition().y+10), sf::Color::Blue));
    line.push_back(sf::Vertex(sf::Vector2f( player.getPosition().x+10 - sinf(player_angle) * 50,
                                            player.getPosition().y+10 + cosf(player_angle) * 50)
                              ,sf::Color::Blue));
    win.draw(&line[0], line.size(), sf::Lines);

    //FOV
    std::vector<sf::Vertex> fov_line;


    fov_line.push_back(sf::Vertex(sf::Vector2f( player.getPosition().x + 10, player.getPosition().y + 10), sf::Color::Blue));
    fov_line.push_back(sf::Vertex(sf::Vector2f( player.getPosition().x + 10 - sinf(player_angle - HALF_FOV) * 50,
                                                player.getPosition().y + 10 + cosf(player_angle - HALF_FOV) * 50), sf::Color::Blue));
    
    fov_line.push_back(sf::Vertex(sf::Vector2f( player.getPosition().x + 10, player.getPosition().y + 10), sf::Color::Blue));
    fov_line.push_back(sf::Vertex(sf::Vector2f( player.getPosition().x + 10 - sinf(player_angle + HALF_FOV) * 50,
                                                player.getPosition().y + 10 + cosf(player_angle + HALF_FOV) * 50), sf::Color::Blue));

    win.draw(&line[0], line.size(), sf::Lines);
    win.draw(&fov_line[0], fov_line.size(), sf::Lines);

}

void CastRays(char (*map)[8], sf::RenderWindow& win)
{
    //Very left ray
    float starting_angle = player_angle - HALF_FOV;
    

    //                     120
    for (size_t i = 0; i < 50; i++)
    {
        //                      480
        for (size_t j = 0; j < MAX_DEPTH; j++)
        {
            float target_x = player_x - sinf(starting_angle) * j;
            float target_y = player_y + cosf(starting_angle) * j;

            // get tile index according to target_x and target_y
            int col = static_cast<int>(target_x/TILE_SIZE);
            int row = static_cast<int>(target_y/TILE_SIZE);

            std::cout << "row: " << row << " col: " << col << "\n";
            /*int square = row * MAP_SIZE * col;
            if (map[square] == '#')
            {

            }*/

            //draw casted ray
            std::vector<sf::Vertex> line;
            line.push_back(sf::Vertex(sf::Vector2f(player_x, player_y), sf::Color::Blue));
            line.push_back(sf::Vertex(sf::Vector2f(target_x, target_y), sf::Color::Blue));
         //   win.draw(&line[0], line.size(), sf::Lines);

            
            
        }
        starting_angle += STEP_ANGLE;
    }

   
}

int main()
{
    std::cout << "sin(PI): " << sinf(player_angle);

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "dirk:-raycaster");
    
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    char MAP[8][8] =
    {
        {'#','#','#','#','#','#','#','#'},
        {'#',' ','#',' ',' ',' ','#','#'},
        {'#',' ','#','#','#',' ',' ','#'},
        {'#',' ',' ',' ',' ','#',' ','#'},
        {'#',' ','#','#',' ','#',' ','#'},
        {'#',' ','#','#',' ','#',' ','#'},
        {'#',' ','#','#',' ',' ',' ','#'},
        {'#','#','#','#','#','#',' ','#'}
    };
    

    //std::cout << sizeof(MAP) / sizeof(*(MAP + 1));
   // std::cout << sizeof(MAP) / sizeof(*(MAP));
    sf::Clock clock;
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                player_angle -= 0.1;
            }if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                player_angle += 0.1;
            }
        }

        window.clear();
       // window.draw(shape);
        DrawMap(MAP,window);
        CastRays(MAP,window);
        window.display();
    }

    return 0;
}