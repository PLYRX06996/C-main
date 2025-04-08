import java.util.Scanner;
public class MultiplicationTable {
    public static void main(String[] args) {
        Scanner scan = new Scanner(System.in);
        System.out.println("Enter a number to show it's\nMultilication Table.");
        int N = scan.nextInt();
        int i = 1;
        while(i <= 10){
            System.out.printf(N*i + " ");
            i++;
        }
    }
}
