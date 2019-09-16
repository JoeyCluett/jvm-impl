

public class math {

    private float a;
    private float b;

    public float get_a() {       return this.a; }
    public float get_b() {       return this.b; }
    public void set_a(float a) { this.a = a; }
    public void set_b(float b) { this.b = b; }

    public float multiply() { return a * b; }
    public float divide() {   return a / b; }
    public float add() {      return a + b; }
    public float subtract() { return a - b; }

    public static final float pi = 3.14159265f;

    public math() {
        this.a = 3.141592654f;
        this.b = 2.0f;
    }

}
