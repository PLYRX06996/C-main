import java.lang.reflect.Array;
import java.util.Arrays;
import java.util.Scanner;
public class Main {
    public static int Sum(int n1, int n2){
        return n1 + n2;
    }
    public static void main(String[] args) {
//        System.out.println("Hello, World!");
//        System.out.println("My name is Abdul Qadir.");
        // Variables --> same as C/C++
        //Data types
        /*1. Primitive type
        byte - 1 byte
        short - 2
        int - 4
        float - 4 use postfix f/F
        long - 8 use postfix l/L and it is an LONG INTEGER
        double - 8
        char - 2
        boolean - 1
        * 2. Non-primitive type
        all non-primitives ko new keyword se declare karna hai
        <type> <name> = new <value>;
        string */
        //Operations on String
        // concatenation
        String s1 = new String("Abdul Qadir");
        String s2 = new String("Memon");
//        System.out.println(s1 + " " + s2);
        System.out.println(s1.charAt(0));
        //replace
        s1.replace('a','u');
//        System.out.println(s1); // it will not change the original string
        String s3 = s1.replace('a','u');
        System.out.println(s3);
        //sub-string
        //string are immutable in java
        System.out.println(s1.substring(0,5));// agar 5 th position tal chahiye to 3 aakhir me likho
        System.out.println(s1.substring(6,11));

        //Arrays
        int[] arr = new int[5];
//        for (int i = 0; i < 5; i++){
//            arr[i] = i+10;
//        }
        System.out.println(arr[0]);
//        System.out.println(arr[1]);
//        System.out.println(arr[2]);
//        System.out.println(arr[3]);
//        System.out.println(arr[4]);
        System.out.println(arr.length); // length is not a method or a function in java
        Arrays.sort(arr); // array is sorted
        int[] marks = {70, 76, 88, 95, 62};
        int[][] final_marks = {{97, 98, 95}, {95, 96, 99}};
        System.out.println(final_marks[0][2]);

        // Casting -->> Implicit and Explicit
        double price = 100.00;
        double final_price = price + 18;
        System.out.println(final_price);
        int p = 100;
        int FP = p + (int)18.99; //double-->int
        System.out.println(FP);

        //Constants
        final float PI = 3.141592F;// final(java) == const(C/C++)

        //Operators
        /*1. Arithmatic (+, -, *, /, %)
        * 2. Assignment (=, +=, -=, *= /=)
        * 3. Unary (c++, ++c, c--, --c)
        * 4. Comparison (>, <, >=, <=, ==, !=)
        * 5. Logical (&&, ||, != ==) */

        // ---------- MATH CLASS -------------
        System.out.println(Math.max(20, 20.000000001));
        // random function --> gives a random number in range 0.0 - 1.0
        System.out.println(Math.random());

        /*--------------------------------------------*/
        // Taking input from the user
        Scanner obj1 = new Scanner(System.in);
        System.out.print("Input you Salary: ");
        float salary = obj1.nextFloat();
        System.out.println("Your salary is " + salary);
        obj1.nextLine();
        // Input of string
        System.out.println("Input you Name:");
        String name = obj1.nextLine();
        // if used obj1.next(); space ka problem aayega
        System.out.println("Your name is " + name);
        // same if-else, switch case, for loops, while loop, do while loop
        int number;
        do{
            System.out.println("Enter a number: ");
            number = obj1.nextInt();
            System.out.println("Your input number is:" + number);
        } while (number >= 0);
        System.out.println("-----THE END-----");

        //Exception Handling TRY - CATCH
        // Errors != Exceptions; we cannot handle errors or catch it.
        int[] sub = {67, 95, 78};
        try{
            System.out.println(sub[2]);
        } catch (Exception e1) {
            // do things after catching
        }
        System.out.println(Sum(sub[0], sub[1]));
        System.out.println(Sum(sub[0], sub[2]));
        System.out.println(Sum(sub[1], sub[2]));

        classNobj Obj1 = new classNobj();
        Obj1.name = "Pikachu";
        Obj1.level = 10;
        System.out.println(Obj1.name + " "  + Obj1.level);

        classNobj obj2 = new classNobj();
        obj2.name = "Eevee";
        obj2.level = 20;
        obj2.attack();
    }
}