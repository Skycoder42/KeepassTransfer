export class QrConfig {
  public keySize: number = 2048;
  public errorLevel: string = 'L';
  public qrSize: number = 512;

  public toQuery(): string {
    return "?keySize=" + this.keySize +
      "&errorLevel=" + this.errorLevel +
      "&qrSize=" + this.qrSize;
  }

  public static fromQuery(query: string): QrConfig {
    let config = new QrConfig();
    let params = QrConfig.extractParams(query);
    config.keySize = params["keySize"] ? params["keySize"] : config.keySize;
    config.errorLevel = params["errorLevel"] ? params["errorLevel"] : config.errorLevel;
    config.qrSize = params["qrSize"] ? params["qrSize"] : config.qrSize;
    return config;
  }

  private static extractParams(query: string): string[] {
    let match,
      pl     = /\+/g,  // Regex for replacing addition symbol with a space
      search = /([^&=]+)=?([^&]*)/g,
      decode = function (s) { return decodeURIComponent(s.replace(pl, " ")); },
      sQuery  = query.substring(1);

    let resArray = [];
    while (match = search.exec(sQuery))
      resArray[decode(match[1])] = decode(match[2]);
    return resArray;
  }
}
