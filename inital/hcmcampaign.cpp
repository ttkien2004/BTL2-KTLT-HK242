#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////

// Các hàm thêm
string Unit::convertVehicleType(VehicleType type) const {
    if (type == TANK) {
        return "TANK";
    }else if (type == ARTILLERY) {
        return "ARTILLERY";
    }else if (type == ARMOREDCAR) {
        return "ARMOREDCAR";
    }else if (type == APC) {
        return "APC";
    }else if (type == TRUCK) {
        return "TRUCK";
    }else if (type == MORTAR) {
        return "MORTAR";
    }
    return "ANTIAIRCRAFT";
}
string Unit::convertInfantryType(InfantryType type) const {
    if (type == SNIPER) {
        return "SNIPER";
    }else if (type == ANTIAIRCRAFTSQUAD) {
        return "ANTIAIRCRAFTSQUAD";
    }else if (type == MORTARSQUAD) {
        return "MORTARSQUAD";
    }else if (type == ENGINEER) {
        return "ENGINEER";
    }else if (type == SPECIALFORCES) {
        return "SPECIALFORCES";
    }
    return "REGULARINFANTRY";
}
bool Unit::checkSquareNumber(int weight) {
    if (weight == 1) return true;
    int num = sqrt(weight);
    if (num * num == weight) {
        return true;
    }
    return false;
}


int Unit::calculateIndividualNum(int num) {
    int sum_digit = 0;
    while (num != 0) {
        sum_digit += (num % 10);
        num /= 10;
    }
    return sum_digit;
}
void Army::backtrack(int idx, int curScore, int target, int &minExcess, vector<Unit *> units, vector<Unit *> curCombo, vector<Unit *> &res) {    
    if (curScore > target) {
        int excess = curScore - target;
        if (excess < minExcess) {
            minExcess = excess;
            res = curCombo;
        }
        return;
    }
    if (idx >= units.size()) {
        return;
    }
    curCombo.push_back(units[idx]);
    backtrack(idx + 1, curScore + units[idx]->getCurAttackScore(), target, minExcess, units, curCombo, res);
    curCombo.pop_back();
    backtrack(idx + 1, curScore, target, minExcess, units, curCombo, res);
}

bool Army::isSpecialNumber(int num, int k) {
    for (int power = (int)(log(num) / log(k)); power >= 0; --power) {
        int value = pow(k, power);
        if (value <= num) {
            num -= value;
        }
    }
    return num == 0;
}

