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

  private static String escape(byte[] bytes) {
    StringBuilder sb = new StringBuilder();
    for (int i=0; i<bytes.length; ++i) {
      char c = (char)bytes[i];
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
  // NOTE: Beware using getBytes() because it adds additional bytes for
  // negative characters.  Use with positive characters only.
  private static void testBuffer1() {

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

  // make sure byte patterns make it out and back through JNI
  public static void testLoopback() {
    // set up byte pattern
    byte[] bytes = new byte[256];
    for (int i=0; i<256; i++) {
      bytes[i]=(byte)i;
    }

    // open scanner with byte pattern
    edu.nps.deep.be_scan.BEScan scanner =
//            new edu.nps.deep.be_scan.BEScan("email", buffer1);
            new edu.nps.deep.be_scan.BEScan("email", bytes, bytes.length);
//    System.out.println(escape(bytes));
//    System.out.println(escape(scanner.javaTestLoopback()));
    testEquals(escape(scanner.javaTestLoopback()), "\\x00\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\x08\\x09\\x0A\\x0B\\x0C\\x0D\\x0E\\x0F\\x10\\x11\\x12\\x13\\x14\\x15\\x16\\x17\\x18\\x19\\x1A\\x1B\\x1C\\x1D\\x1E\\x1F !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\x5C]^_`abcdefghijklmnopqrstuvwxyz{|}~\\x7F\\x80\\x81\\x82\\x83\\x84\\x85\\x86\\x87\\x88\\x89\\x8A\\x8B\\x8C\\x8D\\x8E\\x8F\\x90\\x91\\x92\\x93\\x94\\x95\\x96\\x97\\x98\\x99\\x9A\\x9B\\x9C\\x9D\\x9E\\x9F\\xA0\\xA1\\xA2\\xA3\\xA4\\xA5\\xA6\\xA7\\xA8\\xA9\\xAA\\xAB\\xAC\\xAD\\xAE\\xAF\\xB0\\xB1\\xB2\\xB3\\xB4\\xB5\\xB6\\xB7\\xB8\\xB9\\xBA\\xBB\\xBC\\xBD\\xBE\\xBF\\xC0\\xC1\\xC2\\xC3\\xC4\\xC5\\xC6\\xC7\\xC8\\xC9\\xCA\\xCB\\xCC\\xCD\\xCE\\xCF\\xD0\\xD1\\xD2\\xD3\\xD4\\xD5\\xD6\\xD7\\xD8\\xD9\\xDA\\xDB\\xDC\\xDD\\xDE\\xDF\\xE0\\xE1\\xE2\\xE3\\xE4\\xE5\\xE6\\xE7\\xE8\\xE9\\xEA\\xEB\\xEC\\xED\\xEE\\xEF\\xF0\\xF1\\xF2\\xF3\\xF4\\xF5\\xF6\\xF7\\xF8\\xF9\\xFA\\xFB\\xFC\\xFD\\xFE\\xFF");
  }

  public static void main(String[] args) {

    testVersion();
    testAvailableScanners();
    testBuffer1();
    testLoopback();
  }
}

