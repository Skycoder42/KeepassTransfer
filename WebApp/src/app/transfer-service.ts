import {Injectable} from "@angular/core";
import {DataEntry} from "./data-entry";

@Injectable()
export class TransferService {
  private socket: WebSocket;
  private secret: string;
  private normalClose: boolean = false;

  private errorHandler: (error: string) => any;
  private resultHandler: (result: DataEntry[]) => boolean|string;

  public setErrorHandler(errorHandler: (error: string) => any): void {
    this.errorHandler = errorHandler;
  }

  public setResultHandler(resultHandler: (result: DataEntry[]) => boolean|string): void {
    this.resultHandler = resultHandler;
  }

  public connectService(secret: string): void {
    this.secret = secret;
    this.socket = new WebSocket("wss://" + document.location.host + "/backend/");
    this.socket.onopen = () => this.sendSecret();
    this.socket.onmessage = ev => this.decryptData(ev);
    this.socket.onerror = ev => this.socketError(ev);
    this.socket.onclose = ev => this.socketClosed(ev);
  }

  private sendSecret(): void {
    this.socket.send(JSON.stringify({
      "MessageType": "WC",
      "Secret": this.secret,
    }));
  }

  private decryptData(messageEvent: MessageEvent): void {
    let data = JSON.parse(messageEvent.data);
    let reply: any = false;
    if(this.resultHandler) {
      let dataArray: DataEntry[] = [];
      for(let entry of data)
        dataArray.push(new DataEntry(entry));
      reply = this.resultHandler(dataArray);
    }
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
    console.log("Disconnected with close code: " + closeEvent.code);
    if(!this.normalClose && this.errorHandler)
      this.errorHandler("Connection to server was unexpectedly closed!");
  }

  private socketError(errorEvent: ErrorEvent): void {
    this.normalClose = true;
    console.log(errorEvent.message);
    if(this.errorHandler)
      this.errorHandler("Failed to communicate with backend! Some unknown error occurred.");
  }
}
