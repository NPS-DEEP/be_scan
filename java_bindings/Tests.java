public final class Tests {

  static {
    System.load("/home/bdallen/local/lib/libbe_scan_jni.so");
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
    String buffer1 = "someone@somewhere.com\nsomeone2@somewhere2.com\n";
    System.out.println("test buffer 1 size: " + buffer1.length());
    edu.nps.deep.be_scan.be_scan_t scanner =
            new edu.nps.deep.be_scan.be_scan_t("test setting1");
    scanner.scan("filename1", 1000L, "rp1", buffer1, buffer1.length());
  }

  public static void main(String[] args) {

    testVersion();
    testAvailableScanners();
    testBuffer1();
  }
}

