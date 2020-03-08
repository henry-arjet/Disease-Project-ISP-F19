//Henry Arjet and Sam Adams
//Introduction to scientific computing final project
//Disease Project
//Dec 12 2019

#include <iostream>
#include <fstream>

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::to_string;


std::ofstream csv;
class Writer{ //This is the object that handles writing to the csv file.
public:
    Writer(){}
    void initialize(string name){ // starts writing a file,
        csv.open (name, std::ios::out | std::ios::trunc);
        csv << "id,status,daysLeft\n"; // this is the name for each field
        
    }
    void record(string field1, string field2, string field3){
        csv << field1 << "," << field2 << "," << field3 << "\n"; 
    }
    void close(){
        csv.close();
    }

};
bool waitForInput = true; //This allows me to manualy control what goes on, step through
                          //the days and view debugs. Used only for debugging.

Writer writer;
class Person{
public:
    Person(){
        status = "healthy";
    }
    unsigned int ID; //ID for each person, serial
    string status; //sick, susceptible, recovered, inoculated
    short daysLeft = 0;
    void Infect(int n){ //infects the person, changes status to sick for however many days
                        //Only infects healthy. Deciding who to infect is left to later.
        if(status == "healthy"){
            status = "sick";
            daysLeft = n;
            }
    }
    void Update(){ //Steps through the days they will be sick, until they're healed
        if (status == "sick"){
            if (daysLeft > 0){
                daysLeft--;
            }
            else {status = "recovered";}
        }
    }
    string StatusString(){
        string str = "Person " + std::to_string(ID) + " is " + status;
        if (status == "sick"){
            str += " and will recover in ";
            str += std::to_string(daysLeft);
            str += " day(s)"; 
        }
        str = str + "\n";
        return str;
    }
};

struct Pathogen{
    short daysSick = 4;
    //daysSickSD = 2;
    float closeInfectionChance = 0.13f;
    float farInfectionChance = 0.08f;
};

class Simulation{
public:
    Pathogen path;
    unsigned int currentDay = 0;
    float chanceToTravel = 0.03;

    Person population[1000];  //     POPULATION CONTROL
    Simulation(){
        Populate(0.03f, 0.75f); //populates, infects (f) percent of people
    }

    void Report(){
    for (Person p : population){
        cout << p.StatusString();
        }
    }
    void Populate(float infectChance, float vaccChance){ //fills out population list. Random chance to infect
        for (int i = 0; i < sizeof(population)/sizeof(population[0]); i++){
            Person p;
            p.ID = i;
            if ((float)rand()/RAND_MAX <= vaccChance)
                p.status = "inoculated";
            if ((float)rand()/RAND_MAX <= infectChance)
                p.Infect(path.daysSick);

            population[i] = p;

        }
    }
    void Dio(){
        cout << "You were expecting something useful to happen\n";
        cout << "But it was me, Dio!\n";
    }

    void Update(){
        currentDay++;
        Person swapper;
        for (int i = 0; i < sizeof(population)/sizeof(population[0]); i++){
            Person p = population[i];
            if (p.status == "sick"){
                InfectionCheck(p, population[i - 2], false);
                InfectionCheck(p, population[i - 1], true);
                InfectionCheck(p, population[i + 1], true);
                InfectionCheck(p, population[i + 2], false);
            }
            if ((float)rand()/RAND_MAX <= chanceToTravel){
                int r = rand() % sizeof(population)/sizeof(population[0]); //pick a random spot to travel to
                swapper = population[r]; // moves the person in r to the placeholder 'swapper' spot
                population[r] = p; // moves the person p to spot r
                population[i] = swapper; // moves the person in the placeholder, who was in r, to p's old spot
            }

            population[i].Update();
        }
        /*
        SWAPPER

        For (number of swaps to do = i) //traveling!!
        R1 = person [int rand*populationSize]
        R2 = same
        Swapper = R1
        R1 = R2
        R2 = swapper
        */
    }   
    void InfectionCheck(Person origin, Person &target, bool close){
        float r = (float)rand()/RAND_MAX;
	    if ((r < path.closeInfectionChance && close) || (r < path.farInfectionChance)){
            target.Infect(path.daysSick);//Checking for if the target is succeptable is done target side in the Infect function
		    //target.infect(normal(contagian.daysSick, contagian.daysSickSD)) // infects target, calls normal (to be added once I put in a normal function) 
        }
    }
    void WriteDay(){
        writer.initialize("Day " + to_string(currentDay) + ".txt");
        for (Person p : population){
            writer.record(to_string(p.ID), p.status, to_string(p.daysLeft));
        }
        writer.close();
    }

    bool IsDone(){
        for (Person p : population){
            if (p.status == "sick")
                return false;
        }
        return true;
    }

};


int main(){

    srand(time(NULL));
    Simulation sim;

    while(waitForInput == true){
        //string input;
        //cin >> input;
        string input;
        cout << "\nAwaiting input...\n";
        cin >> input;
        if (input == "test")
            cout << "tested\n";
        else if (input == "abort" || input == "exit")
            waitForInput = false;
        else if (input == "debug" || input == "report")
            sim.Report();
        else if (input == "update" || input == "go")
            sim.Update();
        else if (input == "dio")
            sim.Dio();
        else if (input == "run7"){
            sim.Update();sim.Update();sim.Update();sim.Update();sim.Update();sim.Update();sim.Update();
        }
        else if (input == "write")
            sim.WriteDay();
        else if (input == "check"){
            if (sim.IsDone()){
                cout << "we done\n";
                waitForInput = false;
            }
        }else if (input == "run"){
            while(waitForInput){
                sim.WriteDay();
                if (sim.IsDone()){
                    waitForInput = false;
                }
                sim.Update();
            }
        }
        else 
            cout << "What\n";
    }
}

