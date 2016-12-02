import { Component } from '@angular/core';
import {QrConfig} from "./qr-config";

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  public config: QrConfig = null;
  public loading: boolean = false;

  onConfigReady(config: QrConfig){
    console.log(config);
    this.config = config;
    this.loading = true;
  }
}
