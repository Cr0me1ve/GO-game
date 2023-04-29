#pragma once
#include "List.h"
#include "Vector.h"
#include <exception>
#include "Shape.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

class Game {
private:
	int displaySize = 600;	// in pixels
	int gameSize = 5;		// size of the game

	List<List<int>> gameState;	// game board
	List<List<int>> gameStateLast;	// last game board
	List<std::shared_ptr<Shape>> shapes;	// list of drawing shapes
	sf::RenderWindow* window;	// rendering window
	int pixBtwLines;	// pixels between drawing lines

	int fontSize;		// draw font size
	sf::Font font;		// text font

	int playerTurn = 1;	// 1: first player black; 2: second player white(ai)
	Vector deadStones = Vector(0, 0);	// quantity of dead stiones		1: black; 2: white

	int getFontSizeFromRectSize(int rectWidth, int charCount, int maxFontHeight);	// calculate font size

	bool hasDame(Vector position, List<List<int>>* state) const;		// check if point has dame
	int countDame(Vector position, List<List<int>>* state) const;		// count point dame
	bool checkGroupDame(List<Vector>* group, List<List<int>>* state) const;	// Check if group has at least one dame
	int countGroupDame(List<Vector>* group, List<List<int>>* state) const;	// count group dames

	void goThroughNeighbors(Vector position, int player, List<Vector>* neighbors, List<List<int>>* state) const;	// find all points of group
	int getField(Vector position, List<List<int>>* state) const;	// get player from field (-1 if invalid input)
	void captivateStones(List<Vector>* group, List<List<int>>* state, bool testMove);	// kill group of stones
	bool checkRulesForMove(Vector position);	// check if move is available
	bool verifyMoveArgs(Vector* selectedField) const;	// check if move is available
	List<Vector> getGroup(Vector posotion, List<List<int>>* state) const;	// get group of stones of given player
	bool compareBoards(const List<List<int>>* state1, const  List<List<int>>* state2) const;	// check if boards are equal

	void killGroupsWithoutDame(int player, List<List<int>>* state, bool testMove);	// kill all player's groups without dame
	void killGroupsWithoutDame(List<List<int>>* state, bool testMove);		// kill all groups without dame

	List<Vector> getEmptyGroup(Vector position, int* black, int* white);		// get group of empty stones
	void goThroughEmptyPoints(Vector position, List<Vector>* neighbors, int* black, int* white);		// find all stones of group
public:
	Game(int displaySize, int gameSize, sf::RenderWindow* window, sf::Font font);	// init
	~Game() {}

	void move(Vector selectedField);	// Make a move

	void killSelectedStone(Vector* mousePos);	// kill selected stone in the end
	void killGroupsWithOneDame();	// in the end of the game kill groups with one dame
	Vector getEndScore();	// calculate final score

	Vector mouseNear(Vector* mousePos) const;	// Find circle near mouse

	void drawGame(Vector* mousePos);	// draw game
	void drawStats(std::string message, sf::Font font);		// draw game info
};