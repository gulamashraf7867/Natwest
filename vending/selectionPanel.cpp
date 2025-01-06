#include "selectionPanel.h"
#include <iostream>
#include <vector>
#include <algorithm>
void selectionPanel::get_id(){
    std::cin>>this->id;
}
std::map<Items,int> selectionPanel::setProduct_by_id(){

  std::vector<Items> total_items = vending_items();
    Items productName = static_cast<Items>(id);
    auto it = std::find(total_items.begin(),total_items.end(),productName);
    if(it!=total_items.end()){
        //std::map<Items,int> selected_item =
        product={{*it,id}};
        return  product;
    }
    return std::map<Items,int>();    
}


int main(){
    selectionPanel SelectionPanel;
    SelectionPanel.get_id();
    SelectionPanel.setProduct_by_id();
    return 0;

}




