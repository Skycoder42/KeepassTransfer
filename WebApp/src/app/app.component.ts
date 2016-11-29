import { Component } from '@angular/core';
import {QrConfig} from "./qr-config";

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {

  onConfigReady(config: QrConfig){
    console.log(config);
  }
}
