import java.util.Scanner;
public class leapY {
    public static void main(String[] args) {
        Scanner year = new Scanner(System.in);
        int y = year.nextInt();
        if(y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)){
            System.out.println("The given year is a leap year.");
    } else System.out.println("The given year is not a leap year.");
}
}