bool Army::isPrimeNumber(int num) {
    if (num <= 1) return false;
    if (num == 2) return true;
    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0) return false;
    }
    return true;
}
float calculateRadius(int unit_x, int terrain_x, int unit_y, int terrain_y) {
    int sum1 = (unit_x - terrain_x) * (unit_x - terrain_x);
    int sum2 = (unit_y - terrain_y) * (unit_y - terrain_y) ;
    return sqrt(sum1 + sum2);
}
int Army::calculateLF(unitListNode *head) {
    if (!head) return -1;
    unitListNode *temp = head->next;
    int new_lf = 0;
    while(temp) {
        new_lf += temp->unit->getAttackScore();
        temp = temp->next;
    }
    return new_lf;
}
int Army::calculateEXP(unitListNode *head) {
    if(!head) return -1;
    unitListNode *temp = head->prev;    
    int new_exp = 0;
    while(temp) {
        new_exp += temp->unit->getAttackScore();
        temp = temp->prev;
    }    
    return new_exp;
}
string Configuration::trim(const string &str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

// Các hàm của class Vehicle và Infantry kế thừa từ Unit
Unit::~Unit() {}
string Vehicle::str() const {
    string vehicleStr = "vehicleType=" + convertVehicleType(vehicleType) + ",";
    string quantityStr = "quantity=" + to_string(quantity) + ",";
    string weightStr = "weight=" + to_string(weight) + ',';
    string posStr = "position=" + pos.str();
    string res = "Vehicle[" + vehicleStr + quantityStr + weightStr + posStr + "]";    
    return res;
}
string Infantry::str() const {
    string infantryStr = "infantryType=" + convertInfantryType(infantryType) + ",";
    string quantityStr = "quantity=" + to_string(quantity) + ",";
    string weightStr = "weight=" + to_string(weight) + ",";
    string posStr = "position=" + pos.str();
    string res = "Infantry[" + infantryStr + quantityStr + weightStr + posStr + "]";
    return res;
}
Position Unit::getCurrentPosition() const {
    return pos;
}



int Vehicle::getAttackScore()  {
    int score = ceil(float(vehicleType * 304 + quantity * weight) / 30);
    this->attackScore = score;
    return score;
}
int Infantry::getAttackScore() {        
    int score = infantryType * 56 + quantity * weight;    
    int prev_score = score;
    if (infantryType == SPECIALFORCES && checkSquareNumber(weight)) { 
        score += 75;
        // score += this->get
        // this->attackScore = score;
        // return score;
        
    }        
    int sum_digit_score = calculateIndividualNum(score), sum_digit_y = calculateIndividualNum(1975);
    
    // cout << sum_digit_score << " " << sum_digit_y << endl;
    int individualNum = sum_digit_score + sum_digit_y;       
    // cout << individualNum << endl; 
    while (individualNum >= 10) {
        individualNum = calculateIndividualNum(individualNum);
    }        
    // cout << "INdi " << individualNum << endl;
    if (individualNum > 7) {
        // CHi viện thêm 20%
        float new_quantity = 1.2 * (float)quantity;
        quantity = ceil(new_quantity);
        score = infantryType * 56 + quantity * weight;        
    }else if (individualNum < 3) {
        // Giảm 10%            
        float new_quantity = 0.9 * (float)quantity;
        quantity = ceil(new_quantity);
        score = infantryType * 56 + quantity * weight;
    } else {
        score = prev_score;
    }    
    this->attackScore = score;
    return score;
    // Cập nhật lại score
    // score = infantryType * 56 + quantity * weight;
    // this->attackScore = score;
    // return score;
}

// Các hàm getEffect của TerrainElement
// Forest::Forest(TerrainType t, int x, int y): TerrainElement(t, x, y) {}
River::River(TerrainType t, int x, int y): TerrainElement(t, x, y) {}
Fortification::Fortification(TerrainType t, int x, int y): TerrainElement(t, x, y) {}
SpecialZone::SpecialZone(TerrainType t, int x, int y): TerrainElement(t, x, y) {}
Urban::Urban(TerrainType t, int x, int y): TerrainElement(t, x, y) {}
Mountain::Mountain(TerrainType t, int x, int y): TerrainElement(t, x, y) {}

Mountain::Mountain(Position p): TerrainElement(p) {}
River::River(Position p): TerrainElement(p) {}
Urban::Urban(Position p): TerrainElement(p) {}
SpecialZone::SpecialZone(Position p ): TerrainElement(p) {}
Fortification::Fortification(Position p): TerrainElement(p) {}

void TerrainElement::calculateNewEXP(TerrainType t, int p_x, int p_y, int unit_x, int unit_y, string army_name, UnitType unitType, Unit *unit, int &new_exp, int &new_lf) {
    int radius = ceil(calculateRadius(unit_x, p_x, unit_y, p_y));    
    if (t == MOUNTAIN) {
        if (army_name == "Liberation") {
            if (radius >= 0 && radius <= 2) {
                // Thêm 30% mỗi attackScore cho exp
                // Giảm 10% mỡi attackScore cho lf                        
                if (unitType == INFANTRY) {                                                              
                    new_exp = ceil(new_exp + ceil(0.3 * unit->getCurAttackScore()));
                }else if (unitType == VEHICLE) {                          
                    new_lf = ceil(new_lf - ceil(0.1 * unit->getCurAttackScore()));                    
                }                        
            }
        }else {
            // Thêm 20% mỗi attackScore cho exp
            // Giảm 5% mỡi attackScore cho lf
            if (radius >= 0 && radius <= 4) {                
                if (unitType == INFANTRY) {
                    new_exp = ceil(new_exp + ceil(0.2 * unit->getCurAttackScore()));   
                }else if (unitType == VEHICLE) {                               
                    new_lf = ceil(new_lf - ceil(0.05 * unit->getCurAttackScore()));                                        
                }                                                                 
            }
        }
    }else if (t == RIVER) {
        if (army_name == "Liberation") {
            if (radius >= 0 && radius <= 2) {                
                // attackScore bị giảm 10%
                if (unitType == INFANTRY) {                    
                    unit->setAttackScore(ceil(0.9 * unit->getCurAttackScore()));                
                }                
            }
        }else {
            // attackScore bị giảm 10%
            if (radius >= 0 && radius <= 2) {
                if (unitType == INFANTRY) {
                    unit->setAttackScore(ceil(0.9 * unit->getCurAttackScore()));                
                }                
            }
        }
    }else if (t == URBAN) {
        if (army_name == "Liberation") {
            if (unitType == INFANTRY) {
                string infantry = unit->convertInfantryType(((Infantry *)unit)->getInfantryType());
                if (infantry == "SPECIALFORCES" || infantry == "REGULARINFANTRY") {                                        
                    if (radius > 0 && radius <= 5) {
                        float new_attack_score = unit->getCurAttackScore() + ceil((2/ (float)radius) * unit->getCurAttackScore());
                        unit->setAttackScore(ceil(new_attack_score));                                
                    }
                }
            }else {
                string vehicle = unit->convertVehicleType(((Vehicle *)unit)->getVehicleType());
                if (vehicle == "ARTILLERY") {                    
                    if (radius > 0 && radius <= 2) {
                        float new_attack_score = 0.5 * unit->getCurAttackScore();
                        unit->setAttackScore(ceil(new_attack_score));                                
                    }
                }
            }
        }else {
            // ARVN
            if (unitType == INFANTRY) {                
                InfantryType infantry = ((Infantry *)unit)->getInfantryType();
                if (infantry == REGULARINFANTRY) {                                    
                    if (radius > 0 && radius <= 3) {
                        float new_attack_score = unit->getCurAttackScore() + ceil((1.5 / (float)radius) * unit->getCurAttackScore());
                        // float new_attack_score = unit->getCurAttackScore() + ceil((3 * unit->getCurAttackScore()) / (float)(2 * radius));                        
                        unit->setAttackScore(ceil(new_attack_score));                                
                    }
                }
            }
        }
    }else if (t == FORTIFICATION) {
        if (army_name == "Liberation") {
            if (radius >= 0 && radius <= 2) {
                unit->setAttackScore(ceil(0.8 * unit->getCurAttackScore()));                        
            }
        }else {
            if (radius >= 0 && radius <= 2) {
                unit->setAttackScore(ceil(1.2 * unit->getCurAttackScore()));                        
            }
        }
    }else if (t == SPECIAL_ZONE) {
        if (radius >= 0 && radius <= 1) {
            // attackScore -> 0            
            unit->setAttackScore(0);            
        }
    }
    
}


void Mountain::getEffect(Army *army) {    
    if (army == NULL) return;
    BattleField *battleField = army->getBattleField();
    UnitList *unitList = army->getUnitList();
    if (battleField && unitList) {        
        vector<vector<TerrainElement*>> terrain = battleField->getTerrain();
        unitListNode *tmp1 = unitList->unitListHead, *tmp2 = unitList->unitListHead;
        // int n_cols = battleField->getNCols();
        // int n_rows = battleField->getNRows();
        // vector<Position> forest_pos;
        // for (int i = 0; i < n_rows; ++i) {
        //     for (int j = 0; j < n_cols; ++j) {
        //         if (terrain[i][j]->getTerrainType() == MOUNTAIN) {
        //             Position pos(i, j);
        //             forest_pos.push_back(pos);
        //         }
        //     }
        // }
        int new_exp = army->getEXP();
        int new_lf = army->getLF();
        int p_x = this->getPosX();
        int p_y = this->getPosY();
        while(tmp1) {
            if (tmp1->unit) {
                int unit_x = tmp1->unit->getCurrentPosition().getRow();
                int unit_y = tmp1->unit->getCurrentPosition().getCol();
                calculateNewEXP(MOUNTAIN, p_x, p_y, unit_x, unit_y, army->getName(), tmp1->unit->getUnitType(), tmp1->unit, new_exp, new_lf);                
            }
            tmp1 = tmp1->prev;
        }
        while(tmp2) {
            if (tmp2->unit) {
                int unit_x = tmp2->unit->getCurrentPosition().getRow();
                int unit_y = tmp2->unit->getCurrentPosition().getCol();
                calculateNewEXP(MOUNTAIN, p_x, p_y, unit_x, unit_y, army->getName(), tmp2->unit->getUnitType(), tmp2->unit, new_exp, new_lf);                
            }
            tmp2 = tmp2->next;
        }
        // for (auto p: forest_pos) {
        //     int p_x = p.getRow();
        //     int p_y = p.getCol();
        //     while(tmp1) {
        //         if (tmp1->unit) {
        //             int unit_x = tmp1->unit->getCurrentPosition().getRow();
        //             int unit_y = tmp1->unit->getCurrentPosition().getCol();
        //             calculateNewEXP(MOUNTAIN, p_x, p_y, unit_x, unit_y, army->getName(), tmp1->unit->getUnitType(), tmp1->unit, new_exp, new_lf);                
        //         }
        //         tmp1 = tmp1->prev;
        //     }
        //     while(tmp2) {
        //         if (tmp2->unit) {
        //             int unit_x = tmp2->unit->getCurrentPosition().getRow();
        //             int unit_y = tmp2->unit->getCurrentPosition().getCol();
        //             calculateNewEXP(MOUNTAIN, p_x, p_y, unit_x, unit_y, army->getName(), tmp2->unit->getUnitType(), tmp2->unit, new_exp, new_lf);                
        //         }
        //         tmp2 = tmp2->next;
        //     }
        // }
                
        army->setEXP(new_exp);
        army->setLF(new_lf);             
    }else if(unitList) {        
        unitListNode *tmp1 = unitList->unitListHead, *tmp2 = unitList->unitListHead;
        int new_exp = army->getEXP();
        int new_lf = army->getLF();
        int p_x = this->getPosX();
        int p_y = this->getPosY();
        
        while(tmp1) {
            if (tmp1->unit) {
                int unit_x = tmp1->unit->getCurrentPosition().getRow();
                int unit_y = tmp1->unit->getCurrentPosition().getCol();
                calculateNewEXP(MOUNTAIN, p_x, p_y, unit_x, unit_y, army->getName(), tmp1->unit->getUnitType(), tmp1->unit, new_exp, new_lf);                
            }
            tmp1 = tmp1->prev;
        }
        while(tmp2) {
            if (tmp2->unit) {
                int unit_x = tmp2->unit->getCurrentPosition().getRow();
                int unit_y = tmp2->unit->getCurrentPosition().getCol();
                calculateNewEXP(MOUNTAIN, p_x, p_y, unit_x, unit_y, army->getName(), tmp2->unit->getUnitType(), tmp2->unit, new_exp, new_lf);                
            }
            tmp2 = tmp2->next;
        }
        army->setEXP(new_exp);
        army->setLF(new_lf);
    }
}
void River::getEffect(Army *army) {
    if (army == NULL) return;
    BattleField *battleField = army->getBattleField();
    UnitList *unitList = army->getUnitList();
    
    
    if (battleField && unitList) {        
        vector<vector<TerrainElement*>> terrain = battleField->getTerrain();
        unitListNode *tmp1 = unitList->unitListHead, *tmp2 = unitList->unitListHead;
        int n_cols = battleField->getNCols();
        int n_rows = battleField->getNRows();
        vector<Position> river_pos;
        for (int i = 0; i < n_rows; ++i) {
            for (int j = 0; j < n_cols; ++j) {
                if (terrain[i][j]->getTerrainType() == RIVER) {
                    Position pos(i, j);
                    river_pos.push_back(pos);
                }
            }
        }
        int new_exp = 0, new_lf = 0;
        int p_x = this->getPosX();
        int p_y = this->getPosY();
        while(tmp1) {
            if (tmp1->unit) {
                    int unit_x = tmp1->unit->getCurrentPosition().getRow();
                    int unit_y = tmp1->unit->getCurrentPosition().getCol();                    
                    calculateNewEXP(RIVER, p_x, p_y, unit_x, unit_y, army->getName(), tmp1->unit->getUnitType(), tmp1->unit, new_exp, new_lf);
                }
            tmp1 = tmp1->prev;
        }
        while(tmp2) {
            if (tmp2->unit) {
                int unit_x = tmp2->unit->getCurrentPosition().getRow();
                int unit_y = tmp2->unit->getCurrentPosition().getCol();
                
                calculateNewEXP(RIVER, p_x, p_y, unit_x, unit_y, army->getName(), tmp2->unit->getUnitType(), tmp2->unit, new_exp, new_lf);
            }
            tmp2 = tmp2->next;
        }  
        // for (auto p: river_pos) {
        //     int p_x = p.getRow();
        //     int p_y = p.getCol();
        //     while(tmp1) {
        //     if (tmp1->unit) {
        //             int unit_x = tmp1->unit->getCurrentPosition().getRow();
        //             int unit_y = tmp1->unit->getCurrentPosition().getCol();                    
        //             calculateNewEXP(RIVER, p_x, p_y, unit_x, unit_y, army->getName(), tmp1->unit->getUnitType(), tmp1->unit, new_exp, new_lf);
        //         }
        //     tmp1 = tmp1->prev;
        //     }
        //     while(tmp2) {
        //         if (tmp2->unit) {
        //             int unit_x = tmp2->unit->getCurrentPosition().getRow();
        //             int unit_y = tmp2->unit->getCurrentPosition().getCol();
                    
        //             calculateNewEXP(RIVER, p_x, p_y, unit_x, unit_y, army->getName(), tmp2->unit->getUnitType(), tmp2->unit, new_exp, new_lf);
        //         }
        //         tmp2 = tmp2->next;
        //     }
        // }        
    }else if(unitList) {                
        unitListNode *tmp1 = unitList->unitListHead, *tmp2 = unitList->unitListHead;
        int p_x = this->getPosX();
        int p_y = this->getPosY();
        int new_exp = 0, new_lf = 0;
        while(tmp1) {
            if (tmp1->unit) {
                    int unit_x = tmp1->unit->getCurrentPosition().getRow();
                    int unit_y = tmp1->unit->getCurrentPosition().getCol();                    
                    calculateNewEXP(RIVER, p_x, p_y, unit_x, unit_y, army->getName(), tmp1->unit->getUnitType(), tmp1->unit, new_exp, new_lf);
                }
            tmp1 = tmp1->prev;
        }
        while(tmp2) {
            if (tmp2->unit) {
                int unit_x = tmp2->unit->getCurrentPosition().getRow();
                int unit_y = tmp2->unit->getCurrentPosition().getCol();
                
                calculateNewEXP(RIVER, p_x, p_y, unit_x, unit_y, army->getName(), tmp2->unit->getUnitType(), tmp2->unit, new_exp, new_lf);
            }
            tmp2 = tmp2->next;
        }
        // army->updateEXP();
        // army->updateLF()        ;
    }
}

void Urban::getEffect(Army *army) {
    if (army == NULL) return;
    BattleField *battleField = army->getBattleField();
    UnitList *unitList = army->getUnitList();
    if (battleField && unitList) {
        vector<vector<TerrainElement*>> terrain = battleField->getTerrain();
        unitListNode *tmp1 = unitList->unitListHead, *tmp2 = unitList->unitListHead;
        int n_cols = battleField->getNCols();
        int n_rows = battleField->getNRows();
        vector<Position> urban_pos;
        for (int i = 0; i < n_rows; ++i) {
            for (int j = 0; j < n_cols; ++j) {
                if (terrain[i][j]->getTerrainType() == FORTIFICATION) {
                    Position pos(i, j);
                    urban_pos.push_back(pos);
                }
            }
        }
        int new_exp = 0, new_lf = 0;
        int p_x = this->getPosX();
        int p_y = this->getPosY();
        while(tmp1) {
            if (tmp1->unit) {
                int unit_x = tmp1->unit->getCurrentPosition().getRow();
                int unit_y = tmp1->unit->getCurrentPosition().getCol();
                calculateNewEXP(URBAN, p_x, p_y, unit_x, unit_y, army->getName(), tmp1->unit->getUnitType(), 
                tmp1->unit, new_exp, new_lf);
            }
            tmp1 = tmp1->prev;
        }
        while(tmp2) {
            if (tmp2->unit) {
                int unit_x = tmp2->unit->getCurrentPosition().getRow();
                int unit_y = tmp2->unit->getCurrentPosition().getCol();
                calculateNewEXP(URBAN, p_x, p_y, unit_x, unit_y, army->getName(), tmp2->unit->getUnitType(), 
                tmp2->unit, new_exp, new_lf);
            }
            tmp2 = tmp2->next;
        }   
        // for(auto p: urban_pos) {
        //     int p_x = p.getRow();
        //     int p_y = p.getCol();
        //     while(tmp1) {
        //         if (tmp1->unit) {
        //             int unit_x = tmp1->unit->getCurrentPosition().getRow();
        //             int unit_y = tmp1->unit->getCurrentPosition().getCol();
        //             calculateNewEXP(URBAN, p_x, p_y, unit_x, unit_y, army->getName(), tmp1->unit->getUnitType(), 
        //             tmp1->unit, new_exp, new_lf);
        //         }
        //         tmp1 = tmp1->prev;
        //     }
        //     while(tmp2) {
        //         if (tmp2->unit) {
        //             int unit_x = tmp2->unit->getCurrentPosition().getRow();
        //             int unit_y = tmp2->unit->getCurrentPosition().getCol();
        //             calculateNewEXP(URBAN, p_x, p_y, unit_x, unit_y, army->getName(), tmp2->unit->getUnitType(), 
        //             tmp2->unit, new_exp, new_lf);
        //         }
        //         tmp2 = tmp2->next;
        //     }   
        // }        
    }else {
        int p_x = this->getPosX();
        int p_y = this->getPosY();
        int new_exp = 0, new_lf = 0;
        unitListNode *tmp1 = unitList->unitListHead, *tmp2 = unitList->unitListHead;
        while(tmp1) {
            if (tmp1->unit) {
                int unit_x = tmp1->unit->getCurrentPosition().getRow();
                int unit_y = tmp1->unit->getCurrentPosition().getCol();
                calculateNewEXP(URBAN, p_x, p_y, unit_x, unit_y, army->getName(), tmp1->unit->getUnitType(), 
                tmp1->unit, new_exp, new_lf);
            }
            tmp1 = tmp1->prev;
        }
        while(tmp2) {
            if (tmp2->unit) {
                int unit_x = tmp2->unit->getCurrentPosition().getRow();
                int unit_y = tmp2->unit->getCurrentPosition().getCol();
                calculateNewEXP(URBAN, p_x, p_y, unit_x, unit_y, army->getName(), tmp2->unit->getUnitType(), 
                tmp2->unit, new_exp, new_lf);
            }
            tmp2 = tmp2->next;
        }
        // army->updateEXP();
        // army->updateLF();
    }
}

void Fortification::getEffect(Army *army) {
    if (army == NULL) return;
    BattleField *battleField = army->getBattleField();
    UnitList *unitList = army->getUnitList();
    if (battleField && unitList) {
        vector<vector<TerrainElement*>> terrain = battleField->getTerrain();
        unitListNode *tmp1 = unitList->unitListHead, *tmp2 = unitList->unitListHead;
        int n_cols = battleField->getNCols();
        int n_rows = battleField->getNRows();
        vector<Position> forti_pos;
        for (int i = 0; i < n_rows; ++i) {
            for (int j = 0; j < n_cols; ++j) {
                if (terrain[i][j]->getTerrainType() == FORTIFICATION) {
                    Position pos(i, j);
                    forti_pos.push_back(pos);
                }
            }
        }
        int new_lf = 0, new_exp = 0;
        int p_x = this->getPosX();
        int p_y = this->getPosY();
        
        while(tmp1) {
            if (tmp1->unit) {                    
                int unit_x = tmp1->unit->getCurrentPosition().getRow();
                int unit_y = tmp1->unit->getCurrentPosition().getCol();
                calculateNewEXP(FORTIFICATION, p_x, p_y, unit_x, unit_y, army->getName(), tmp1->unit->getUnitType(), 
                tmp1->unit, new_exp, new_lf);
            }
            tmp1 = tmp1->prev;
        }
        while(tmp2) {
            if (tmp2->unit) {
                int unit_x = tmp2->unit->getCurrentPosition().getRow();
                int unit_y = tmp2->unit->getCurrentPosition().getCol();
                calculateNewEXP(FORTIFICATION, p_x, p_y, unit_x, unit_y, army->getName(), tmp2->unit->getUnitType(), 
                tmp2->unit, new_exp, new_lf);
            }
            tmp2 = tmp2->next;
        }
        // for(auto p: forti_pos) {
        //     int p_x = p.getRow();
        //     int p_y = p.getCol();
            
        //     while(tmp1) {
        //         if (tmp1->unit) {                    
        //             int unit_x = tmp1->unit->getCurrentPosition().getRow();
        //             int unit_y = tmp1->unit->getCurrentPosition().getCol();
        //             calculateNewEXP(FORTIFICATION, p_x, p_y, unit_x, unit_y, army->getName(), tmp1->unit->getUnitType(), 
        //             tmp1->unit, new_exp, new_lf);
        //         }
        //         tmp1 = tmp1->prev;
        //     }
        //     while(tmp2) {
        //         if (tmp2->unit) {
        //             int unit_x = tmp2->unit->getCurrentPosition().getRow();
        //             int unit_y = tmp2->unit->getCurrentPosition().getCol();
        //             calculateNewEXP(FORTIFICATION, p_x, p_y, unit_x, unit_y, army->getName(), tmp2->unit->getUnitType(), 
        //             tmp2->unit, new_exp, new_lf);
        //         }
        //         tmp2 = tmp2->next;
        //     }
        // }        
    }else {
        int p_x = this->getPosX();
        int p_y = this->getPosY();
        unitListNode *tmp1 = unitList->unitListHead, *tmp2 = unitList->unitListHead;
        int new_lf = 0, new_exp = 0;
        while(tmp1) {
            if (tmp1->unit) {                    
                int unit_x = tmp1->unit->getCurrentPosition().getRow();
                int unit_y = tmp1->unit->getCurrentPosition().getCol();
                calculateNewEXP(FORTIFICATION, p_x, p_y, unit_x, unit_y, army->getName(), tmp1->unit->getUnitType(), 
                tmp1->unit, new_exp, new_lf);
            }
            tmp1 = tmp1->prev;
        }
        while(tmp2) {
            if (tmp2->unit) {
                int unit_x = tmp2->unit->getCurrentPosition().getRow();
                int unit_y = tmp2->unit->getCurrentPosition().getCol();
                calculateNewEXP(FORTIFICATION, p_x, p_y, unit_x, unit_y, army->getName(), tmp2->unit->getUnitType(), 
                tmp2->unit, new_exp, new_lf);
            }
            tmp2 = tmp2->next;
        }
        // army->updateEXP();
        // army->updateLF();
    }
}

void SpecialZone::getEffect(Army *army) {
    if (army == NULL) return;
    BattleField *battleField = army->getBattleField();
    UnitList *unitList = army->getUnitList();
    if (battleField && unitList) {
        vector<vector<TerrainElement*>> terrain = battleField->getTerrain();
        unitListNode *tmp1 = unitList->unitListHead, *tmp2 = unitList->unitListHead;
        int n_cols = battleField->getNCols();
        int n_rows = battleField->getNRows();
        vector<Position> special_zone_pos;
        for (int i = 0; i < n_rows; ++i) {
            for (int j = 0; j < n_cols; ++j) {
                if (terrain[i][j]->getTerrainType() == SPECIAL_ZONE) {
                    Position pos(i, j);
                    special_zone_pos.push_back(pos);
                }
            }
        }
        int new_lf = 0, new_exp = 0;
        int p_x = this->getPosX();
        int p_y = this->getPosY();
        
        while(tmp1) {
            if (tmp1->unit) {                    
                int unit_x = tmp1->unit->getCurrentPosition().getRow();
                int unit_y = tmp1->unit->getCurrentPosition().getCol();
                calculateNewEXP(SPECIAL_ZONE, p_x, p_y, unit_x, unit_y, army->getName(), tmp1->unit->getUnitType(), 
                tmp1->unit, new_exp, new_lf);
            }
            tmp1 = tmp1->prev;
        }
        while(tmp2) {
            if (tmp2->unit) {
                int unit_x = tmp2->unit->getCurrentPosition().getRow();
                int unit_y = tmp2->unit->getCurrentPosition().getCol();
                calculateNewEXP(SPECIAL_ZONE, p_x, p_y, unit_x, unit_y, army->getName(), tmp2->unit->getUnitType(), 
                tmp2->unit, new_exp, new_lf);
            }
            tmp2 = tmp2->next;
        }
        // for(auto p: special_zone_pos) {
        //     int p_x = p.getRow();
        //     int p_y = p.getCol();
            
        //     while(tmp1) {
        //         if (tmp1->unit) {                    
        //             int unit_x = tmp1->unit->getCurrentPosition().getRow();
        //             int unit_y = tmp1->unit->getCurrentPosition().getCol();
        //             calculateNewEXP(SPECIAL_ZONE, p_x, p_y, unit_x, unit_y, army->getName(), tmp1->unit->getUnitType(), 
        //             tmp1->unit, new_exp, new_lf);
        //         }
        //         tmp1 = tmp1->prev;
        //     }
        //     while(tmp2) {
        //         if (tmp2->unit) {
        //             int unit_x = tmp2->unit->getCurrentPosition().getRow();
        //             int unit_y = tmp2->unit->getCurrentPosition().getCol();
        //             calculateNewEXP(SPECIAL_ZONE, p_x, p_y, unit_x, unit_y, army->getName(), tmp2->unit->getUnitType(), 
        //             tmp2->unit, new_exp, new_lf);
        //         }
        //         tmp2 = tmp2->next;
        //     }
        // }        
    }else {
        unitListNode *tmp1 = unitList->unitListHead, *tmp2 = unitList->unitListHead;
        int new_exp = 0, new_lf = 0;
        int p_x = this->getPosX();
        int p_y = this->getPosY();
        while(tmp1) {
            if (tmp1->unit) {                    
                int unit_x = tmp1->unit->getCurrentPosition().getRow();
                int unit_y = tmp1->unit->getCurrentPosition().getCol();
                calculateNewEXP(SPECIAL_ZONE, p_x, p_y, unit_x, unit_y, army->getName(), tmp1->unit->getUnitType(), 
                tmp1->unit, new_exp, new_lf);
            }
            tmp1 = tmp1->prev;
        }
        while(tmp2) {
            if (tmp2->unit) {
                int unit_x = tmp2->unit->getCurrentPosition().getRow();
                int unit_y = tmp2->unit->getCurrentPosition().getCol();
                calculateNewEXP(SPECIAL_ZONE, p_x, p_y, unit_x, unit_y, army->getName(), tmp2->unit->getUnitType(), 
                tmp2->unit, new_exp, new_lf);
            }
            tmp2 = tmp2->next;
        }
        // army->updateEXP();
        // army->updateLF();
    }
}
// class Mountain: public TerrainElement {
// private:
//     vector<Position *> arrayMoutain;
// public:
//     Mountain(vector<Position *> a) {
//         this->arrayMoutain = a;
//     }
//     void getEffect(Army *army) {
//         return;
//     }
// };


// Các hàm của class BattleField
BattleField::BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,    
    vector<Position *> arrayRiver,
    vector<Position *> arrayFortification, vector<Position *> arrayUrban, 
    vector<Position *> arraySpecialZone) {
    
    this->n_rows = n_rows;
    this->n_cols = n_cols;    
    for (int i = 0; i < n_rows; ++i) {
        vector<TerrainElement *> vector_tmp;
        for (int j = 0; j < n_cols; ++j) {
            Road *tmp = new Road(ROAD, i, j);
            vector_tmp.push_back(tmp);
        }
        terrain.push_back(vector_tmp);
    }
    
    for (auto tmp: arrayForest) {
        int x_idx = tmp->getRow();
        int y_idx = tmp->getCol();      
        if (x_idx < 0 || x_idx >= n_rows || y_idx < 0 || y_idx >= n_cols) continue;  
        if (terrain[x_idx][y_idx]) {
            if (terrain[x_idx][y_idx]->getTerrainType() == ROAD) {
                delete terrain[x_idx][y_idx];
                terrain[x_idx][y_idx] = new Mountain(MOUNTAIN, x_idx, y_idx);
            }
        }
    }
    
    for (auto tmp: arrayRiver) {        
        int x_idx = tmp->getRow();
        int y_idx = tmp->getCol();
        if (x_idx < 0 || x_idx >= n_rows || y_idx < 0 || y_idx >= n_cols) continue;  

        if (terrain[x_idx][y_idx]) {            
            if ( terrain[x_idx][y_idx]->getTerrainType() == ROAD) {
                delete terrain[x_idx][y_idx];
                terrain[x_idx][y_idx] = new River(RIVER, x_idx, y_idx);
            }
        }
    }
    
    for (auto tmp: arrayFortification) {        
        int x_idx = tmp->getRow();
        int y_idx = tmp->getCol();        
        if (x_idx < 0 || x_idx >= n_rows || y_idx < 0 || y_idx >= n_cols) continue;  
        if (terrain[x_idx][y_idx]) {            
            if (terrain[x_idx][y_idx]->getTerrainType() == ROAD) {                
                delete terrain[x_idx][y_idx];
                terrain[x_idx][y_idx] = new Fortification(FORTIFICATION, x_idx, y_idx);
            }
        }
    }
    for (auto tmp: arraySpecialZone) {
        int x_idx = tmp->getRow();
        int y_idx = tmp->getCol();
        if (x_idx < 0 || x_idx >= n_rows || y_idx < 0 || y_idx >= n_cols) continue;  
        if (terrain[x_idx][y_idx]) {
            if (terrain[x_idx][y_idx]->getTerrainType() == ROAD) {
                delete terrain[x_idx][y_idx];
                terrain[x_idx][y_idx] = new SpecialZone(SPECIAL_ZONE, x_idx, y_idx);
            }
        }
    }
    for (auto tmp: arrayUrban) {
        int x_idx = tmp->getRow();
        int y_idx = tmp->getCol();
        if (x_idx < 0 || x_idx >= n_rows || y_idx < 0 || y_idx >= n_cols) continue;  
        if (terrain[x_idx][y_idx]) {
            if (terrain[x_idx][y_idx]->getTerrainType() == ROAD) {
                delete terrain[x_idx][y_idx];
                terrain[x_idx][y_idx] = new Urban(URBAN, x_idx, y_idx);
            }
        }
    }
}
TerrainElement * BattleField::getElement(int p_x, int p_y) {
    int r = this->n_rows;
    int c = this->n_cols;
    if (p_x < 0 || p_y < 0) return NULL;    
    if (p_x  >= r || p_y >= c) return NULL;
    return terrain[p_x][p_y];
}
string BattleField::str() const {
    return "BattleField[n_rows=" + to_string(n_rows) + ",n_cols=" + to_string(n_cols) + ']';
}
void BattleField::printBattleField() {    
    for (int i = 0; i < n_rows; ++i) {
        for(int j = 0; j < n_cols; ++j) {
            cout << terrain[i][j]->str() << " ";
        }
        cout << endl;
    }
}
BattleField::~BattleField() {
    // Giải phóng bộ nhớ trong terrain        
    for (int i = 0; i < n_rows; ++i) {
        for (int j = 0; j < n_cols; ++j) {
            delete terrain[i][j];
            terrain[i][j] = NULL;
        }
    }        
}

