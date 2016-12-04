import {UrlQuery} from "./url-query";
export class QrConfig {
  public keySize: number = 2048;
  public errorLevel: string = 'L';
  public qrSize: number = 512;

  public toQuery(): string {
    return "?keySize=" + this.keySize +
      "&errorLevel=" + this.errorLevel +
      "&qrSize=" + this.qrSize;
  }

  public static fromQuery(query: UrlQuery = new UrlQuery()): QrConfig {
    let config = new QrConfig();
    config.keySize = query.query["keySize"] ? query.query["keySize"] : config.keySize;
    config.errorLevel = query.query["errorLevel"] ? query.query["errorLevel"] : config.errorLevel;
    config.qrSize = query.query["qrSize"] ? query.query["qrSize"] : config.qrSize;
    return config;
  }
}
