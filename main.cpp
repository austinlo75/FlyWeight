#include <iostream>
#include <initializer_list>
#include <string>
#include <utility>
#include <unordered_map>

using namespace std;

/*
enum class Color 
{
    red,
    blue,
    green
};

enum class Make
{
    toyota,
    mitsubishi,
    ford
};
*/

struct SharedState_Car
{
    string brand;
    string model;
    string color;
    
    SharedState_Car(const string &brand, const string &model, const string &color)
        : brand(brand), model(model), color(color)
    {
    }
    friend ostream& operator<<(ostream &os, const SharedState_Car &car)
    {
        return os << "[ " << car.brand << ", " << car.model << ", " << car.color << " ]";
    }
};

struct UniqueState_Car
{
    string owner;
    string address;
    
    UniqueState_Car(const string &owner, const string &address)
        :owner(owner), address(address)
    {}
    friend ostream& operator<<(ostream &os, const UniqueState_Car &uscar)
    {
        return os << "[ " << uscar.owner << ", " << uscar.address << " ]";
    }
};

class FlyWeight
{
private:
    SharedState_Car *shared_state;

public:
    FlyWeight(const SharedState_Car *ss) : shared_state(new SharedState_Car(*ss))
    {
    }
    FlyWeight(const FlyWeight *fly_wt) : shared_state(new SharedState_Car(*fly_wt->shared_state))
    {
    }

    ~FlyWeight()
    {
        delete shared_state;
    }

    SharedState_Car *get_shared_state() const
    {
        return shared_state;
    }
    
    void Operation (const UniqueState_Car &us_car) const
    {
        cout << "Flyweight displaying "  << us_car << " and " <<  *shared_state << endl;
    }
};


class FlyWeightFactory
{
private:
    unordered_map<string, FlyWeight> flyweight;

    string GetKey(const SharedState_Car &ss_car)
    {
        return ss_car.brand + ss_car.model + ss_car.color;
    }
       

public:
    FlyWeightFactory(initializer_list <SharedState_Car> shared_states)
    {
        for (const SharedState_Car &ss : shared_states)
        {
            this->flyweight.insert(make_pair<string, FlyWeight> (this->GetKey(ss), FlyWeight(&ss)));
        }
    }
    
    FlyWeight GetFlyWeight(const SharedState_Car &shared_state)
    {
        string key = this->GetKey(shared_state);
        if (this->flyweight.find(key) == this->flyweight.end())
            cout << "FlyweightFactory: Can't find flyweight, creating new one \n";
        else
            cout << "Reusing existing flyweight \n";
        return this->flyweight.at(key);
    }
    void ListFlyWeight() const
    {
        size_t count = this->flyweight.size();
        cout << "Flyweight Factory: I have " << count << "flyweights \n";
        for (pair<string, FlyWeight> pair : this->flyweight )
            cout << pair.first << endl;
    }

};


void AddCarToPoliceDatabase(
        FlyWeightFactory &ff, const string &plates, const string &owner,
        const string &brand, const string &model, const string &color
)
{
    cout << "\n Adding Car to data base \n";
    const FlyWeight &flyweight = ff.GetFlyWeight({brand, model, color});
    flyweight.Operation({plates, owner});
}


int main() {
    FlyWeightFactory *factory =
            new FlyWeightFactory({{"Mitsubishi", "Pajero", "Blue"},
                                  {"Toyota",     "Kluger", "Silver"},
                                  {"Toyota",     "Hilux",  "Grey"}});

    factory->ListFlyWeight();

    AddCarToPoliceDatabase(*factory, "UDD610", "Austin", "Toyota", "Kluger", "Silver");
    AddCarToPoliceDatabase(*factory, "KA01MH7022", "Austin", "Toyota", "Innova", "Champagne");

}






