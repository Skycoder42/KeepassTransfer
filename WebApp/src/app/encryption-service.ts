import {Injectable} from "@angular/core";
import {QrConfig} from "./qr-config";

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

  public decryptData(encrypted: string): string {
    return this.crypt.decrypt(encrypted);
  }
}
