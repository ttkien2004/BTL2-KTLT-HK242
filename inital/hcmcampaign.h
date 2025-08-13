/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2025
 * Date: 02.02.2025
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_HCM_CAMPAIGN_H_
#define _H_HCM_CAMPAIGN_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class Unit;
class UnitList;
class Army;
class TerrainElement;

class Vehicle;
class Infantry;

class LiberationArmy;
class ARVN;

class Position;

class Road;
class Mountain;
class River;
class Urban;
class Fortification;
class SpecialZone;

class BattleField;

class HCMCampaign;
class Configuration;

enum VehicleType
{
    TRUCK, // 0
    MORTAR, // 1
    ANTIAIRCRAFT, // 2
    ARMOREDCAR, // 3
    APC, // 4
    ARTILLERY, //5
    TANK // 6
};
enum InfantryType
{
    SNIPER, // 0
    ANTIAIRCRAFTSQUAD, // 1
    MORTARSQUAD, // 2
    ENGINEER, // 3
    SPECIALFORCES, // 4
    REGULARINFANTRY // 5
};

enum UnitType 
{
    VEHICLE,
    INFANTRY
};
enum TerrainType
{
    ROAD,
    MOUNTAIN,
    RIVER,
    FORTIFICATION,
    URBAN,
    SPECIAL_ZONE
};
class unitListNode {
public:
    Unit *unit;
    unitListNode *prev;
    unitListNode *next;
    unitListNode(Unit *unit, unitListNode *prev, unitListNode *next) {
        this->unit = unit;
        this->prev = prev;
        this->next = next;
    }
    ~unitListNode() {}
};

class Army
{
protected:
    // LF [0, 1000], EXP [0, 500]
    int LF, EXP; 
    string name;
    UnitList *unitList;
    BattleField *battleField;
    int unit_array_size;
    Unit **unitArray;
public:
    Army() {}
    Army(Unit **unitArray, int size, string name, BattleField *battleField);
    virtual void fight(Army *enemy, bool defense = false) = 0;
    virtual string str() const = 0;
    int getLF() {
        return LF;
    }
    int getEXP() {
        return EXP;
    }
    void setLF(int LF) {
        this->LF = max(0, min(LF, 1000));
    }
    void setEXP(int EXP) {
        this->EXP = max(0, min(EXP, 500));
    }
    string getName() {
        return name;
    }
    int getUnitArraySize() {
        return unit_array_size;
    }
    Unit **getUnitArray() {
        return unitArray;
    }
    UnitList *getUnitList() {
        return unitList;
    }
    vector<Unit *> findOptimal(vector<Unit *> units, int targetScore);
    void backtrack(int idx, int curScore, int target, int &minExcess, vector<Unit *> units, vector<Unit *> curCombo, vector<Unit *> &res);
    int nearestFibonacci(int num) const;
    void printUnitList();
    BattleField *getBattleField() {
        return battleField;
    }
    void updateEXP();
    void updateLF();
    
    
    int calculateLF(unitListNode *head);
    int calculateEXP(unitListNode *head);
    bool isPrimeNumber(int num);
    bool isSpecialNumber(int num, int k);
    ~Army();
};

class Position
{
private:
    int r, c;

public:
    Position(int r = 0, int c = 0) {
        this->r = r;
        this->c = c;
    }
    Position(const string &str_pos);            
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const; // Example: returns "(1,15)"
    ~Position() {}
};

class Unit
{
protected:
    int quantity, weight;
    Position pos;
    UnitType unitType;
    int attackScore;
public:
    Unit() {}
    Unit(int quantity, int weight, Position pos);
    virtual ~Unit();
    virtual int getAttackScore() = 0;
    Position getCurrentPosition() const;
    virtual string str() const = 0;
    UnitType getUnitType() {
        return this->unitType;
    };
    void setQuantity(int quantity) {
        this->quantity = quantity;
    }
    void setWeight(int weight) {
        this->weight = weight;
    }
    int getQuantity() {
        return this->quantity;
    }
    int getWeight() {
        return this->weight;
    }
    void setAttackScore(int attackScore) {
        this->attackScore = attackScore;
    }
    int getCurAttackScore() {
        return this->attackScore;
    }
    bool checkSquareNumber(int weight);
    int calculateIndividualNum(int num);
    string convertVehicleType(VehicleType type) const;
    string convertInfantryType(InfantryType type) const;
};
struct UnitContainer {
    UnitContainer() {}
    UnitContainer(Unit *unit) {
        this->unit = unit;
    }
    Unit *unit;
    UnitContainer *nextUnit;
};

