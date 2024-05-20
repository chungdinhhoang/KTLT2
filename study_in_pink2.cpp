#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
/// Utilities
////////////////////////////////////////////////

string IntToStr(int num) {
    if (num == 0) return "0";
    string result = "";
    while (num) {
        result = char((num % 10) + '0') + result;
        num /= 10;
    }
    return result;
}

int stringToInt(string str) {
    int result = 0;
    for (char c : str) {
        result = result * 10 + (c - '0');
    }
    return result;
}

/////////////////

MapElement::MapElement(ElementType in_type) {
    type = in_type;
}

MapElement::~MapElement() {}

ElementType MapElement::getType() const {
    return type;
}

Path::Path(): MapElement(ElementType::PATH) {
}

Wall::Wall(): MapElement(ElementType::WALL) {
}

FakeWall::FakeWall(int in_req_exp): MapElement(ElementType::FAKE_WALL) {
    req_exp = in_req_exp;
}

// Page 3
int FakeWall::getReqExp() const {
    return req_exp;
}

int CalcInReqExp(int r, int c) {
    return ((r*257 + c*139 + 89)%900) + 1;
}

Map::Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls): num_rows(num_rows), num_cols(num_cols) {
    //map = new MapElement[num_rows + 1][num_cols + 1];

    map = (MapElement ***)malloc(num_rows * sizeof(MapElement **));
    for (int i = 0; i < num_rows; i++) {
        map[i] = (MapElement **)malloc(num_cols * sizeof(MapElement *));
    }

    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            map[i][j] = NULL;
        }
    }

    for (int i = 0; i < num_walls; i++) {
        int r = array_walls[i].getRow(), c = array_walls[i].getCol();
        map[r][c] = new Wall();
    }

    for (int i = 0; i < num_fake_walls; i++) {
        int r = array_fake_walls[i].getRow(), c = array_fake_walls[i].getCol();
        map[r][c] = new FakeWall(CalcInReqExp(r,c));
    }

    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            if (map[i][j] == NULL) {
                map[i][j] = new Path();
            }
        }
    }
}

Map::~Map() {
    for (int i = 0; i < num_rows; i++) {
        delete [] map[i];
    }
    delete [] map;
}

bool Map::isValid(const Position & pos, MovingObject * mv_obj) const {
    int r = pos.getRow(), c = pos.getCol();

    if (r < 0 || r > this->num_rows - 1) return false;
    if (c < 0 || c > this->num_cols - 1) return false;

    ElementType type = this->map[r][c]->getType();

    if (type == ElementType::WALL) {
        return false;
    }

    if (type == ElementType::FAKE_WALL) {
        //cout << fake_wall->getReqExp() << endl;
        if (mv_obj->getName() != "Watson") {
            return true;
        } else if ((((Watson *)mv_obj)->exp > ((FakeWall *)(this->map[r][c]))->getReqExp())) {
            cout << "OK";
            return true;
        } else return false;
    }

    return true;
}

int Map::getNumRows() const {
    return this->num_rows;
}

int Map::getNumCols() const {
    return this->num_cols;
}

////////////////////////////////////////////////
// Position
////////////////////////////////////////////////

Position::Position(int r, int c) {
    this->r = r, this->c = c;
}

bool operator != (Position x, Position y) {
    return x.getRow() != y.getRow() || x.getCol() != y.getCol();
}
bool Position::isEqual(Position other) const {
    return this->getRow() == other.getRow() && this->getCol() == other.getCol();
}

Position::Position(const string& str_pos) {
    int r=0, c=0;
    int n = str_pos.size();

    int perimeter_pos = 0;
    for (; perimeter_pos < n; perimeter_pos++) {
        if (str_pos[perimeter_pos] == ',') {
            break;
        }
    }

    for (int i = 1; i < perimeter_pos; i++) {
        r = r*10 + (str_pos[i] - '0');
    }

    for (int i = perimeter_pos + 1; i < n - 1; i++) {
        c = c*10 + (str_pos[i] - '0');
    }

    this->r = r, this->c = c;
}

int Position::getRow() const {return r;}
int Position::getCol() const {return c;}
void Position::setRow(int r) {this->r=r;}
void Position::setCol(int c) {this->c=c;}

string Position::str() const {
    return "(" + IntToStr(this->r) + "," + IntToStr(this->c) + ")";
}

bool Position::isEqual(int in_r, int in_c) const {
    return (this->r == in_r, this->c == in_c);
}

static const Position npos = Position(-1,-1);

////////////////////////////////////////////////
/// MovingObject
////////////////////////////////////////////////

MovingObject::MovingObject() {}

MovingObject::MovingObject(int index, const Position pos, Map * map, const string & name) {
    this->index = index;
    this->pos = pos;
    this->map = map;
    this->name = name;
}

MovingObject::~MovingObject() {
}

Position MovingObject::getCurrentPosition() const {
    return this->pos;
}

string MovingObject::getName() const {
    return this->name;
}

int MovingObject::getIndex() const {
    return this->index;
}

////////////////////////////////////////////////
/// Character

////////////////////////////////////////////////
/// Sherlock

Sherlock::Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp) {
    this->index = index;
    this->moving_rule = moving_rule;
    this->map = map;
    this->init_exp = this->exp = init_exp;
    this->init_hp = this->hp = init_hp;

    this->pos = init_pos;
    this->name = "Sherlock";

    moving_index = 0;
}

Position Sherlock::getNextPosition() {
    Position cur = getCurrentPosition();
    int r = cur.getRow(), c = cur.getCol();

    switch (this->moving_rule[this->moving_index]) {
        case 'L':
            c--;
            break;
        case 'R':
            c++;
            break;
        case 'U':
            r--;
            break;
        case 'D':
            r++;
            break;
    }
    this->moving_index++;
    if (this->moving_index == this->moving_rule.size()) this->moving_index = 0;

    Position nextPosition = Position(r,c);

    if (!this->map->isValid(nextPosition, this)) {
        return npos;
    } else return nextPosition;
}

