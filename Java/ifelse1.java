import java.util.Scanner;
public class ifelse1 {
    public static void main(String[] args) {
        Scanner Obj1 = new Scanner(System.in);
        int x = Obj1.nextInt();
        if(x == 0){
            System.out.println("The given number is zero");
        } else if(x < 0){
            System.out.println("The given number is negative");
        } else {
            System.out.println("The given number is positive");
        }
    }
}
