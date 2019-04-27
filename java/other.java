// ===================================
// test file made for jvm-impl
// ===================================

public class other {

    public void say_hello() {
        System.out.println("Hello World");
    }

    public static final int b    = -1;
    public static final long c   = 3L;
    public static final float d  = 4.0f;
    public static final double e = 5.0;

    public int get_int() {
        return other.b;
    }

    public long get_long() {
        return other.c;
    }

    public float get_float() {
        return other.d;
    }

    public double get_double() {
        return other.e;
    }
    public static void main(String[] args) {
        System.out.println(other.b);
        System.out.println(other.b);
    }

}
