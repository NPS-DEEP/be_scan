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
    edu.nps.deep.be_scan.ScanEngine scanEngine =
                             new edu.nps.deep.be_scan.ScanEngine("email");

    edu.nps.deep.be_scan.Scanner scanner =
        new edu.nps.deep.be_scan.Scanner(scanEngine, "unused output filename");

    testEquals(scanner.scan("test_buffer1", java.math.BigInteger.valueOf(0), "", buffer1, buffer1.length),
               "");
/*



    testEquals(scanner.getBadAlloc(), false);

    edu.nps.deep.be_scan.Artifact artifact;
    artifact = scanner.next();
    testEquals(artifact.getArtifactClass(), "email");
    testEquals(artifact.getBufferOffset(), 0);
    testEquals(artifact.javaArtifact(), "someone@somewhere.com".getBytes());
    testEquals(artifact.javaContext(), "someone@somewhere.com\0someone2@somewh".getBytes());

    artifact = scanner.next();
    testEquals(artifact.getArtifactClass(), "email");
    testEquals(artifact.getBufferOffset(), 22);
    testEquals(artifact.javaArtifact(), "someone2@somewhere2.com".getBytes());
    testEquals(artifact.javaContext(), "e@somewhere.com\0someone2@somewhere2.com\n".getBytes());

    artifact = scanner.next();
    testEquals(artifact.getArtifactClass(), "");
    testEquals(artifact.getBufferOffset(), 0);
    testEquals(artifact.javaArtifact(), "".getBytes());
    testEquals(artifact.javaContext(), "".getBytes());
*/
  }

/*
  // check artifact accessor names
  private static void testArtifactAccessors() {
    edu.nps.deep.be_scan.Artifact artifact = new 
                                   edu.nps.deep.be_scan.Artifact();
    testEquals(artifact.getArtifactClass(), "");
    testEquals(artifact.getBufferOffset(), 0);
    testEquals(artifact.javaArtifact(), "".getBytes());
    testEquals(artifact.javaContext(), "".getBytes());
    testEquals(artifact.hasNewBuffer(), false);
    artifact.deleteNewBuffer();
  }
*/

  public static void main(String[] args) {

    testVersion();
    testAvailableScanners();
    testBuffer1();
//    testArtifactAccessors();
  }
}

