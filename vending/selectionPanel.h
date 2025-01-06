#ifndef SELECTION_PANEL_H
#define SELECTION_PANEL_H

#include <map>
#include <vector>

enum class Items{
    Parle_G = 58,
    lays_Green = 59,
    lays_red = 60,
    lays_orange = 61,
    lassi = 62,
    icetea = 63,
    lipton = 64,
    cold_coffee = 65,
    vanilla_shake = 66,
    chocalate_shake = 67
};

class selectionPanel{

    private:
      std::map<Items,int>product;
      int id;
    public:
    std::vector<Items> vending_items(){
        return {
           Items::Parle_G, Items::lays_Green, Items::lays_red, Items::lays_orange, Items::lassi, Items::icetea, Items::lipton,
           Items::cold_coffee, Items::vanilla_shake, Items::chocalate_shake 
        };
    }

    void get_id();
    std::map<Items,int> setProduct_by_id();
};
#endif
