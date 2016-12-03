import {Injectable} from "@angular/core";
import {QrConfig} from "./qr-config";

declare var JSEncrypt: any;

@Injectable()
export class EncryptionService {

  private privateKey: string;
  private publicKey: string;

  generateTransferData(qrConfig : QrConfig, secret: string) : Promise<string> {
    return new Promise(resolve => {
      let crypt = new JSEncrypt({default_key_size: qrConfig.keySize});
      crypt.getKey(() => {
        this.privateKey = crypt.getPrivateKey();
        this.publicKey = crypt.getPublicKey();

        let sendKey = this.publicKey.replace(/\n/g, '');
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
}
