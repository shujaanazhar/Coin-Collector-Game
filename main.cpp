/*
    Before playing the game please read the README file for game instructions
*/

#include<iostream>
#include<stdlib.h>
#include<unistd.h>
// #include<thread>
#include<wait.h>
#include<SFML/Graphics.hpp>
#include"SFML/Audio.hpp"
#include <X11/Xlib.h>
#include<pthread.h>

using namespace std;
using namespace sf;

void pl1(Sprite *player, int size)
{
    if(Keyboard::isKeyPressed(Keyboard::Key::Left))
    {
        if(player->getPosition().x - 50 >= -10)
            player->move(-50.0f, 0.0f);
    
    }
    else if(Keyboard::isKeyPressed(Keyboard::Key::Right))
    {
        if(player->getPosition().x + 50 <= size - 55)
            player->move(50.0f, 0.0f);
    
    }
    else if(Keyboard::isKeyPressed(Keyboard::Key::Up))
    {
        if(player->getPosition().y - 50 >= 0)
        player->move(0.0f, -50.0f);
    
    }
    else if(Keyboard::isKeyPressed(Keyboard::Key::Down))
    {
        if(player->getPosition().y + 50 <= size - 80)
            player->move(0.0f, 50.0f);
    
    }
}

void pl2(Sprite &player, float size)
{    
    if(Keyboard::isKeyPressed(Keyboard::Key::A))
    {
        if(player.getPosition().x - 25 >= -10)
            player.move(-25.0f, 0.0f);

    }
    //Right
    else if(Keyboard::isKeyPressed(Keyboard::Key::D))
    {
        if(player.getPosition().x + 25 <= size - 25)
            player.move(25.0f, 0.0f);

    }
    //Up
    else if(Keyboard::isKeyPressed(Keyboard::Key::W))
    {
        if(player.getPosition().y - 25 >= 0)
            player.move(0.0f, -25.0f);

    }
    //Down
    else if(Keyboard::isKeyPressed(Keyboard::Key::S))
    {
        if(player.getPosition().y + 25 <= size - 80)
            player.move(0.0f, 50.0f);

    }
}
//Calculate board Size
int boardSize()
{
    srand(time(0));
    int num = (rand() % 89) + 10;
    num *= 6;
    num = 406 / num;
    num %= 25;
    if(num < 10)
        num += 15;
    
    return num;
}

struct Player
{
    Sprite* player;
    int size;
};

//Player 1 Movement Function
void* p1Movement(void* peep)
{
    Player* player = (Player* )peep;

    if(Keyboard::isKeyPressed(Keyboard::Key::Left))
    {
        if(player->player->getPosition().x - 50 >= -10)
            player->player->move(-50.0f, 0.0f);
    
    }
    else if(Keyboard::isKeyPressed(Keyboard::Key::Right))
    {
        if(player->player->getPosition().x + 50 <= player->size - 55)
            player->player->move(50.0f, 0.0f);
    
    }
    else if(Keyboard::isKeyPressed(Keyboard::Key::Up))
    {
        if(player->player->getPosition().y - 50 >= 0)
        player->player->move(0.0f, -50.0f);
    
    }
    else if(Keyboard::isKeyPressed(Keyboard::Key::Down))
    {
        if(player->player->getPosition().y + 50 <= player->size - 80)
            player->player->move(0.0f, 50.0f);
    
    }
    pthread_exit(NULL);
}

//Player 2 Movement Function
void p2Movement(void* peep)
{    
    Player* player = (Player* )peep;

    if(Keyboard::isKeyPressed(Keyboard::Key::A))
    {
        if(player->player->getPosition().x - 25 >= -10)
            player->player->move(-25.0f, 0.0f);

    }
    //Right
    else if(Keyboard::isKeyPressed(Keyboard::Key::D))
    {
        if(player->player->getPosition().x + 25 <= player->size - 25)
            player->player->move(25.0f, 0.0f);

    }
    //Up
    else if(Keyboard::isKeyPressed(Keyboard::Key::W))
    {
        if(player->player->getPosition().y - 25 >= 0)
            player->player->move(0.0f, -25.0f);

    }
    //Down
    else if(Keyboard::isKeyPressed(Keyboard::Key::S))
    {
        if(player->player->getPosition().y + 25 <= player->size - 80)
            player->player->move(0.0f, 50.0f);

    }
    pthread_exit(NULL);
}

