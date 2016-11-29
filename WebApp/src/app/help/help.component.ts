import { Component } from '@angular/core';
import { QrConfig } from "../qr-config";

@Component({
  selector: 'app-help',
  templateUrl: './help.component.html',
  styleUrls: ['./help.component.css']
})
export class HelpComponent {
  public keySizes = [512, 1024, 2048, 4096, 8192];
  public errorLevels = [
    {key:"Level L (Low)",value:1},
    {key:"Level M (Medium)",value:0},
    {key:"Level Q (Quartile)",value:3},
    {key:"Level H (High)",value:2}
  ];

  public currentConfig = new QrConfig();

  onSubmit() {
    console.log(this.currentConfig.qrSize);
  }
}
