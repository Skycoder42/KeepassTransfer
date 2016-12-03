import {Component, OnInit, OnDestroy} from '@angular/core';
import {QrConfig} from "./qr-config";
import {EncryptionService} from "./encryption-service";
import {TransferService} from "./transfer-service";

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements OnInit, OnDestroy {

  public loading: boolean = false;
  public loadTitle: string;
  public loadDesc: string;

  public config: QrConfig = null;
  public qrData: string = null;

  constructor(private encService: EncryptionService, private transService: TransferService) {}

  public ngOnInit(): void {
    this.transService.setResultHandler(result => this.resultHandler(result));
    this.transService.setErrorHandler(error => this.errorHandler(error));
  }

  public ngOnDestroy(): void {
    this.transService.setResultHandler(null);
    this.transService.setErrorHandler(null);
  }

  onConfigReady(config: QrConfig){
    this.config = config;

    this.loadTitle = "Generating Key";
    this.loadDesc = "Please wait while your transfer key is being generated. Depending on the key size, this may take a while.";
    this.loading = true;

    let secret = AppComponent.generateRandom(16);
    this.transService.connectService(secret);
    this.encService.generateTransferData(config, secret)
      .then(data => {
        this.qrData = data;
        this.loading = false;
      });
  }

  private resultHandler(result: any): void {
    console.log(result);
  }

  private errorHandler(error: string): void {
    console.log(error);
  }

  private static generateRandom(count: number) : string {
    const possible = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    let text = "";

    for(let i = 0; i < count; i++)
      text += possible.charAt(Math.floor(Math.random() * possible.length));

    return text;
  }
}
