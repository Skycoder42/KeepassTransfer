import { Component } from '@angular/core';
import {QrConfig} from "./qr-config";
import {EncryptionService} from "./encryption-service";

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  public config: QrConfig = null;
  public loading: boolean = false;

  constructor(private encService: EncryptionService) {}

  onConfigReady(config: QrConfig){
    console.log(config);
    this.config = config;
    this.loading = true;

    this.encService.generateTransferData(config, "test123")
      .then(pubKey => console.log(pubKey));
  }
}
