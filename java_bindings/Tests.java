public final class Tests {

  public static void main(String[] args) {

    System.load("/home/bdallen/local/lib/libbe_scan.la");
    String version = edu.nps.deep.be_scan.be_scan.version();
    System.out.println("Version: " + version);
  }
}

