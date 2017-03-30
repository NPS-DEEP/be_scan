public final class Tests {

  static {
    System.load(new java.io.File(".libs/libbe_scan_jni.so").getAbsolutePath());
  }

  private static void testEquals(String a, String b) {
    if (!a.equals(b)) {
      throw new RuntimeException("Error: '" + a + "' not equal to '" + b + "'");
    }
  }

  private static void testVersion() {
    String version = edu.nps.deep.be_scan.be_scan_jni.version();
    System.out.println("Version: " + version);
  }

  private static void testAvailableScanners() {
    String available_scanners =
                      edu.nps.deep.be_scan.be_scan_jni.available_scanners();
    System.out.println("Available scanners: '" + available_scanners + "'");
  }

  private static void testBuffer1() {
    byte[] buffer1 = "someone@somewhere.com\tsomeone2@somewhere2.com\n".getBytes();
    System.out.println("test buffer 1 size: " + buffer1.length);
    edu.nps.deep.be_scan.BEScan scanner =
            new edu.nps.deep.be_scan.BEScan("email", buffer1, buffer1.length);

    edu.nps.deep.be_scan.Artifact artifact;
    artifact = scanner.nextArtifact();
    testEquals(artifact.getArtifactClass(), "email");
    testEquals(artifact.getArtifact(), "someone@somewhere.com");
    testEquals(artifact.getContext(), "someone@somewhere.com\tsomeone2@somewh");

    artifact = scanner.nextArtifact();
    testEquals(artifact.getArtifactClass(), "email");
    testEquals(artifact.getArtifact(), "someone2@somewhere2.com");
    testEquals(artifact.getContext(), "e@somewhere.com\tsomeone2@somewhere2.com\n");

    artifact = scanner.nextArtifact();
    testEquals(artifact.getArtifactClass(), "");
    testEquals(artifact.getArtifact(), "");
    testEquals(artifact.getContext(), "");
  }

  public static void main(String[] args) {

    testVersion();
    testAvailableScanners();
    testBuffer1();
  }
}

