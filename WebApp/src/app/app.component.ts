import {Component, OnInit, OnDestroy, ViewContainerRef} from '@angular/core';
import {QrConfig} from "./qr-config";
import {EncryptionService} from "./encryption-service";
import {TransferService} from "./transfer-service";
import {Overlay, Modal} from "angular2-modal";
import {DataEntry} from "./data-entry";
import {UrlQuery} from "./url-query";

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

  private entryList: DataEntry[] = null;

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

    let query = new UrlQuery();
    if(query.query["autoload"])
      this.onConfigReady(QrConfig.fromQuery());
  }

  public ngOnDestroy(): void {
    this.transService.setResultHandler(null);
    this.transService.setErrorHandler(null);
  }

  private onConfigReady(config: QrConfig): void{
    this.config = config;
    this.entryList = null;

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

  private resultHandler(result: DataEntry[]): boolean|string {
    for(let entry of result) {
      if(!this.encService.tryDecryptEntry(entry)) {
        let error = "Failed to decrypt data with the given key!";
        this.errorHandler(error);
        return error;
      }
    }

    this.entryList = result;
    this.qrData = null;
    this.config = null;
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
