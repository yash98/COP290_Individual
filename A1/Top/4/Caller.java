public class Caller {
    public static void main(String[] args) {
        Sqr s = new Sqr();
        System.out.print(s.square(Integer.parseInt(args[0])));
    }
}