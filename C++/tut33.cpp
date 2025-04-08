#include <iostream>
#include <conio.h>
using namespace std;

class bankDeposite
{
    float principal;
    float interestRate;
    int timeY;
    float returnValue;

public:
    bankDeposite() {}
    // bankDeposite(float p, int r, int y);
    // bankDeposite(int p, float r, int y);
    bankDeposite(float p, float r, int y);
    void show();
};
// bankDeposite::bankDeposite(float p, int r, int y)
// {
//     principal = p;
//     interestRate = (float)r / 100;
//     timeY = y;
//     returnValue = principal;
//     for (int i = 0; i < timeY; i++)
//     {
//         returnValue = returnValue * (1 + interestRate);
//     }
// }
bankDeposite::bankDeposite(float p, float r, int y)
{
    principal = p;
    interestRate = (r>1) ? r/100 : r;
    timeY = y;
    returnValue = principal;
    for (int i = 0; i < timeY; i++)
    {
        returnValue = returnValue * (1 + interestRate);
    }
}
void bankDeposite::show()
{
    cout << "Your principal amount was: " << principal << " After investing for " << timeY << " years, your portfolio is: " << returnValue << endl;
}
int main()
{
    bankDeposite bd1, bd2, bd3;
    float p, r;
    int y;
    cout << "Enter your principal amount: ";
    cin >> p;
    // char key;
    // cout << "Press 'R' for entering interest rate in %\nOr press 'r' for entering interest rate in fraction [0-1]" << endl;
    // key = _getch();
    // cout << "\nPressed key: " << (char)_putch(key) << endl;
    // if (key == 'R')
    // {
    //     cout << "Enter interest rate: ";
    //     cin >> r;
    // }
    // else if (key == 'r')
    // {
    //     cout << "Enter interest rate: ";
    //     cin >> r/100;
    // }
    cout << "Enter interest rate: ";
    cin >> r;
    cout << "Enter your investment time-period(in years): ";
    cin >> y;
    bd1 = bankDeposite(p, static_cast<float>(r), y);
    bd1.show();

    cout << "Enter your principal amount: ";
    cin >> p;
    cout << "Enter interest rate: ";
    cin >> r;
    cout << "Enter your investment time-period(in years): ";
    cin >> y;
    bd2 = bankDeposite(p, static_cast<float>(r), y);
    bd2.show();
    

    return 0;
}