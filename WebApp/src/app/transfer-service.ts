import {Injectable} from "@angular/core";

@Injectable()
export class TransferService {
  private socket: WebSocket;
  private secret: string;
  private normalClose: boolean = false;

  private errorHandler: (error: string) => any;
  private resultHandler: (result: any) => boolean|string;

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
    let data = JSON.parse(messageEvent.data);
    let reply: any = false;
    if(this.resultHandler)
      reply = this.resultHandler(data);
    if(typeof reply == "boolean") {
      this.socket.send(JSON.stringify({
        "Successful": reply,
        "Error": null
      }));
    } else if(typeof reply == "string") {
      this.socket.send(JSON.stringify({
        "Successful": false,
        "Error": reply
      }));
    }

    this.normalClose = true;
    this.socket.close();
  }

  private socketClosed(closeEvent: CloseEvent): void {
    console.log("Disconnected with close code: " + closeEvent.code)
    if(!this.normalClose && this.errorHandler)
      this.errorHandler("Connection to server was unexpectedly closed!");
  }

  private socketError(errorEvent: ErrorEvent): void {
    console.log(errorEvent.message);
    if(this.errorHandler)
      this.errorHandler("Failed to communicate with backend! Some unknown error occurred.");
  }
}
