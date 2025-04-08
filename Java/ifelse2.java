import java.util.Scanner;
public class ifelse2 {
    public static void main(String[] args) {
        Scanner obj1 = new Scanner(System.in);
    int x = obj1.nextInt();
    if(x % 5 == 0){
        System.out.println("x is divisible by 5");
    } else System.out.println("x is not divisible by 5");
    }
}
