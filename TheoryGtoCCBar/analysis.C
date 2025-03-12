#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

struct dimuonchain{
    std::vector<int> mu1c;
    std::vector<int> mu2c;
};

struct EventData {
    std::vector<int> pdg;
    std::vector<int> idx;
};

struct Event {
    int eventID;
    std::vector<EventData> entries;
};

int mother_same_decay(EventData m1, EventData m2){
    // 0 not same charge
    // 0 no common parents
    // 1 same single b hadron
    if(m1.pdg[0] == m2.pdg[0]){
        return 0;
    }
    for(int i = 0; i< m1.idx.size(); i++){
        if((abs(m1.pdg[i]) == 2212)||(abs(m1.pdg[i]) <100)){continue;}
        for(int j = 0; j<m2.idx.size(); j++){
            if((abs(m2.pdg[j]) == 2212)||(abs(m2.pdg[j]) <100)){continue;}
            //cout << m1.pdg[i] << " " << m2.pdg[j] << endl;
            if(m1.idx[i] == m2.idx[j]){
                return m1.pdg[i];
            }
        }
    }
    return 0;

}

dimuonchain chainform(EventData m1, EventData m2, int mother){

        dimuonchain c;

        int i = 0;
        while(m1.pdg[i-1] != mother){
            c.mu1c.push_back(abs(m1.pdg[i]));
            i+=1;
        }

        int j = 0;
        while(m2.pdg[j-1] != mother){
            c.mu2c.push_back(abs(m2.pdg[j]));
            j+=1;
        }
    
    
    return c;
}

int congruent(dimuonchain d1, dimuonchain d2){

    int swap = 1;
    int noswap = 1;
    if((d1.mu1c.size() != d2.mu1c.size())||(d1.mu2c.size() != d2.mu2c.size())){
        noswap = 0;
    }    
    else{
        for(int i = 0; i< d1.mu1c.size(); i++){
            if(d1.mu1c[i] != d2.mu1c[i]){
                noswap = 0;
                break;
            }
        }
        for(int i = 0; i< d1.mu2c.size(); i++){
            if(d1.mu2c[i] != d2.mu2c[i]){
                noswap = 0;
                break;
            }
        }
    }
    if((d1.mu1c.size() != d2.mu2c.size())||(d1.mu2c.size() != d2.mu1c.size())){
        swap = 0;
    }    
    else{
        for(int i = 0; i< d1.mu1c.size(); i++){
            if(d1.mu1c[i] != d2.mu2c[i]){
                swap = 0;
                break;
            }
        }
        for(int i = 0; i< d1.mu2c.size(); i++){
            if(d1.mu2c[i] != d2.mu1c[i]){
                swap = 0;
                break;
            }
        }
    }
    
    if((swap == 1)||(noswap == 1)){return 1;}
    else{
        return 0;
    }

}


int analysis() {
    std::ifstream file("output/file_1/events.csv");
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::string line;
    std::vector<Event> events;
    std::unordered_map<int, size_t> eventIndexMap;  // Maps eventID to position in events vector

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        int eventID;
        EventData eventData;

        // Read eventID
        std::getline(ss, value, ',');
        eventID = std::stoi(value);

        // Read the rest of the values
        int i = 0;
        while (std::getline(ss, value, ',')) {
            int num = std::stoi(value);
            if (i % 2 == 0) {
                eventData.pdg.push_back(num);
            } else {
                eventData.idx.push_back(num);
            }
            i++;
        }

        // Check if the event already exists
        if (eventIndexMap.find(eventID) == eventIndexMap.end()) {
            // Create a new event and store its index
            eventIndexMap[eventID] = events.size();
            events.push_back({eventID, {}});
        }

        // Add this row to the correct event
        events[eventIndexMap[eventID]].entries.push_back(eventData);
    }

    file.close();

    int mother = 0;
    // Print the stored data in the correct order
    /*
    for (const auto& event : events) {
        if(event.entries.size()==2){
        std::cout << "Event ID: " << event.eventID << std::endl;
        for (const auto& entry : event.entries) {
            std::cout << "  PDG: ";
            for (int p : entry.pdg) std::cout << p << " ";
            std::cout << "\n  IDX: ";
            for (int i : entry.idx) std::cout << i << " ";
            //std::cout << "-----" << endl;
    
            std::cout << "\n-----------------------------" << std::endl;
        }
        
        
        mother = mother_same_decay(event.entries[0],event.entries[1]); 
        std:: cout << " mother " << mother << endl;
        std::cout << endl;
        dimuonchain a = chainform(event.entries[0],event.entries[1],mother);
        std::cout << "____-  _ - _  -_ chain 1  __ - ________ -- " << endl;
        for (int v : a.mu1c) std::cout << v << " ";
        std::cout << "\n ____ - _--_ __-- chain 2 - -___  _ ____ - _ \n";
        for (int v : a.mu2c) std::cout << v << " ";
        std::cout << "\n";
        std::cout << endl;
        std::cout << "congruent w jpsi: " << congruent(guy1,a) << endl;
        std::cout << endl;
        }
        
    }*/

    vector<int> populations = {};
    vector<dimuonchain> chains = {};
    int isnewchain=1;
    int counter = 0;


    for(int i = 0; i< events.size(); i++){
        if(events[i].entries.size()!=2){continue;}

        counter +=1;
        mother = mother_same_decay(events[i].entries[0],events[i].entries[1]); 
        dimuonchain a = chainform(events[i].entries[0],events[i].entries[1],mother);
        isnewchain = 1;

        for(int q = 0; q<chains.size(); q++){
            if(congruent(a,chains[q])){
                populations[q]+=1;
                isnewchain = 0;
                break;
            }
        }
        if(isnewchain == 1){
            chains.push_back(a);
            populations.push_back(1);

            /*std::cout << "chain 1: ";
            for (int v : a.mu1c) std::cout << v << " ";
            std::cout << "\nchain 2: ";
            for (int v : a.mu2c) std::cout << v << " ";
            std::cout << "\n";
            std::cout << endl;*/
        }

    }
    int sanity = 0;
    for(int i = 0; i<populations.size();i++){
        
        if(((populations[i]*1.0)/counter)*100 < 0.1){
            continue;
        }
        sanity += populations[i];
        std::cout << "Decay #" << i << endl;
        dimuonchain ab = chains[i];
        std::cout << "decayed: " << populations[i] << " for " << ((populations[i]*1.0)/counter)*100 << " % of all events looked at " << endl;
        std::cout << "chain 1: ";
        for (int v : ab.mu1c) std::cout << v << " ";
        std::cout << "\nchain 2: ";
        for (int v : ab.mu2c) std::cout << v << " ";
        std::cout << "\n";
        std::cout << endl;

    
        
         
    }
    
    cout << sanity << " " << counter << endl;
    
    return 0;
}