class UnitList
{
private:
    int capacity;
    // TODO
    UnitContainer *unitContainer;
    int curNumUnits;
    int infantryUnits;
    int vehicleUnits;
public:
    UnitList(int S);
    bool insert(Unit *unit);                   // return true if insert successfully
    bool isContain(VehicleType vehicleType);   // return true if it exists
    bool isContain(InfantryType infantryType); // return true if it exists
    string str() const;
    // TODO
    unitListNode *unitListHead;
    // UnitList *prev;
    // UnitList *next;
    // Unit *unit;    

    // UnitList *createUnitList(Unit **unitArray, int size);
    void setUnitContainer(UnitContainer *container) {
        this->unitContainer = container;
    }
    int getNumUnits() {
        return this->curNumUnits;
    }
    void setNumUnits(int res) {
        this->curNumUnits = res;
    }
    void deleteUnitList(Unit *);
    
    ~UnitList();
};

class TerrainElement
{
private:
    TerrainType terrainType;
    int x, y;
public:
    TerrainElement() {};
    TerrainElement(Position pos) {
        this->x = pos.getRow();
        this->y = pos.getCol();
    }
    TerrainElement(TerrainType terrainType, int x, int y) {
        this->terrainType = terrainType;
        this->x = x;
        this->y = y;
    }
    
    virtual void getEffect(Army *army) = 0;
    TerrainType getTerrainType() {
        return this->terrainType;
    }
    string str() const {
        if (terrainType == ROAD) {
            return "ROAD";
        }else if (terrainType == MOUNTAIN) {
            return "MOUNTAIN";
        }else if (terrainType == RIVER) {
            return "RIVER";
        }else if (terrainType == URBAN) {
            return "URBAN";
        }else if (terrainType == SPECIAL_ZONE) {
            return "SPECIAL_ZONE";
        }
        return "FORTIFICATION";
    }
    int getPosX() {
        return this->x;
    }
    int getPosY() {
        return this->y;
    }
    void calculateNewEXP(TerrainType t, int p_x, int p_y, int unit_x, int unit_y, string army_name, UnitType unitType, Unit *unit, int &new_exp, int &new_lf);
    ~TerrainElement() {};
};

class BattleField
{
private:
    int n_rows, n_cols;
    // TODO
    vector<vector<TerrainElement*>> terrain; // Mỗi ptu là TerrainElement
public:
    BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                vector<Position *> arrayUrban, vector<Position *> arraySpecialZone);
    ~BattleField();
    string str() const;
    void printBattleField();
    int getNRows() {
        return n_rows;
    }
    int getNCols() {
        return n_cols;
    }
    vector<vector<TerrainElement*>> getTerrain() {
        return terrain;
    }
    TerrainElement *getElement(int p_x, int p_y);
};

class HCMCampaign
{
private:
    Configuration *config;
    BattleField *battleField;
    LiberationArmy *liberationArmy;
    ARVN *arvn;

public:
    HCMCampaign() {}
    HCMCampaign(const string &config_file_path);
    void run();
    string printResult();
    ~HCMCampaign();
};

// Các class kế thừa từ Unit
class Vehicle: public Unit {
private:
    VehicleType vehicleType;
public:
    Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType) {
        this->quantity = quantity;
        this->weight = weight;
        this->pos = pos;
        this->vehicleType = vehicleType;
        this->unitType = VEHICLE;
    }
    int getAttackScore();
    string str() const;
    VehicleType getVehicleType() {
        return this->vehicleType;
    }
    ~Vehicle() {}
};

class Infantry: public Unit {
private:
    InfantryType infantryType;
public:
    Infantry(int quantity, int weight, const Position pos, InfantryType infantryType) {
        this->quantity = quantity;
        this->weight = weight;
        this->pos = pos;
        this->infantryType = infantryType;
        this->unitType = INFANTRY;
    }
    int getAttackScore();
    string str() const;
    InfantryType getInfantryType() {
        return this->infantryType;
    }
    ~Infantry() {}
};

