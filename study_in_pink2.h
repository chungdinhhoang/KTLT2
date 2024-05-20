/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class Position;
class Configuration;
class Map;

class Character;
class Robot;

class Criminal;
/*
class RobotS;
class RobotW;
class RobotSW;
class RobotC;
*/

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;

class TestStudyInPink;

enum ItemType { MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD };
enum ElementType { PATH, WALL, FAKE_WALL };
enum RobotType { C=0, S, W, SW };

class MapElement {
protected:
    ElementType type;
public:
    MapElement(ElementType in_type);
    virtual ~MapElement();
    virtual ElementType getType() const;
};

class Path : public MapElement {
public:
    Path();
};

class Wall : public MapElement {
public:
    Wall();
};

class FakeWall : public MapElement {
private:
    int req_exp;
public:
    FakeWall(int in_req_exp);
    int getReqExp() const;
};

class Map {
private:
    int num_rows, num_cols;

    MapElement*** map;

public:
    Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls);
    int getNumRows() const;
    int getNumCols() const;
    ~Map();
    bool isValid ( const Position & pos , MovingObject * mv_obj ) const ;
};

class Position {
private:
    int r, c;
public:
    static const Position npos;

    Position(int r=0, int c=0);

    Position(const string & str_pos);

    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);

    string str() const;

    bool isEqual(int in_r, int in_c) const;
    bool isEqual(Position other) const;
};

class MovingObject {
public:
    int index;
    Position pos;
    Map * map;
    string name;

    MovingObject();
    MovingObject(int index, const Position pos, Map * map, const string & name);
    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const;
    virtual void move() = 0;
    virtual string str() const = 0;

    virtual string getName() const;
    virtual int getIndex() const;
};

class Character : public MovingObject {
public:
    int hp, exp;
};

class Sherlock: /* TODO */ public Character {
private:
    // TODO
    string moving_rule;
    int init_hp;
    int init_exp;
    int moving_index;

public:
    int hp, exp;

    Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp);
    Position getNextPosition();
    void move();
    string str() const;
    // ...
};

class Watson : public Character /* TODO */ {
private:
    // TODO
    string moving_rule;
    int init_hp;
    int init_exp;
    int moving_index;
    
public:
    int hp, exp;

    Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp);
    Position getNextPosition();
    void move();
    string str() const;
    // ...
};

class Criminal: public Character /* TODO */ {
private:
    // TODO
    Sherlock * sherlock;
    Watson * watson;
    
    Position lastPosition;
public:
    int moved_times;

    Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson);
    Position getNextPosition();
    Position getLastPosition();
    void move();
    string str() const;
    // ...
};

class ArrayMovingObjectItem {
public:
    MovingObject * obj;

    ArrayMovingObjectItem * next;
    ArrayMovingObjectItem * previous;

    ArrayMovingObjectItem(MovingObject * obj);
    ~ArrayMovingObjectItem();
};

class ArrayMovingObject {
private:
    // TODO
    int count;
    int capacity;

public:
    ArrayMovingObjectItem * head;
    ArrayMovingObject(int capacity);

    ~ArrayMovingObject() ;
    bool isFull() const;
    bool add(MovingObject * mv_obj);
    void remove(ArrayMovingObjectItem * item);
    //MovingObject * get(int index);
    int size() const; // return current number of elements in the array
    string str() const;
};

class Configuration {
    friend class StudyPinkProgram;

private:
    // TODO
    int map_num_rows, map_num_cols;

    int max_num_moving_objects;
    int num_walls, num_fake_walls;
    Position * arr_walls;
    Position * arr_fake_walls;

    string sherlock_moving_rule;
    Position sherlock_init_pos;
    int sherlock_init_hp, sherlock_init_exp;

    string watson_moving_rule;
    Position watson_init_pos;
    int watson_init_hp, watson_init_exp;

    Position criminal_init_pos;
    int num_steps;

    string prebuilt_str;
public:
    Configuration(const string & filepath);
    ~Configuration();
    string str() const;
};

// Robot, BaseItem, BaseBag,...

class Robot : public MovingObject {
public:
    RobotType robot_type;
    BaseItem * item;

    virtual void createItem();
    virtual int getIndex() const;
};

class RobotC : public Robot {
public:
    Criminal * criminal;

    RobotC(int index, const Position & init_pos, Map * map, Criminal * criminal) ;
    Position getNextPosition();
    void move();
    int getDistance(Character * character) const;
    string str() const;
};

class RobotS : public Robot {
public:
    Criminal * criminal;
    Sherlock * sherlock;

    RobotS(int index, const Position & init_pos, Map * map, Criminal * criminal, Sherlock * sherlock);
    Position getNextPosition();
    void move();    
    int getDistance() const;
    string str() const;
};

