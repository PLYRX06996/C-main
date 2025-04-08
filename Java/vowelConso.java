import java.util.Scanner;
public class vowelConso {
    public static void main(String[] args) {
        Scanner myObj = new Scanner(System.in);
        System.out.println("Enter a character: ");
        char c = myObj.next().charAt(0);
        if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u'){
            System.out.println("The entered character is a vowel.");
        } else System.out.println("The entered character is a consonant.");
    }
}
