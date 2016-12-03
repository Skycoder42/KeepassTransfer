import {Component, OnInit, OnDestroy, ViewContainerRef} from '@angular/core';
import {QrConfig} from "./qr-config";
import {EncryptionService} from "./encryption-service";
import {TransferService} from "./transfer-service";
import {Overlay, Modal} from "angular2-modal";

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements OnInit, OnDestroy {

  private loading: boolean = false;
  private loadTitle: string;
  private loadDesc: string;

  private config: QrConfig = null;
  private qrData: string = null;

  public constructor(private encService: EncryptionService,
              private transService: TransferService,
              private overlay: Overlay,
              private vcRef: ViewContainerRef,
              private modal: Modal) {
    overlay.defaultViewContainer = vcRef;
  }

  public ngOnInit(): void {
    this.transService.setResultHandler(result => this.resultHandler(result));
    this.transService.setErrorHandler(error => this.errorHandler(error));
  }

  public ngOnDestroy(): void {
    this.transService.setResultHandler(null);
    this.transService.setErrorHandler(null);
  }

  private onConfigReady(config: QrConfig): void{
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

  private resultHandler(result: any): boolean|string {
    for(let i = 0; i < result.length; i++) {
      let data = result[i];

      if(data.Encrypted)
        data.Value = this.encService.decryptData(data.Value);

      if(data.Value == null) {
        let error = "Failed to decrypt data with the given key!";
        this.errorHandler(error);
        return error;
      }
    }

    return true;
  }

  private errorHandler(error: string): void {
    //noinspection TypeScriptUnresolvedFunction
    this.modal.alert()
      .size('lg')
      .isBlocking(true)
      .showClose(true)
      .keyboard(27)
      .headerClass('modal-header alert-danger')
      .title('Error occurred!')
      .body(error)
      .okBtnClass('btn btn-danger')
      .open();
  }

  private static generateRandom(count: number) : string {
    const possible = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    let text = "";

    for(let i = 0; i < count; i++)
      text += possible.charAt(Math.floor(Math.random() * possible.length));

    return text;
  }
}
