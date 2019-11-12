// ===================================
// test file made for jvm-impl
// ===================================

public class other {

    public static math M_math;

    public static void main(String[] args) {
        other.M_math = new math();

        System.out.println(M_math.add());
        System.out.println(math.pi);

        String s = "Hello World";
        System.out.println(s);

        for(String str : args)
            System.out.println(str);

    }

}
