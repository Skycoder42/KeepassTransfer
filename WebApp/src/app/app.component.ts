import {Component, ViewChild} from '@angular/core';
import {QrConfig} from "./qr-config";
import {EncryptionService} from "./encryption-service";

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {

  public loading: boolean = false;
  public loadTitle: string;
  public loadDesc: string;

  public config: QrConfig = null;
  public qrData: string = null;

  constructor(private encService: EncryptionService) {}

  onConfigReady(config: QrConfig){
    this.config = config;

    this.loadTitle = "Generating Key";
    this.loadDesc = "Please wait while your transfer key is being generated. Depending on the key size, this may take a while.";
    this.loading = true;

    this.encService.generateTransferData(config, "test123")
      .then(data => {
        this.qrData = data;
        this.loading = false;
      });
  }
}