// Các hàm của class Liberation và ARVN từ Army
Army::Army(Unit **unitArray, int size, string name, BattleField *battleField) {
    // Tính LF: tổng score từ các phương tiện (Vehicle), LF: [0, 1000]
    // UnitContainer *units = new UnitContainer();
    // UnitContainer *head = units;
    // UnitList *head = this->unitList;    
    unit_array_size = size;
    this->unitArray = unitArray;
    
    int vehicleScore = 0, infantryScore = 0, cnt = 0;
    for (int i = 0; i < size; ++i) {
        if(unitArray[i] != NULL) {
            if (unitArray[i]->getUnitType() == INFANTRY) {                
                infantryScore += unitArray[i]->getAttackScore();                
            } else {                
                vehicleScore += unitArray[i]->getAttackScore();
            }
            // if (i == size - 1) {
            //     head->nextUnit = NULL;
            // }else {
            //     head->nextUnit = new UnitList(unitArray[i]);
            //     head = head->nextUnit;
            // }
        }
    }    
    // Tính LF
    this->LF = max(0, min(1000, vehicleScore));
    // Tính EXP: tổng score từ các llbb (infantry), EXP: [0, 500]
    
    this->EXP = max(0, min(500, infantryScore));
    
    // unitList: Linked List Unit *
    // Tạo unitList, kiểm tra S có phải số đặc biệt hay không?
    int S = this->LF + this->EXP;    
            
    // cout << "q " << quantity << "size " << size << " " << name << endl;    
    int quantity = 8;    
    for (int i = 3; i < 11; ++i) {
        if (isPrimeNumber(i) && i % 2 != 0){            
            if (isSpecialNumber(S, i)) {
                quantity = 12;
                break;
            }
        }        
    }
    this->unitList = new UnitList(quantity);
    
    // unitList->unitListHead = unitList->createUnitList(unitArray, size); 
    int cnt_new = 0;   
    for (int i = 0; i < size; ++i) {
        // if (i >= quantity) {
        //     break;
        // }
        if (unitList) {                        
           if (unitList->insert(unitArray[i])) {  
                // cnt_new += 1;                    
                continue;
           }
        }
    }        
    // UnitList *temp = unitList->unitListHead->next;
    // while(temp) {
    //     cout << temp->unit->str() << " " << name << endl;
    //     temp = temp->next;
    // }
       
    this->battleField = battleField;
    this->name = name;

}
Army::~Army() {
    // if (battleField) {
    //     delete battleField;
    // }
    if (unitList) {
        delete unitList;

    }
}
void Army::updateEXP() {
    UnitList *unitList = this->unitList;
    if (!unitList) {
        return;
    }
    unitListNode *head = unitList->unitListHead;
    if(!head) {
        return;
    }
    unitListNode *temp = head->prev;
    int new_exp = 0;
    while(temp) {
        new_exp += temp->unit->getCurAttackScore();
        temp = temp->prev;
    }    
    this->setEXP(new_exp);
}
void Army::updateLF() {
    UnitList *unitList = this->unitList;
    if (!unitList) {
        return;
    }
    unitListNode *head = unitList->unitListHead;
    if(!head) {
        return;
    }
    unitListNode *temp = head->next;
    int new_lf = 0;
    while(temp) {
        new_lf += temp->unit->getCurAttackScore();
        temp = temp->next;
    }
    this->setLF(new_lf);
}
ARVN::ARVN(Unit **unitArray, int size, string name, BattleField *battleField): Army(unitArray, size, "ARVN", battleField) {}
LiberationArmy::LiberationArmy(Unit **unitArray, int size, string name, BattleField *battleField): Army(unitArray, size, "Liberation", battleField) {}
ARVN::~ARVN() {}
LiberationArmy::~LiberationArmy() {}
string LiberationArmy::str() const {    
    string LFstr = "LF=" + to_string(LF) + ',';
    string EXPstr = "EXP=" + to_string(EXP) + ',';
    string unitListstr = "unitList=" + unitList->str() + ',';
    string battleFieldStr = "battleField=";
    if (battleField) {
        battleFieldStr += battleField->str();
    }    
    string res = "LiberationArmy[" + LFstr + EXPstr + unitListstr + battleFieldStr + "]";
    return res;
}
string ARVN::str() const {
    string LFstr = "LF=" + to_string(LF) + ',';
    string EXPstr = "EXP=" + to_string(EXP) + ',';
    string unitListstr = "unitList=" + unitList->str() + ',';
    string battleFieldStr = "battleField=";
    if (battleField) {
        battleFieldStr += battleField->str();
    }
    string res = "ARVN[" + LFstr + EXPstr + unitListstr + battleFieldStr + "]";
    return res;
}
void Army::printUnitList() {
    if (unitList == NULL) {
        cout << "UnitList is empty\n";
        return;
    }
    // cout << "This is unitList\n";
    cout << unitList->str() << endl;
}
vector<Unit *> Army::findOptimal(vector<Unit *> units, int targetScore) {
    int n = units.size();    
    vector<Unit *> bestCombination;
    vector<Unit *> curCombine;
    int mini = INT_MAX;

    backtrack(0, 0, targetScore, mini, units, curCombine, bestCombination);
    return bestCombination;
}
int Army::nearestFibonacci(int num) const {
    if (num <= 0) return 1;    
    int a = 1, b = 1;
    while(b <= num) {
        int temp = a + b;
        a = b;
        b = temp;
    }    
    return b;
    // return (b - num < num - a) ? b : a;
}
void LiberationArmy::fight(Army *enemy, bool defense) {
    if (!defense) {
        // cout << "DEFENSE" << endl;
        float new_lf = LF * 1.5;
        float new_exp = EXP * 1.5;
        this->setLF(ceil(new_lf)), this->setEXP(ceil(new_exp));
        // Chọn ra tổ hợp A: từ unitList, chọn tổng nhỏ nhất > EXP của enemy
        // Chọn ra tổ hợp B: từ unitList, chọn tổng nhỏ nhất > LF của enemy

        // Chia danh sách thành infantry và vehicle
        unitListNode *unitListHead = unitList->unitListHead;
        vector<Unit *> infantryUnits, vehicleUnits;
        unitListNode *temp = NULL;
        unitListNode *temp1 = NULL;
        if (unitListHead) {
            temp = unitListHead->prev;
            temp1 = unitListHead->next;
        }
        while(temp != NULL) {
            infantryUnits.push_back(temp->unit);
            temp = temp->prev;
        }
        while (temp1 != NULL) {
            vehicleUnits.push_back(temp1->unit);
            temp1 = temp1->next;
        }
        // Tìm ra tổ hợp A và B        
        
        vector<Unit *> A = findOptimal(infantryUnits, enemy->getEXP());
        vector<Unit *> B = findOptimal(vehicleUnits, enemy->getLF());
        // cout << A.size() << " "  << B.size() << endl;        
        // for (auto unit: A) {
        //     cout << unit->str() << " " << unit->getCurAttackScore()  << " " << enemy->getEXP() << endl;
        // }
        // for (auto unit: B) {
        //     cout << unit->str() << " " << unit->getAttackScore() << " " << enemy->getLF() << endl;
        // }
        // Nếu tìm thấy, xóa các tổ hợp tương ứng khỏi unitList
        
        if(A.size() > 0 && B.size() > 0) { 
            // cout << "A\n";
            // Xóa tổ hợp tương ứng
            int numUnits = unitList->getNumUnits();
            // cout << "befroe numUnits" << numUnits << endl;
            for (auto unit: A) {                
                unitList->deleteUnitList(unit);
                unitList->setNumUnits(--numUnits);
            }
            // cout << "out1\n";
            // if (unitList) {
            //     cout << unitList->str() << endl;
            // }
            // cout << "delete" << endl;
            for (auto unit: B) {                
                unitList->deleteUnitList(unit);
                unitList->setNumUnits(--numUnits);
            }
            // cout << unitList->str() << endl;
            // cout << unitList->getNumUnits() << " now\n";
            // Cập nhật unitArray
            UnitList *enemyList = enemy->getUnitList();
            unitListNode *enemyListHead = enemy->getUnitList()->unitListHead;
            unitListNode *tmp1 = NULL, *tmp2 = NULL;
            if (enemyListHead) {
                tmp1 = enemyListHead->prev;
                tmp2 = enemyListHead->next;
            }else {
                return;
            }
            // cout << enemy->getUnitList()->str() << endl;
            while (tmp1) {
                bool tmp_insert = unitList->insert(tmp1->unit);
                if (tmp_insert || !tmp_insert) {
                    unitListNode *tmp_del = tmp1;
                    tmp1 = tmp1->prev;
                    enemyList->deleteUnitList(tmp_del->unit);
                    continue;
                }                
                tmp1 = tmp1->prev;
            }
            // UnitList *test = unitList->unitListHead->prev;
            // cout << unitList->getNumUnits() << endl;
            while (tmp2) {
                bool tmp_insert = unitList->insert(tmp2->unit);
                if (tmp_insert || !tmp_insert) {
                    unitListNode *tmp_del = tmp2;
                    tmp2 = tmp2->next;
                    enemyList->deleteUnitList(tmp_del->unit);
                    continue;
                }            
                tmp2 = tmp2->next;    
            }
            // cout << "out\n";
            // Xóa enemyListHead
            // delete enemyListHead;
            // enemyList->unitListHead = NULL;
            // delete enemyListHead;
            // enemy->getUnitList()->unitListHead = NULL;
            enemy->setEXP(0);
            enemy->setLF(0);
            // cout << "here\n";
            // cập nhật lại LF và EXP
            int new_lf = 0, new_exp = 0;
            if (unitListHead) {
                tmp1 = unitListHead->prev;
                tmp2 = unitListHead->next;
            }
            while (tmp1) {
                new_exp += tmp1->unit->getAttackScore();                
                tmp1 = tmp1->prev;
            }
            // cout << "out\n";
            // cout << tmp2->unit->str() << endl;
            while(tmp2) {
                new_lf += tmp2->unit->getAttackScore();
                // cout << "dangoday\n";
                tmp2 = tmp2->next;
            }
            // cout << "out\n";
            this->setLF(new_lf);
            this->setEXP(new_exp);   
            // cout << "wtf\n"         ;
            return;
        }
        // Nếu tìm thấy một trong hai
        else if(A.size() > 0 || B.size() > 0) {
            // cout << "B\n";
            // Kiểm tra chỉ số của toàn quân đội thuộc tổ hợp không thỏa mãn
            bool isValidate = false;
            if (A.size() == 0 ){
                // Nếu A không thỏa mãn
                // for(auto infantry: infantryUnits) {
                //     if (infantry->getAttackScore() <= enemy->getLF()) {
                //         isValidate = false;
                //         break;
                //     }
                // }
                if (EXP > enemy->getEXP()) {
                    isValidate = true;
                }
            }else {
                // Nếu B không thỏa mãn
                // for (auto vehicle: vehicleUnits) {
                //     if (vehicle->getAttackScore() <= enemy->getLF()) {
                //         isValidate = false;
                //         break;
                //     }
                // }
                if (LF > enemy->getLF()) {
                    isValidate = true;
                }
            }
            
            // Nếu true
            if (isValidate) {                
                // Xóa các tổ hợp thõa mãn và toàn bộ quân thuộc tổ hợp không thỏa mãn
                if (A.size() > 0) {
                    // Xóa các tổ hợp thõa mãn của A
                    for (auto unit: A) {
                        unitList->deleteUnitList(unit);
                    }                    
                    for (auto vehicle: vehicleUnits) {
                        unitList->deleteUnitList(vehicle);
                    }
                }else {
                    // Xóa các tổ hợp thõa mãn của B
                    for (auto unit: B) {
                        unitList->deleteUnitList(unit);                    
                    }
                    for (auto infantry: infantryUnits) {
                        unitList->deleteUnitList(infantry);
                    }
                }                                       
                //Xóa danh sách đơn vị của địch                
                UnitList *enemyList = enemy->getUnitList();
                unitListNode *enemyListHead = NULL;
                if (enemyList) {
                    enemyListHead = enemyList->unitListHead;
                }else {
                    return;
                }
                unitListNode *tmp1 = NULL, *tmp2 = NULL;
                if (enemyListHead) {
                    tmp1 = enemyListHead->prev;
                    tmp2 = enemyListHead->next;
                }
                // cout << enemy->getUnitList()->str() << endl;                
                while (tmp1) {
                    bool tmp_insert = unitList->insert(tmp1->unit);
                    if (tmp_insert || !tmp_insert) {
                        unitListNode *tmp_del = tmp1;
                        tmp1 = tmp1->prev;
                        enemyList->deleteUnitList(tmp_del->unit);
                        continue;
                    }
                    tmp1 = tmp1->prev;
                }
                int cnt = 0;
                while (tmp2) {       
                    bool tmp_insert = unitList->insert(tmp2->unit);
                    if (tmp_insert || !tmp_insert) {                                                
                        unitListNode *tmp_del = tmp2;
                        tmp2 = tmp2->next;
                        enemyList->deleteUnitList(tmp_del->unit);
                        continue;
                    }
                    tmp2 = tmp2->next;
                }
                
                // delete enemyListHead;                
                // enemy->getUnitList()->unitListHead = NULL;
                // UnitList *temp2 = unitList->unitListHead->next;
                // while(temp2) {
                //     cout << temp2->unit->str() << endl;
                //     temp2 = temp2->next;
                // }
                enemy->setEXP(0);
                enemy->setLF(0);
            }else {
                // Nếu false, giao tranh không xảy ra                
                temp = unitListHead->prev;
                temp1 = unitListHead->next;
                while(temp) {
                    float new_w = temp->unit->getWeight() * 0.9;
                    temp->unit->setWeight(ceil(new_w));
                    temp = temp->prev;
                }
                while(temp1) {
                    float new_w = temp1->unit->getWeight() * 0.9;
                    temp1->unit->setWeight(ceil(new_w));
                    temp1 = temp1->next;
                }
                
            }
            // Cập nhật lại LF, EXP
            temp = unitListHead->prev;
            temp1 = unitListHead->next;
            float new_lf = 0, new_exp = 0;
            while(temp) {
                new_exp += temp->unit->getAttackScore();                
                temp = temp->prev;
            }
            while(temp1) {
                new_lf += temp1->unit->getAttackScore();                
                temp1 = temp1->next;
            }            
            this->setLF(ceil(new_lf));
            this->setEXP(ceil(new_exp));            
            // Cập nhật lại của quân địch            
            return;
        }
        // Nếu không tìm thấy
        else {            
            temp = unitListHead->prev;
            temp1 = unitListHead->next;
            while(temp) {
                float new_w = temp->unit->getWeight() * 0.9;
                temp->unit->setWeight(ceil(new_w));
                temp = temp->prev;
            }
            while(temp1) {
                float new_w = temp1->unit->getWeight() * 0.9;
                temp1->unit->setWeight(ceil(new_w));
                temp1 = temp1->next;
            }
            // Cập nhật lại LF, EXP của quân ta
            temp = unitListHead->prev;
            temp1 = unitListHead->next;
            int new_lf = 0, new_exp = 0;
            while(temp) {
                new_exp += temp->unit->getAttackScore();
                temp = temp->prev;
            }
            while(temp1) {
                new_lf += temp1->unit->getAttackScore();
                temp1 = temp1->next;
            }
            this->setLF(new_lf), this->setEXP(new_exp);
            // Cập nhật LF, EXP của quân địch
            return;
        }
    }else {
        // Defense = true
        float new_lf = LF * 1.3;
        float new_exp = EXP * 1.3;
        this->setEXP(ceil(new_exp));
        this->setLF(ceil(new_lf));        
        // cout << LF << " " << EXP << endl;
        // cout <<  enemy->getLF() << " " << enemy->getEXP() << endl;
        if (LF > enemy->getLF() && EXP > enemy->getEXP()) {            
            return;
        }else if (LF > enemy->getLF() || EXP > enemy->getEXP()) {
            unitListNode *temp1 = NULL;
            unitListNode *temp2 = NULL;
            unitListNode *unitListHead = NULL;
            if(unitList) {                
                unitListHead = unitList->unitListHead;
                temp1 = unitListHead->prev;
                temp2 = unitListHead->next;
            }
            while(temp1) {
                float new_q = temp1->unit->getQuantity() * 0.9;
                temp1->unit->setQuantity(ceil(new_q));
                temp1 = temp1->prev;
            }   
            while(temp2) {
                float new_q = temp2->unit->getQuantity() * 0.9;
                temp2->unit->setQuantity(ceil(new_q));
                temp2 = temp2->next;
            }
            // Tính lại LF, EXP                        
            LF = max(0, min(1000, calculateLF(unitListHead)));
            EXP = max(0, min(500, calculateEXP(unitListHead)));

            
            // fight(enemy, true);
        }else {
            // Cập nhật quantity là số Fibonacci
            unitListNode*temp1 = NULL;
            unitListNode*temp2 = NULL;
            unitListNode*unitListHead = NULL;
            if(unitList) {
                unitListHead = unitList->unitListHead;
                temp1 = unitListHead->prev;
                temp2 = unitListHead->next;
            }
            while(temp1) {
                int new_q = nearestFibonacci(temp1->unit->getQuantity());                
                temp1->unit->setQuantity(new_q);
                temp1 = temp1->prev;
            }   
            while(temp2) {
                int new_q = nearestFibonacci(temp2->unit->getQuantity());
                temp2->unit->setQuantity(new_q);
                temp2 = temp2->next;
            }
            temp1 = unitListHead->prev;
            temp2 = unitListHead->next;
            this->setLF(calculateLF(temp2));
            this->setEXP(calculateEXP(temp1));
            // Ssau khi cập nhật, tiếp tục thực hiện hàm fight
            fight(enemy, true);
        }
    }
}
void ARVN::fight(Army *enemy, bool defense) {
    if (!defense) {
        // cout << "defendse\n";
        unitListNode *unitListHead = unitList->unitListHead;
        if (unitListHead) {
            // cout << "head\n";
            unitListNode *temp1 = unitListHead->prev;
            unitListNode *temp2 = unitListHead->next;
            while(temp1) {
                float new_q = temp1->unit->getQuantity() * 0.8;
                if (ceil(new_q) == 1) {
                    // Nếu quantity = 1, thì xóa;
                    Unit *tmp_del = temp1->unit;
                    temp1 = temp1->prev;
                    unitList->deleteUnitList(tmp_del);                    
                    continue;
                }else {
                    temp1->unit->setQuantity(ceil(new_q));
                }                
                
                temp1 = temp1->prev;
            }
            while(temp2) {
                float new_q = temp2->unit->getQuantity() * 0.8;
                if (ceil(new_q) == 1) {
                    // Nếu quantity = 1, thì xóa;
                    Unit *tmp_del = temp2->unit;
                    temp2 = temp2->next;
                    unitList->deleteUnitList(tmp_del);
                    continue;
                }
                temp2->unit->setQuantity(ceil(new_q));
                
                temp2 = temp2->next;
            }
            // Cập nhật LF, EXP
            int new_lf = 0, new_exp = 0;
            temp1 = unitListHead->prev;
            temp2 = unitListHead->next;
            while (temp1) {
                new_exp += temp1->unit->getAttackScore();
                temp1 = temp1->prev;
            }
            while(temp2) {
                new_lf += temp2->unit->getAttackScore();
                temp2 = temp2->next;
            }
            this->setLF(new_lf), this->setEXP(new_exp);            
        }else {
            LF = EXP = 0;
        }
    }else {
        // Nếu thua, các đơn vị trong unitList sẽ bị xóa hết, sau đó cập nhật lại unitArray
        unitListNode *unitListHead = unitList->unitListHead;
        if (unitListHead) {
            return;
        }else {          
            // DO danh sách đơn vị bị xóa hết  
            LF = 0, EXP = 0;
            // Cập nhật weight cho các unit còn lại trong unitArray
            for (int i = 0; i < this->getUnitArraySize(); ++i) {
                float new_w = unitArray[i]->getWeight() * 0.8;
                unitArray[i]->setWeight(ceil(new_w));
            }                                    
            
        }
        return;
    }
}
// Định nghĩa các hàm cho UnitList
UnitList::UnitList(int capacity) {
    // Kiểm tra capacity
    // UnitList *unitList = new UnitList();
    
    this->capacity = capacity;
    this->unitListHead = new unitListNode(NULL, NULL, NULL);
    this->curNumUnits = 0;
    this->infantryUnits = this->vehicleUnits = 0;
}
bool UnitList::insert(Unit *unit) {    
    if (unit == NULL) return false;
    // cout << "insert unit addr: " << unit << endl;        
    if(curNumUnits >= capacity) return false;
    unitListNode *temp = this->unitListHead;    
    if (!temp) return false;
    if (unit->getUnitType() == INFANTRY) {
        // if (infantryUnits >= capacity) return false;
        // cout << "insert\n";
        // Thêm vào đầu danh sách        
        while(temp != NULL && temp->prev != NULL) {
            // cout << temp->unit->getUnitType() << " " << unit->getUnitType() << endl;            
            if (!temp->unit) {                
                temp = temp->prev;
                continue;
            }
            InfantryType infantryType = ((Infantry *)unit)->getInfantryType();
            InfantryType tempUnitInfantry = ((Infantry *)temp->unit)->getInfantryType();
            // cout << infantryType << " " << tempUnitInfantry << endl;
            if (infantryType == tempUnitInfantry) {
                int new_q = temp->unit->getQuantity() + unit->getQuantity();
                temp->unit->setQuantity(new_q);                
                temp->unit->setAttackScore(temp->unit->getAttackScore());
                return false;
            }
            // curNumUnits += 1;
            temp = temp->prev;
        }        
        if (temp->unit) {
            InfantryType infantryType = ((Infantry *)unit)->getInfantryType();
            InfantryType tempUnitInfantry = ((Infantry *)temp->unit)->getInfantryType();            
            if (infantryType == tempUnitInfantry) {
                int new_q = temp->unit->getQuantity() + unit->getQuantity();
                temp->unit->setQuantity(new_q);
                // curNumUnits += 1;
                temp->unit->setAttackScore(temp->unit->getAttackScore());
                return false;
            }
        }
        // cout << unit->str() << endl;
        curNumUnits += 1;
        // infantryUnits += 1;
        Unit *newUnit = new Infantry(unit->getQuantity(), unit->getWeight(), unit->getCurrentPosition(), ((Infantry *)unit)->getInfantryType());
        newUnit->setAttackScore(unit->getCurAttackScore());
        // if (curNumUnits == 1) {
        //     UnitList *tmp = new UnitList(NULL, NULL, newUnit);
        //     // temp->prev = new UnitList(NULL, temp, unit);
        //     temp->prev = tmp;
        // }else {
            
        // }
        unitListNode *tmp = new unitListNode(newUnit, NULL, temp);
        // temp->prev = new UnitList(NULL, temp, unit);
        temp->prev = tmp;
        
        // cout << tmp->unit->str() << endl;
    } else {
        // Thêm vào cuối danh sách               
        // if (vehicleUnits >= capacity) return false;
        while(temp != NULL && temp->next != NULL) {
            if (!temp->unit) {
                temp = temp->next;
                continue;
            }
            // cout << temp->unit->getUnitType() << " " << unit->getUnitType() << endl;
            VehicleType tempUnitVehicle = ((Vehicle *)temp->unit)->getVehicleType();
            VehicleType unitVehicle = ((Vehicle *)unit)->getVehicleType();
            if (tempUnitVehicle == unitVehicle) {
                int new_q = temp->unit->getQuantity() + unit->getQuantity();
                temp->unit->setQuantity(new_q);
                // cout << "hello\n";
                temp->unit->setAttackScore(temp->unit->getAttackScore());
                // curNumUnits += 1;
                return false;
            }
            // curNumUnits += 1;
            temp = temp->next;
        }
        if (temp->unit) {
            VehicleType unitVehicle = ((Vehicle *)unit)->getVehicleType();
            VehicleType tempUnitVehicle = ((Vehicle *)temp->unit)->getVehicleType();
            if (unitVehicle == tempUnitVehicle) {
                int new_q = temp->unit->getQuantity() + unit->getQuantity();
                temp->unit->setQuantity(new_q);
                // curNumUnits += 1;
                temp->unit->setAttackScore(temp->unit->getAttackScore());
                return false;
            }
        }
        
        curNumUnits += 1;
        // vehicleUnits += 1;
        Unit *newUnit = new Vehicle(unit->getQuantity(), unit->getWeight(), unit->getCurrentPosition(), ((Vehicle *)unit)->getVehicleType());
        newUnit->setAttackScore(unit->getCurAttackScore());
        // if (curNumUnits == 1) {
        //     UnitList *tmp = new UnitList(NULL, NULL, newUnit);
        //     temp->next = tmp;
        // }else {
        //     UnitList *tmp = new UnitList(temp, NULL, newUnit);
        //     temp->next = tmp;
        // }
        unitListNode *tmp = new unitListNode(newUnit, temp, NULL);
        temp->next = tmp;
        // temp->next = new UnitList(temp, NULL, unit);
        // temp->next = tmp;
    }    
    return true;
}
bool UnitList::isContain(VehicleType vehicleType) {
    unitListNode *temp = this->unitListHead;    
    unitListNode *temp1 = NULL;
    if (temp) {
        temp1 = temp->next;
    }else{
        return false;
    }
    
    while(temp1 != NULL) {
        // Vehicle *child = static_cast<Vehicle *>(temp->unit);
        // string unitVehicle = convertVehicleType(((Vehicle *)unit)->getVehicleType());
        VehicleType tempUnitVehicle = ((Vehicle *)temp1->unit)->getVehicleType();
        VehicleType unitVehicle = vehicleType;
        if (unitVehicle == tempUnitVehicle) {
            return true;
        }
        temp1 = temp1->next;
    }
    return false;
}
bool UnitList::isContain(InfantryType infantryType) {
    unitListNode *temp = this->unitListHead;
    unitListNode *temp1 = NULL;
    if (temp) {
        temp1 = temp->prev;
    }else {
        return false;
    }
    while(temp1 != NULL) {
        // Infantry *child = static_cast<Infantry *>(temp->unit);
        InfantryType InfantryTypeTemp = infantryType;
        InfantryType tempUnitInfantry = ((Infantry *)temp1->unit)->getInfantryType();            
        if (InfantryTypeTemp == tempUnitInfantry) {
            return true;
        }
        temp1 = temp1->prev;
    }
    return false;
}
string UnitList::str() const {
    if (!unitListHead) {         
        return "UnitList[count_vehicle=0;count_infantry=0]";
    }
    int v_c = 0, i_c = 0;
    unitListNode *temp = unitListHead->prev;
    unitListNode *temp1 = unitListHead->next;
    string v_str = "", i_str = "";
    while (temp != NULL) {
        if (i_c == 0) {
            i_str = temp->unit->str() + i_str;
        }else {
            i_str = temp->unit->str() + ',' + i_str;
        }
        i_c += 1;
        temp = temp->prev;
    }
    while(temp1 != NULL) {
        if (v_c == 0) {
            v_str += temp1->unit->str();
        }else {
            v_str += ',' + temp1->unit->str();
        }
        v_c += 1;
        temp1 = temp1->next;
    }
    if (i_str != "" && v_str != "") {
        return "UnitList[count_vehicle=" + to_string(v_c) + ";count_infantry=" + to_string(i_c) + 
        ';' + i_str + ',' + v_str + ']';
    }else if (i_str != "") {
        return "UnitList[count_vehicle=" + to_string(v_c) + ";count_infantry=" + to_string(i_c) + 
        ';' + i_str + ']';
    }else if (v_str != "") {
        return "UnitList[count_vehicle=" + to_string(v_c) + ";count_infantry=" + to_string(i_c) + 
        ';' + v_str + ']';
    }else {
        return "UnitList[count_vehicle=" + to_string(v_c) + ";count_infantry=" + to_string(i_c) + ']';
    }
    
}

