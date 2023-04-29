#include "Game.h"
#include <exception>
#include <memory>
#include "List.h"
#include "Line.h"
#include "Circle.h"
#include "Shape.h"
#include "SFML/Graphics.hpp"

Game::Game(int displaySize, int gameSize, sf::RenderWindow* window, sf::Font font):gameState(gameSize), gameStateLast(gameSize), shapes(gameSize*2+gameSize*gameSize) {	// init
	this->displaySize = displaySize;
	this->gameSize = gameSize;
	this->window = window;
	this->font = font;

	pixBtwLines = displaySize / (gameSize + 1);		// distance between drawing lines and circles
	fontSize = pixBtwLines / 2;		// default font size

	for (int i = 0; i < gameSize; i++) {	// init game state with zeros
		gameState[i].setSize(gameSize, 0);
		gameStateLast[i].setSize(gameSize, 0);
	}
}

bool Game::hasDame(Vector pos, List<List<int>>* state) const {		// check if stone has dame
	if (getField(pos + Vector(1, 0), state) == 0)
		return true;
	else if (getField(pos + Vector(-1, 0), state) == 0)
		return true;
	else if (getField(pos + Vector(0, 1), state) == 0)
		return true;
	else if (getField(pos + Vector(0, -1), state) == 0)
		return true;
	else
		return false;
}

int Game::countDame(Vector pos, List<List<int>>* state) const {		// count stone dame
	int count = 0;
	if (getField(pos + Vector(1, 0), state) == 0)
		count++;
	if (getField(pos + Vector(-1, 0), state) == 0)
		count++;
	if (getField(pos + Vector(0, 1), state) == 0)
		count++;
	if (getField(pos + Vector(0, -1), state) == 0)
		count++;
	return count;
}

void Game::move(Vector selectedField) {		// Make a move
	if (verifyMoveArgs(&selectedField) && checkRulesForMove(selectedField)) {
		gameStateLast = gameState;
		gameState[selectedField[0]][selectedField[1]] = playerTurn;
		playerTurn = playerTurn == 1 ? 2 : 1;
		killGroupsWithoutDame(&gameState, false);
	}
}

int Game::getField(Vector position, List<List<int>>* state) const {		// get player from field (-1 if invalid input)
	if (position.x >= gameSize || position.y >= gameSize || position.x < 0 || position.y < 0)
		return -1;
	else {
		return state->getPtr(position.x)->get(position.y);
	}
}

List<Vector> Game::getGroup(Vector position, List<List<int>>* state) const {		// get group of stones of given player
	List<Vector> group = List<Vector>();
	goThroughNeighbors(position, getField(position, state), &group, state);
	return group;
}

void Game::goThroughNeighbors(Vector position, int player, List<Vector>* neighbors, List<List<int>>* state) const {	// find all stones of group
	if (!neighbors->contain(position) && getField(position, state) == player && player != 0) {
		neighbors->push_back(position);		// add stone to group list

		goThroughNeighbors(position + Vector(1, 0), player, neighbors, state);
		goThroughNeighbors(position + Vector(-1, 0), player, neighbors, state);
		goThroughNeighbors(position + Vector(0, 1), player, neighbors, state);
		goThroughNeighbors(position + Vector(0, -1), player, neighbors, state);
	}
}

bool Game::verifyMoveArgs(Vector* selectedField) const {	// check if move is available
	if (selectedField->x >= gameSize || selectedField->y >= gameSize || selectedField->x < 0 || selectedField->y < 0)
		throw std::out_of_range("Index out of range");
	else if (gameState.getConst(selectedField->get(0)).getConst(selectedField->get(1)) != 0)
		throw std::invalid_argument("Field is not empty");
	else
		return true;
	return false;
}

Vector Game::mouseNear(Vector* mousePos) const {	// Find circle near mouse
	Vector result = Vector();
	for (int i = 0; i < gameSize; i++)
		for (int k = 0; k < gameSize; k++) {
			result.x = pixBtwLines * (i + 1);
			result.y = pixBtwLines * (k + 1);
			result.subtract(mousePos);	// vector from mouse to point
			if (result.distanceSquared() <= (pixBtwLines / 2) * (pixBtwLines / 2))
				return Vector(i, k);
	}
	return Vector(-1, -1);
}

bool Game::checkGroupDame(List<Vector>* group, List<List<int>>* state) const {	// Check if group has at least one dame
	for (int i = 0; i < group->size(); i++) {
		Vector v = group->get(i);
		if (hasDame(v, state))
			return true;
	}
	return false;
}

int Game::countGroupDame(List<Vector>* group, List<List<int>>* state) const {	// count group dames
	int count = 0;
	for (int i = 0; i < group->size(); i++)
		count += countDame(group->get(i), state);
	return count;
}