// Các class kế thừa từ Army
class LiberationArmy: public Army {
public:
    LiberationArmy(Unit **unitArray, int size, string name, BattleField *battleField);
    void fight(Army *enemy, bool defense = false);
    string str() const;
    ~LiberationArmy();
};

class ARVN: public Army {
public:
    ARVN(Unit **unitArray, int size, string name, BattleField *battleField);
    void fight(Army *enemy, bool defense = false);
    string str() const;
    ~ARVN();
};

// Class Configuration

class Configuration {
private:
    int num_rows, num_cols;
    vector<Position *> arrayForest, arrayRiver, arrayFortification, arrayUrban, arraySpecialZone;
    Unit **liberationUnits;
    Unit **ARVNUnits;
    // event code: [00, 99]
    int eventCode;
    // arvnUnit and liberation size
    int liberationUnits_size;
    int arvnUnits_size;
public:
    Configuration() {}
    Configuration(const string &filepath);
    ~Configuration();
    int getArvnUnitsSize() {
        return this->arvnUnits_size;
    }
    int getLiberationUnitsSize() {
        return this->liberationUnits_size;
    }
    int getEventCode() {
        return eventCode;
    }
    int getNumRows() {
        return this->num_rows;
    }
    int getNumCols() {
        return this->num_cols;
    }
    string getLiberUnitStr() const;
    string getARVNUnitStr() const;
    string getArrayForestStr() const;
    string getArrayRiverStr() const;
    string getArrayFortiStr() const;
    string getArrayUrbanStr() const;
    string getArraySpecialZoneStr() const;

    Unit **getLiberationUnits() {
        return liberationUnits;
    }
    Unit **getARVNUnits() {
        return ARVNUnits;
    }
    vector<Position *> getArrayForest() {
        return arrayForest;
    }
    vector<Position *> getArrayRiver() {
        return arrayRiver;
    }
    vector<Position *> getArrayFori() {
        return arrayFortification;
    }
    vector<Position *> getArrayUrban() {
        return arrayUrban;
    }
    vector<Position *> getArraySpecial() {
        return arraySpecialZone;
    }
    string str() const;    
    string trim(const string &str);
};
// Các class kế thừa từ TerrainElement
class Road: public TerrainElement {
public:
    Road(Position pos): TerrainElement(pos) {}
    Road(TerrainType t, int x, int y): TerrainElement(t, x, y) {}
    void getEffect(Army *army) {
        // Không gây ảnh hưởng cho ai cả

        return;
    }
};

// class Forest: public TerrainElement {
// private:
//     vector<Position *> arrayForest;
// public:
//     Forest(vector<Position *> a) {
//         this->arrayForest = a;
//     }
//     Forest(TerrainType t, int x, int y);
//     void getEffect(Army *army);
// };
class River: public TerrainElement {
private:
    vector<Position *> arrayRiver;
public:
    River(Position pos);
    River(TerrainType t, int x, int y);
    River(vector<Position *> array) {
        this->arrayRiver = array;
    }
    void getEffect(Army *army);
};

class Fortification: public TerrainElement {
private:
    vector<Position *> arrayFortification;
public:
    Fortification(Position pos);
    Fortification(TerrainType t, int x, int y);
    Fortification(vector<Position *> array) {
        this->arrayFortification = array;
    }
    void getEffect(Army *army);
};

class SpecialZone: public TerrainElement {
private:
    vector<Position *> arraySpecialZone;
public:
    SpecialZone(Position pos) ;
    SpecialZone(TerrainType t, int x, int y);
    SpecialZone(vector<Position *> arraySpecialZone) {
        this->arraySpecialZone = arraySpecialZone;
    }
    void getEffect(Army *army);
};

class Urban: public TerrainElement {
public:
    Urban(Position pos);
    Urban(TerrainType t, int x, int y);
    void getEffect(Army *army);
};
class Mountain: public TerrainElement {
public:
    Mountain(Position pos);
    Mountain(TerrainType t, int x, int y);
    void getEffect(Army *army);
};
#endif