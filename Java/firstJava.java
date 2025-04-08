import java.util.Scanner;
public  class firstJava {
    public static void main(String[] args) {
        Scanner myObj = new Scanner(System.in);
        short s = myObj.nextShort();
        int x = myObj.nextInt();
        byte b = myObj.nextByte();
        float f = myObj.nextFloat();
        double d = myObj.nextDouble();
        boolean B = myObj.nextBoolean();
        long l = myObj.nextLong();
        String S = myObj.nextLine();
        System.out.println(S + "\t" + s + "\t" + x + "\t" + b + "\t" + f + "\t" + d + "\t" + B + "\t" + l);
        
    }
}