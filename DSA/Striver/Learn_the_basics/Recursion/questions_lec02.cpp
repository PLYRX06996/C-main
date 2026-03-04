#include <bits/stdc++.h>
using namespace std;
// string makeStrforCheck(string s){
// 	string S;
// 	for(int pt = 0; pt < s.size(); pt++){
// 		if((s[pt] >= 48 && s[pt] <= 57) || (s[pt] >= 97 && s[pt] <= 122)) S.push_back(s[pt]);
// 		else if(s[pt] >= 65 && s[pt] <= 90) S.push_back(s[pt] + 32);
// 	}
// 	return S;
// }

string makeStrforCheck(string s) //const string& s --> good practice, not necessary
{
	string S;
	for (char c : s) //unsigned char c : s --> good practice, not necessary
	{
		if (isalnum(c))
		{
			S.push_back(tolower(c));
		}
		// cout << S << endl;
	}
	return S;
} //---> good practice

void printFiboncci(int n){
	if(n == 0 || n == 1) cout << n << " ";
	

}

bool isPalindrome(string s, int l, int r)
{
	// cout << s;
	if (l >= r)
		return true; // we passed half the mark w/o returning false, so the condition is true.
	if (s[l] != s[r])
		return false; // simple check, and pelli fursat me nikal
	return isPalindrome(s, l + 1, r - 1);
}

void revArr(int arr[], int l, int r)
{
	if (l < r) // imp if done i < n then it will be reversed in original order
	{
		// cout << i << n << " " << endl;
		swap(arr[l], arr[r]); // imp
		// we can also use two pointer to track both the i & n, coz currently we have only one pointer i, and based on that we are changnig t's value... I guess two pointer are changed independently.
		revArr(arr, l + 1, r - 1);
	}
	return;
}

int factN(int n)
{
	if (n == 0)
		return 1;
	return n * factN(--n);
}

void factorialN(int prod, int n)
{
	if (n == 0)
	{
		cout << prod << endl;
		return;
	}
	prod *= n;
	factorialN(prod, --n);
}

void sumofN(int sum, int n)
{
	if (n == 0)
	{
		cout << sum << endl;
		return;
	}
	sum = sum + n;
	sumofN(sum, --n);
}

void printrevNnum(int n)
{
	if (n == 0)
	{
		cout << endl;
		return;
	}
	cout << n << " ";
	printrevNnum(--n);
}

void printNnum(int i, int n)
{
	if (i == n)
	{
		cout << endl;
		return;
	}
	cout << i << " ";
	printNnum(++i, n);
}

void printName(int n)
{
	if (n == 0)
		return;
	cout << "Abdul" << endl;
	printName(--n);
}

int main()
{
	int n;
	cin >> n;
	// printName(n);
	// printrevNnum(n);
	// // cout << n << endl;
	// printNnum(1, n);
	// // cout << n << endl;
	// sumofN(0, n);
	// factorialN(1, n);
	// factN(n);
	// int arr[n];
	// for(int i = 0; i < n; i++) cin >> arr[i];
	// revArr(arr, 0, n-1);
	// for(int i = 0; i < n; i++) cout << arr[i] << " ";
	// cout << endl;
	// // for int array=> never use int i : arr -> it will give segmenttatiion fault always
	
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //  FIX for string input after an integer

	string s;
	// cin >> s;
	getline(cin, s);
	s = makeStrforCheck(s);
	cout << "[" << s << "]\n";
	cout << isPalindrome(s, 0, s.size() - 1) << endl;


	return 0;
}