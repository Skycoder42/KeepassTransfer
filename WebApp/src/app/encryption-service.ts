import {Injectable} from "@angular/core";
import {QrConfig} from "./qr-config";
import {DataEntry} from "./data-entry";

declare var JSEncrypt: any;

@Injectable()
export class EncryptionService {

  private crypt:any;

  public generateTransferData(qrConfig : QrConfig, secret: string) : Promise<string> {
    return new Promise(resolve => {
      this.crypt = new JSEncrypt({default_key_size: qrConfig.keySize});
      this.crypt.getKey(() => {
        let sendKey = this.crypt.getPublicKey().replace(/\n/g, '');
        sendKey = sendKey.replace("-----BEGIN PUBLIC KEY-----", '');
        sendKey = sendKey.replace("-----END PUBLIC KEY-----", '');
        let sendData = JSON.stringify({
          "Secret": secret,
          "PublicKey": sendKey,
        });

        resolve(sendData);
      });
    });
  }

  public tryDecryptEntry(entry: DataEntry): boolean {
    if(entry.encrypted)
      entry.value = this.crypt.decrypt(entry.value);
    return entry.value != null;
  }
}
