import java.util.Arrays;

public final class Tests {

  static {
    System.load(new java.io.File(".libs/libbe_scan_jni.so").getAbsolutePath());
  }

  private static void testEquals(String a, String b) {
    if (!a.equals(b)) {
      throw new RuntimeException("Error: '" + a + "' not equal to '" + b + "'");
    }
  }

  private static void testEquals(long a, long b) {
    if (a != b) {
      throw new RuntimeException("Error: " + a + " not equal to " + b);
    }
  }

  private static void testEquals(boolean a, boolean b) {
    if (a != b) {
      throw new RuntimeException("Error: " + a + " not equal to " + b);
    }
  }

  private static void testEquals(byte[] a, byte[]b) {
    if (!Arrays.equals(a, b)) {
      throw new RuntimeException("Error: " + new String(a) + " not equal to " + new String(b));
    }
  }

  private static String escape(String input) {
    StringBuilder sb = new StringBuilder();
    for (int i=0; i<input.length(); ++i) {
      char c = input.charAt(i);
      if (c < ' ' || c > '~' || c == '\\') {
        // show as \xXX
        sb.append(String.format("\\x%02X", (int)c&0xff));
      } else {
        // show ascii character
        sb.append(c);
      }
    }
    return sb.toString();
  }

  private static void testVersion() {
    String version = edu.nps.deep.be_scan.be_scan_jni.version();
    System.out.println("Version: " + version);
  }

  private static void testAvailableScanners() {
    String availableScanners =
                      edu.nps.deep.be_scan.be_scan_jni.availableScanners();
    System.out.println("Available scanners: '" + availableScanners + "'");
  }

static byte[] buffer1 = "someone@somewhere.com\0someone2@somewhere2.com\n".getBytes();
  private static void testBuffer1() {

/*
// http://stackoverflow.com/questions/12192624/swig-convert-return-type-stdstringbinary-to-java-byte
edu.nps.deep.be_scan.Bar b = new edu.nps.deep.be_scan.Bar();
byte[] bb = b.foo();
System.out.println("bb length: " + bb.length);
System.out.println(bb[0] + "," + bb[1] + "," + bb[2] + "," + bb[3] + "," + bb[4] + ".");
testEquals(bb, "Ho\0la".getBytes());
*/

//    static byte[] buffer1 = "someone@somewhere.com\tsomeone2@somewhere2.com\n".getBytes();
    System.out.println("test buffer 1 size: " + buffer1.length);
    edu.nps.deep.be_scan.BEScan scanner =
//            new edu.nps.deep.be_scan.BEScan("email", buffer1);
            new edu.nps.deep.be_scan.BEScan("email", buffer1, buffer1.length);

    testEquals(scanner.getIsInitialized(), true);

    edu.nps.deep.be_scan.Artifact artifact;
    artifact = scanner.nextArtifact();
    testEquals(artifact.getArtifactClass(), "email");
    testEquals(artifact.getBufferOffset(), 0);
    testEquals(artifact.javaArtifact(), "someone@somewhere.com".getBytes());
    testEquals(artifact.javaContext(), "someone@somewhere.com\0someone2@somewh".getBytes());

    artifact = scanner.nextArtifact();
    testEquals(artifact.getArtifactClass(), "email");
    testEquals(artifact.getBufferOffset(), 22);
    testEquals(artifact.javaArtifact(), "someone2@somewhere2.com".getBytes());
    testEquals(artifact.javaContext(), "e@somewhere.com\0someone2@somewhere2.com\n".getBytes());

    artifact = scanner.nextArtifact();
    testEquals(artifact.getArtifactClass(), "");
    testEquals(artifact.getBufferOffset(), 0);
    testEquals(artifact.javaArtifact(), "".getBytes());
    testEquals(artifact.javaContext(), "".getBytes());
  }

  // make sure 0x00 and 0x0ff go through JNI
  public static void testBytes() {
    final char x00 = '\0';
    final char x7f = (char)((byte)127);
    final char x80 = (char)((byte)128);
    final char xfd = (char)((byte)253);
    final char xfe = (char)((byte)254);
    final char xff = (char)((byte)255);
    StringBuilder sb = new StringBuilder();
    sb.append(x00); sb.append(x7f); sb.append(x80); 
    sb.append(xfd); sb.append(xfe); sb.append(xff); 
    sb.append("someone3@somewhere3.com");
    sb.append(x00); sb.append(x7f); sb.append(x80); 
    sb.append(xfd); sb.append(xfe); sb.append(xff); 
    final byte[] buffer = sb.toString().getBytes();
    edu.nps.deep.be_scan.BEScan scanner =
//            new edu.nps.deep.be_scan.BEScan("email", buffer);
            new edu.nps.deep.be_scan.BEScan("email", buffer, buffer.length);
    edu.nps.deep.be_scan.Artifact artifact;
    artifact = scanner.nextArtifact();
    testEquals(artifact.javaArtifact(), "someone3@somewhere3.com".getBytes());
    testEquals(artifact.javaContext(), buffer);

    // also test escape function
    final String escapedString="\\x00\\x7F\\x80\\xFD\\xFE\\xFFsomeone3@somewhere3.com\\x00\\x7F\\x80\\xFD\\xFE\\xFF";
    testEquals(escape(new String(artifact.javaContext(), java.nio.charset.Charset.forName("UTF-8"))), escapedString);
    testEquals(escape(new String(artifact.javaContext())), escapedString);
  }

  public static void main(String[] args) {

    testVersion();
    testAvailableScanners();
    testBuffer1();
    testBytes();
  }
}