void UnitList::deleteUnitList(Unit *temp) {
    if (!temp) return;
    unitListNode *tmp1 = NULL;
    unitListNode *tmp2 = NULL;
    if (unitListHead) {
        tmp1 = unitListHead->prev;
        tmp2 = unitListHead->next;
    }
    while(tmp1) {
        if (tmp1->unit == temp) {
            unitListNode *p = tmp1->prev;
            unitListNode *n = tmp1->next;

            if (tmp1->prev) {
                // cout << "hello" << endl;
                if (n) {
                    tmp1->prev->next = n;
                }else {
                    tmp1->prev->next = NULL;
                }
            }
            if (tmp1->next) {
                // cout << "wetf" << endl;
                if (p) {
                    tmp1->next->prev = p;
                }else {
                    tmp1->next->prev = NULL;
                }
                
            }

            tmp1->prev = tmp1->next = NULL;
            // Xóa unit và Node unitList 
            delete tmp1->unit;
            tmp1->unit = NULL;
            // tmp1 = NULL;
            delete tmp1;
            tmp1 = NULL;
            return;
        }
        tmp1 = tmp1->prev;
    }
    while(tmp2 ) {
        if (tmp2->unit == temp) {
            unitListNode *p = tmp2->prev;
            unitListNode *n = tmp2->next;

            if (tmp2->prev) {
                if (n) {
                    tmp2->prev->next = n;
                }else {
                    tmp2->prev->next = NULL;
                }
            }
            if (tmp2->next) {
                if (p) {
                    tmp2->next->prev =p;
                }else {
                    tmp2->next->prev = NULL;
                }
            }

            tmp2->prev = tmp2->next = NULL;
            // Xóa unit và Node unitList 
            delete tmp2->unit;
            tmp2->unit = NULL;
            delete tmp2;
            tmp2 = NULL;
            return;
        }
        tmp2 = tmp2->next;
    }
}

