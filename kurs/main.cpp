#include <iostream>
#include <SFML/Graphics.hpp>
#include "List.h"
#include "Vector.h"
#include "Game.h"
#include "Timer.h"
#include "Button.h"
#include <fstream>
#include <string>

using namespace sf;
using namespace std;

void resetString(string* var) {     // set string value to default
    *var = "";
}

bool isMouseBtnPressed(bool oldValue, bool newValue) {  // check if mouse pressed since last frame
    if (!oldValue && newValue) return true;
    else return false;
}

int getFontSizeFromRectSize(int rectWidth, int charCount, int maxFontHeight) {  // calculate font size
    float ratioHWFont = 1.681f;		// font ratio height/width
    int letterSpacingsCount = charCount - 1;	// quantity of spacings btw letters

    int result = (rectWidth - letterSpacingsCount * 2) / (charCount / ratioHWFont);		// letter spacing depends on font size and not linear, it is approximate ratio

    if (result < maxFontHeight)
        return result;
    else
        return maxFontHeight;
}

int readIntFromFile(ifstream* file) {
    string s;
    *file >> s;
    return atoi(s.c_str());
}

int main() {
    ifstream file("gameSize.txt");
    int gameSize;
    try {
        gameSize = readIntFromFile(&file);
    }
    catch (exception& e) {
        gameSize = 19;
    }
    file.close();

    int displaySize = 600;      // pixels
    RenderWindow window(VideoMode(displaySize, displaySize+displaySize/10), "GO", Style::Default);  // sfml window
    Font font;  // text font
    window.setVerticalSyncEnabled(true);
    Timer timer;
    bool lastMouseLeft = false;     // left mouse btn in last frame

    if (!font.loadFromFile("Comfortaa.ttf")) {  // load font
        cout << "Load font error" << endl;
        exit(-1);
    }

    Game game = Game(displaySize, gameSize, &window, font);    // init game class
    Vector mousePos = Vector();     // mouse position
    string message = "";    // message about problem

    Button endBtn(&window);     // button ending game
    endBtn.setColor(sf::Color::Color(250, 250, 250), sf::Color::Color(50, 50, 50));
    endBtn.setButtonText("END GAME");
    endBtn.setFont(font);
    endBtn.setSize(Vector(120, 50));
    endBtn.setPosition(Vector(displaySize/2-120/2, displaySize + displaySize/10 - 40));
    int kk = 0;

    while (window.isOpen()) {       // Game cycle
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();
        window.clear(Color(140, 95, 32));

        mousePos = Vector(Mouse::getPosition(window));
        try {
            if (isMouseBtnPressed(lastMouseLeft, Mouse::isButtonPressed(Mouse::Left)) && window.hasFocus()) {   // left mouse pressed
                lastMouseLeft = Mouse::isButtonPressed(Mouse::Left);
                if (endBtn.checkPressed(mousePos)) break;   // go to the next step
                game.move(game.mouseNear(&mousePos));       // make a move
            }
        }
        catch (exception e) {
            message = e.what();     // error message
            timer.once<string*>(2000, resetString, &message, true);     // clear error message
        }

        lastMouseLeft = Mouse::isButtonPressed(Mouse::Left);
        game.drawGame(&mousePos);       // draw game state
        game.drawStats(message, font);  // draw game info
        endBtn.draw();                  // draw end game btn
        window.display();
    }
    game.killGroupsWithOneDame();


    while (window.isOpen()) {       // marking dead stones
        message = "Mark dead stones";
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();
        window.clear(Color(140, 95, 32));

        mousePos = Vector(Mouse::getPosition(window));
        try {
            if (isMouseBtnPressed(lastMouseLeft, Mouse::isButtonPressed(Mouse::Left)) && window.hasFocus()) {   // left mouse pressed
                if (endBtn.checkPressed(mousePos)) break;   // go to the next step
                game.killSelectedStone(&mousePos);          // kill selected stone
            }
        }
        catch (exception e) {}

        lastMouseLeft = Mouse::isButtonPressed(Mouse::Left);
        game.drawGame(&mousePos);       // draw game state
        game.drawStats(message, font);  // draw game info
        endBtn.draw();                  // draw end game btn
        window.display();
    }
    Vector score = game.getEndScore();


    message = "Score: " + score.toString();     // Draw final score
    int fontSize = getFontSizeFromRectSize(displaySize - displaySize / 10, message.length(), displaySize);
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();
        window.clear(Color(140, 95, 32));

        Text text;      // text with score
        text.setFont(font);
        text.setPosition(displaySize / 10, displaySize / 2 - fontSize / 2);
        text.setFillColor(sf::Color::Black);
        text.setCharacterSize(fontSize);
        text.setString(message);

        window.draw(text);
        window.display();
    }

    return 0;
}