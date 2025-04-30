#include<iostream>
#include<cmath>
#include<cstring>
using namespace std;
/*----------- Pure Virtual function ----------*/
class CWH {
    protected:
    string title;
    float rating;
    public:
    CWH(string s, float f) : title(s), rating(f) {}
    virtual void display() = 0; // Duniya kehti hai hamko Pureee
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