UnitList::~UnitList() {
    // Giải phóng bộ nhớ cho unitList
    if (!unitListHead) {
        return;
    }    
    // UnitList *tmp = unitListHead;
    // if (curNumUnits == 0) {
    //     return;
    // }
    unitListNode *temp = unitListHead->prev;
    unitListNode *temp1 = unitListHead->next;
    
    while(temp != NULL) {
        unitListNode *temp2 = temp; 
        temp = temp->prev;        
        if(temp2->unit) {            
            // temp2->unit = NULL;
            delete temp2->unit;
            temp2->unit = NULL;
        }    
        // temp2 = NULL;
        delete temp2;
        temp2 = NULL;
    }        
    while(temp1 != NULL) {
        unitListNode *temp2 = temp1;    
        // cout << temp2 << endl;
        // if(temp2 == unitListHead) cout << "yes\n";
        temp1 = temp1->next;
        if(temp2->unit) {
            delete temp2->unit;
            temp2->unit = NULL;
        }                
        // temp2 = NULL;
        delete temp2;
        temp2 = NULL;
    }
    // unitListHead = nullptr;
    // unitListHead->prev = NULL;
    // unitListHead->next = NULL;
    // UnitList *tmp = unitListHead;
    // delete tmp;
    delete unitListHead;
    unitListHead = NULL;
    
    // delete unitListHead; 
}