void Sherlock::move() {
    Position nextPosition = getNextPosition();

    if (nextPosition != npos) {
        pos = nextPosition;
    }
}

string Sherlock::str() const {
    return "Sherlock[index=" + IntToStr(this->index) + ";pos=" + this->pos.str() + ";moving_rule=" + this->moving_rule + "]";
}

////////////////////////////////////////////////
/// Watson

Watson::Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp) {
    this->index = index;
    this->moving_rule = moving_rule;
    this->map = map;
    this->init_exp = this->exp = init_exp;
    this->init_hp = this->hp = init_hp;

    this->pos = init_pos;
    this->name = "Watson";
    this->moving_index = 0;
}

Position Watson::getNextPosition() {
    Position cur = getCurrentPosition();
    int r = cur.getRow(), c = cur.getCol();

    switch (this->moving_rule[this->moving_index]) {
        case 'L':
            c--;
            break;
        case 'R':
            c++;
            break;
        case 'U':
            r--;
            break;
        case 'D':
            r++;
            break;
    }
    this->moving_index++;
    if (this->moving_index == this->moving_rule.size()) this->moving_index = 0;

    Position nextPosition = Position(r,c);

    if (!this->map->isValid(nextPosition, this)) {
        return npos;
    } else return nextPosition;
}

void Watson::move() {
    Position nextPosition = getNextPosition();

    if (nextPosition != npos) {
        pos = nextPosition;
    }
}

string Watson::str() const {
    return "Watson[index=" + IntToStr(this->index) + ";pos=" + this->pos.str() + ";moving_rule=" + this->moving_rule + "]";
}


////////////////////////////////////////////////
/// Criminals

Criminal::Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson) {
    this->sherlock = sherlock;
    this->watson = watson;

    this->pos = init_pos;
    this->map = map;
    this->name = "Criminal";
    this->index = index;

    this->moved_times = 0;
    this->lastPosition = npos;
}

int ManhattanDistance(Position a, Position b) {
    return abs(a.getRow() - b.getRow()) + abs(a.getCol() - b.getCol());
}

int sumOfDistance(Position pos, Sherlock * sherlock, Watson * watson) {
    return ManhattanDistance(pos,sherlock->getCurrentPosition()) + ManhattanDistance(pos,watson->getCurrentPosition());
}

Position Criminal::getNextPosition() {
    int cur_r = this->pos.getRow(), cur_c = this->pos.getCol();

    char choice = 'S'; int dis;
    Position nxt_pos;

    if (cur_r > 0) {
        Position tmp_nxt_pos = Position(cur_r-1,cur_c);
        int tmp_dis = sumOfDistance(tmp_nxt_pos, this->sherlock, this->watson);
        if (this->map->isValid(tmp_nxt_pos, this) && (choice == 'S' || dis < tmp_dis)) {
            choice = 'U';
            dis = tmp_dis;
            nxt_pos = tmp_nxt_pos;
        }
    } 
    
    if (cur_c > 0) {
        Position tmp_nxt_pos = Position(cur_r,cur_c-1);
        int tmp_dis = sumOfDistance(tmp_nxt_pos, this->sherlock, this->watson);
        if (this->map->isValid(tmp_nxt_pos, this) && (choice == 'S' || dis < tmp_dis)) {
            choice = 'L';
            dis = tmp_dis;
            nxt_pos = tmp_nxt_pos;
        }
    } 
    if (cur_r < this->map->getNumRows() - 1) {
        Position tmp_nxt_pos = Position(cur_r+1,cur_c);
        int tmp_dis = sumOfDistance(tmp_nxt_pos, this->sherlock, this->watson);
        if (this->map->isValid(tmp_nxt_pos, this) && (choice == 'S' || dis < tmp_dis)) {
            choice = 'D';
            dis = tmp_dis;
            nxt_pos = tmp_nxt_pos;
        }
    } 
    if (cur_c < this->map->getNumCols() - 1) {
        Position tmp_nxt_pos = Position(cur_r,cur_c+1);
        int tmp_dis = sumOfDistance(tmp_nxt_pos, this->sherlock, this->watson);
        if (this->map->isValid(tmp_nxt_pos, this) && (choice == 'S' || dis < tmp_dis)) {
            choice = 'R';
            dis = tmp_dis;
            nxt_pos = tmp_nxt_pos;
        }
    }

    if (choice == 'S') return npos;
    return nxt_pos;
}

void Criminal::move() {
    Position nextPosition = getNextPosition();

    //cout << nextPosition.getRow() << " " << nextPosition.getCol() << endl;

    if (nextPosition != npos) {
        this->moved_times++;

        if (this->moved_times >= 3) this->lastPosition = this->pos; 
        this->pos = nextPosition; 
    }
}

Position Criminal::getLastPosition() {
    return this->lastPosition;
}

string Criminal::str() const {
    return "Criminal[index=" + IntToStr(this->index) + ";pos=" + this->pos.str() + "]";
}

////////////////////////////////////////////////
/// ArrayMovingObject
////////////////////////////////////////////////

ArrayMovingObjectItem::ArrayMovingObjectItem(MovingObject * obj): obj(obj) {
    next = previous = NULL;
}

ArrayMovingObjectItem::~ArrayMovingObjectItem() {
    //if (obj != NULL) delete obj;
}

ArrayMovingObject::ArrayMovingObject(int capacity): capacity(capacity) {
    count = 0;
    head = NULL;
}

ArrayMovingObject::~ArrayMovingObject() {
    ArrayMovingObjectItem * cur = head;
    while (cur != NULL) {
        ArrayMovingObjectItem * tmp = cur;
        cur = cur->next;
        delete tmp;
    }
}