//Main Thread
int main()
{    
    XInitThreads();
    int size = boardSize() * 50;
    pthread_t tid[2];

    float windowHeight = size, windowWidth = size;
    
    //Creating the window
    RenderWindow window(VideoMode(windowWidth, windowHeight + 50), "Coin Collector");
    
    // Render
    RectangleShape bar(Vector2f(size, 50));
    RectangleShape board(Vector2f(size, size));
    
    bar.setFillColor(Color::Magenta);
    bar.setPosition(0, size);

    board.setFillColor(Color::Red);

    
    Texture p1, p2, collectable[11];
    
    if(!p1.loadFromFile("images/p1_resize.png"))
        return 0;
    if(!p2.loadFromFile("images/p2_resize.png"))
        return 0;

    for(int i = 0; i < 11; i++)  
        if(!collectable[i].loadFromFile("images/coin.png"))
            return 0;

    int max_player = 2, current_player = 1;

    Sprite player[max_player];
    
    player[0].setTexture(p1);
    player[0].setPosition(0, 0);

    player[1].setTexture(p2);
    player[1].setPosition(0, 0);

    int tcoins = 11;
    Sprite coins[11];
    for(int i = 0; i < 11; i++)
        coins[i].setTexture(collectable[i]);
    
    coins[0].setPosition(50, 50);
    coins[1].setPosition(100, 200);
    coins[2].setPosition(150, 50);
    coins[3].setPosition(350, 200);
    coins[4].setPosition(250, 400);
    coins[5].setPosition(550, 50);
    coins[6].setPosition(400, 350);
    coins[7].setPosition(500, 500);
    coins[8].setPosition(200, 250);
    coins[9].setPosition(300, 150);
    coins[10].setPosition(50, 200);

    //Music
    Music gameMusic;
    gameMusic.openFromFile("music/music.wav");
    gameMusic.setLoop(true);
    gameMusic.play();

    //Font
    sf::Font font;
    if(!font.loadFromFile("font/georgia bold italic.ttf"))
    {
        return 0;
    }
    int score1 = 0, score2 = 0;
    sf::Text text[2];
    //Player Score
    text[0].setFont(font);
    text[0].setPosition(5, size);
    text[0].setColor(sf::Color::Black);

    //Player 2 Score:
    text[1].setFont(font);
    text[1].setPosition(size - 250, size);
    text[1].setColor(sf::Color::Black);

    //Winner State
    Text winnerText;
    winnerText.setFont(font);
    winnerText.setPosition((size / 2) - 150, size / 2);
    winnerText.setColor(Color::Red);

    Player* peep1 = new Player();
    Player* peep2 = new Player();

    peep1->player = &player[0];
    peep2->player = &player[1];

    peep1->size = size;
    peep2->size = size;

    

    // pthread_attr_t t1, t2;

    // pthread_attr_init(&t1);
    // pthread_attr_init(&t2);

    // pthread_attr_setdetachstate(&t1, PTHREAD_CREATE_DETACHED);
    // pthread_attr_setdetachstate(&t2, PTHREAD_CREATE_DETACHED);

    pthread_create(&tid[0], NULL, p1Movement, (void *)peep1);
    
    pthread_create(&tid[1], NULL, p1Movement, (void *)peep2);

    // pthread_create(&tid[0], NULL, )
    while (window.isOpen())
    {
        //Scoring
        text[0].setString("P1 Score: " + to_string(score1));
        text[1].setString("P2 Score: " + to_string(score2));

        Event event;
        while (window.pollEvent(event))
        {
            //EXIT
            if (event.type == Event::Closed or Keyboard::isKeyPressed(Keyboard::Key::Escape))
                window.close();
            pl1(&player[0], size);
            //Creates Player 2 in the Game
            if(Keyboard::isKeyPressed(Keyboard::Key::N) and (current_player < max_player))
            {
                current_player++;
            }
            //PLAYER 2
            if(current_player == 2)
            {
                //Player Movement
                //Left
                // if(Keyboard::isKeyPressed(Keyboard::Key::A))
                // {
                //     if(player[1].getPosition().x - 25 >= -10)
                //         player[1].move(-25.0f, 0.0f);
            
                // }
                // //Right
                // else if(Keyboard::isKeyPressed(Keyboard::Key::D))
                // {
                //     if(player[1].getPosition().x + 25 <= size - 25)
                //         player[1].move(25.0f, 0.0f);
            
                // }
                // //Up
                // else if(Keyboard::isKeyPressed(Keyboard::Key::W))
                // {
                //     if(player[1].getPosition().y - 25 >= 0)
                //         player[1].move(0.0f, -25.0f);
            
                // }
                // //Down
                // else if(Keyboard::isKeyPressed(Keyboard::Key::S))
                // {
                //     if(player[1].getPosition().y + 25 <= size - 80)
                //         player[1].move(0.0f, 50.0f);
            
                // }

                pl2(player[1], size);
            }
        }

        
        window.clear();
        window.draw(bar);
        window.draw(board);
        
        if(current_player == 1)
        {    
            window.draw(*(peep1->player));
            window.draw(text[0]);
        }
        else if(current_player == 2)
        {
            window.draw(*(peep1->player));
            window.draw(*(peep2->player));

            window.draw(text[0]);
            window.draw(text[1]);
        }
        for(int i = 0; i < 11; i++)
        {
            //If collided
            if(((peep1->player->getPosition().x == coins[i].getPosition().x) and (peep1->player->getPosition().y == coins[i].getPosition().y)))
            {
                coins[i].setPosition(1000, 1000);
                ++score1;
                tcoins--;
            }
            else if(((peep2->player->getPosition().x == coins[i].getPosition().x) and (peep2->player->getPosition().y == coins[i].getPosition().y)))
            {
                coins[i].setPosition(1000, 1000);
                ++score2;
                tcoins--;
            }
            //If not collided
            else
            {
                window.draw(coins[i]);
            }
            
                
        }
        
        if(tcoins == 0)
        {
            window.clear();
            
            if(score1 > score2)
                winnerText.setString("Player 1 WON!!!");
            else if(score2 > score1)
                winnerText.setString("Player 2 WON!!!");
            
            window.draw(winnerText);
        }
        
        window.display();
        
        
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    pthread_exit(NULL);
}