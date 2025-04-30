#include<iostream>
#include<cmath>
#include<cstring>
using namespace std;
class CWH {
    protected:
    string title;
    float rating;
    public:
    CWH(string s, float f) : title(s), rating(f) {}
    virtual void display() { cout << "bogus code" << endl;}
};
class CWHvideo : public CWH {
    float vidLen;
    public:
    CWHvideo(string s, float f, float vl) : CWH(s, f), vidLen(vl) {}
    void display() {
        cout << "This is an amazing video with title "<< title << endl;
        cout << "Ratings:"<< rating << " out of 5." << endl;
        cout << "Lenght of the video is: "<< vidLen <<  endl; 
    }
};
class CWHtext : public CWH {
    int wordCount;
    public:
    CWHtext(string s, float f, int tl) : CWH(s, f), wordCount(tl) {}
    void display() {
        cout << "This is an amazing video with title "<< title << endl;
        cout << "Ratings of this text tutorial is: "<< rating << " out of 5" << endl;
        cout << "Number of words in this yext tutorial is: "<< wordCount <<  endl; 
    }
};
int main() { 
    string title;
    float vidlen, rating;
    int wordC;

    title = "Django tutorial"; vidlen = 4.56; rating = 4.89;
    CWHvideo djvideo(title, rating, vidlen);
    // djvideo.display();

    title = "Django text tutorial "; wordC = 433; rating = 4.19;
    CWHtext djtext(title, rating, wordC);
    // djtext.display();

    CWH *tuts[2];
    tuts[0] = &djvideo;
    tuts[1] = &djtext;
    
    tuts[0]->display();
    printf("\n");
    tuts[1]->display();
    return 0;
}

/*--------- Rules for Virtual Functions ---------
1. They cannot be static.
2. They are accessed by only object pointers
3. Virtual functions can be a freind with another class.
4. A virtual function in base class might not be used.
5. If a virtual function is defined in a base class, then
there is no need to redefind virtual function in derived class.
6. Like jab bhi display() call hoga tab vo dhoonde ga ki Derived class me definition hai ki nahi.
Agar definition nahi mili to vo virtual void display() --> base class ka run kar dega.*/