bool ArrayMovingObject::isFull() const {
    return this->count == this->capacity;
}

int ArrayMovingObject::size() const {
    return this->count;
}

bool ArrayMovingObject::add(MovingObject * mv_obj) {
    if (this->isFull()) {
        return false;
    }

    if (head == NULL) {
        head = new ArrayMovingObjectItem(mv_obj);
        count++;
        return true;
    }

    ArrayMovingObjectItem * cur = head;

    while (cur->next != NULL) cur = cur->next;
    cur->next = new ArrayMovingObjectItem(mv_obj);

    count++;

    return true;
}

void ArrayMovingObject::remove(ArrayMovingObjectItem * item) {
    if (this->head == item) {
        this->head = this->head->next;
        count--;

        delete item;

        return ;
    }

    ArrayMovingObjectItem * old_previous = item->previous, * old_next = item->next;
    if (old_previous != NULL) old_previous->next = old_next;
    if (old_next != NULL) old_next->previous = old_previous;

    delete item;
    count--;
}

string ArrayMovingObject::str() const {
    string result = "ArrayMovingObject[";
    result += "count=" + IntToStr(this->count) + ";";
    result += "capacity=" + IntToStr(this->capacity) + ";";

    //old: for (int i = 0; i < count; i++) result += this->arr_mv_objs[i]->str();

    /*
    ArrayMovingObjectItem have indexes and iterating process has to follow it
    ArrayMovingObjectItem * cur = head;
    while (cur != NULL) {
        result += cur->obj->str();
        cur = cur->next;
    }*/

    int looped = 0;
    int id = 0;
    while (looped < this->count) {
        ArrayMovingObjectItem * cur = head;
        while (cur != NULL && cur->obj->index != id) {
            //cout << cur->obj->getIndex() << endl;
            cur = cur->next;
        }
        if (cur == NULL) {
            id++;
            continue;
        }
        result += cur->obj->str();
        if (looped < count - 1) result += ";";
        id++;
        looped++;
    }

    result += "]";
    
    return result;
}

////////////////////////////////////////////////
/// Configuration
////////////////////////////////////////////////

/*return size*/
int readfile_getMapOfPositions(Position ** arr_pos, string str) {
    int numOfPos = 0;
    for (char c : str) if (c == ')') numOfPos++;

    //cout << numOfPos << endl;
    //cout << str << endl;

    *arr_pos = (Position *)malloc(numOfPos * sizeof(Position));

    int id = 0;
    int cur_num = 0;
    bool is_num_valid = false;

    for (char c : str) {
        if ('0' <= c && c <= '9') {
            cur_num = cur_num * 10 + (c - '0');
            is_num_valid = true;
        } else {
            if (!is_num_valid) continue;

            //cout << id << endl;
            if ((id & 1) == 0) {
                (*arr_pos)[id / 2].setRow(cur_num);
            } else (*arr_pos)[id / 2].setCol(cur_num);

            id++;
            cur_num = 0;
            is_num_valid = false;
        }
    }

    return numOfPos;
}

Position readfile_getPosition(string str) {
    int id = 0;
    int cur_num = 0;

    Position result;

    cout << str << endl;

    for (char c : str) {
        if ('0' <= c && c <= '9') {
            cur_num = cur_num * 10 + (c - '0');
        } else {
            if (c == ',' || c == ')') {
                if (id == 0) result.setRow(cur_num);
                else result.setCol(cur_num);

                id++;
                cur_num = 0;
            }
        }
    }

    return result;
}

Configuration::Configuration(const string & filepath) {
    ifstream infile(filepath);

    string line;

    prebuilt_str = "Configuration[\n";

    string lines[17];

    while (getline(infile, line)) {
        //prebuilt_str += line + "\n";

        int i = 0;
        while (i < line.length() && line[i] != '=') i++;

        string key = line.substr(0, i); // i is the length
        string value = line.substr(i + 1, line.length() - i - 1);

        if (key == "MAP_NUM_ROWS") {
            lines[0] = line + "\n";
            map_num_rows = stringToInt(value);
        }

        if (key == "MAP_NUM_COLS") {
            lines[1] = line + "\n";
            map_num_cols = stringToInt(value);
        }

        if (key == "MAX_NUM_MOVING_OBJECTS") {
            lines[2] = line + "\n";
            max_num_moving_objects = stringToInt(value);
        }

        if (key == "ARRAY_WALLS") {
            num_walls = readfile_getMapOfPositions(&arr_walls, value);
            lines[3] = "NUM_WALLS=" + IntToStr(num_walls) + '\n';
            lines[4] = line + "\n";
        }

        if (key == "ARRAY_FAKE_WALLS") {
            num_fake_walls = readfile_getMapOfPositions(&arr_fake_walls, value);
            lines[5]= "NUM_FAKE_WALLS=" + IntToStr(num_fake_walls) + '\n';
            lines[6] = line + "\n";
        }

        if (key == "SHERLOCK_MOVING_RULE") {
            lines[7] = line + "\n";
            sherlock_moving_rule = value;
        }

        if (key == "SHERLOCK_INIT_POS") {
            //lines[8] = line + "\n";
            sherlock_init_pos = readfile_getPosition(value);
            lines[8] = "SHERLOCK_INIT_POS=(" + IntToStr(sherlock_init_pos.getRow()) + "," + IntToStr(sherlock_init_pos.getCol()) + ")\n";
        }

        if (key == "SHERLOCK_INIT_HP") {
            lines[9] = line + "\n";
            sherlock_init_hp = stringToInt(value);
        }

        if (key == "SHERLOCK_INIT_EXP") {
            lines[10] = line + "\n";
            sherlock_init_exp = stringToInt(value);
        }

        if (key == "WATSON_MOVING_RULE") {
            lines[11] = line + "\n";
            watson_moving_rule = value;
        }

        if (key == "WATSON_INIT_POS") {
            watson_init_pos = readfile_getPosition(value);
            lines[12] = "WATSON_INIT_POS=(" + IntToStr(watson_init_pos.getRow()) + "," + IntToStr(watson_init_pos.getCol()) + ")\n";
        }

        if (key == "WATSON_INIT_HP") {
            lines[13] = line + "\n";
            watson_init_hp = stringToInt(value);
        }

        if (key == "WATSON_INIT_EXP") {
            lines[14] = line + "\n";
            watson_init_exp = stringToInt(value);
        }

        if (key == "CRIMINAL_INIT_POS") {
            criminal_init_pos = readfile_getPosition(value);
            lines[15] = "CRIMINAL_INIT_POS=(" + IntToStr(criminal_init_pos.getRow()) + "," + IntToStr(criminal_init_pos.getCol()) + ")\n";
        }

        if (key == "NUM_STEPS") {
            lines[16] = line + "\n";
            num_steps = stringToInt(value);
        }
    }

    for (int i = 0; i < 17; i++) prebuilt_str += lines[i];

    prebuilt_str += "]";
}