// Các hàm của Configuration
VehicleType convertVehicleUnit(string unitType) {
    if (unitType == "TANK") {
        return TANK;
    }else if (unitType == "ARTILLERY") {
        return ARTILLERY;
    }else if (unitType == "ARMOREDCAR") {
        return ARMOREDCAR;
    }else if (unitType == "APC") {
        return APC;
    }else if (unitType == "TRUCK") {
        return TRUCK;
    }else if (unitType == "MORTAR") {
        return MORTAR;
    }
    return ANTIAIRCRAFT;
}
InfantryType convertInfantryUnit(string unitType) {
    if (unitType == "SNIPER") {
        return SNIPER;
    }else if (unitType == "ANTIAIRCRAFTSQUAD") {
        return ANTIAIRCRAFTSQUAD;
    }else if (unitType == "MORTARSQUAD") {
        return MORTARSQUAD;
    }else if (unitType == "ENGINEER") {
        return ENGINEER;
    }else if (unitType == "SPECIALFORCES") {
        return SPECIALFORCES;
    }
    return REGULARINFANTRY;
}
bool checkUnitType(const string unitType) {
    if (unitType == "TANK" || unitType == "ARTILLERY" || unitType == "ARMOREDCAR"
    || unitType == "APC" || unitType == "TRUCK" || unitType == "MORTAR"
    || unitType == "ANTIAIRCRAFT") {
        return true;
    }
    return false;
}
Configuration::Configuration(const string &filepath) {
    this->arvnUnits_size = this->liberationUnits_size = 0;
    this->ARVNUnits = this->liberationUnits = NULL;
    ifstream fileAss(filepath);

    if (!fileAss) {
        cout << "Error opening file: " << filepath << endl;
        return;
    }
    string line;
    while(getline(fileAss, line)) {
        string key, value;
        istringstream extractLine(line);
        getline(extractLine, key, '=');
        if (key == "UNIT_LIST") {
            string res = "";
            getline(extractLine, value);
            
            if (value == "[") {
                res += value;
                string newLine;
                while(getline(fileAss, newLine)) {
                    if (newLine == "]") {
                        res += newLine;
                        break;
                    }                    
                    res += newLine;
                }
                value = res;
            }            
            
        }else {
            getline(extractLine,value);
        }
        
        // cout << key << " " << value << endl;    
        key = trim(key);
        if (key == "EVENT_CODE") {
            value = trim(value);
        }
        // cout << key << " " << value << endl;  
        if (key == "NUM_ROWS") {
            this->num_rows = stoi(value);
        }else if (key == "NUM_COLS") {
            this->num_cols = stoi(value);
        } else if (value.front() == '[' && (value.back() == ']') && key != "UNIT_LIST") {
            stringstream ss(value);
            string pair_str;
            while(getline(ss, pair_str, ')')) {
                if (pair_str.empty() || pair_str.size() == 1) {
                    continue;
                }                
                size_t start = pair_str.find('(');
                if (start != -1) {
                    pair_str = pair_str.substr(start+1);
                }                
                istringstream pair_ss(pair_str);
                int x, y;
                char comma;
                                
                Position *pos = nullptr;
                if (pair_ss >> x >> comma >> y) {                    
                    pos = new Position(x, y);
                }
                if (pos == nullptr) {
                    // cout << "NULL" << endl;
                    continue;
                }
                if (key == "ARRAY_FOREST") {
                    this->arrayForest.push_back(pos);
                }else if (key == "ARRAY_FORTIFICATION") {
                    this->arrayFortification.push_back(pos);
                }else if (key == "ARRAY_RIVER") {
                    this->arrayRiver.push_back(pos);
                }else if (key == "ARRAY_URBAN") {
                    this->arrayUrban.push_back(pos);
                }else if (key == "ARRAY_SPECIAL_ZONE") {
                    this->arraySpecialZone.push_back(pos);
                }
            }
        }
        else if (key == "UNIT_LIST") {                            
            value = value.substr(1, value.size() - 2); // Remove []             
            stringstream ss(value);            
            string unit_str;
            vector<string> str_units;
            // start and end of units, ex: TANK(5,2,(1,2),0)
            size_t start = 0;
            size_t end = 0;
            size_t open_paren = 0;            
            for (size_t i = 0; i < value.length(); ++i) {
                if (value[i] == '(') open_paren++;
                if (value[i] == ')') open_paren--;
                if (open_paren == 0 && value[i] == ')') {
                    end = i;
                    str_units.push_back(value.substr(start, end - start + 1));
                    start = end + 2;                    
                }
            }
            vector<Unit *> arvn;
            vector<Unit *> liber;
            for (auto unit: str_units) {
                stringstream unitss(unit);
                string unitType;
                string value;
                getline(unitss, unitType, '(');
                getline(unitss, value);
                // Extract value
                value = value.substr(0, value.length() - 1);                
                istringstream unitiss(value);
                char comma;
                int quantity, weight, r, c;
                int armyBelong = value[value.length() - 1] == '1' ? 1 : 0 ;
                unitiss >> quantity >> comma >> weight;
                size_t start = value.find('(');
                size_t end = value.find(')');
                // 5,2,(1,2),0
                const string position = value.substr(start + 1, end - start - 1); // (1,2)
                istringstream posiss(position);
                posiss >> r >> comma >> c;
                //armyBelong = 0: quân Giải phóng, armyBelong = 1: quân chính quyền
                quantity = max(1, quantity);
                weight = max(1, weight);
                Position pos(r, c);                     
                if (armyBelong == 1) {                                        
                    if (checkUnitType(unitType)) {
                        // Kiểm tra unitType là vehicle hay infantry
                        VehicleType vehicleType = convertVehicleUnit(unitType);                        
                        // cout << "type" << vehicleType << "weight" << weight << "quantity" << quantity << endl;
                        Vehicle *vehicle = new Vehicle(quantity, weight, pos, vehicleType);                        
                        arvn.push_back(vehicle);
                    }else {
                        InfantryType infantryType = convertInfantryUnit(unitType);                        
                        Infantry *infantry = new Infantry(quantity, weight, pos, infantryType);
                        // this->ARVNUnits[arvnUnits_size] = infantry;
                        arvn.push_back(infantry);
                    }
                }else {                    
                    if (checkUnitType(unitType)) {
                        // Kiểm tra unitType là vehicle hay infantry                        
                        VehicleType vehicleType = convertVehicleUnit(unitType);                        
                        Vehicle *vehicle = new Vehicle(quantity, weight, pos, vehicleType);
                        // this->liberationUnits[arvnUnits_size] = vehicle;                        
                        liber.push_back(vehicle);
                    }else {
                        InfantryType infantryType = convertInfantryUnit(unitType);                        
                        Infantry *infantry = new Infantry(quantity, weight, pos, infantryType);
                        // this->liberationUnits[arvnUnits_size] = infantry;
                        liber.push_back(infantry);
                    }
                }
            }
            this->arvnUnits_size = arvn.size();
            this->liberationUnits_size = liber.size();
            // 
            // cout << arvnUnits_size << " " << liberationUnits_size << endl;
            this->ARVNUnits = new Unit*[arvnUnits_size];
            for (int i = 0; i < arvnUnits_size; ++i) {
                this->ARVNUnits[i] = arvn[i];
            }
            this->liberationUnits = new Unit*[liberationUnits_size];
            for (int i = 0; i < liberationUnits_size; ++i) {
                this->liberationUnits[i] = liber[i];
            }
        }else if (key == "EVENT_CODE") {       
            if (stoi(value) <= 0) {
                this->eventCode = 0;
            }else if (stoi(value) > 99) {
                this->eventCode = stoi(value) % 100;
            }else {
                this->eventCode = abs(stoi(value));
            }            
        }
    }
    // cout << arvnUnits_size << " " << liberationUnits_size << endl;
    // cout << this->getArvnUnitsSize() << " " << this->getLiberationUnitsSize() << endl;    
    // for (int i = 0; i < arvnUnits_size; ++i) {
    //     if (ARVNUnits[i] != NULL) {
    //         cout << "ARN " << ARVNUnits[i]->str() << endl;
    //     }
    // }
    // for (int i = 0 ; i < liberationUnits_size; ++i) {
    //     cout << "LIBER " << liberationUnits[i]->str() << endl;
    // }
    // Kiểm tra UnitList
    // ARVN *arvn = new ARVN(ARVNUnits, arvnUnits_size, "ARVN", NULL);
    // if (arvn != NULL) {
    //     cout << "arvn" << endl;
    //     arvn->printUnitList();
    // }
    fileAss.close();
}
string Configuration::getARVNUnitStr() const{
    string res = "";
    for (int i = 0; i < arvnUnits_size; ++i) {
        if (i == 0) {
            res += ARVNUnits[i]->str();
        }else {
            res += ',' + ARVNUnits[i]->str();
        }
    }
    return res;
}
string Configuration::getLiberUnitStr() const{
    string res = "";
    for (int i = 0; i < liberationUnits_size; ++i) {
        if (i == 0) {
            res += liberationUnits[i]->str();
        }else {
            res += ',' + liberationUnits[i]->str();
        }
    }
    return res;
}
string Configuration::getArrayForestStr() const {
    string res = "";
    int length = arrayForest.size();
    for(int i = 0; i < length; ++i) {
        if (i == 0) {
            res += arrayForest[i]->str();
        }else {
            res += ',' + arrayForest[i]->str();
        }
    }
    return res;
}
string Configuration::getArrayRiverStr() const{
    string res = "";
    int length = arrayRiver.size();
    for(int i = 0; i < length; ++i) {
        if (i == 0) {
            res += arrayRiver[i]->str();
        }else {
            res += ',' + arrayRiver[i]->str();
        }
    }
    return res;
}
string Configuration::getArrayFortiStr() const {
    string res = "";
    int length = arrayFortification.size();
    for(int i = 0; i < length; ++i) {
        if (i == 0) {
            res += arrayFortification[i]->str();
        }else {
            res += ',' + arrayFortification[i]->str();
        }
    }
    return res;
}
string Configuration::getArrayUrbanStr() const{
    string res = "";
    int length = arrayUrban.size();
    for(int i = 0; i < length; ++i) {
        if (i == 0) {
            res += arrayUrban[i]->str();
        }else {
            res += ',' + arrayUrban[i]->str();
        }
    }
    return res;
}
string Configuration::getArraySpecialZoneStr() const{
    string res = "";
    int length = arraySpecialZone.size();
    for(int i = 0; i < length; ++i) {
        if (i == 0) {
            res += arraySpecialZone[i]->str();
        }else {
            res += ',' + arraySpecialZone[i]->str();
        }
    }
    return res;
}

