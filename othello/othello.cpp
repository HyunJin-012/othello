#include <bangtal>
using namespace bangtal;

ObjectPtr board[8][8]; //전역변수로 보드선언

//상태 enum class
enum  class State {
	BLANK,
	POSSIBLE,
	BLACK,
	WHITE
};

//Possible상태에서 차례나누기
enum class Turn {
	BLACK,
	WHITE
};
Turn turn = Turn::BLACK;  //시작은 검은돌

 //상태에 따라 이미지를 바꾸는 함수
void setState(int x, int y, State state) {
	switch (state) {
	case State::BLANK: board[y][x]->setImage("Images/blank.png"); break;
	case State::POSSIBLE: board[y][x]->setImage(turn == Turn::BLACK ? "Images/blank possible.png" : "Imgaes/white possible.png"); break;
	case State::BLACK: board[y][x]->setImage("Images/black.png"); break;
	case State::WHITE: board[y][x]->setImage("Images/white.png"); break;
	}
}


int main() {
	//필요없는 옵션 제거
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	auto scene = Scene::create("Othello", "Images/background.png");

	
	//보드 오브젝트 생성
	for (int y = 0;y < 8;y++) {
		for (int x = 0;x < 8;x++) {
			board[y][x] = Object::create("Images/blank.png", scene, 40 + x * 80, 40 + y * 80);
			board[y][x]->setOnMouseCallback([&, x, y](ObjectPtr object, int, int, MouseAction action)->bool {   //보드를 클릭하면 상대방 돌의 차례(보드의 x,y를 &로 두어 값에 접근함)
				if (turn == Turn::BLACK) {
					setState(x, y, State::BLACK);
					turn = Turn::WHITE;
				}
				else {
					setState(x, y, State::WHITE);
					turn = Turn::BLACK;
				}


				return true;
				});
		}
	}

	//시작화면 세팅
	setState(3, 3, State::BLACK);
	setState(4, 4, State::BLACK);
	setState(3, 4, State::BLACK);
	setState(4, 3, State::BLACK);










	startGame(scene);


}