string Configuration::str() const {
    return prebuilt_str;
}

Configuration::~Configuration() {
    delete [] arr_walls;
    delete [] arr_fake_walls;
}

////////////////////////////////////////////////
/// Robot
////////////////////////////////////////////////

void Robot::createItem() {
    int i = this->getCurrentPosition().getRow();
    int j = this->getCurrentPosition().getCol();

    int p = i * j;

    int s = p; // So chu dao
    while (s > 9) {
        int tmp = s, ss = 0;
        while (tmp) {
            ss += tmp % 10;
            tmp /= 10;
        }
        s = ss;
    }

    if (s <= 1) {
        this->item = new MagicBook();
    }
    if (s <= 3) {
        this->item = new EnergyDrink();
    }
    if (s <= 5) {
        this->item = new FirstAid();
    }
    if (s <= 7) {
        this->item = new ExcemptionCard();
    }
    if (s <= 9) {
        int t = (i * 11 + j) % 4;
        if (t == 0) this->item = new PassingCard("RobotS");
        if (t == 1) this->item = new PassingCard("RobotC");
        if (t == 2) this->item = new PassingCard("RobotSW");
        if (t == 3) this->item = new PassingCard("all");
    }
}

int Robot::getIndex() const {
    return this->index;
}

RobotC::RobotC(int index , const Position & init_pos , Map * map , Criminal * criminal): criminal(criminal) {
    this->index = index;
    this->pos = init_pos;
    this->map = map;

    this->robot_type = RobotType::C;
    createItem();
}

Position RobotC::getNextPosition() {
    return this->criminal->getLastPosition();
}

void RobotC::move() {
    Position nextPosition = this->getNextPosition();
    if (nextPosition != npos) {
        this->pos = nextPosition;
    }
}

int RobotC::getDistance(Character * character) const {
    Position char_pos = character->getCurrentPosition();
    Position this_pos = this->getCurrentPosition();
    return ManhattanDistance(this_pos, char_pos);
}

string RobotC::str() const {
    return "Robot[pos=" + this->pos.str() + ";type=" + "C" + ";dist=" + "" + "]";
}

/////////

RobotS::RobotS(int index, const Position & init_pos, Map * map, Criminal * criminal, Sherlock * sherlock): criminal(criminal), sherlock(sherlock) {
    this->index = index;
    this->pos = init_pos;
    this->map = map;

    this->robot_type = RobotType::S;
    createItem();
}

Position RobotS::getNextPosition() {
    Position currentPosition = this->getCurrentPosition();

    int current_sherlock_dist = 1000000000;
    Position sherlock_pos = this->sherlock->getCurrentPosition();

    Position nxt_pos = npos;

    int r = currentPosition.getRow(), c = currentPosition.getCol();

    if (r > 0) {
        Position tmp_nxt_pos = Position(r - 1, c);
        if (this->map->isValid(tmp_nxt_pos, this)) {
            int tmp_sherlock_dist = ManhattanDistance(tmp_nxt_pos, sherlock_pos);

            if (tmp_sherlock_dist < current_sherlock_dist) {
                nxt_pos = tmp_nxt_pos;
                current_sherlock_dist = tmp_sherlock_dist;
            }
        }
    }

    if (c < this->map->getNumCols() - 1) {
        Position tmp_nxt_pos = Position(r, c + 1);
        if (this->map->isValid(tmp_nxt_pos, this)) {
            int tmp_sherlock_dist = ManhattanDistance(tmp_nxt_pos, sherlock_pos);

            if (tmp_sherlock_dist < current_sherlock_dist) {
                nxt_pos = tmp_nxt_pos;
                current_sherlock_dist = tmp_sherlock_dist;
            }
        }
    }

    if (r < this->map->getNumRows() - 1) {
        Position tmp_nxt_pos = Position(r + 1, c);
        if (this->map->isValid(tmp_nxt_pos, this)) {
            int tmp_sherlock_dist = ManhattanDistance(tmp_nxt_pos, sherlock_pos);

            if (tmp_sherlock_dist < current_sherlock_dist) {
                nxt_pos = tmp_nxt_pos;
                current_sherlock_dist = tmp_sherlock_dist;
            }
        }
    }

    if (c > 0) {
        Position tmp_nxt_pos = Position(r, c - 1);
        if (this->map->isValid(tmp_nxt_pos, this)) {
            int tmp_sherlock_dist = ManhattanDistance(tmp_nxt_pos, sherlock_pos);

            if (tmp_sherlock_dist < current_sherlock_dist) {
                nxt_pos = tmp_nxt_pos;
                current_sherlock_dist = tmp_sherlock_dist;
            }
        }
    }

    return nxt_pos;
}