string Configuration::str() const {
    // cout << "Hello BTL2";
    string numRowsStr = "num_rows=" + to_string(num_rows) + ',';
    string numColsStr = "num_cols=" + to_string(num_cols) + ',';
    string arrayForestStr = "arrayForest=[" + getArrayForestStr() + "],";
    string arrayRiverStr = "arrayRiver=[" + getArrayRiverStr() + "],";
    string arrayFortificationStr = "arrayFortification=[" + getArrayFortiStr() + "],";
    string arrayUrbanStr = "arrayUrban=[" + getArrayUrbanStr() + "],";
    string arraySpecZoneStr = "arraySpecialZone=[" + getArraySpecialZoneStr() + "],";
    string liberUnitStr = "liberationUnits=[" + getLiberUnitStr() + "],";
    string arvnUnitStr = "ARVNUnits=[" + getARVNUnitStr() + "],";
    string eventCodeStr = "eventCode=" + to_string(eventCode);
    string res = '[' + numRowsStr + numColsStr  + 
        arrayForestStr + arrayRiverStr + arrayFortificationStr + arrayUrbanStr + arraySpecZoneStr
        + liberUnitStr + arvnUnitStr +
        eventCodeStr + 
    "]";
    return res;
}
Configuration::~Configuration() {
     // Giải phóng Position * trong các vector<Position *>
    for (int i = 0; i < arrayForest.size(); ++i) {
        delete arrayForest[i];
    }
    for (int i = 0; i < arrayFortification.size(); ++i) {
        delete arrayFortification[i];
    }
    for (int i = 0; i < arrayRiver.size(); ++i) {
        delete arrayRiver[i];
    }
    for (int i = 0; i < arraySpecialZone.size(); ++i) {
        delete arraySpecialZone[i];
    }
    for (int i = 0; i < arrayUrban.size(); ++i) {
        delete arrayUrban[i];
    }
    
    // Giải phóng ARVNUnits, LiberationUnits
    if(arvnUnits_size > 0) {
        for (int i = 0; i < arvnUnits_size; i++) {
            if (ARVNUnits[i]) {
                delete ARVNUnits[i];
            }
            
        }
        delete []ARVNUnits;
    }else if (ARVNUnits) {
        delete [] ARVNUnits;
    }
    
    if (liberationUnits_size > 0) {
        // cout << "yes\n";
        for (int i = 0; i < liberationUnits_size; ++i) {
            if(liberationUnits[i]) {
                delete liberationUnits[i];
            }
        }
        delete []liberationUnits;  
    }else if (liberationUnits)  {
        
        delete [] liberationUnits;
    }
      
    arvnUnits_size = liberationUnits_size = 0;
}
// Hàm cho position
string Position::str() const {
    return '(' + to_string(r) + ',' + to_string(c) + ')';
}
int Position::getRow() const {
    return this->r;
}
int Position::getCol() const {
    return this->c;
}
void Position::setCol(int c) {
    this->c = c;
}
void Position::setRow(int r) {
    this->r = r;
}
Position::Position(const string& str) {
    int x = 0, y = 0, sign1 = 1, sign2 = 1;    
    int comma = str.find(',');
    string tmp1 = str.substr(1, comma - 1);
    string tmp2 = str.substr(comma + 1, str.size() - comma - 2);
    // cout << tmp1 << " " << tmp2 << endl;
    for(size_t i = 0; i < tmp1.size(); ++i) {
        if (tmp1[i] == '-') {
            sign1 = -1;
            continue;
        }
        if (tmp1[i] != ' ') {
            x = x * 10 + (tmp1[i] - '0');
        }
    }
    for (size_t i = 0; i < tmp2.size(); ++i) {
        if (tmp2[i] == '-') {
            sign2 = -1;
            continue;
        }
        if (tmp2[i] != ' ') {
            y = y * 10 + (tmp2[i] - '0');
        }
    }
    this->r = x * sign1;
    this->c = y * sign2;
}
// Hiện thực các hàm của class HCMCampaign
HCMCampaign::HCMCampaign(const string &config_file_path) {    
    config = new Configuration(config_file_path);
    // cout << config->getEventCode() << endl;
    if (!config) return;
    battleField = new BattleField(config->getNumRows(), config->getNumCols(), config->getArrayForest(), config->getArrayRiver(),
    config->getArrayFori(), config->getArrayUrban(), config->getArraySpecial());
    
    liberationArmy = new LiberationArmy(config->getLiberationUnits(), config->getLiberationUnitsSize(), "Liberation", battleField);
    
    arvn = new ARVN(config->getARVNUnits(), config->getArvnUnitsSize(), "ARVN", battleField);
    // Print battle field
    // cout << "Battle Field\n";
    // if (battleField) {
    //     battleField->printBattleField();
    // }
    // cout << endl;
}
string HCMCampaign::printResult() {
    string liberStr = "", arvnStr = "";
    if (liberationArmy) {
        liberStr = "LIBERATIONARMY[LF=" + to_string(liberationArmy->getLF()) + ",EXP=" + to_string(liberationArmy->getEXP()) + ']';
    }
    if (arvn) {
        arvnStr = "-ARVN[LF=" + to_string(arvn->getLF()) + ",EXP=" + to_string(arvn->getEXP()) + ']';
    }
    return liberStr + arvnStr;
}
HCMCampaign::~HCMCampaign() {
    if (config) {
        delete config;
    }
    config = NULL;
    if (battleField) {
        delete battleField;
    }
    battleField = NULL;
    if (liberationArmy) {
        delete liberationArmy;
    }
    liberationArmy = NULL;
    // if (battleField) {
    //     cout << "yes\n";
    // }else {
    //     cout << "no\n";
    // }
    
    if (arvn) {
        delete arvn;
    }
    arvn = NULL;
    // if (config) {
    //     delete config;
    // }
}
void HCMCampaign::run() {
    if(!config) {
        // cout << "NULL\n";
        return;
    }
    if (config) {
        // Trước khi giao tranh, các yếu tố địa hình sẽ ảnh hưởng     
        if (battleField)    {
            vector<vector<TerrainElement*>> terrain = battleField->getTerrain();                
            int n_rows = battleField->getNRows();
            int n_cols = battleField->getNCols();
            for (int i = 0; i < n_rows; ++i) {
                for (int j = 0; j < n_cols; ++j) {
                    if (!terrain[i][j]) {
                    }else {
                        terrain[i][j]->getEffect(arvn);
                        terrain[i][j]->getEffect(liberationArmy);
                    }   
                    
                }
            }
            if (config->getEventCode() < 75) {
                if (liberationArmy && arvn) {                
                    
                    liberationArmy->fight(arvn, false);
                    arvn->fight(liberationArmy, true);
                }
            }else {
                // cout << "No\n";
                arvn->fight(liberationArmy, false); // arvn tấn công
                // liberationArmy->fight(arvn, true); // quân Giải phóng phòng thủ
                // cout << liberationArmy->getEXP() << " " << liberationArmy->getLF() << endl;
                liberationArmy->fight(arvn, false); // quân giải phóng phản công
                // arvn->fight(liberationArmy, true);
            }
        }

        // Saau giap tranh, nếu có đvqs có attackScore <= 5 sẽ bị xóa
        UnitList *liberUnit = liberationArmy->getUnitList();
        UnitList *arvnUnit = arvn->getUnitList();
        if (liberUnit) {
            unitListNode *head = liberUnit->unitListHead;
            unitListNode *tmp = head->prev;
            unitListNode *tmp1 = head->next;
            while(tmp) {
                if (tmp->unit->getCurAttackScore() <= 5) {
                    Unit *tmp_del = tmp->unit;
                    tmp = tmp->prev;
                    liberUnit->deleteUnitList(tmp_del);
                    continue;                    
                }
                tmp = tmp->prev;
            }
            while(tmp1) {
                if (tmp1->unit->getCurAttackScore() <= 5) {
                    Unit *tmp_del = tmp1->unit;
                    tmp1 = tmp1->next;
                    arvnUnit->deleteUnitList(tmp_del);
                    continue;
                }
                tmp1 = tmp1->next;
            }
            // Cập nhật lại chỉ số            
            liberationArmy->setEXP(liberationArmy->calculateEXP(head));
            liberationArmy->setLF(liberationArmy->calculateLF(head));            
        }
        if (arvnUnit) {
            unitListNode *head = arvnUnit->unitListHead;
            unitListNode *tmp = NULL;
            unitListNode *tmp1 = NULL;
            if (head) {
                tmp = head->prev;
                tmp1 = head->next;
            }

            while(tmp) {
                
                if (tmp->unit->getCurAttackScore() <= 5) {
                    Unit *tmp_del = tmp->unit;
                    tmp = tmp->prev;
                    arvnUnit->deleteUnitList(tmp_del);
                    continue;                    
                }
                tmp = tmp->prev;
            }
            while(tmp1) {
                if (tmp1->unit->getCurAttackScore() <= 5) {
                    Unit *tmp_del = tmp1->unit;
                    tmp1 = tmp1->next;
                    arvnUnit->deleteUnitList(tmp_del);
                    continue;
                }
                tmp1 = tmp1->next;
            }
            // Cập nhật lại chỉ số            
            arvn->setEXP(arvn->calculateEXP(head));
            arvn->setLF(arvn->calculateLF(head));
            // arvn->printUnitList();
        }
    }
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////