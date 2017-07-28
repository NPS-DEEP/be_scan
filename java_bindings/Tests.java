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

  private static void testBuffer() {

    // NOTE: Beware using getBytes() because it adds additional bytes for
    // negative characters.  Use with positive characters only.
    final byte[] previousBuffer = "".getBytes();
    final byte[] buffer = "someone@somewhere.com\0someone2@somewhere2.com\n".getBytes();

    System.out.println("test buffer size: " + buffer.length);
    edu.nps.deep.be_scan.ScanEngine scanEngine =
                             new edu.nps.deep.be_scan.ScanEngine("email");
    testEquals(scanEngine.getStatus(), "");

    edu.nps.deep.be_scan.Scanner scanner =
        new edu.nps.deep.be_scan.Scanner(scanEngine);

    String status = scanner.scanSetup("stream name", 0, "recursion prefix");
    testEquals(status, "");
    status = scanner.scan(previousBuffer, buffer);
    testEquals(status, "");
    status = scanner.scanFinalize();
    testEquals(status, "");

    // check artifact interfaces
    testEquals(scanner.empty(), false);

    edu.nps.deep.be_scan.Artifact artifact;
    artifact = scanner.get();
    testEquals(artifact.getArtifactClass(), "email");
    testEquals(artifact.getStreamName(), "stream name");
    testEquals(artifact.getRecursionPrefix(), "recursion prefix");
    testEquals(artifact.getOffset(), 0);
    testEquals(artifact.javaArtifact(), "someone@somewhere.com".getBytes());
    testEquals(artifact.javaContext(), "someone@somewhere.com\0someone2@somewh".getBytes());

    testEquals(scanner.empty(), false);

    artifact = scanner.get();
    testEquals(artifact.getArtifactClass(), "email");
    testEquals(artifact.getStreamName(), "stream name");
    testEquals(artifact.getRecursionPrefix(), "recursion prefix");
    testEquals(artifact.getOffset(), 22);
    testEquals(artifact.javaArtifact(), "someone2@somewhere2.com".getBytes());
    testEquals(artifact.javaContext(), "e@somewhere.com\0someone2@somewhere2.com\n".getBytes());

    testEquals(scanner.empty(), true);

    artifact = scanner.get();
    testEquals(artifact.getArtifactClass(), "");
    testEquals(artifact.getStreamName(), "");
    testEquals(artifact.getRecursionPrefix(), "");
    testEquals(artifact.getOffset(), 0);
    testEquals(artifact.javaArtifact(), "".getBytes());
    testEquals(artifact.javaContext(), "".getBytes());
    testEquals(artifact.toString(), "  0		");
  }

  private static void testEscape() {
    testEquals(edu.nps.deep.be_scan.be_scan_jni.escape("a\0b"), "a\\xC0\\x80b");
    testEquals(edu.nps.deep.be_scan.be_scan_jni.javaEscape(
                                            "a\0b".getBytes()), "a\\x00b");
  }

  public static void main(String[] args) {
    testVersion();
    testAvailableScanners();
    testBuffer();
    testEscape();
  }
}