int RobotS::getDistance() const {
    Position sherlock_pos = this->sherlock->getCurrentPosition();
    Position this_pos = this->getCurrentPosition();
    return ManhattanDistance(this_pos, sherlock_pos);
}

void RobotS::move() {
    Position nextPosition = this->getNextPosition();

    if (nextPosition != npos) {
        this->pos = nextPosition;
    }
}

string RobotS::str() const {
    return "Robot[pos=" + this->pos.str() + ";type=" + "S" + ";dist=" + IntToStr(this->getDistance()) + "]";
}

/////////

RobotW::RobotW(int index, const Position & init_pos, Map * map, Criminal * criminal, Watson * watson): criminal(criminal), watson(watson) {
    this->index = index;
    this->pos = init_pos;
    this->map = map;

    this->robot_type = RobotType::W;
    createItem();
}

Position RobotW::getNextPosition() {
    Position currentPosition = this->getCurrentPosition();

    int current_watson_dist = 1000000000;
    Position watson_pos = this->watson->getCurrentPosition();

    Position nxt_pos = npos;

    int r = currentPosition.getRow(), c = currentPosition.getCol();

    if (r > 0) {
        Position tmp_nxt_pos = Position(r - 1, c);
        if (this->map->isValid(tmp_nxt_pos, this)) {
            int tmp_watson_dist = ManhattanDistance(tmp_nxt_pos, watson_pos);

            if (tmp_watson_dist < current_watson_dist) {
                nxt_pos = tmp_nxt_pos;
                current_watson_dist = tmp_watson_dist;
            }
        }
    }

    if (c < this->map->getNumCols() - 1) {
        Position tmp_nxt_pos = Position(r, c + 1);
        if (this->map->isValid(tmp_nxt_pos, this)) {
            int tmp_watson_dist = ManhattanDistance(tmp_nxt_pos, watson_pos);

            if (tmp_watson_dist < current_watson_dist) {
                nxt_pos = tmp_nxt_pos;
                current_watson_dist = tmp_watson_dist;
            }
        }
    }

    if (r < this->map->getNumRows() - 1) {
        Position tmp_nxt_pos = Position(r + 1, c);
        if (this->map->isValid(tmp_nxt_pos, this)) {
            int tmp_watson_dist = ManhattanDistance(tmp_nxt_pos, watson_pos);

            if (tmp_watson_dist < current_watson_dist) {
                nxt_pos = tmp_nxt_pos;
                current_watson_dist = tmp_watson_dist;
            }
        }
    }

    if (c > 0) {
        Position tmp_nxt_pos = Position(r, c - 1);
        if (this->map->isValid(tmp_nxt_pos, this)) {
            int tmp_watson_dist = ManhattanDistance(tmp_nxt_pos, watson_pos);

            if (tmp_watson_dist < current_watson_dist) {
                nxt_pos = tmp_nxt_pos;
                current_watson_dist = tmp_watson_dist;
            }
        }
    }

    return nxt_pos;
}


int RobotW::getDistance() const {
    Position watson_pos = this->watson->getCurrentPosition();
    Position this_pos = this->getCurrentPosition();
    return ManhattanDistance(this_pos, watson_pos);
}

void RobotW::move() {
    Position nextPosition = this->getNextPosition();

    if (nextPosition != npos) {
        this->pos = nextPosition;
    }
}

string RobotW::str() const {
    return "Robot[pos=" + this->pos.str() + ";type=" + "W" + ";dist=" + IntToStr(this->getDistance()) + "]";
}

/////////

RobotSW::RobotSW(int index, const Position & init_pos, Map * map, Criminal * criminal, Sherlock * sherlock, Watson * watson): criminal(criminal), sherlock(sherlock), watson(watson) {
    this->index = index;
    this->pos = init_pos;
    this->map = map;

    this->robot_type = RobotType::SW;
    createItem();
}

Position RobotSW::getNextPosition() {
    int dr[8] = {-2, -1, 0, 1, 2, 1, 0, -1};
    int dc[8] = {0, 1, 2, 1, 0, -1, -2, -1};

    Position currentPosition = this->pos;    

    int current_dist = 1000000000;
    Position sherlock_pos = this->sherlock->getCurrentPosition();
    Position watson_pos = this->watson->getCurrentPosition();

    int r = currentPosition.getRow(), c = currentPosition.getCol();

    Position nxt_pos = npos;

    for (int k = 0; k < 8; k++) {
        int r2 = r + dr[k], c2 = c + dc[k];
        Position tmp_nxt_pos = Position(r2,c2);

        if (this->map->isValid(tmp_nxt_pos, this)) {
            int tmp_dist = ManhattanDistance(tmp_nxt_pos, sherlock_pos) + ManhattanDistance(tmp_nxt_pos, watson_pos);

            if (tmp_dist < current_dist) {
                current_dist = tmp_dist;
                nxt_pos = tmp_nxt_pos;
            }
        }
    }

    return nxt_pos;
}

void RobotSW::move() {
    Position nextPosition = this->getNextPosition();

    if (nextPosition != npos) {
        this->pos = nextPosition;
    }
}

int RobotSW::getDistance() const {
    return ManhattanDistance(this->getCurrentPosition(), this->sherlock->getCurrentPosition())
        + ManhattanDistance(this->getCurrentPosition(), this->watson->getCurrentPosition());
}

string RobotSW::str() const {
    return "Robot[pos=" + this->pos.str() + ";type=" + "SW" + ";dist=" + IntToStr(this->getDistance()) + "]";
}

////////////////////////////////////////////////
/// BaseItem
////////////////////////////////////////////////

MagicBook::MagicBook() {
    item_type = ItemType::MAGIC_BOOK;
    name = "MagicBook";
};

bool MagicBook::canUse(Character * obj, Robot * robot) {
    return obj->exp <= 350;
}