bool Game::compareBoards(const List<List<int>>* state1, const List<List<int>>* state2) const {	// check if boards are equal
	for (int i = 0; i < gameSize; i++)
		if (!(state1->getConst(i) == state2->getConst(i)))
			return false;
	return true;
}

void Game::captivateStones(List<Vector>* group, List<List<int>>* state, bool testMove) {		// kill group of stones
	for (int i = 0; i < group->size(); i++) {
		Vector v = group->get(i);
		if (!testMove) {	// if "test" dont change score
			if (getField(v, state) == 1) deadStones.x += 1;
			else if (getField(v, state) == 2) deadStones.y += 1;
		}
		state->getChangeable(group->get(i).x)[group->get(i).y] = 0;
	}
}

void Game::drawGame(Vector* mousePos) {		// draw game
	int count = 0;

	for (int i = 0; i < gameSize; i++)		// Push lines
		shapes[count++].reset(new Line(window, displaySize - pixBtwLines * 2, 2, Vector(pixBtwLines, pixBtwLines * (i + 1)), 0.f, sf::Color(0, 0, 0)));
	for (int i = 0; i < gameSize; i++)
		shapes[count++].reset(new Line(window, displaySize - pixBtwLines * 2, 2, Vector(pixBtwLines * (i + 1), pixBtwLines), 90.f, sf::Color(0, 0, 0)));

	for (int i = 0; i < gameSize; i++)		// Push circles
		for (int k = 0; k < gameSize; k++)
			if (gameState[i][k] != 0)
				shapes[count++].reset(new Circle(window, pixBtwLines / 2 - pixBtwLines / 20, Vector(pixBtwLines * (i + 1), pixBtwLines * (k + 1)), gameState[i][k] == 1 ? sf::Color(10, 10, 10) : sf::Color(220, 220, 220)));

	Vector selected = mouseNear(mousePos);	// Push transparent circle near mouse
	if (selected.x != -1 && gameState[selected.x][selected.y] == 0)
		shapes[count++].reset(new Circle(window, pixBtwLines / 2 - pixBtwLines / 20, Vector(pixBtwLines * (selected.x + 1), pixBtwLines * (selected.y + 1)), playerTurn == 1 ? sf::Color(10, 10, 10, 50) : sf::Color(255, 255, 255, 50)));

	for (int i = 0; i < count; i++)		// Draw everything
		shapes[i]->draw();
}

int Game::getFontSizeFromRectSize(int rectWidth, int charCount, int maxFontHeight) {	// calculate font size
	float ratioHWFont = 1.681f;		// font ratio height/width
	int letterSpacingsCount = charCount - 1;	// quantity of spacings btw letters

	int result = (rectWidth - letterSpacingsCount * 2) / (charCount / ratioHWFont);		// letter spacing depends on font size and not linear, it is approximate ratio

	if (result < maxFontHeight)
		return result;
	else
		return maxFontHeight;
}

void Game::drawStats(std::string message, sf::Font font) {		// draw game info
	int heightCorrction = -fontSize / 5;
	int widthCorrction = -fontSize / 15;

	sf::Text textBlack;		// black dead stones
	textBlack.setFont(font);
	textBlack.setCharacterSize(fontSize);
	textBlack.setFillColor(sf::Color::Black);
	textBlack.setString("dead: " + std::to_string(deadStones.x));
	textBlack.setPosition(pixBtwLines + widthCorrction, pixBtwLines / 10 + heightCorrction);
	window->draw(textBlack);

	sf::Text textWhite;		// white dead stones
	textWhite.setFont(font);
	textWhite.setCharacterSize(fontSize);
	textWhite.setFillColor(sf::Color::White);
	textWhite.setString("dead: " + std::to_string(deadStones.y));
	textWhite.setPosition(displaySize - pixBtwLines*3 - widthCorrction, pixBtwLines / 10 + heightCorrction);
	window->draw(textWhite);

	sf::Text textMessage;	// error info
	textMessage.setFont(font);
	textMessage.setCharacterSize(getFontSizeFromRectSize(displaySize-pixBtwLines, message.length(), pixBtwLines));
	textMessage.setFillColor(sf::Color::Black);
	textMessage.setString(message);
	textMessage.setPosition(pixBtwLines + widthCorrction, pixBtwLines * ((float)gameSize + 0.7) + heightCorrction - pixBtwLines/3);
	window->draw(textMessage);
}

