#include <iostream>
#include <math.h>
using namespace std;
class point
{
    int x, y;

public:
    friend float distance(point a, point b);
    point(int a, int b)
    {
        x = a;
        y = b;
    }
    void displayPoint()
    {
        cout << "The point is: (" << x << "," << y << ")" << endl;
    }
};
float distance(point a, point b)
{
    return sqrt((pow(a.x - b.x, 2)) + (pow(a.y - b.y, 2)));
}
int main()
{
    point p1(1, 2);
    p1.displayPoint();
    point p2(5, 4);
    p2.displayPoint();
    cout << "The distance b/w p1 and p2 is: " << distance(p1, p2) << endl;
    return 0;
}