import {Injectable} from "@angular/core";
import {EncryptionService} from "./encryption-service";

@Injectable()
export class TransferService {
  private socket: WebSocket;
  private secret: string;
  private normalClose: boolean = false;

  private errorHandler: (error: string) => any;
  private resultHandler: (result: any) => any;

  constructor(private encrService: EncryptionService) {}

  public setErrorHandler(errorHandler: (error: string) => any): void {
    this.errorHandler = errorHandler;
  }

  public setResultHandler(resultHandler: (result: any) => any): void {
    this.resultHandler = resultHandler;
  }

  public connectService(secret: string): void {
    this.secret = secret;
    this.socket = new WebSocket("wss://kpt.skycoder42.de/backend/");//TODO generate url
    this.socket.onopen = () => this.sendSecret();
    this.socket.onmessage = ev => this.decryptData(ev);
    this.socket.onerror = ev => this.socketError(ev);
    this.socket.onclose = ev => this.socketClosed(ev);
  }

  private sendSecret(): void{
    this.socket.send(JSON.stringify({
      "MessageType": "WC",
      "Secret": this.secret,
    }));
  }

  private decryptData(messageEvent: MessageEvent): void {
    //GuiController.setProgressText("Decrypting Data\u2026");
    //GuiController.showProgress(true);

    let resultArray = [];

    try {
      let data = JSON.parse(messageEvent.data);
      for(let i = 0; i < data.length; i++) {
        let cData = data[i];

        if(cData.Encrypted)
          cData.Value = this.encrService.decryptData(cData.Value);

        if(cData.Value != null)
          resultArray[resultArray.length] = cData;
        else
          throw new Error("Failed to decrypt data with the given key!");
      }

      this.socket.send(JSON.stringify({
        "Successful": true,
        "Error": null
      }));

      if(this.resultHandler)
        this.resultHandler(resultArray);
    } catch(except) {
      this.socket.send(JSON.stringify({
        "Successful": false,
        "Error": except.message
      }));

      if(this.errorHandler)
        this.errorHandler(except.message);
    } finally {
      this.normalClose = true;
      this.socket.close();
    }
  }

  private socketClosed(closeEvent: CloseEvent): void {
    console.log("Disconnected with close code: " + closeEvent.code)
    if(!this.normalClose && this.errorHandler)
      this.errorHandler("Connection to server was unexpectedly closed!");
  }

  private socketError(errorEvent: ErrorEvent): void {
    console.log(errorEvent.message);
    if(this.errorHandler)
      this.errorHandler("Failed to communicate with backend! Some unknown error occured");
  }
}
