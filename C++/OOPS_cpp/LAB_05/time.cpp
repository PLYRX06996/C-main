#include<iostream>
#include<time.h>

using namespace std;

class time {
    private:
    int hours;
    int minutes;
    int seconds;
    public:
    void getTime(){
        cout << "Enter the time in 24 hour format (HH:MM:SS): ";
        cin >> hours >> minutes >> seconds;
    }
   
    void displayTime(){
        if(seconds > 59){
            minutes = minutes + (seconds / 60);
            seconds = seconds % 60;
        }
        if(minutes > 59){
            hours = hours + (minutes / 60);
            minutes = minutes % 60;
        }
        if(hours > 23){
            int daysAhead = hours / 24;
            hours = hours % 24;
            if(daysAhead > 0){
                cout << "You are "<< daysAhead << " days ahead of the current time-line." << endl;
            }
        }
        cout << hours << ":" << minutes << ":" << seconds << endl;
    }

} T;

int main() {

    T.getTime();
    T.displayTime();
    
    return 0;
}