class RobotW : public Robot {
public:
    Criminal * criminal;
    Watson * watson;

    RobotW(int index, const Position & init_pos, Map * map, Criminal * criminal, Watson * watson);
    Position getNextPosition();
    void move();    
    int getDistance() const;
    string str() const;
};

class RobotSW : public Robot {
public:
    Criminal * criminal;
    Sherlock * sherlock;
    Watson * watson;

    RobotSW(int index, const Position & init_pos, Map * map, Criminal * criminal, Sherlock * sherlock, Watson * watson);
    Position getNextPosition();
    void move();    
    int getDistance() const;
    string str() const;
};

class BaseItem {
public:
    ItemType item_type;
    string name;

    string getName() const;

    virtual bool canUse(Character * obj, Robot * robot) = 0;
    virtual void use(Character * obj, Robot * robot) = 0;
};

/* To my understanding, in the first 3 items, robot parameters are useless
*/

class MagicBook : public BaseItem {
public:
    MagicBook();

    bool canUse(Character * obj, Robot * robot);
    void use(Character * obj, Robot * robot);
};

class EnergyDrink : public BaseItem {
public:
    EnergyDrink();

    bool canUse(Character * obj, Robot * robot);
    void use(Character * obj, Robot * robot);
};

class FirstAid : public BaseItem {
public:
    FirstAid();

    bool canUse(Character * obj, Robot * robot);
    void use(Character * obj, Robot * robot);
};

class ExcemptionCard : public BaseItem {
public:
    ExcemptionCard();

    bool canUse(Character * obj, Robot * robot);
    void use(Character * obj, Robot * robot);
};

class PassingCard : public BaseItem {
public:
    string challenge;

    PassingCard(string challenge);

    bool canUse(Character * obj, Robot * robot);
    void use(Character * obj, Robot * robot);
};

class LinkedListItem {
public:
    BaseItem * item;
    LinkedListItem * next;

    LinkedListItem(BaseItem * item);
    ~LinkedListItem();
};

class LinkedList {
public:
    int count;
    int capacity;
    LinkedListItem * head;

    LinkedList(int capacity);
    ~LinkedList();
    //void swap(LinkedListItem * a);
    bool insert(BaseItem * item);
    BaseItem * get(LinkedListItem * linkedlist_item);
};

class BaseBag {
public:
    Character * obj;
    LinkedList * linkedlist_items;  

    virtual bool insert (BaseItem * item);
    virtual BaseItem* get();
    virtual BaseItem* get(ItemType itemType);
    virtual string str() const;
};


class SherlockBag : public BaseBag {
public:
    SherlockBag(Sherlock * sherlock);
    //BaseItem* get();
};

class WatsonBag : public BaseBag {
public:
    WatsonBag(Watson * watson);
};

class StudyPinkProgram {
private:
    // Sample attributes
    Configuration * config;
    
    Sherlock * sherlock;
    Watson * watson;
    Criminal * criminal;
    
    Map * map;
    ArrayMovingObject * arr_mv_objs;

    bool criminal_caught;
    SherlockBag * sherlock_bag;
    WatsonBag * watson_bag;


public:
    StudyPinkProgram(const string & config_file_path);

    bool isStop() const;
    void checkMeeting();
    void checkCriminalCreateRobot();

    void printResult() const {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
            cout << "Sherlock caught the criminal" << endl;
        }
        else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
            cout << "Watson caught the criminal" << endl;
        }
        else {
            cout << "The criminal escaped" << endl;
        }
    }

    void printStep(int si) const {
        cout << "Step: " << setw(4) << setfill('0') << si
            << "--"
            << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }

    

    void run(bool verbose) {
        // Note: This is a sample code. You can change the implementation as you like.
        // TODO
        /*
        In each step, move all the MovingObject before check Meeting
        */
        //cout << config->num_steps << endl;
        for (int istep = 0; istep < config->num_steps; ++istep) {
            //cout << istep << endl;
            ArrayMovingObjectItem * cur_mv_obj = arr_mv_objs->head;
            while (cur_mv_obj != NULL) {
                MovingObject * cur_obj = cur_mv_obj->obj;
                cur_obj->move();
                //cout << cur_obj->getName() << endl;
                //if (cur_obj->getName() == "Criminal") checkCriminalCreateRobot();
                cur_mv_obj = cur_mv_obj->next;
            }

            //cout << istep << endl;
            checkCriminalCreateRobot();
            checkMeeting();

            if (isStop()) {
                printStep(istep);
                break;
            }

            if (verbose) {
                printStep(istep);
            }
        }
        printResult();
    }

    ~StudyPinkProgram();
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
