public final class Tests {

  static {
//    System.loadLibrary("be_scan_jni");
    System.load(new java.io.File(".libs/libbe_scan_jni.so").getAbsolutePath());
  }

  private static void testVersion() {
    String version = edu.nps.deep.be_scan.be_scan_jni.version();
    System.out.println("Version: " + version);
  }

  private static void testAvailableScanners() {
    String available_scanners =
                      edu.nps.deep.be_scan.be_scan_jni.available_scanners();
    System.out.println("Available scanners: " + available_scanners);
  }

  private static void testBuffer1() {
    byte[] buffer1 = "someone@somewhere.com\nsomeone2@somewhere2.com\n".getBytes();
    System.out.println("test buffer 1 size: " + buffer1.length);
    edu.nps.deep.be_scan.BEScan scanner =
            new edu.nps.deep.be_scan.BEScan("test setting1");
    scanner.scan("filename1", 1000L, "rp1", buffer1, buffer1.length);
  }

  public static void main(String[] args) {

    testVersion();
    testAvailableScanners();
    testBuffer1();
  }
}

