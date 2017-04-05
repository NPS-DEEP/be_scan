import java.util.Arrays;

public final class TestByte {


  public static void main(String[] args) {

  StringBuilder sb = new StringBuilder();
    for (int i=0; i<256; i++) {
      byte b=(byte)i;
      char c = (char)b;
      if (c < ' ' || c > '~' || c == '\\') {
        sb.append(String.format("\\x%02X", (int)c&0xff));
//        System.out.println((int)c + ", " + String.format("\\x%02X", (int)c&0xff));
      } else {
        sb.append(c);
//        System.out.println(c);
      }
    }
    System.out.println(sb.toString());

/*
    StringBuilder sb = new StringBuilder();
    sb.append("\\x");
    sb.append((char)((c/16)+'0'));
    sb.append((char)((c&0x0f)+'0'));
 
    System.out.println(sb.toString());
*/
  }
}