void Game::killGroupsWithoutDame(int player, List<List<int>>* state, bool testMove) {	// kill all groups without dame
	List<Vector> checked = List<Vector>();	// checked coordinates
	Vector check = Vector();		// coordinates to check

	for (int i = 0; i < gameSize; i++) {
		for (int k = 0; k < gameSize; k++) {
			check.x = i;
			check.y = k;
			if (checked.contain(&check) || getField(check, state) != player)
				continue;
			else {
				List<Vector> group = getGroup(check, state);		// get group and kill it if need
				if (!checkGroupDame(&group, state))
					captivateStones(&group, state, testMove);
				checked.concat(&group);
			}
		}
	}	
}

void Game::killGroupsWithoutDame(List<List<int>>* state, bool testMove) {	// check stones of the one who moves next first
	killGroupsWithoutDame(playerTurn, state, testMove);
	killGroupsWithoutDame(playerTurn == 2 ? 1 : 2, state, testMove);
}

bool Game::checkRulesForMove(Vector position) {		// check if move is available
	List<List<int>> testState(gameState);					// copy game state list 
	testState[position[0]][position[1]] = playerTurn;		// make a test move
	killGroupsWithoutDame(playerTurn == 1 ? 2 : 1, &testState, true);

	List<Vector> group = getGroup(position, &testState);			// check if new position have dame
	if (!checkGroupDame(&group, &testState))
		throw std::exception("Group wont have dame");
	
	bool areEqual = compareBoards(&testState, &gameStateLast);	// check KO rule
	if (areEqual)
		throw std::exception("KO rule is not respected");

	return true;
}

void Game::killSelectedStone(Vector* mousePos) {	// kill selected stone in the end
	Vector selected = mouseNear(mousePos);
	if (selected.x == -1)		// if wrong input
		return;
	int player = getField(selected, &gameState);
	if (player == 0 || player == -1)
		return;
	if (player == 1)
		deadStones.x += 1;
	else
		deadStones.y += 1;
	gameState[selected.x][selected.y] = 0;
}


List<Vector> Game::getEmptyGroup(Vector position, int* black, int* white) {		// get group of empty stones
	List<Vector> group = List<Vector>();
	if (getField(position, &gameState) != 0)	// leave if stone is player
		return group;
	goThroughEmptyPoints(position, &group, black, white);
	return group;
}

void Game::goThroughEmptyPoints(Vector position, List<Vector>* neighbors, int* black, int* white) {		// find all stones of group
	if (!neighbors->contain(position) && position.x >= 0 && position.x < gameSize && position.y >= 0 && position.y < gameSize) {
		int field = getField(position, &gameState);
		if (field == 1)
			*black += 1;
		else if (field == 2)
			*white += 1;
		else {
			neighbors->push_back(position);		// add stone to group list

			goThroughEmptyPoints(position + Vector(1, 0), neighbors, black, white);
			goThroughEmptyPoints(position + Vector(-1, 0), neighbors, black, white);
			goThroughEmptyPoints(position + Vector(0, 1), neighbors, black, white);
			goThroughEmptyPoints(position + Vector(0, -1), neighbors, black, white);
		}
	}
	else
		return;
}

void Game::killGroupsWithOneDame() {	// in the end of the game kill groups with one dame
	List<Vector> checked = List<Vector>();	// checked coordinates
	Vector check = Vector();		// coordinates to check

	for (int i = 0; i < gameSize; i++) {
		for (int k = 0; k < gameSize; k++) {
			check.x = i;
			check.y = k;
			if (checked.contain(&check))
				continue;
			else {
				List<Vector> group = getGroup(check, &gameState);	// get group and kill if need
				int dameCount = countGroupDame(&group, &gameState);

				if (dameCount == 1)
					captivateStones(&group, &gameState, false);
				checked.concat(&group);
			}
		}
	}
}

Vector Game::getEndScore() {	// calculate final score
	List<Vector> checked = List<Vector>();	// checked coordinates
	Vector check = Vector();		// coordinates to check
	Vector finalScore = Vector();

	for (int i = 0; i < gameSize; i++) {
		for (int k = 0; k < gameSize; k++) {
			check.x = i;
			check.y = k;
			if (checked.contain(check))
				continue;
			int playerBlackCount = 0;
			int playerWhiteCount = 0;
			List<Vector> group = getEmptyGroup(check, &playerBlackCount, &playerWhiteCount);	// group of empty stones
			if (playerBlackCount >= 1 && playerWhiteCount == 0)		// if only one player touches group give him points
				finalScore.x += group.size();
			else if (playerBlackCount == 0 && playerWhiteCount >= 1)
				finalScore.y += group.size();
			checked.concat(&group);
		}
	}

	finalScore = finalScore - deadStones;		// decrease score by dead stones
	if (finalScore.x < 0) finalScore.x = 0;
	if (finalScore.y < 0) finalScore.y = 0;
	return finalScore;
}