void MagicBook::use(Character * obj, Robot * robot) {
    obj->exp = ceil(obj->exp * 1.25);
    if (obj->exp > 900) {
        obj->exp = 900;
    }
}

EnergyDrink::EnergyDrink() {
    item_type = ItemType::ENERGY_DRINK;
    name = "EnergyDrink";
};

bool EnergyDrink::canUse(Character * obj, Robot * robot) {
    return obj->hp <= 350;
}

void EnergyDrink::use(Character * obj, Robot * robot) {
    obj->hp = ceil(obj->hp * 1.2);
    if (obj->hp > 500) {
        obj->hp = 500;
    }
}

FirstAid::FirstAid() {
    item_type = ItemType::FIRST_AID;
    name = "FirstAid";
};

bool FirstAid::canUse(Character * obj, Robot * robot) {
    return obj->hp <= 100 || obj->exp <= 350;
}

void FirstAid::use(Character * obj, Robot * robot) {
    obj->hp = ceil(obj->hp * 1.5);
    if (obj->hp > 500) {
        obj->exp = 500;
    }
}

ExcemptionCard::ExcemptionCard() {
    item_type = ItemType::EXCEMPTION_CARD;
    name = "ExcemptionCard";
};

bool ExcemptionCard::canUse(Character * obj, Robot * robot) {
    return (obj->getName() == "sherlock") && (obj->hp & 1);
}

void ExcemptionCard::use(Character * obj, Robot * robot) {}

PassingCard::PassingCard(string challenge): challenge(challenge) {
    item_type = ItemType::PASSING_CARD;
    name = "PassingCard";
}

bool PassingCard::canUse(Character * obj, Robot * robot) {
    if (!((obj->getName() == "watson") && !(obj->hp & 1))) return false;
    if (challenge == "all") return true;
    if ((challenge == "RobotS") && (robot->robot_type == RobotType::S)) return true;
    if ((challenge == "RobotC") && (robot->robot_type == RobotType::C)) return true;
    if ((challenge == "RobotSW") && (robot->robot_type == RobotType::SW)) return true;

    return false;
}

void PassingCard::use(Character * obj, Robot * robot) {}

////////////////////////////////////////////////
/// LinkedList
////////////////////////////////////////////////

LinkedListItem::LinkedListItem(BaseItem * item): item(item) {
    next = NULL;
}

LinkedListItem::~LinkedListItem() {
    if (item != NULL) delete item;
}

LinkedList::LinkedList(int capacity): capacity(capacity) {
    count = 0;
    head = NULL;
}

LinkedList::~LinkedList() {
    LinkedListItem * old_head = this->head;
    while (old_head != NULL) {
        this->head = this->head->next;
        delete old_head;
    }
}

// swap HEAD with a
/*
void LinkedList::swap(LinkedListItem * a) {
    LinkedListItem * old_head = this->head;
}*/

BaseItem * LinkedList::get(LinkedListItem * linkedlist_item) {
    if (this->head == linkedlist_item) {
        //cout << "xx\n";
        this->head = linkedlist_item->next;
        BaseItem * item_to_return = linkedlist_item->item;
        linkedlist_item->item = NULL;
        delete linkedlist_item;
        count--;
        //cout << "yy\n";
        return item_to_return;
    }

    if (this->head->next == linkedlist_item) {
        this->head->next = linkedlist_item->next;

        BaseItem * item_to_return = linkedlist_item->item;
        linkedlist_item->item = NULL;
        delete linkedlist_item;
        count--;

        return item_to_return;
    }

    LinkedListItem * previous_linkedlist_item = this->head;

    while (previous_linkedlist_item->next != linkedlist_item) previous_linkedlist_item = previous_linkedlist_item->next;

    //swap this with head
    LinkedListItem * old_head = this->head;
    LinkedListItem * old_linkedlist_item_next = linkedlist_item->next;

    this->head = linkedlist_item;
    linkedlist_item->next = old_head->next;

    previous_linkedlist_item->next = old_head;
    old_head->next = old_linkedlist_item_next;

    // remove head
    old_head = this->head;
    this->head = this->head->next;

    BaseItem * item_to_return = old_head->item;
    old_head->item = NULL;
    delete old_head;

    count--;

    return item_to_return;
}

bool LinkedList::insert(BaseItem * item) {
    if (count + 1 > capacity) {
        return false;
    }

    this->count++;

    if (this->head == NULL) {
        this->head = new LinkedListItem(item);
        return true;
    }

    LinkedListItem * new_item = new LinkedListItem(item);
    LinkedListItem * old_head = this->head;

    new_item->next = old_head;

    this->head = new_item;

    return true;
}

////////////////////////////////////////////////
/// Bag...
////////////////////////////////////////////////
string BaseItem::getName() const {return this->name;}

BaseItem * BaseBag::get() {
    if (this->linkedlist_items->count > 0) return this->get(this->linkedlist_items->head->item->item_type);
    return NULL;
}

BaseItem * BaseBag::get(ItemType itemType) {
    LinkedListItem * cur_linkedlist_item = this->linkedlist_items->head;

    while (cur_linkedlist_item != NULL) {
        if (cur_linkedlist_item->item->item_type == itemType) {
            return this->linkedlist_items->get(cur_linkedlist_item);
        }

        cur_linkedlist_item = cur_linkedlist_item->next;
    }

    return NULL;
}

bool BaseBag::insert(BaseItem * item) {
    return this->linkedlist_items->insert(item);
}

string BaseBag::str() const {
    string result = "Bag[count=" + IntToStr(this->linkedlist_items->count);
    
    LinkedListItem * cur_linkedlist_item = this->linkedlist_items->head;

    if (cur_linkedlist_item != NULL) result += ";";

    while (cur_linkedlist_item != NULL) {
        result = result + cur_linkedlist_item->item->getName() + ",";
        cur_linkedlist_item = cur_linkedlist_item->next;
    }

    if (this->linkedlist_items->head != NULL) result.pop_back();
    result = result + "]";

    return result;
}


