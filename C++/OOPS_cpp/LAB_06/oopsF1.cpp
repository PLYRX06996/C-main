#include <iostream>
using namespace std;
#define K 20
typedef struct{
    unsigned int quantity = 0;
    double price;
    unsigned int year;
} edition;
class book
{
    string title;
    string author;
    string ISBN;
    edition E[K];
public:
    book(){}
    book(string title, string author, string ISBN, double price, unsigned int e){
        this->title = title;
        this->author = author;
        this->ISBN = ISBN;
        this->E[e].price = price;
    }
    void addBook(book *B, unsigned int quantity, unsigned int e);
    void display(book *B, unsigned int e);
    bool isavailable(book *B, unsigned int e);
    void checkout(book *B, unsigned int e);
    void returnBook(book *B, unsigned int e);
};
bool book::isavailable(book *B, unsigned int e)
{
    return B->E[e].quantity != 0;
}
void book::addBook(book *B, unsigned int quantity, unsigned int e)
{
    B->E[e].quantity += quantity;
}
void book::checkout(book *B, unsigned int e)
{
    B->E[e].quantity--;
}
void book::returnBook(book *B, unsigned int e)
{
    B->E[e].quantity++;
}
void book::display(book *B, unsigned int e)
{
    if (isavailable(B, e))
    {
        cout << "Title: " << B->title << endl;
        cout << "Author: " << B->author << endl;
        cout << "ISBN: " << B->ISBN << endl;
        cout << "Edition: " << e << endl;
        cout << "Quantity: " << B->E[e].quantity << endl;
        cout << "Price: " << B->E[e].price << endl;
    }
    else
    {
        cout << "The book is not available" << endl;
    }
}
int main()
{
    book *B1 = new book("Object-Oriented Programming in C++", "Robert Lafore", "0672323087", 5659.77, 4);
    book *B2 = new book("The C Programming Language", "Dennis M. Ritchie", "0131103628", 2183.75, 2);
    book *B3 = new book("The C++ Programming Language", "Bjarne Stroustrup", "0321563840", 4792.57, 4);
    book *B4 = new book("C++ Primer", "Stanley B. Lippman", "0321714113", 5832.45, 5);
    book *B5 = new book("Object-Oriented Programming with C++", "E. Balagurusamy", "9352607996", 611.45, 7);    
    B1->addBook(B1, 5, 4);
    B1->checkout(B1, 4);
    B1->checkout(B1, 4);
    B1->checkout(B1, 4);
    B1->display(B1, 4);
    B1->returnBook(B1, 4);
    B1->display(B1, 4);
    return 0;
}