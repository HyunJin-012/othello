#include <bangtal>
#include <iostream>

using namespace bangtal;
using namespace std;

int addBlack, addWhite;
ObjectPtr board[8][8];
ObjectPtr scoreBlack1, scoreBlack2, scoreWhite1, scoreWhite2;


enum class State {
	BLANK,
	POSSIBLE,
	BLACK,
	WHITE
};
State board_state[8][8];   //보드의 상태를 저장하는 배열변수

enum class Turn {
	BLACK,
	WHITE
};
Turn turn = Turn::BLACK; // 시작은 BlACK

void setState(int x, int y, State state) {   //돌의 상태를 바꾸는 함수     
	switch (state) {
	case State::BLANK: board[y][x]->setImage("Images/blank.png"); break;
	case State::POSSIBLE: board[y][x]->setImage(turn == Turn::BLACK ? "Images/black possible.png" : "Images/white possible.png"); break;
	case State::BLACK: board[y][x]->setImage("Images/black.png"); break;
	case State::WHITE: board[y][x]->setImage("Images/white.png"); break;
	}
	board_state[y][x] = state;
}

void calcScore() {    //점수를 계산해서 표시하는 함수
	addBlack = 0;
	addWhite = 0;

	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (board_state[y][x] == State::BLACK) 
				addBlack++;
			else if (board_state[y][x] == State::WHITE) 
				addWhite++;
		}
	}
	int b1 = addBlack / 10; int b2 = addBlack % 10;
	int w1 = addWhite / 10; int w2 = addWhite % 10;

	string scoreB1 = "Images/L" + to_string(b1) + ".png";
	string scoreB2 = "Images/L" + to_string(b2) + ".png";
	string scoreW1 = "Images/L" + to_string(w1) + ".png";
	string scoreW2 = "Images/L" + to_string(w2) + ".png";

	scoreBlack1->setImage(scoreB1);
	scoreBlack2->setImage(scoreB2);
	scoreWhite1->setImage(scoreW1);
	scoreWhite2->setImage(scoreW2);
}


bool checkPossible(int x, int y, int dx, int dy) {
	State self = turn == Turn::BLACK ? State::BLACK : State::WHITE;
	State other = turn == Turn::BLACK ? State::WHITE : State::BLACK;

	bool possible = false;
	for (x += dx, y += dy; x >= 0 && x < 8 && y >= 0 && y < 8; x += dx, y += dy) {    
		if (board_state[y][x] == other) {
			possible = true;
		}
		else if (board_state[y][x] == self) {
			return possible;
		}
		else {
			return false;
		}
	}
	return false;
}

//보드 상태에 따라 돌을 놓을 수 있는지 확인
bool checkPossible(int x, int y) {
	if (board_state[y][x] == State::BLACK) return false;
	if (board_state[y][x] == State::WHITE) return false;
	setState(x, y, State::BLANK);

	int delta[8][2] = {
		{0, 1},
		{1, 1},
		{1, 0},
		{1, -1},
		{0, -1},
		{-1, -1},
		{-1, 0},
		{-1, 1},
	};

	bool possible = false;

	for (auto d : delta) {		        // c++ range for 
		if (checkPossible(x, y, d[0], d[1])) possible = true;   //한 방향만 가능->possible
	}

	return possible;
}

void reverse(int x, int y, int dx, int dy) {
	State self = turn == Turn::BLACK ? State::BLACK : State::WHITE;
	State other = turn == Turn::BLACK ? State::WHITE : State::BLACK;

	bool possible = false;
	for (x += dx, y += dy; x >= 0 && x < 8 && y >= 0 && y < 8; x += dx, y += dy) {    
		if (board_state[y][x] == other) {
			possible = true;
		}
		else if (board_state[y][x] == self) {
			if (possible) {
				for (x -= dx, y -= dy; x >= 0 && x < 8 && y >= 0 && y < 8; x -= dx, y -= dy) {
					if (board_state[y][x] == other) {
						setState(x, y, self);
					}
					else {
						return;
					}
				}
			}
		}
		else {
			return;
		}
	}
	return;
}

void reverse(int x, int y) {
	int delta[8][2] = {
		{0, 1},
		{1, 1},
		{1, 0},
		{1, -1},
		{0, -1},
		{-1, -1},
		{-1, 0},
		{-1, 1},
	};

	bool possible = false;

	for (auto d : delta) {		        // range for 
		reverse(x, y, d[0], d[1]);
	}
}

bool setPossible() {
	//8x8의 모든 위치에 대해 
	//놓여질 수 있으면 상태를 possible로 바꾼다
	bool possible = false;
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (checkPossible(x, y)) {
				setState(x, y, State::POSSIBLE);
				possible = true;
			}
		}
	}
	return possible;
}

int main()
{
	//필요없는 옵션 제거
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	auto scene = Scene::create("Othello", "Images/background.png");
	auto turnCheck = Object::create("Images/black turn.png", scene, 750, 380);
	scoreBlack1 = Object::create("Images/L0.png", scene, 750, 220);
	scoreBlack2 = Object::create("Images/L2.png", scene, 830, 220);
	scoreWhite1 = Object::create("Images/L0.png", scene, 1070, 220);
	scoreWhite2 = Object::create("Images/L2.png", scene, 1150, 220);

	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			board[y][x] = Object::create("Images/blank.png", scene, 40 + x * 80, 40 + y * 80);
			board[y][x]->setOnMouseCallback([&, x, y](ObjectPtr object, int, int, MouseAction action)-> bool {
				if (board_state[y][x] == State::POSSIBLE) {
					if (turn == Turn::BLACK) {
						setState(x, y, State::BLACK);
						reverse(x, y);
						turn = Turn::WHITE;
						turnCheck->setImage("Images/white turn.png");
					}
					else {
						setState(x, y, State::WHITE);
						reverse(x, y);
						turn = Turn::BLACK;
						turnCheck->setImage("Images/black turn.png");
					}
					if (!setPossible()) {
						turn = turn == Turn::BLACK ? Turn::WHITE : Turn::BLACK;   // toggle
						if (!setPossible()) {   
							if (addBlack > addWhite) showMessage("~Black Win~");
							else if (addBlack < addWhite) showMessage("~White WIn~");
							else if (addBlack == addWhite) showMessage("~Draw~");
						}
					}
				}
				calcScore();
				return true;
				});
			board_state[y][x] = State::BLANK;
		}
	}
	setState(3, 3, State::BLACK);
	setState(4, 4, State::BLACK);
	setState(3, 4, State::WHITE);
	setState(4, 3, State::WHITE);

	setPossible();

	startGame(scene);
}