SherlockBag::SherlockBag(Sherlock * sherlock) {
    this->obj = sherlock;
    this->linkedlist_items = new LinkedList(13);
}

WatsonBag::WatsonBag(Watson * watson) {
    this->obj = watson;
    this->linkedlist_items = new LinkedList(15);
}

/*
WatsonBag::WatsonBag(Watson * watson) {
    this->obj = watson;
}
*/

////////////////////////////////////////////////
/// StudyPinkProgram
////////////////////////////////////////////////

StudyPinkProgram::StudyPinkProgram(const string & config_file_path) {
    config = new Configuration(config_file_path);

    map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
    arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);

    sherlock = new Sherlock(0, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
    arr_mv_objs->add(sherlock);
    watson = new Watson(1, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
    arr_mv_objs->add(watson);
    criminal = new Criminal(2, config->criminal_init_pos, map, sherlock, watson);
    arr_mv_objs->add(criminal);

    criminal_caught = false;
    sherlock_bag = new SherlockBag(sherlock);
    watson_bag = new WatsonBag(watson);
}

bool StudyPinkProgram::isStop() const {
    return sherlock->hp == 0 || watson->hp == 0 || criminal_caught;
}

void StudyPinkProgram::checkCriminalCreateRobot() {
    if (criminal->moved_times == 3 && !arr_mv_objs->isFull()) {
        Robot * new_robot = NULL;
        int next_num = arr_mv_objs->size() + 1;

        if (this->criminal->moved_times == 3) {
            new_robot = new RobotC(next_num, criminal->getCurrentPosition(), map, criminal);
        } else {
            int sherlock_dist = ManhattanDistance(criminal->getCurrentPosition(), sherlock->getCurrentPosition());
            int watson_dist = ManhattanDistance(criminal->getCurrentPosition(), watson->getCurrentPosition());

            if (sherlock_dist < watson_dist) {
                new_robot = new RobotS(next_num, criminal->getCurrentPosition(), map, criminal, sherlock);
            } else if (sherlock_dist > watson_dist) {
                new_robot = new RobotW(next_num, criminal->getCurrentPosition(), map, criminal, watson);
            } else {
                new_robot = new RobotSW(next_num, criminal->getCurrentPosition(), map, criminal, sherlock, watson);
            }

            // add it to arr_mv_objs
            new_robot->createItem();
            arr_mv_objs->add(new_robot);
        }
    }
}

void StudyPinkProgram::checkMeeting() {
    // This function checks all cases, though only 1 of them would occur at a time

    // check if Sherlock and Watson meet
    if (sherlock->getCurrentPosition().isEqual(watson->getCurrentPosition())) {
        // Sherlock gives Watson all his PassingCard
        PassingCard * sherlock_passing_card = (PassingCard *)sherlock_bag->get(ItemType::PASSING_CARD);

        while (sherlock_passing_card != NULL) {
            watson_bag->insert(sherlock_passing_card);
        }

        ExcemptionCard * watson_excemption_card = (ExcemptionCard *)watson_bag->get(ItemType::EXCEMPTION_CARD);

        while (watson_excemption_card != NULL) {
            sherlock_bag->insert(watson_excemption_card);
        }

        return ;
    }

    // Check if Sherlock or Watson meet Criminal

    if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
        criminal_caught = true;
        return ;
    }

    if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
        criminal_caught = true;
        return ;
    }

    // check if Sherlock meet any robot
    ArrayMovingObjectItem * cur_robot_in_arr = arr_mv_objs->head;
    for (int i = 0; i < 3; i++) cur_robot_in_arr = cur_robot_in_arr->next;
    // the first 3 in array is sherlock, watson, criminal

    while (cur_robot_in_arr != NULL) {
        Robot * cur_robot = (Robot *)cur_robot_in_arr->obj;
        if (sherlock->getCurrentPosition().isEqual(cur_robot->getCurrentPosition())) {
            // Before fighting the robot, Sherlock use a ExcemptionCard if he has one
            // Take it out even if sherlock wont use it
            BaseItem * excemption_card_to_use = sherlock_bag->get(ItemType::EXCEMPTION_CARD);
            bool can_use_excemption_card = (excemption_card_to_use != NULL && excemption_card_to_use->canUse(sherlock, cur_robot));
            if (excemption_card_to_use != NULL) delete excemption_card_to_use;

            //Fight the robot
            if (cur_robot->robot_type == RobotType::S) {
                if (sherlock->exp > 400) {
                    // Codes below get the item and kill the robot
                    // You used it for the next if blocks to (copy & paste). Be careful
                    sherlock_bag->insert(cur_robot->item);
                    cur_robot->item = NULL;
                    
                    ArrayMovingObjectItem * old_robot_in_arr = cur_robot_in_arr;
                    cur_robot_in_arr = cur_robot_in_arr->next;

                    arr_mv_objs->remove(old_robot_in_arr);
                    continue;
                } else {
                    if (!can_use_excemption_card) {
                        sherlock->exp = ceil(sherlock->exp * 0.9);
                    }
                    cur_robot_in_arr = cur_robot_in_arr->next;
                    continue;
                }
            } else if (cur_robot->robot_type == RobotType::W) {
                sherlock_bag->insert(cur_robot->item);
                cur_robot->item = NULL;
                
                ArrayMovingObjectItem * old_robot_in_arr = cur_robot_in_arr;
                cur_robot_in_arr = cur_robot_in_arr->next;

                arr_mv_objs->remove(old_robot_in_arr);
                continue;
            } else if (cur_robot->robot_type == RobotType::SW) {
                if ((sherlock->exp > 300 && sherlock->hp > 335) || can_use_excemption_card) {
                    sherlock_bag->insert(cur_robot->item);
                    cur_robot->item = NULL;
                    
                    ArrayMovingObjectItem * old_robot_in_arr = cur_robot_in_arr;
                    cur_robot_in_arr = cur_robot_in_arr->next; // next here

                    arr_mv_objs->remove(old_robot_in_arr);
                    continue;
                } else {
                    sherlock->exp = ceil(sherlock->exp * 0.85);
                    sherlock->hp = ceil(sherlock->hp * 0.85);

                    if (sherlock->hp == 0) {
                        return ;
                    }

                    cur_robot_in_arr = cur_robot_in_arr->next; // next here
                    continue;
                }
            } else {
                if (sherlock->exp > 500) {
                    criminal_caught = true;
                    return ;
                } else {
                    sherlock_bag->insert(cur_robot->item);
                    cur_robot->item = NULL;
                    
                    ArrayMovingObjectItem * old_robot_in_arr = cur_robot_in_arr;
                    cur_robot_in_arr = cur_robot_in_arr->next;

                    arr_mv_objs->remove(old_robot_in_arr);
                    continue;
                }
            }

            //After the fight, find items to use
            while (true) {
                LinkedListItem * cur_sherlock_bag_linkedlist_item = sherlock_bag->linkedlist_items->head;
                while (cur_sherlock_bag_linkedlist_item != NULL &&
                    !((cur_sherlock_bag_linkedlist_item->item->item_type == ItemType::EXCEMPTION_CARD)
                    || (cur_sherlock_bag_linkedlist_item->item->item_type == ItemType::PASSING_CARD)) && 
                    cur_sherlock_bag_linkedlist_item->item->canUse(sherlock, NULL)

                ) {
                    cur_sherlock_bag_linkedlist_item = cur_sherlock_bag_linkedlist_item->next;
                }

                if (cur_sherlock_bag_linkedlist_item == NULL) break;
                
                // the get function removes the linkedlist_item themselves
                BaseItem * item = sherlock_bag->linkedlist_items->get(cur_sherlock_bag_linkedlist_item);
                item->use(sherlock, NULL);

                delete item;
            }
        }
    }

    // check if Watson meet any robot
    cur_robot_in_arr = arr_mv_objs->head;
    for (int i = 0; i < 3; i++) cur_robot_in_arr = cur_robot_in_arr->next;
    // the first 3 in array is sherlock, watson, criminal

    while (cur_robot_in_arr != NULL) {
        Robot * cur_robot = (Robot *)cur_robot_in_arr->obj;
        if (watson->getCurrentPosition().isEqual(cur_robot->getCurrentPosition())) {
            BaseItem * passing_card_to_use = watson_bag->get(ItemType::PASSING_CARD);
            bool can_use_passing_card = (passing_card_to_use != NULL && passing_card_to_use->canUse(watson, cur_robot));
            string passing_card_challenge = "";
            if (can_use_passing_card) passing_card_challenge = ((PassingCard *)passing_card_to_use)->challenge;
            if (passing_card_to_use != NULL) delete passing_card_to_use;

            if (cur_robot->robot_type == RobotType::S) {}
            else if (cur_robot->robot_type == RobotType::W) {
                if ((watson->hp > 350) || can_use_passing_card) {
                    watson_bag->insert(cur_robot->item);
                    cur_robot->item = NULL;

                    ArrayMovingObjectItem * old_robot_in_arr = cur_robot_in_arr;
                    cur_robot_in_arr = cur_robot_in_arr->next;

                    arr_mv_objs->remove(old_robot_in_arr);
                } else {
                    watson->hp = ceil(watson->hp * 0.95);

                    if (watson->hp == 0) {
                        return ;
                    }

                    cur_robot_in_arr = cur_robot_in_arr->next;
                }
            } else if (cur_robot->robot_type == RobotType::SW) {
                if (((watson->exp > 600) && (watson->hp > 165)) || can_use_passing_card) {
                    watson_bag->insert(cur_robot->item);
                    cur_robot->item = NULL;

                    ArrayMovingObjectItem * old_robot_in_arr = cur_robot_in_arr;
                    cur_robot_in_arr = cur_robot_in_arr->next;

                    arr_mv_objs->remove(old_robot_in_arr);
                } else {
                    watson->hp = ceil(watson->hp * 0.85);
                    watson->exp = ceil(watson->exp * 0.85);

                    if ((watson->hp == 0) || (watson->exp == 0)) {
                        return ;
                    }

                    cur_robot_in_arr = cur_robot_in_arr->next;
                }
            } else {
                watson_bag->insert(cur_robot->item);
                cur_robot->item = NULL;

                ArrayMovingObjectItem * old_robot_in_arr = cur_robot_in_arr;
                cur_robot_in_arr = cur_robot_in_arr->next;

                arr_mv_objs->remove(old_robot_in_arr);
            }

            while (true) {
                LinkedListItem * cur_watson_bag_linkedlist_item = watson_bag->linkedlist_items->head;
                while (cur_watson_bag_linkedlist_item != NULL &&
                    !((cur_watson_bag_linkedlist_item->item->item_type == ItemType::EXCEMPTION_CARD)
                    || (cur_watson_bag_linkedlist_item->item->item_type == ItemType::PASSING_CARD)) && 
                    cur_watson_bag_linkedlist_item->item->canUse(sherlock, NULL)

                ) {
                    cur_watson_bag_linkedlist_item = cur_watson_bag_linkedlist_item->next;
                }

                if (cur_watson_bag_linkedlist_item == NULL) break;
                
                // the get function removes the linkedlist_item themselves
                BaseItem * item = watson_bag->linkedlist_items->get(cur_watson_bag_linkedlist_item);
                item->use(sherlock, NULL);

                delete item;
            }
        }
    }
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////